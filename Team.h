#ifndef WET2_TEAM_H
#define WET2_TEAM_H
#include <iostream>
#include "AVLTree.h"
#include "Contestant.h"

class Team;
class TeamInTournament
{
public:
    int m_teamId,m_teamPower,m_teamRank;
    TeamInTournament(int teamId):m_teamId(teamId),m_teamPower(0),m_teamRank(0){}
    bool operator>(const TeamInTournament& team)
    {
        if(m_teamPower>team.m_teamPower){return true;}
        else if(m_teamPower<team.m_teamPower){return false;}
        else{return m_teamId<team.m_teamId?true:false;}
    }
    bool operator==(const TeamInTournament& team)
    {
        return m_teamId==team.m_teamId && m_teamPower==team.m_teamPower;
    }
    bool operator<(const TeamInTournament& team)
    {
        return (!((*this)>team) && !((*this)==team));
    }
};

class Team {
public:
    int m_teamId,m_teamRank, m_teamPower,m_teamCounter;
    int m_numOfWins,m_numOfPlayersInTeam;
    TeamInTournament m_myTournamentTeam;
    AVLTree<int,Contestant> m_playersStrengthTree;
    LinkedList<Contestant> m_playersList;
    void makeUnited(Team &team2);
    void putStrengthInArray(Node<int,Contestant>** teamArr);
    Node<int,Contestant>* makeUnitedStrengthTree(Node<int,Contestant>* arr[],int start,int end);
    Team(int teamId,TeamInTournament myTeam):m_teamId(teamId),m_teamRank(0),m_teamPower(0),m_teamCounter(1),m_numOfWins(0),m_numOfPlayersInTeam(0),m_myTournamentTeam(myTeam),m_playersStrengthTree(AVLTree<int,Contestant>()),
    m_playersList(LinkedList<Contestant>()){}
    void updateIdBeforeUnite(int toAdd);
    void insertToTeam(Contestant& player)
    {
        int sumExtras=0;
        m_playersStrengthTree.find(player,sumExtras)->Data.m_teamContestantsList=m_playersList.insert(player);
        m_numOfPlayersInTeam++;
        m_teamCounter++;
    }
    
    void removeFromTeam(NodeOnList<Contestant>* player)
    {
        int sumExtras=0;
        m_playersStrengthTree.remove(player->element,sumExtras);
        m_playersList.remove(player);
        m_numOfPlayersInTeam--;
        m_teamCounter--;
    } 

    void updatePowerAndRank(int sumExtras)
    {
        int halfInTeam=(m_numOfPlayersInTeam/2)+1;
        Node<int,Contestant>* halfInTeamInTree=m_playersStrengthTree.Select(halfInTeam);
        if(halfInTeamInTree)
        {
            m_teamPower=halfInTeamInTree->Data.m_strength*m_numOfPlayersInTeam;
            m_numOfWins+=sumExtras;
            m_teamRank=m_teamPower+m_numOfWins;
            return;
        }
        m_teamPower=0;
        m_numOfWins+=sumExtras;
        m_teamRank=0;
    }
    
    bool operator>(const Team& team)
    {
        return m_teamId>team.m_teamId?true:false;
    }
    bool operator==(const Team& team)
    {
        return m_teamId==team.m_teamId;
    }
    bool operator < (const Team& team)
    {
        return  m_teamId<team.m_teamId?true:false;
    }
};

void mergeStrength(Node<int, Contestant>** arr1,Node<int,Contestant>** arr2,Node<int,Contestant>** merged,int numInFirst,int numInSecond);
void updateStrengthTreeID(Node<int,Contestant>** arr,int toAdd,int numOfPlayers);

#endif //WET2_TEAM_H
