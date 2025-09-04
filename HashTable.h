#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "AVLTree.h"

template<class Key,class Value>
class HashTable
{
    private:
        static const int INTIAL_SIZE=2;
        int tableSize,numberOfElement;
        double a_factor;
        AVLTree<Key,Value>** hashTable;
    public:
        HashTable();
        ~HashTable();
        void insert(Key& key,Value& element);
        void remove(Key& key);
        int hashFunction(Key& value) const;
        AVLTree<Key,Value>* operator[](int index);
        bool isTableFull() const {return a_factor==1;}
        bool isQuarterFull() const {return numberOfElement<=tableSize/4;} 
        void reHasing(int currentSize);
        Node<Key,Value>* find(Key& key);
};

template<class Key,class Value>
HashTable<Key,Value>::HashTable():tableSize(INTIAL_SIZE),numberOfElement(0),
a_factor(0),hashTable(new AVLTree<Key,Value>*[INTIAL_SIZE])
{
    for(int i=0;i<tableSize;i++)
    {
        hashTable[i]=nullptr;
    }
}

template<class Key,class Value>
HashTable<Key,Value>::~HashTable()
{
    for(int i=0;i<tableSize;i++)
    {
        if(!hashTable[i])
        {
            continue;
        }
        delete hashTable[i];
    }
    delete[] hashTable;
}

template<class Key,class Value>
void HashTable<Key,Value>::insert(Key& key,Value& element)
{
    int whereToPut=hashFunction(key);
    if(!hashTable[whereToPut])
    {
        hashTable[whereToPut]=new AVLTree<Key,Value>();
    }
    hashTable[whereToPut]->insert(key,element);
    ++numberOfElement;
    a_factor=double(numberOfElement)/tableSize;
    if(isTableFull())
    {
        int currentSize=tableSize;
        tableSize*=2;
        reHasing(currentSize);
    }
}

template<class Key,class Value>
void HashTable<Key,Value>::remove(Key& key)
{
    int whereToPut=hashFunction(key);
    if(numberOfElement==0){return;}
    hashTable[whereToPut]->remove(key);
    if(hashTable[whereToPut]->root==nullptr)
    {
        delete hashTable[whereToPut];
        hashTable[whereToPut]=nullptr;
    }
    --numberOfElement;
    a_factor=double(numberOfElement)/tableSize;
    if(isQuarterFull())
    {
        int currentSize=tableSize;
        tableSize/=2;
        reHasing(currentSize);
    }
}

template<class Key,class Value>
int HashTable<Key,Value>::hashFunction(Key& value) const
{
    return value%tableSize;
}

template<class Key,class Value>
void HashTable<Key,Value>::reHasing(int currentSize)
{
    AVLTree<Key,Value>** newHashTable;
    try
    {
        int newIndex=0;
        newHashTable=new AVLTree<Key,Value>*[tableSize];
        for(int j=0;j<tableSize;j++)
        {
            newHashTable[j]=nullptr;
        }
        for(int i=0;i<currentSize;i++)
        {
            if(hashTable[i])
            {
                int index=0,amount=0;
                amount=hashTable[i]->root->m_amount;
                Node<Key,Value>* teamArr[amount];
                hashTable[i]->InOrder(hashTable[i]->root,teamArr,index);
                for(int i=0;i<amount;i++)
                {
                    Node<Key,Value>* node=teamArr[i];
                    newIndex=hashFunction(node->key);
                    node->Left=node->Right=node->Parent=nullptr;
                    if(!newHashTable[newIndex])
                    {
                        newHashTable[newIndex]=new AVLTree<Key,Value>();
                        newHashTable[newIndex]->root=node;
                        newHashTable[newIndex]->updateValues(newHashTable[newIndex]->root);              
                    }
                    else
                    {
                        newHashTable[newIndex]->updateValues(node);   
                        newHashTable[newIndex]->root=newHashTable[newIndex]->insertNode(newHashTable[newIndex]->root,node,0);
                    }
                }
                hashTable[i]->root=nullptr;
                delete hashTable[i];
            }
        }
        delete[] hashTable;
        hashTable=newHashTable;
    }
    catch(const std::exception& e)
    {
        delete[] newHashTable;
    }
}

template<class Key,class Value>
Node<Key,Value>* HashTable<Key,Value>::find(Key& key)
{
    if(!hashTable[hashFunction(key)])
    {
        return nullptr;
    }
    return hashTable[hashFunction(key)]->find(key);
}

template<class Key,class Value>
AVLTree<Key,Value>* HashTable<Key,Value>::operator[](int index)
{
    return hashTable[index];
}

#endif
