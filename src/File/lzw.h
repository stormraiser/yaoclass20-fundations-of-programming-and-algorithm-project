/**
  * lzw.h
  * Defines functions for lzw compressing and decompressing.
  */

#ifndef lzw_lzw_h
#define lzw_lzw_h

#include "DST.h"
using std::vector;
using std::string;

vector<unsigned short> lzw(vector<unsigned char> input, int total){
    vector<unsigned short> output;
    vector<unsigned char> temp;
    DST_node* ptr1;
    DST dict(total);
    for (int i=0;i<total;i++){
        temp.push_back(i);
        dict.Insert(&temp);
        temp.clear();
    }

    int k(0);
    while (input.size() > k){
        DST_node* ptr = dict.Search(&input,k);
        if (dict.Search_success(&input,k)){
            if (!ptr->isleave)
                ptr = ptr->find(total);
            output.push_back(ptr->ret_cod());
            break;
        }
        ptr1 = ptr->find(input[ptr->pre_bit+k]);
        if (ptr1 != NULL && ptr1->isleave
            && ptr1->ret_rep().size()<=input.size()-k)
        {
            bool same = true;
            for (int i=0; i<ptr1->ret_rep().size(); i++){
                if (input[i+k] != ptr1->ret_rep()[i])
                    same = false;
            }
            if (same)
                ptr = ptr1;
        }
        if (ptr != ptr1)
            ptr = ptr->find(total);
        temp = ptr->ret_rep();
        temp.push_back(input[ptr->ret_rep().size()+k]);
        output.push_back(ptr->ret_cod());
        dict.Insert(&temp);
        k += ptr->ret_rep().size();
        temp.clear();
    }
    return output;
}

vector<unsigned char> de_lzw(vector<unsigned short> input, int total){
    vector<unsigned char> output;
    vector<vector<unsigned char> > dict;
    vector<unsigned char> temp,temp1;
    for (int i=0;i<total;i++){
        temp.push_back(i);
        dict.push_back(temp);
        temp.clear();
    }
    int i(0);
    while (input.size()>i){
        temp = dict[input[i]];
        for (int i=0;i<temp.size();i++)
            output.push_back(temp[i]);
        if (input.size() != 1){
            if (input[1+i]>=dict.size())
                temp.push_back(temp[0]);
            else {
                temp1 = dict[input[1+i]];
                temp.push_back(temp1[0]);
            }
            dict.push_back(temp);
        }
        temp.clear();
        i++;
    }
    return output;
}


#endif
