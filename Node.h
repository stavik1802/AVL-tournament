#ifndef NODE_H
#define NODE_H

#include <iostream>

template<class Key,class Value>
class Node
{
    public:
        Node<Key,Value>* Left;
        Node<Key,Value>* Right;
        Node<Key,Value>* Parent;
        Value Data;
        Key key;
        int m_height,m_BF,m_amount,m_Extra,m_maxRankBeforeExtras,m_maxRankAfterExtras,m_myRank,m_updatedRank,m_sumTil;
        Node(const Key& key,const Value& data):Left(nullptr),Right(nullptr),Parent(nullptr),Data(data),
        key(key),m_height(0),m_BF(0),m_amount(1),m_Extra(0),m_maxRankBeforeExtras(0),m_maxRankAfterExtras(0),m_myRank(0),m_updatedRank(0),
        m_sumTil(0){}
};

template<class Value> class LinkedList;
template<class Value>
class NodeOnList
{
    public:
        NodeOnList* next;
        NodeOnList* prev;
        Value element;
        NodeOnList(Value& element):next(nullptr),prev(nullptr),
        element(element){}
};

template<class Value>
class LinkedList
{
    public:
        NodeOnList<Value>* head;
        NodeOnList<Value>* tail;
        int numOfElements;
        LinkedList():head(nullptr),tail(nullptr),numOfElements(0){}
        ~LinkedList();
        NodeOnList<Value>* insert(Value& newElement);
        void remove(NodeOnList<Value>* removeElement);
        NodeOnList<Value>* findOnList(Value& val);
};

template<class Value>
LinkedList<Value>::~LinkedList()
{
    if(!head)
    {
        return;
    }
    NodeOnList<Value>* tmp=head;
    while (tmp!=nullptr)
    {
        head=head->next;
        delete tmp;
        tmp=head;
    }
}

template<class Value>
NodeOnList<Value>* LinkedList<Value>::insert(Value& newElement)
{
    NodeOnList<Value>* newNode=new NodeOnList<Value>(newElement);
    if(!head)
    {
        head=newNode;
        tail=newNode;
        numOfElements++;
        return head;
    }
    else
    {
        tail->next=newNode;
        newNode->prev=tail;
        tail=tail->next;
        numOfElements++;
        return tail;
    }
}

template<class Value>
void LinkedList<Value>::remove(NodeOnList<Value>* removeElement)
{
    NodeOnList<Value>* tmp;
    if(!head)
    {
        return;
    }
    else if(removeElement==head)
    {
        tmp=head;
        head=head->next;
        tmp->next=nullptr;
        delete tmp;
    }
    else if(removeElement==tail)
    {
        tmp=tail;
        tail=tail->prev;
        tail->next=nullptr;
        tmp->prev=nullptr;
        delete tmp;
    }
    else
    {
        removeElement->prev->next=removeElement->next;
        removeElement->next->prev=removeElement->prev;
        removeElement->next=nullptr;
        removeElement->prev=nullptr;
        delete removeElement;
    }
    numOfElements--;
}

template<class Value>
NodeOnList<Value>* LinkedList<Value>::findOnList(Value& element)
{
    bool foundValue=false;
    NodeOnList<Value>* tmp=head;
    while (tmp!=nullptr && !foundValue)
    {
        if(tmp->element==element)
        {
            foundValue=true;
        }
        else
        {
            tmp=tmp->next;
        }
    }
    return tmp;
}

#endif
