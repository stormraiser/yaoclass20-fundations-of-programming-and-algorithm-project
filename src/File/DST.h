/**
  * DST.h
  * Implements a digital search tree for use in lzw compression.
  */

#ifndef lzw_DST_h
#define lzw_DST_h

#include <cassert>

using std::string;
using std::vector;

// this DST is specifically designed for LZW in the sense that they contains the bits of code

class DST_node {
private:
    int num;  // this is the number of possible bits
    int code;  // this is specially prepared for lzw, will not be considered in DST
    vector<unsigned char> rep;  // this is its repre string
    vector<DST_node*> array;  // this is used to point to children
public:
    bool isleave;
    int pre_bit;  // this is the number of bits differed in this node, for leave, this will be -1
public:
    DST_node (int nnum, int npre_bit, int ncode=-1);
    // this i should range from 0 to num-1
    DST_node* find(int i);  // find the i^th children of this node
    vector<unsigned char> ret_rep();
    int ret_cod();
    void set_rep (vector<unsigned char> * nrep, int index = 0);
    void set_cod(int ncode);
    void set(DST_node* ptr, int i);  // let ptr become its i^th children
};

DST_node::DST_node (int nnum, int npre_bit, int ncode) {
    // there will be totally num pointers in each node and array[num] will points toward the number itself
    num = nnum;
    pre_bit = npre_bit;
    code = ncode;
    // let array be an empty vector with length num+1
    vector<DST_node*> narray(num+1, NULL);
    array = narray;
    if (pre_bit>=0){
        vector<unsigned char> nnarray(pre_bit,0);
        rep = nnarray;
    }
    isleave = false;
}

DST_node* DST_node::find (int i){
    assert(i>=0);
    assert(i<array.size());
    return array[i];
}

vector<unsigned char> DST_node::ret_rep(){
    return rep;
}

int DST_node::ret_cod(){
    return code;
}

void DST_node::set_rep(vector<unsigned char> * nrep, int index){
    rep.clear();
    for (int i=index;i<nrep->size();i++)
        rep.push_back(nrep->at(i));
}

void DST_node::set(DST_node* ptr, int i){
    assert(i>=0);
    assert(i<array.size());
    array[i] = ptr;
}

void DST_node::set_cod(int ncode){
    code = ncode;
}



class DST {
private:
    int bits;
    int num;  // invited to convinient the process of allocating code
    bool isempty;
    DST_node* root;
    void del(DST_node* ptr);
public:
    int ret_num();
    DST (int nbits);
    ~DST ();
public:
    DST_node* ret_root();
    DST_node* Search(vector<unsigned char> * rep, int index = 0);  // return the closest node above the failure point
    DST_node* Insert(vector<unsigned char> * rep, int index = 0);
    DST_node* Delete(DST_node* * ptr);
    bool Search_success(vector<unsigned char> * rep, int index = 0);
};

DST::DST(int nbits){
    bits = nbits;
    isempty = true;
    num = 0;
    root = NULL;
}

DST::~DST(){
    //del(root);
}

void DST::del(DST_node* ptr){
    if (ptr->isleave){
        delete ptr;
        return;
    }
    for (int i=0;i<bits+1;i++)
        if (ptr->find(i) != NULL)
            delete ptr->find(i);
    delete ptr;
}

int DST::ret_num(){
    return num;
}

DST_node* DST::ret_root(){
    return root;
}

// Search will the last node at which node.rep match with the searched rep
DST_node* DST::Search(vector<unsigned char> * rep, int index){
    DST_node* ptr = root;
    DST_node* last_ptr = NULL;
    bool same = true;
    if (isempty)
        return NULL;
    if (rep->size() == index)
        return root;
    do {
        last_ptr = ptr;
        ptr = ptr->find((*rep)[ptr->pre_bit+index]);
        if (ptr == NULL)
            return last_ptr;
        if (rep->size()-index < ptr->ret_rep().size())
            return last_ptr;
        for (int i=0; i<ptr->ret_rep().size(); i++){
            if ((*rep)[i+index] != ptr->ret_rep()[i])
                same = false;
        }
        if (!same)
            return last_ptr;
        if (rep->size()-index == ptr->ret_rep().size())
            return ptr;
    } while (!ptr->isleave);
    return last_ptr;
}

bool DST::Search_success(vector<unsigned char> * rep, int index){
    DST_node* ptr = Search(rep, index);
    if (ptr == NULL)
        return false;
    if (rep->size() == index)
        return (ptr->find(bits)!=NULL);
    if (ptr->isleave)
        return true;
    if (rep->size()-index == ptr->ret_rep().size())
        return (ptr->find(bits)!=NULL);
    return false;
}

// Insert will return the leaf attached
DST_node* DST::Insert(vector<unsigned char> * nrep, int index){
    DST_node* ptr2 = Search(nrep, index);
    if (Search_success(nrep, index))
        return ptr2;

    num++;
    DST_node* ptr = new DST_node(bits,-1,num-1);
    ptr -> set_rep(nrep, index);
    ptr -> isleave = true;
    if (isempty) {
        DST_node* ptr1 = new DST_node(bits,0);
        assert(ptr->ret_rep()[0]<=bits);
        if (ptr->ret_rep().size() == 0)
            ptr1->set(ptr,bits);
        else
            ptr1->set(ptr,ptr->ret_rep()[0]);
        root = ptr1;
        isempty = false;
        return ptr;
    }

    if (nrep->size()-index == ptr2->ret_rep().size()) {
        if (ptr2->isleave)
            return ptr2;
        else {
            ptr2->set(ptr,bits);
            return ptr;
        }
    }

    DST_node* ptr3 = ptr2->find((*nrep)[ptr2->pre_bit+index]);
    if (ptr3 == NULL) {
        ptr2->set(ptr,(*nrep)[ptr2->pre_bit+index]);
        return ptr;
    }
    int i(0);
    vector<unsigned char> str;
    while (i<ptr3->ret_rep().size() && (*nrep)[i+index]==ptr3->ret_rep()[i]){
        str.push_back((*nrep)[i+index]);
        i++;
    }
    DST_node* ptr1 = new DST_node(bits,i);
    ptr1->set_rep(&str);

    ptr2->set(ptr1,(*nrep)[ptr2->pre_bit+index]);
    ptr1->set(ptr,(*nrep)[i+index]);
    if (i == ptr3->ret_rep().size())
        ptr1->set(ptr3,bits);
    else
        ptr1->set(ptr3,ptr3->ret_rep()[i]);

    return ptr;
}


#endif
