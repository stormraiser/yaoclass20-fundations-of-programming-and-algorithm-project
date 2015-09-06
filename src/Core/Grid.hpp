/**
  * Grid.hpp
  * Defines class template Grid. This class template is
  * one of the three cores of our cell machine model.
  * The main purpose of this part is to manage the whole
  * bunch of Region which is smaller divided part of the cell.
  * Author: Wu Zhelun
  */

#pragma once

#include "Common.h"
#include "Region.hpp"
#include "Rule.h"
#include <cstdlib>
#include <iostream>
#include <set>

using std::cout;
using std::endl;

namespace CACore{

template <typename ValType> class Grid;

enum colortype {RED, BLACK};
template <typename ValType>
class RBTnode
{
public :
    colortype color;
    RBTnode<ValType> *parent, *left, *right;// the neighbor node on the RedBlack tree
    Region<ValType>* key;// storing the region
    int rx, ry;// the region location
    RBTnode() // Default constructor of RBTnode
    {
        parent = left = right = NULL;
        key = NULL;
        color = BLACK;
    };
    RBTnode(int rx, int ry, Region<ValType>* Reg): key(Reg), rx(rx), ry(ry)
		// Constructor of RBT node with a value
    {
        RBTnode();
        key = Reg;
    };
};

template <typename ValType> class RegionWrapper{
public:
	int regionX, regionY;
	Region<ValType> *region;
	RegionWrapper<ValType> *next;

	RegionWrapper(int rx, int ry):
		regionX(rx), regionY(ry), region(NULL), next(NULL) {};
	~RegionWrapper(){};
	void create(Grid<ValType> *parent)
	{region = new Region<ValType>(parent, regionX, regionY);};
};


template <typename ValType>
class Grid{
private:
    int m_minX, m_minY, m_maxX, m_maxY, minRX, maxRX, minRY, maxRY;
	//the border of our board map
    long long time;// number of generations now
    Rule<ValType> *rule;//current rule

	//This one is used for cleanUp function
	RegionWrapper<ValType> *regionList;

public:
	//root of Red Black Tree
	RBTnode<ValType> *RBTroot;
	//general NULL node
	RBTnode<ValType> *sentry;

	//for other function to change the root
	inline void changeRoot(RBTnode<ValType> *RBTnode)
	{
		RBTroot = RBTnode;
	}
	//get the NULL node
	inline RBTnode<ValType>* getSentry()
	{
		return sentry;
	}
	//find the region with wanted (rx,ry) location
	Region<ValType> *findRegion(int rx, int ry);

	//find the region with wanted (rx,ry) location and
	//if there is no such region, create one
    Region<ValType> *getRegion(int rx, int ry);

	//randomize all regions
	void randomize(double *pos);

	//set new rule to each region
    void setRule(Rule<ValType> *p_rule);

	//evolve each region
    void evolve();

	//clear all the region in the Grid and be ready for the new evolution
    void clear();

	//initialize the Grid with a certain width and height
    void setup(int w, int h);

	//put a certain cell on the board map
    void putCell(int x, int y, ValType state);

	//return the current rule
    inline Rule<ValType> *getRule()
        {return rule;};

	//return the current time(generation number)
    inline long long getTime()
        {return time;};

	//obtain a certain cell in the region
    ValType getCell(int x, int y);

	//return how many cells are alive on the board map
	long long population(){return m_population;};

	//return how many cells are on the board map of the status index
	long long population(int index){return stat[index];};

	//return left border
    int minX()
        {return m_minX;};
	//return right border
    int maxX()
        {return m_maxX;};
	//return lower border
    int minY()
        {return m_minY;};
	//return upper border
    int maxY()
        {return m_maxY;};
    Grid();
    ~Grid();
    friend class MainWindow;
	friend class RBTnode<ValType>;
	friend void RB_fix_1(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
	friend void RB_fix_2(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
	friend void RB_fix_3(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
	friend void RB_fix_4(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
	friend void RB_fix_5(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
	friend void RB_insert(Grid<ValType>* thisGrid, RBTnode<ValType>* sentry,
	int rx, int ry, Region<ValType>* Reg);
	friend RBTnode<ValType>* bi_insert(Grid<ValType>* thisGrid, int rx, int ry, Region<ValType>* Reg);
	friend bool notnil(Grid<ValType>* thisGrid, RBTnode<ValType>* n);
	friend void Void_clear(Grid<ValType>* thisGrid, RBTnode<ValType>* temp);
	long long m_population;
	//store the number of cells under different status
	long long stat[256];
};


// The removal of a node with its 6 cases
// We first find the successor and exchange the node value we want to delete and the successor of
// the node we want to delete if its both children are not leaf node and thus transform the
// problem to a one child form and further fix it after the deletion of the successor.
// Since the exchange did not change the structure of the tree, this is a very important step to
// solve the problem

template<typename ValType>
void delete_one_child(Grid<ValType>* thisGrid, RBTnode<ValType>* n);

template<typename ValType>
void RB_removal(Grid<ValType>* thisGrid, RBTnode<ValType>* n);
template<typename ValType>
void delete_fix1(Grid<ValType>* thisGrid, RBTnode<ValType> * n);
template<typename ValType>
void delete_fix2(Grid<ValType>* thisGrid, RBTnode<ValType> * n);
template<typename ValType>
void delete_fix3(Grid<ValType>* thisGrid, RBTnode<ValType> * n);
template<typename ValType>
void delete_fix4(Grid<ValType>* thisGrid, RBTnode<ValType> * n);
template<typename ValType>
void delete_fix5(Grid<ValType>* thisGrid, RBTnode<ValType> * n);
template<typename ValType>
void delete_fix6(Grid<ValType>* thisGrid, RBTnode<ValType> * n);


//These are for the RB tree insertion
template<typename ValType>
void RB_fix_1(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
template<typename ValType>
void RB_fix_2(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
template<typename ValType>
void RB_fix_3(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
template<typename ValType>
void RB_fix_4(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
template<typename ValType>
void RB_fix_5(Grid<ValType>* thisGrid, RBTnode<ValType> *n);
template<typename ValType>
void RB_fix_6(Grid<ValType>* thisGrid, RBTnode<ValType> *n);

template<typename ValType>
void RB_insert(Grid<ValType>* thisGrid,RBTnode<ValType>* RBTroot,
			   RBTnode<ValType>* sentry, int rx, int ry, Region<ValType>* Reg);

//to determine whether it is the NULL node
template<typename ValType>
bool notnil(Grid<ValType>* thisGrid, RBTnode<ValType>* n)
{
	bool temp;
	temp = !(n == thisGrid->getSentry());
	return temp;
}

//leaf is the so called NULL node
template<typename ValType>
bool is_leaf(Grid<ValType>* thisGrid, RBTnode<ValType>* n)
{
	return (!notnil<ValType>(thisGrid, n));
}


//tree left rotation over node x
template<typename ValType>
void rotate_left(Grid<ValType>* thisGrid, RBTnode<ValType>* x)
{
	RBTnode<ValType>* y;
	y = x->right;
	x->right = y->left;
	if (notnil<ValType>(thisGrid, y->left))
	{
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (!notnil<ValType>(thisGrid, x->parent))
	{
		thisGrid->changeRoot(y);
	}
	else if (x == x->parent->left)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

//tree left rotation over node y
template<typename ValType>
void rotate_right(Grid<ValType>* thisGrid, RBTnode<ValType>* y)
{
	RBTnode<ValType>* x;
	x = y->left;
	y->left = x->right;
	if (notnil<ValType>(thisGrid, x->right))
	{
		x->right->parent = y;
	}
	x->parent = y->parent;
	if (!notnil<ValType>(thisGrid, y->parent))
	{
		thisGrid->changeRoot(x);
	}
	else if (y == y->parent->left)
	{
		y->parent->left = x;
	}
	else
	{
		y->parent->right = x;
	}
	x->right = y;
	y->parent = x;
}

// these functions are for finding the relatives of a certain node
//start relatives
template<typename ValType>
RBTnode<ValType>* grandpa(RBTnode<ValType>* n)
{
	if ((n != NULL) && (n->parent != NULL))
	{
		return n->parent->parent;
	}
	else
		return NULL;
}

template<typename ValType>
RBTnode<ValType>* uncle(RBTnode<ValType> *n)
{
	RBTnode<ValType> *g = grandpa(n);
	if (g == NULL)
		return new RBTnode<ValType>();
	if (n -> parent == g -> left)
		return g->right;
	else
		return g->left;
}

template<typename ValType>
RBTnode<ValType>* sibling(RBTnode<ValType> *n)
{
	if (n == n->parent->left)
	{
		return n->parent->right;
	}
	else
	{
		return n->parent->left;
	}
}

template<typename ValType>
RBTnode<ValType>* successor(Grid<ValType>* thisGrid, RBTnode<ValType> *n)
{
	RBTnode<ValType> *temp_node = n;
	if (notnil<ValType>(thisGrid, temp_node->right))
	{
		temp_node = temp_node->right;
		while (notnil<ValType>(thisGrid, temp_node->left))
		{
			temp_node = temp_node->left;
		}
		return temp_node;
	}
	temp_node = n->parent;
	while (notnil<ValType>(thisGrid, temp_node) && (n == temp_node->right))
	{
		n = temp_node;
		temp_node = temp_node->parent;
	}
	return temp_node;
}

//end relatives

template<typename ValType>
void RB_removal(Grid<ValType>* thisGrid, RBTnode<ValType>* n)
{
	RBTnode<ValType> *succ;
	Region<ValType>* temp;
	int tempint;
	if (!is_leaf<ValType>(thisGrid, n->left) && !is_leaf<ValType>(thisGrid, n->right))
	{
		//find the successor so that we could change the two-childs
		//node into one-child node and make the deletion easier
		succ = successor<ValType>(thisGrid, n);
		if (notnil<ValType>(thisGrid, succ))
		{
			temp = succ->key;
			succ->key = n->key;
			n->key = temp;
			tempint = succ->rx;
			succ->rx = n->rx;
			n->rx = tempint;
			tempint = succ->ry;
			succ->ry = n->ry;
			n->ry = tempint;
			n = succ;
		}
	}
	delete_one_child<ValType>(thisGrid, n);
}

template<typename ValType>
void delete_one_child(Grid<ValType>* thisGrid, RBTnode<ValType>* n)
{
	RBTnode<ValType>* child = is_leaf<ValType>(thisGrid, n->left)? n->right: n->left;
	if (!notnil<ValType>(thisGrid, n->parent))
	{
		thisGrid->changeRoot(child);
	}
	else
	{
		if (n->parent->left == n)
		{
			n->parent->left = child;
		}
		else
		{
			n->parent->right = child;
		}
	}
	if (notnil<ValType>(thisGrid, child))
	{
		child->parent = n->parent;
	}
	if (n->color == BLACK && notnil<ValType>(thisGrid, child))
	{
		if (child->color == RED)
		{
			child->color = BLACK;
		}
		else
		{
			delete_fix1<ValType>(thisGrid, child);
		}
	}
	//after fixation, release the region and the tree node
	if (notnil<ValType>(thisGrid,n))
	{
	    delete n->key;
		delete n;
	}
}

template<typename ValType>
void delete_fix1(Grid<ValType>* thisGrid, RBTnode<ValType> * n)
{
	if (notnil<ValType>(thisGrid, n->parent))
	{
		delete_fix2<ValType>(thisGrid, n);
	}
}

template<typename ValType>
void delete_fix2(Grid<ValType>* thisGrid, RBTnode<ValType> * n)
{
	RBTnode<ValType>* s = sibling(n);
	if (s->color == RED)
	{
		n->parent->color = RED;
		s->color = BLACK;

		if (n == n->parent->left)
		{
			rotate_left<ValType>(thisGrid, n->parent);
		}
		else
		{
			rotate_right<ValType>(thisGrid, n->parent);
		}
	}
	delete_fix3<ValType>(thisGrid, n);
}

template<typename ValType>
void delete_fix3(Grid<ValType>* thisGrid, RBTnode<ValType> *n)
{
	RBTnode<ValType> *s = sibling(n);

	if ((n->parent->color == BLACK) && (s->color == BLACK) &&
		(s->left->color == BLACK) && (s->right->color == BLACK))
	{
		s->color = RED;
		delete_fix1<ValType>(thisGrid, n->parent);
	}
	else
	{
		delete_fix4<ValType>(thisGrid, n);
	}
}

template<typename ValType>
void delete_fix4(Grid<ValType>* thisGrid, RBTnode<ValType> * n)
{
	RBTnode<ValType> *s = sibling<ValType>(n);
	if ((n->parent->color == RED) && (s->color == BLACK) &&
		(s->left->color == BLACK) && (s->right->color == BLACK))
	{
		s->color = RED;
		n->parent->color = BLACK;
	}
	else
	{
		delete_fix5<ValType>(thisGrid, n);
	}
}

template<typename ValType>
void delete_fix5(Grid<ValType>* thisGrid, RBTnode<ValType> * n)
{
	RBTnode<ValType>* s = sibling<ValType>(n);
	if (s->color == BLACK)
	{
		if ((n == n->parent->left) && (s->right->color == BLACK) && (s->left->color == RED))
		{
			s->color = RED;
			s->left->color = BLACK;
			rotate_right<ValType>(thisGrid, s);
		}
		else if ((n == n->parent->right) && (s->left->color == BLACK) && (s->right->color == RED))
		{
			s->color = RED;
			s->right->color = BLACK;
			rotate_left<ValType>(thisGrid, s);
		};
	}
	delete_fix6<ValType>(thisGrid, n);
}

template<typename ValType>
void delete_fix6(Grid<ValType>* thisGrid, RBTnode<ValType>* n)
{
	RBTnode<ValType> *s=sibling(n);
	s->color = n->parent->color;
	n->parent->color = BLACK;
	if (n == n->parent->left)
	{
		s->right->color = BLACK;
		rotate_left<ValType>(thisGrid, n->parent);
	}
	else
	{
		s->left->color = BLACK;
		rotate_right<ValType>(thisGrid, n->parent);
	}
}


template<typename ValType>
RBTnode<ValType>* bi_insert(Grid<ValType>* thisGrid, RBTnode<ValType>* root, RBTnode<ValType>* sentry,
							RBTnode<ValType>* node, int rx, int ry, Region<ValType>* Reg)
{
	if (!(notnil<ValType>(thisGrid, root)))
	{
		root = new RBTnode<ValType>(rx, ry, Reg);
		root->color = BLACK;
		root->parent = root->right
			= root->left = sentry;
		thisGrid->changeRoot(root);
		return root;
	}
	if (rx < node->rx || (rx == node->rx && ry < node->ry))
	{
		if (notnil<ValType>(thisGrid, node->left))
		{
			return bi_insert<ValType>(thisGrid, root, sentry, node->left, rx, ry, Reg);
		}
		else
		{
			RBTnode<ValType>* temp_node_left;
			temp_node_left = node->left;
			node->left = new RBTnode<ValType>(rx, ry, Reg);
			node->left->parent = node;
			node->left->color = RED;
			node->left->left = temp_node_left;
			node->left->right = sentry;
			node = node->left;
		}
	}
	else
	{
		if (notnil<ValType>(thisGrid, node->right))
		{
			return bi_insert<ValType>(thisGrid, root, sentry, node->right, rx, ry, Reg);
		}
		else
		{
			RBTnode<ValType>* temp_node_right;
			temp_node_right = node->right;
			node->right = new RBTnode<ValType>(rx, ry, Reg);
			node->right->parent = node;
			node->right->color = RED;
			node->right->right = temp_node_right;
			node->right->left = sentry;
			node = node->right;
		}
	}
	return node;
}

template<typename ValType>
void RB_fix_1(Grid<ValType>* thisGrid, RBTnode<ValType> *n)//n is root
{
	if (!notnil<ValType>(thisGrid, n->parent))
		n->color = BLACK;
	else
		RB_fix_2<ValType>(thisGrid, n);
}

template<typename ValType>
void RB_fix_2(Grid<ValType>* thisGrid, RBTnode<ValType> *n)
	//n's parent is black
{
	if (n->parent->color == BLACK)
		return;
	else
		RB_fix_3<ValType>(thisGrid, n);
}

template<typename ValType>
void RB_fix_3(Grid<ValType>* thisGrid, RBTnode<ValType> *n)
	//parent and uncle are red, n right child of P, and its mirror reflection
{
	RBTnode<ValType> *u = uncle<ValType>(n), *g;
	if (notnil<ValType>(thisGrid, u) && (u -> color == RED))
	{
		n -> parent -> color = BLACK;
		u -> color = BLACK;
		g = grandpa<ValType>(n);
		g -> color = RED;
		RB_fix_1<ValType>(thisGrid, g);
	}
	else
		RB_fix_4<ValType>(thisGrid, n);
}

template<typename ValType>
void RB_fix_4(Grid<ValType>* thisGrid, RBTnode<ValType> *n)
	// red parent and black uncle n left child of P and its mirror reflection occasion
{
	RBTnode<ValType> *g = grandpa<ValType>(n);
	if ((n == n->parent->right) && (n->parent == g->left))
	{
		rotate_left<ValType>(thisGrid, n->parent);
		n = n -> left;
	}
	else if ((n == n->parent->left) && (n->parent == g->right))
	{
		rotate_right<ValType>(thisGrid, n->parent);
		n = n -> right;
	}
	RB_fix_5<ValType>(thisGrid, n);
}

template<typename ValType>
void RB_fix_5(Grid<ValType>* thisGrid, RBTnode<ValType> *n)
	// red p, black u, right child of P and its mirror reflection occasion
{
	RBTnode<ValType> *g = grandpa(n);
	n -> parent -> color = BLACK;
	g -> color = RED;
	if (n == n->parent->left)
		rotate_right<ValType>(thisGrid, g);
	else
		rotate_left<ValType>(thisGrid, g);
}

template<typename ValType>
void RB_insert(Grid<ValType>* thisGrid, RBTnode<ValType>* RBTroot, RBTnode<ValType>* sentry,
			   int rx, int ry, Region<ValType>* Reg)
{
	// first insert into the binary search tree and then make fixation
	RB_fix_1<ValType>(thisGrid, bi_insert(thisGrid, RBTroot, sentry, RBTroot, rx, ry, Reg));
}
// the end of RB tree node function


// start of definitions of Grid class
template <typename ValType>
Region<ValType> *findRegion(int rx, int ry);

template <typename ValType>
void Void_clear(Grid<ValType>* thisGrid, RBTnode<ValType>* temp)
{
	if (!notnil<ValType>(thisGrid, temp))
		return;
	if (notnil<ValType>(thisGrid, temp->left))
		Void_clear<ValType>(thisGrid, temp->left);
	if (notnil<ValType>(thisGrid, temp->right))
		Void_clear<ValType>(thisGrid, temp->right);
	if (temp != thisGrid->sentry)
	{
	    delete temp->key;
		delete temp;
	}
	return;
}

template <typename ValType>
void Grid<ValType>::clear()
{
    int i;
	RegionWrapper<ValType> *p = regionList, *q;
	while (p != NULL){
		q = p->next;
		delete p;
		p = q;
	}
	if (notnil<ValType>(this, RBTroot))
    {
		Void_clear<ValType>(this, RBTroot);
		delete sentry;
	}
	RBTroot = sentry = NULL;
	for (i = 0; i < 256; i++)
		stat[i] = 0;
	  m_population = 0;
}

template <typename ValType>
Grid<ValType>::Grid(){
	RBTroot = sentry = new RBTnode<ValType>();
	regionList = NULL;
    rule = NULL;
    time = 0;
}

template <typename ValType>

Grid<ValType>::~Grid()
{
    Grid<ValType>::clear();
}

template <typename ValType>

void Grid<ValType>::setup(int w, int h)
{
	RegionWrapper<ValType> *p = regionList, *q;
	while (p != NULL){
		q = p->next;
		delete p;
		p = q;
	}
	regionList = NULL;
    if (time > 0 || sentry != NULL)
    {
		Grid<ValType>::clear();
		time = 0;
	}
	RBTroot = sentry = new RBTnode<ValType>();
	m_population = 0;
    if (w == 0)
    {
        w = 0x80000000;
    }
    if (h == 0)
    {
        h = 0x80000000;
    }

    m_maxX = (w - 1) / 2;
    m_minX = m_maxX - w + 1;
    m_maxY = (h - 1) / 2;
    m_minY = m_maxY - h + 1;

    maxRX = m_maxX >> REGION_SCALE_FULL;
    minRX = m_minX >> REGION_SCALE_FULL;
    maxRY = m_maxY >> REGION_SCALE_FULL;
    minRY = m_minY >> REGION_SCALE_FULL;

}


bool insideRegion(int x, int y, int rx, int ry)
{
    if (x == rx && y == ry)
    {
        return true;
    }
    else
    {
        return false;
    }
}


template <typename ValType>
Region<ValType>* Grid<ValType>::findRegion(int rx, int ry)
{
    RBTnode<ValType> *temp;

    // compute the corresponding coordinates of
    // the current region we want to find
    while (rx > maxRX)
        rx -= maxRX - minRX + 1;
    while (rx < minRX)
        rx += maxRX - minRX + 1;
    while (ry > maxRY)
        ry -= maxRY - minRY + 1;
    while (ry < minRY)
        ry += maxRY - minRY + 1;


    temp = RBTroot;
    if (!notnil<ValType>(this, temp))
        return NULL;
    while (notnil<ValType>(this, temp))
        if (insideRegion(rx, ry, temp->rx, temp->ry))
            return temp->key;
        else if ((rx < temp->rx) || ((rx == temp->rx) && (ry < temp->ry)))
            temp = temp->left;
        else
            temp = temp->right;

    return NULL;
}

template <typename ValType>
Region<ValType> *Grid<ValType>::getRegion(int rx, int ry)
{
    RBTnode<ValType> *temp;
	Region<ValType>* tempRegion;
	RegionWrapper<ValType> *p;
    while (rx > maxRX)
        rx -= maxRX - minRX + 1;
    while (rx < minRX)
        rx += maxRX - minRX + 1;
    while (ry > maxRY)
        ry -= maxRY - minRY + 1;
    while (ry < minRY)
        ry += maxRY - minRY + 1;

	temp = RBTroot;
	while (notnil<ValType>(this, temp))
		if (insideRegion(rx, ry, temp->rx, temp->ry))
			return temp->key;
        else if ((rx < temp->rx) || ((rx == temp->rx) && (ry < temp->ry)))
			temp = temp->left;
		else
			temp = temp->right;
	tempRegion = new Region<ValType>(this, rx, ry);
	RB_insert<ValType>(this, this->RBTroot, this->sentry, rx, ry, tempRegion);
	p = regionList;
	p = new RegionWrapper<ValType>(rx, ry);
	p->region = tempRegion;
	p->next = regionList;
	regionList = p;
	temp = RBTroot;
	return tempRegion;
}

template <typename ValType>
void Void_setRule(Grid<ValType>* thisGrid, RBTnode<ValType>* temp, Rule<ValType> *p_rule)
{
	if (notnil<ValType>(thisGrid, temp))
	{
		temp->key->setRule(p_rule);
	}
	else
	{
		return;
	}
	if (notnil<ValType>(thisGrid, temp->left))
	{
		Void_setRule(thisGrid, temp->left, p_rule);
	}

	if (notnil<ValType>(thisGrid, temp->right))
	{
		Void_setRule(thisGrid, temp->right, p_rule);
	}
	return;
}

template <typename ValType>
void Grid<ValType>::setRule(Rule<ValType> *p_rule)
{
	rule = p_rule;
    if (rule->period() == 0){
        int i, j;
        for (i = minRX; i <= maxRX; i++)
            for (j = minRY; j <= maxRY; j++)
                getRegion(i, j)->setRule(rule);
    }
	Void_setRule<ValType>(this, RBTroot, p_rule);
}

template <typename ValType>
void Reg_prepare(Grid<ValType>* thisGrid, RBTnode<ValType>* temp)
{
	if (notnil<ValType>(thisGrid, temp))
	{
		temp->key->prepare();
	}
	else
	{
		return;
	}
	if (notnil<ValType>(thisGrid, temp->left))
	{
		Reg_prepare<ValType>(thisGrid, temp->left);
	}
	if (notnil<ValType>(thisGrid, temp->right))
	{
		Reg_prepare<ValType>(thisGrid, temp->right);
	}
}

template <typename ValType>
void Reg_evolve(RBTnode<ValType>* temp, Grid<ValType>* thisGrid)
{
	if (notnil<ValType>(thisGrid, temp))
	{
		temp->key->evolve();
		thisGrid->m_population += temp->key->population();
	}
	else
	{
		return;
	}
	if (notnil<ValType>(thisGrid, temp->left))
	{
		Reg_evolve<ValType>(temp->left, thisGrid);
	}
	if (notnil<ValType>(thisGrid, temp->right))
	{
		Reg_evolve<ValType>(temp->right, thisGrid);
	}
}

template <typename ValType>
void Reg_isDead(Grid<ValType>* thisGrid, RBTnode<ValType>* temp)
{
	if (!notnil<ValType>(thisGrid, temp))
	{
		return;
	}
	if (notnil<ValType>(thisGrid, temp->left))
	{
		Reg_isDead<ValType>(thisGrid, temp->left);
	}
	if (notnil<ValType>(thisGrid, temp->right))
	{
		Reg_isDead<ValType>(thisGrid, temp->right);
	}
	if (temp->key->isDead())
	{
		RB_removal<ValType>(thisGrid,temp);
	}
}

template <typename ValType>
void Grid<ValType>::evolve()
{
	RegionWrapper<ValType> *p, *q;
	RBTnode<ValType>* temp;
	int i;
	temp = RBTroot;
	// we should fix the clean-up list first and then could we do the cleanup
	// because there could be insertion during the evolution

	p = regionList;
	if (p == NULL)
		return;
	while (p != NULL){
		p->region->cleanUp();
		p = p->next;
	}
	++	this->time;
	Reg_prepare<ValType>(this,RBTroot);
    m_population = 0;
	Reg_evolve<ValType>(RBTroot,this);
	if (rule->period() != 0){
		while ((regionList != NULL) && (regionList->region->isDead())){
			p = regionList->next;
			delete regionList;
			regionList = p;
		}
		p = regionList;
		if (p != NULL)
			while ((p != NULL) && (p->next != NULL)){
				while (p->next != NULL && p->next->region->isDead()){
					q = p->next;
					p->next = q->next;
					delete q;
				}
				p = p->next;
				if (p == NULL)
				{
					break;
				}
			}
	}
	Reg_isDead<ValType>(this, RBTroot);
	return;
}

template <typename ValType>
void Grid<ValType>::putCell(int x, int y, ValType state)
{
    Region<ValType>* node;
	int rx = x >> REGION_SCALE_FULL;
	int ry = y >> REGION_SCALE_FULL;
    node = getRegion(rx, ry);
	m_population += node->putCell(x - rx * REGION_SIZE, y - ry * REGION_SIZE, state);
}

template <typename ValType>
ValType Grid<ValType>::getCell(int x, int y)
{
    Region<ValType> *p;
    int rx = x >> REGION_SCALE_FULL, ry = y >> REGION_SCALE_FULL;
    p = findRegion(rx, ry);
    if (p == NULL)
        return ValType(0);
    else
        return p->getCell(x - rx * REGION_SIZE, y - ry * REGION_SIZE);
}

template <typename ValType>
void Grid<ValType>::randomize(double *pos)
{
	int i, j;
	m_population = 0;
	for (i = minRX; i <= maxRX; i++)
		for (j = minRY; j <= maxRY; j++)
			m_population += getRegion(i, j)->randomize(pos);
}

}
