//
// Created by stavo on 09/03/2024.
//
#include "Team.h"

void Team::putStrengthInArray(Node<int, Contestant> **teamArr) {
    int index=0;
    m_playersStrengthTree.InOrder(m_playersStrengthTree.root,teamArr,index);
}

Node<int,Contestant>* Team::makeUnitedStrengthTree(Node<int, Contestant> *arr[], int startLimit, int endLimit)
{
    if (startLimit > endLimit){return nullptr;}
    int middle = (startLimit + endLimit)/2;
    Node<int,Contestant>* root = arr[middle];
    root->Left = makeUnitedStrengthTree(arr, startLimit, middle-1);
    root->Right = makeUnitedStrengthTree(arr, middle+1, endLimit);
    return root;
}

void Team::makeUnited(Team &team2)
{
    int numOfPlayersInTeam1=this->m_numOfPlayersInTeam;
    int numOfPlayersInTeam2=team2.m_numOfPlayersInTeam;
    Node<int,Contestant>* team1arr[numOfPlayersInTeam1];
    Node<int,Contestant>* team2arr[numOfPlayersInTeam2];
    putStrengthInArray(team1arr);
    team2.putStrengthInArray(team2arr);
    updateStrengthTreeID(team2arr,(this->m_teamCounter-1),team2.m_numOfPlayersInTeam);
    Node<int,Contestant>* merged[numOfPlayersInTeam1+numOfPlayersInTeam2];
    mergeStrength(team1arr,team2arr,merged,numOfPlayersInTeam1,numOfPlayersInTeam2);
    m_playersStrengthTree.root= makeUnitedStrengthTree(merged,0,(numOfPlayersInTeam1+numOfPlayersInTeam2)-1);
    if(m_playersStrengthTree.root!= nullptr){
        m_playersStrengthTree.root->Parent= nullptr;
    }
    m_playersStrengthTree.PostOrderUpdateValue(m_playersStrengthTree.root);
    team2.m_playersStrengthTree.root= nullptr;
    if(team2.m_playersList.head)
    {
        NodeOnList<Contestant>* toConnect=team2.m_playersList.head;
        NodeOnList<Contestant>* end=team2.m_playersList.tail;
        if(m_playersList.head)
        {
            m_playersList.tail->next=toConnect;
            toConnect->prev=m_playersList.tail;
            m_playersList.tail=end;
        }
        else
        {
            m_playersList.head=toConnect;
            m_playersList.tail=end;
        }
        team2.m_playersList.head= nullptr;
        team2.m_playersList.tail= nullptr;
    }
    m_playersList.numOfElements+=team2.m_playersList.numOfElements;
    m_numOfPlayersInTeam+=numOfPlayersInTeam2;
}

void mergeStrength(Node<int, Contestant>** arr1,Node<int,Contestant>** arr2,Node<int,Contestant>** merged,int numInFirst,int numInSecond)
{
    int team1Index=0,team2Index=0,amount=0;
    while(team1Index<numInFirst&&team2Index<numInSecond)
    {
        if(arr2[team2Index]->Data.m_strength>arr1[team1Index]->Data.m_strength)
        {
            merged[amount]=arr1[team1Index];
            team1Index++;
        }
        else if(arr2[team2Index]->Data.m_strength==arr1[team1Index]->Data.m_strength)
        {
            if(arr2[team2Index]->Data.m_contestantId>arr1[team1Index]->Data.m_contestantId)
            {
                merged[amount]=arr1[team1Index];
                team1Index++;
            }
            else{
                merged[amount]=arr2[team2Index];
                team2Index++;
            }
        }
        else
        {
            merged[amount]=arr2[team2Index];
            team2Index++;
        }
        amount++;
    }
    while (team1Index<numInFirst)
    {
        merged[amount]=arr1[team1Index];
        amount++;
        team1Index++;
    }
    while (team2Index<numInSecond)
    {
        merged[amount]=arr2[team2Index];
        amount++;
        team2Index++;
    }
}

void Team::updateIdBeforeUnite(int toAdd)
{
    NodeOnList<Contestant>* contestant=m_playersList.head;
    while (contestant!=nullptr)
    {
        contestant->element.m_contestantId+=toAdd;
        contestant=contestant->next;
    } 
}

void updateStrengthTreeID(Node<int,Contestant>** arr,int toAdd,int numOfPlayers)
{
    int i=0;
    while(i<numOfPlayers)
    {
        arr[i]->Data.m_contestantId+=toAdd;
        i++;
    }
}