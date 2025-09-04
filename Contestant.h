//
// Created by stavo on 13/02/2024.
//

#ifndef WET2_CONTESTANT_H
#define WET2_CONTESTANT_H
#include "Node.h"

class Contestant{
public:
    int m_contestantId;
    int m_strength;
    NodeOnList<Contestant>* m_teamContestantsList;
    Contestant(int contestantID,int strength):m_contestantId(contestantID),m_strength(strength),m_teamContestantsList(nullptr){}
    bool operator>(const Contestant& contestant)
    {
        bool is=false;
        if(m_strength>contestant.m_strength){return true;}
        else if(m_strength<contestant.m_strength){return false;}
        else m_contestantId>contestant.m_contestantId?is=true:is=false;
        return is;
    }
    
    bool operator==(const Contestant& contestant)
    {
        return this->m_contestantId==contestant.m_contestantId && m_strength==contestant.m_strength;
    }

    bool operator<(const Contestant& contestant)
    {
        return (!((*this)>contestant) && !((*this)==contestant));
    }
};

#endif //WET2_CONTESTANT_H
