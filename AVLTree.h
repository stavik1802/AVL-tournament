
//

#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include "Node.h"

template<class Key,class Value>
class AVLTree
{
    public:
        Node<Key,Value>* root;
        bool madeRotating;
        AVLTree():root(nullptr),madeRotating(false){}
        ~AVLTree();
        void freeTree(Node<Key,Value>* node);
        Node<Key,Value>* find(Key& key);
        Node<Key,Value>* findNodeOnTree(Node<Key,Value>* node,Key& key);
        Node<Key,Value>* find(Value& val,int sumExtras);
        Node<Key,Value>* findNodeOnTree(Node<Key,Value>* node,Value& val,int sumExtras);
        void rightRotating(Node<Key,Value>* node);
        void leftRotating(Node<Key,Value>* node);
        void leftRightRotating(Node<Key,Value>* node);
        void rightLeftRotating(Node<Key,Value>* node);
        void insert(Key& key,Value& data,int Rank=0);
        Node<Key,Value>* insertNode(Node<Key,Value>* node,Node<Key,Value>* newNode,int sumExtras);
        void updateValues(Node<Key,Value>* node);
        void removeNode(Node<Key,Value>* node,bool change);
        void remove(Key& key);
        void remove(Value& val,int sumExtra);
        Node<Key,Value>* Select(int index);
        Node<Key,Value>* SelectRecuv(Node<Key,Value>*node,int index);
        void fixTreeAfterRemove(Node<Key,Value>* node);
        Node<Key,Value>* findSequentNum(Node<Key,Value>* node);
        bool checkBalanceFactor(Node<Key,Value>* node);
        Node<Key,Value>* findLargest();
        void addValue(Value& val,int element);
        void addValueAux(Node<Key,Value>* node,Value& val,int element,bool madeRight,int sumExtra);
        int findAndSum(Value& val);
        int findAndSumAux(Node<Key,Value>* node,Value& val);
        int findClosetFromRight(Key& key,Node<Key,Value>** node);
        int findClosetFromRightAux(Key& key,Node<Key,Value>** updatedNode,Node<Key,Value>* node);
        int findClosetFromLeft(Key& key);
        int findClosetFromLeftAux(Key& key,Node<Key,Value>* node);
        void InOrder(Node<Key,Value>* first,Node<Key,Value>** teamArr,int& index);
        void PostOrderUpdateValue(Node<Key,Value>* node);
        void updateHighestRankedPath();


};

template<class Key,class Value>
void AVLTree<Key,Value>::freeTree(Node<Key,Value>* node)
{
    if(!node)
    {
        return;
    }
    freeTree(node->Left);
    freeTree(node->Right);
    delete node;
}

template<class Key,class Value>
AVLTree<Key,Value>::~AVLTree()
{
    freeTree(root);
    root=nullptr;
}

template<class Key,class Value>
Node<Key,Value>* AVLTree<Key,Value>::findNodeOnTree(Node<Key,Value>* node,Key& key)
{
    if(!node)
    {
        return nullptr;
    }
    if(key==node->key)
    {
        return node;
    }
    return key > node->key?findNodeOnTree(node->Right,key):findNodeOnTree(node->Left,key); 
}

template<class Key,class Value>
Node<Key,Value>* AVLTree<Key,Value>::find(Key& key)
{
    return findNodeOnTree(root,key);
}

template<class Key,class Value>
Node<Key,Value>* AVLTree<Key,Value>::findNodeOnTree(Node<Key,Value>* node,Value& val,int sumExtras)
{
    if(!node)
    {
        return nullptr;
    }
    sumExtras+=node->m_Extra;//sum extras
    node->m_updatedRank=node->m_myRank+sumExtras;
    node->m_sumTil=sumExtras-node->m_Extra;// update rank of the node in the route
    if(val==node->Data)
    {
        return node;
    }
    return val > node->Data?findNodeOnTree(node->Right,val,sumExtras):findNodeOnTree(node->Left,val,sumExtras); 
}

template<class Key,class Value>
Node<Key,Value>* AVLTree<Key,Value>::find(Value& val,int sumExtras)
{
    return findNodeOnTree(root,val,sumExtras);
}

template<class Key,class Value>
Node<Key,Value>* AVLTree<Key,Value>::insertNode(Node<Key,Value>* node,Node<Key,Value>* newNode,int sumExtras)
{
    if(!node)
    {
        newNode->m_updatedRank+=sumExtras;
        newNode->m_Extra-=sumExtras;
        newNode->m_sumTil=sumExtras;
        return newNode;
    }
    else if(node->Data>newNode->Data)
    {
        sumExtras+=node->m_Extra;//sum extra
        node->m_updatedRank=node->m_myRank+sumExtras;
        node->Left=insertNode(node->Left,newNode,sumExtras);
        node->Left->Parent=node;
    }
    else
    {
        sumExtras+=node->m_Extra;//sum extra
        node->m_updatedRank=node->m_myRank+sumExtras;
        node->Right=insertNode(node->Right,newNode,sumExtras);
        node->Right->Parent=node;
    }
    int prevHeight=node->m_height;
    updateValues(node);
    if(prevHeight!=node->m_height)
    {
        madeRotating=checkBalanceFactor(node);
        if(madeRotating)
        {
            return node->Parent;
        }
    }
    return node;
}

template<class Key,class Value>
bool AVLTree<Key,Value>::checkBalanceFactor(Node<Key,Value>* node)
{
    if(node->m_BF==2)
    {
        if(node->Left->m_BF>=0)
        {
            leftRotating(node);
        }
        else
        {
            leftRightRotating(node);
        }
        return true;
    }
    else if(node->m_BF==-2)
    {
        if(node->Right->m_BF<=0)
        {
            rightRotating(node);
        }
        else
        {
            rightLeftRotating(node);
        }
        return true;
    }
    return false; 
}

template<class Key,class Value>
void AVLTree<Key,Value>::insert(Key& key,Value& data,int Rank)
{
    Node<Key,Value>* node=find(data,0);
    if(node)
    {
        return;
    }
    if(!root)
    {
        root=new Node<Key,Value>(key,data);
        root->m_myRank=root->m_updatedRank=Rank;
        updateValues(root);
        return;
    }
    Node<Key,Value>* newNode=new Node<Key,Value>(key,data);
    newNode->m_myRank=newNode->m_updatedRank=Rank;
    updateValues(newNode);
    root=insertNode(root,newNode,0);
}

template<class Key,class Value>
void AVLTree<Key,Value>::leftRotating(Node<Key,Value>* node)
{
    Node<Key,Value>* tmp=node->Left;
    node->Left=tmp->Right;
    tmp->Right=node;
    if(node->Left)
    {
        node->Left->Parent=node;
    }
    tmp->Parent=node->Parent;
    if(node->Parent)
    {
        if(node->Parent->Left==node)
        {
            node->Parent->Left=tmp;
        }
        else
        {
            node->Parent->Right=tmp;
        }
    }
    node->Parent=tmp;
    if(node==root)
    {
        root=tmp;
        node=root->Right;
    }
    int tempResult=node->Parent->m_Extra;
    node->Parent->m_Extra+=node->m_Extra;
    node->m_Extra=(-1)*tempResult;
    bool haveGrandpa=node->Parent->Parent!=nullptr;
    if(haveGrandpa)
    {
        node->m_sumTil=node->Parent->Parent->m_sumTil+node->Parent->Parent->m_Extra+node->Parent->m_Extra;
        node->Parent->m_sumTil=node->Parent->Parent->m_sumTil+node->Parent->Parent->m_Extra;
    }
    else
    {
        node->m_sumTil=node->Parent->m_Extra;
        node->Parent->m_sumTil=0;
    }
    if(node->Left)
    {
        node->Left->m_Extra+=tempResult;
        node->Left->m_sumTil=node->Parent->m_sumTil+node->Left->m_Extra;
    }
    updateValues(node);
    updateValues(node->Parent);
}

template<class Key,class Value>
void AVLTree<Key,Value>::leftRightRotating(Node<Key,Value>* node)
{
    rightRotating(node->Left);
    leftRotating(node);
}

template<class Key,class Value>
void AVLTree<Key,Value>::rightLeftRotating(Node<Key,Value>* node)
{
    leftRotating(node->Right);
    rightRotating(node);
}

template<class Key,class Value>
void AVLTree<Key,Value>::rightRotating(Node<Key,Value>* node)
{
    Node<Key,Value>* tmp=node->Right;
    node->Right=tmp->Left;
    tmp->Left=node;
    if(node->Right)
    {
        node->Right->Parent=node;
    }
    tmp->Parent=node->Parent;
    if(node->Parent)
    {
        if(node->Parent->Left==node)
        {
            node->Parent->Left=tmp;
        }
        else
        {
            node->Parent->Right=tmp;
        }
    }
    node->Parent=tmp;
    if(root==node)
    {
        root=tmp;
        node=root->Left;
    }
    int tempResult=node->Parent->m_Extra;
    node->Parent->m_Extra+=node->m_Extra;
    node->m_Extra=(-1)*tempResult;
    bool haveGrandpa=node->Parent->Parent!=nullptr;
    if(haveGrandpa)
    {
        node->m_sumTil=node->Parent->Parent->m_sumTil+node->Parent->Parent->m_Extra+node->Parent->m_Extra;
        node->Parent->m_sumTil=node->Parent->Parent->m_sumTil+node->Parent->Parent->m_Extra;
    }
    else
    {
        node->m_sumTil=node->Parent->m_Extra;
        node->Parent->m_sumTil=0;
    }
    if(node->Right)
    {
        node->Right->m_Extra+=tempResult;
        node->Right->m_sumTil=node->Parent->m_sumTil+node->Right->m_Extra;
    }
    updateValues(node);
    updateValues(node->Parent);
}

template<class Key,class Value>
Node<Key,Value>* AVLTree<Key,Value>::Select(int index)
{
    if(!root)
    {
        return nullptr;
    }
    return SelectRecuv(root,index);
}

template<class Key,class Value>
Node<Key,Value>* AVLTree<Key,Value>::SelectRecuv(Node<Key,Value>* node,int index)
{
    
    if(!node->Left)
    {
        if(index==1)
        {
            return node;
        }
        else
        {
            return SelectRecuv(node->Right,(index-1));
        }
    }
    else if(node->Left->m_amount==index-1)
    {
        return node;
    }
    else if(node->Left->m_amount>index-1)
    {
        return SelectRecuv(node->Left,index);
    }
    else
    {
        return SelectRecuv(node->Right,(index-node->Left->m_amount-1));
    }
}

template<class Key,class Value>
void AVLTree<Key,Value>::updateValues(Node<Key,Value>* node)
{
    if(!node->Left && !node->Right)
    {
        node->m_height=0;
        node->m_BF=0;
        node->m_amount=1;
        node->m_updatedRank=node->m_myRank+node->m_Extra+node->m_sumTil;
        node->m_maxRankAfterExtras=node->m_updatedRank;//update maxAfter
        node->m_maxRankBeforeExtras=node->m_myRank;
    }
    else if(!node->Left)
    {
        node->m_height=1+node->Right->m_height;
        node->m_BF=-1-node->Right->m_height;
        node->m_amount=1+node->Right->m_amount;
        node->m_maxRankAfterExtras=std::max(node->m_myRank+node->m_Extra+node->m_sumTil,node->Right->m_maxRankAfterExtras);//update maxAfter
        node->m_maxRankBeforeExtras=std::max(node->m_myRank+node->m_Extra+node->m_sumTil,node->Right->m_maxRankBeforeExtras);//update maxBefore
    }
    else if(!node->Right)
    {
        node->m_height=1+node->Left->m_height;
        node->m_BF=1+node->Left->m_height;
        node->m_amount=1+node->Left->m_amount;
        node->m_maxRankAfterExtras=std::max(node->m_myRank+node->m_Extra+node->m_sumTil,node->Left->m_maxRankAfterExtras);//update maxAfter
        node->m_maxRankBeforeExtras=std::max(node->m_myRank+node->m_Extra+node->m_sumTil,node->Left->m_maxRankBeforeExtras);//update maxBefore
    }
    else
    {
        node->m_height=1+std::max(node->Left->m_height,node->Right->m_height);
        node->m_BF=(node->Left->m_height)-(node->Right->m_height);
        node->m_amount=1+node->Left->m_amount+node->Right->m_amount;
        node->m_maxRankAfterExtras=std::max(node->m_myRank+node->m_Extra+node->m_sumTil,std::max(node->Left->m_maxRankAfterExtras,node->Right->m_maxRankAfterExtras));//update maxAfter
        node->m_maxRankBeforeExtras=std::max(node->m_myRank+node->m_Extra+node->m_sumTil,std::max(node->Left->m_maxRankBeforeExtras,node->Right->m_maxRankBeforeExtras));//update maxBefore
    }
}

template<class Key,class Value>
void AVLTree<Key,Value>::remove(Key& key)
{
    Node<Key,Value>* nodeToRemove=find(key);
    if(!nodeToRemove)
    {
        return;
    }
    removeNode(nodeToRemove,true);
}

template<class Key,class Value>
void AVLTree<Key,Value>::remove(Value& val,int sumExtras)
{
    Node<Key,Value>* nodeToRemove=find(val,0);
    if(!nodeToRemove)
    {
        return;
    }
    removeNode(nodeToRemove,true);
}

template<class Key,class Value> 
void AVLTree<Key,Value>::removeNode(Node<Key,Value>* nodeToRemove,bool change)
{
    if(!nodeToRemove->Left && !nodeToRemove->Right)
    {
        if(!nodeToRemove->Parent)
        {
            delete nodeToRemove;
            root=nullptr;
            return;
        }
        else if(nodeToRemove->Parent->Left==nodeToRemove)
        {
            nodeToRemove->Parent->Left=nullptr;
        }
        else
        {
            nodeToRemove->Parent->Right=nullptr;
        }
        fixTreeAfterRemove(nodeToRemove->Parent);
        delete nodeToRemove;
    }
    else if(!nodeToRemove->Left)
    {
        if(!nodeToRemove->Parent)
        {
            Node<Key,Value>* tmp=root;
            root=root->Right;
            root->m_Extra+=tmp->m_Extra;
            root->Parent=nullptr;
            tmp->Right=nullptr;
            delete tmp;
            return;
        }
        else if(nodeToRemove->Parent->Left==nodeToRemove)
        {
            if(change)
            {
                nodeToRemove->Right->m_Extra+=nodeToRemove->m_Extra;
            }
            nodeToRemove->Parent->Left=nodeToRemove->Right;
        }
        else
        {
            if(change)
            {
                nodeToRemove->Right->m_Extra+=nodeToRemove->m_Extra;
            }
            nodeToRemove->Parent->Right=nodeToRemove->Right;
        }
        Node<Key,Value>* tmp=nodeToRemove->Right;
        nodeToRemove->Right->Parent=nodeToRemove->Parent;
        nodeToRemove->Right=nullptr;
        find(tmp->Data,0);
        updateValues(tmp);
        fixTreeAfterRemove(nodeToRemove->Parent);
        delete nodeToRemove;
    }
    else if(!nodeToRemove->Right)
    {
        if(!nodeToRemove->Parent)
        {
            Node<Key,Value>* tmp=root;
            root=root->Left;
            root->m_Extra+=tmp->m_Extra;
            root->Parent=nullptr;
            tmp->Left=nullptr;
            delete tmp;
            return;
        }
        else if(nodeToRemove->Parent->Left==nodeToRemove)
        {
            if(change)
            {
                nodeToRemove->Left->m_Extra+=nodeToRemove->m_Extra;
            }
            nodeToRemove->Parent->Left=nodeToRemove->Left;
        }
        else
        {
            if(change)
            {
                nodeToRemove->Left->m_Extra+=nodeToRemove->m_Extra;
            }
            nodeToRemove->Parent->Right=nodeToRemove->Left;
        }
        Node<Key,Value>* tmp=nodeToRemove->Left;
        nodeToRemove->Left->Parent=nodeToRemove->Parent;
        nodeToRemove->Left=nullptr;
        find(tmp->Data,0);
        updateValues(tmp);
        fixTreeAfterRemove(nodeToRemove->Parent);
        delete nodeToRemove;
    }
    else 
    {
        Node<Key,Value>* nodeToSwap=findSequentNum(nodeToRemove->Right);
        find(nodeToSwap->Data,0);
        updateValues(nodeToSwap);
        nodeToSwap->m_sumTil=nodeToRemove->m_sumTil;
        if(nodeToSwap->Left && nodeToRemove->Left!=nodeToSwap)
        {
            nodeToSwap->Left->m_Extra+=nodeToSwap->m_Extra;
        }
        if(nodeToSwap->Right && nodeToRemove->Right!=nodeToSwap)
        {
            nodeToSwap->Right->m_Extra+=nodeToSwap->m_Extra;
        }
        int extra=findAndSumAux(nodeToRemove->Right,nodeToSwap->Data);
        nodeToSwap->m_Extra=extra+nodeToRemove->m_Extra;
        if(nodeToRemove->Left!=nodeToSwap)
        {
            nodeToRemove->Left->m_Extra-=extra;
            nodeToRemove->Left->m_sumTil=nodeToSwap->m_sumTil+nodeToSwap->m_Extra;
        }
        if(nodeToRemove->Right!=nodeToSwap)
        {
            nodeToRemove->Right->m_Extra-=extra;
            nodeToRemove->Right->m_sumTil=nodeToSwap->m_sumTil+nodeToSwap->m_Extra;
        }
        if(!nodeToRemove->Parent)
        {
            if(nodeToSwap->Parent==nodeToRemove)
            {
                nodeToSwap->Left=nodeToRemove;
                nodeToRemove->Right=nullptr;
                nodeToRemove->Parent=nodeToSwap;
                nodeToSwap->Parent=nullptr;
            }
            else
            {
                nodeToSwap->Left=nodeToRemove->Left;
                nodeToRemove->Left=nullptr;
                nodeToSwap->Parent->Left=nodeToRemove;
                nodeToSwap->Left->Parent=nodeToSwap;
                nodeToRemove->Parent=nodeToSwap->Parent;
                nodeToSwap->Parent=nullptr;
                Node<Key,Value>* tmp=nodeToRemove->Right;
                nodeToRemove->Right=nodeToSwap->Right;
                nodeToSwap->Right=tmp;
                nodeToSwap->Right->Parent=nodeToSwap;
            }
        }
        else
        {
            if(nodeToRemove->Parent->Left==nodeToRemove)
            {
                nodeToRemove->Parent->Left=nodeToSwap;
            }
            else
            {
                nodeToRemove->Parent->Right=nodeToSwap;
            }
            if(nodeToSwap->Parent!=nodeToRemove)
            {
                nodeToSwap->Left=nodeToRemove->Left;
                nodeToRemove->Left->Parent=nodeToSwap;
                nodeToRemove->Left=nullptr;
                nodeToSwap->Parent->Left=nodeToRemove;
                if(!nodeToSwap->Right)
                {
                    nodeToSwap->Right=nodeToRemove->Right;
                    nodeToSwap->Right->Parent=nodeToSwap;
                    nodeToRemove->Right=nullptr;
                }
                else
                {
                    Node<Key,Value>* tmp=nodeToRemove->Right;
                    nodeToRemove->Right=nodeToSwap->Right;
                    nodeToSwap->Right->Parent=nodeToRemove;
                    nodeToSwap->Right=tmp;
                    nodeToSwap->Right->Parent=nodeToSwap;
                }
                Node<Key,Value>* newTmp=nodeToSwap->Parent;
                nodeToSwap->Parent=nodeToRemove->Parent;
                nodeToRemove->Parent=newTmp;
            }
            else
            {
                nodeToSwap->Left=nodeToRemove;
                nodeToSwap->Parent=nodeToRemove->Parent;
                nodeToRemove->Parent=nodeToSwap;
                nodeToRemove->Right=nullptr;
            }
        }
        find(nodeToSwap->Data,0);
        updateValues(nodeToSwap);
        removeNode(nodeToRemove,false);
    } 
}

template<class Key,class Value>
void AVLTree<Key,Value>::fixTreeAfterRemove(Node<Key,Value>* node)
{
    if(!node->Parent)
    {
        updateValues(node);
        checkBalanceFactor(node);
        if(!node->Parent)
        {
            root=node;
        }
        else
        {
            fixTreeAfterRemove(node->Parent);
        }
        return;
    }
    updateValues(node);
    checkBalanceFactor(node);
    fixTreeAfterRemove(node->Parent);
}

template<class Key,class Value>
Node<Key,Value>* AVLTree<Key,Value>::findSequentNum(Node<Key,Value>* node)
{
    if(!node->Left)
    {
        return node;
    }
    return findSequentNum(node->Left);
}

template<class Key,class Value>//find the Largest Node
Node<Key,Value>* AVLTree<Key,Value>::findLargest()
{
    if(root == nullptr){return nullptr;}
    Node<Key,Value>* temp=root;
    while(temp->Right!= nullptr)
    {
        temp=temp->Right;
    }
    return temp;
}

template<class Key,class Value>
int AVLTree<Key,Value>::findClosetFromRight(Key& key,Node<Key,Value>** winner)
{
    return findClosetFromRightAux(key,winner,root);
}

template<class Key,class Value>
int AVLTree<Key,Value>::findClosetFromRightAux(Key& key,Node<Key,Value>** updatedNode,Node<Key,Value>* node)
{
    if(!node)
    {
        return 0;
    }
    if(node->key>key)
    {
        return findClosetFromRightAux(key,updatedNode,node->Left);
    }
    else
    {
        *updatedNode=node;
        if(node->Left)
        {
            return findClosetFromRightAux(key,updatedNode,node->Right)+1+node->Left->m_amount;
        }
        else
        {
            return findClosetFromRightAux(key,updatedNode,node->Right)+1;
        }
    }
}

template<class Key,class Value>
int AVLTree<Key,Value>::findClosetFromLeft(Key& key)
{
    return findClosetFromLeftAux(key,root);
}

template<class Key,class Value>
int AVLTree<Key,Value>::findClosetFromLeftAux(Key& key,Node<Key,Value>* node)
{
    if(!node)
    {
        return 0;
    }
    if(node->key<=key)
    {
        if(node->Left)
        {
            return findClosetFromLeftAux(key,node->Right)+1+node->Left->m_amount;
        }
        else
        {
            return findClosetFromLeftAux(key,node->Right)+1;
        }
    }
    else
    {
        return findClosetFromLeftAux(key,node->Left);
    }
}

template<class Key,class Value>
void AVLTree<Key,Value>::addValue(Value& val,int element)
{
    addValueAux(root,val,element,false,0);
}

template<class Key,class Value>
void AVLTree<Key,Value>::addValueAux(Node<Key,Value>* node,Value& val,int element,bool madeRight,int sumExtra)
{
    if(!node)
    {
        return;
    }
    if(node->Data<val)
    {
        if(!madeRight)
        {
            node->m_Extra+=element;
        }
        sumExtra+=node->m_Extra;
        node->m_sumTil=sumExtra-node->m_Extra;
        addValueAux(node->Right,val,element,true,sumExtra);
    }
    else if(node->Data>val)
    {
        if(madeRight)
        {
            node->m_Extra-=element;
        }
        sumExtra+=node->m_Extra;
        node->m_sumTil=sumExtra-node->m_Extra;
        addValueAux(node->Left,val,element,false,sumExtra);
    }
    else
    {
        if(!madeRight)
        {
            node->m_Extra+=element;
        }
        sumExtra+=node->m_Extra;
        if(node->Right)
        {
            node->Right->m_Extra-=element;
            node->Right->m_sumTil=sumExtra;
        }
        node->m_sumTil=sumExtra-node->m_Extra;
    }
}

template<class Key,class Value>
int AVLTree<Key,Value>::findAndSum(Value& val)
{
    return findAndSumAux(root,val);
}

template<class Key,class Value>
int AVLTree<Key,Value>::findAndSumAux(Node<Key,Value>* node, Value& val)
{
    if(!node)
    {
        return 0;
    }
    else if(node->Data>val)
    {
        return node->m_Extra+findAndSumAux(node->Left,val);
    }
    else if(node->Data<val)
    {

        return node->m_Extra+findAndSumAux(node->Right,val);
    }
    else
    {
        return node->m_Extra;
    }
}

template<class Key,class Value>//put all the items in array in the order of the numbers
void AVLTree<Key,Value>::InOrder(Node<Key,Value>* node,Node<Key,Value>** teamArr,int& index)
{
    if(node== nullptr){
        return;
    }
    InOrder(node->Left,teamArr,index);
    teamArr[index++]=node;
    InOrder(node->Right,teamArr,index);
}

template<class Key,class Value>//put all the items in array in the order of the numbers
void AVLTree<Key,Value>::PostOrderUpdateValue(Node<Key,Value>* node)
{
    if(node== nullptr){
        return;
    }
    PostOrderUpdateValue(node->Left);
    PostOrderUpdateValue(node->Right);
    updateValues(node);
    if(node->Left!=nullptr)
    {
        node->Left->Parent=node;
    }
    if(node->Right!=nullptr)
    {
        node->Right->Parent=node;
    }
}

#endif
