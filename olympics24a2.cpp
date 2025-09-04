#include "olympics24a2.h"

int olympics_t::updateWinningTeam(Node<int,Team>* winningTeam)
{
	winningTeam->Data.m_numOfWins++;
    int sumExtra=m_tournamentTree.findAndSum(winningTeam->Data.m_myTournamentTeam);// no need anymore?
	m_tournamentTree.remove(winningTeam->Data.m_myTournamentTeam,0);
	winningTeam->Data.updatePowerAndRank(sumExtra);
	int newRank=winningTeam->Data.m_teamRank;
    int newPower=winningTeam->Data.m_teamPower;
    winningTeam->Data.m_myTournamentTeam.m_teamPower=newPower;
    winningTeam->Data.m_myTournamentTeam.m_teamRank=newRank;
	m_tournamentTree.insert(newPower,winningTeam->Data.m_myTournamentTeam,newRank);//there is no insert for this inputs
    m_highestRank=m_tournamentTree.root->m_maxRankAfterExtras;
	return winningTeam->key;
}

bool olympics_t::IsPowerOf2(int num)
{
    if (num <= 0) {
        return false;
    }
    while (num > 1) {
        if (num % 2 != 0) {
            return false;
        }
        num /= 2;
    }
    return true;
}

olympics_t::olympics_t():m_teamsTable(HashTable<int,Team>()),m_tournamentTree(AVLTree<int,TeamInTournament>()),
m_highestRank(0),m_numOfTeams(0){}

olympics_t::~olympics_t()=default;

StatusType olympics_t::add_team(int teamId)
{
	if(teamId<=0){return StatusType::INVALID_INPUT;}
	if(m_teamsTable.find(teamId)!=nullptr){return StatusType::FAILURE;}
	try
	{
        TeamInTournament newTeamInTournament(teamId);
		Team newTeam(teamId,newTeamInTournament);
		m_teamsTable.insert(teamId,newTeam);
	}
	catch(...){return StatusType::ALLOCATION_ERROR;}
    m_numOfTeams++;
	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_team(int teamId)
{
	if(teamId<=0){return StatusType::INVALID_INPUT;}
	Node<int,Team>* team=m_teamsTable.find(teamId);
	if(team==nullptr){return StatusType::FAILURE;}
	try
	{
        m_tournamentTree.remove(team->Data.m_myTournamentTeam,0);
		m_teamsTable.remove(teamId);
        if(m_tournamentTree.root!= nullptr) 
        {
            m_highestRank = m_tournamentTree.root->m_maxRankAfterExtras;
        }
        else
        {
            m_highestRank=0;
        }
	}
	catch(...){return StatusType::ALLOCATION_ERROR;}
    m_numOfTeams--;
	return StatusType::SUCCESS;
}

StatusType olympics_t::add_player(int teamId,int playerStrength)
{
	if(teamId<=0 || playerStrength<=0){return StatusType::INVALID_INPUT;}
	Node<int,Team>* team=m_teamsTable.find(teamId);
	if(team==nullptr){return StatusType::FAILURE;}
	try
	{
        int sumExtra=0;
        if(team->Data.m_teamCounter>1)
        {
            sumExtra=m_tournamentTree.findAndSum(team->Data.m_myTournamentTeam);
        }
		int playerIndex=team->Data.m_teamCounter;
		Contestant newPlayer(playerIndex,playerStrength);
		team->Data.m_playersStrengthTree.insert(playerStrength,newPlayer);
		team->Data.insertToTeam(newPlayer);
        m_tournamentTree.remove(team->Data.m_myTournamentTeam,0);
		team->Data.updatePowerAndRank(sumExtra);
        int newPower=team->Data.m_teamPower;
		int newRank=team->Data.m_teamRank;
		team->Data.m_myTournamentTeam.m_teamPower=newPower;
        team->Data.m_myTournamentTeam.m_teamRank=newRank;
		m_tournamentTree.insert(newPower,team->Data.m_myTournamentTeam,newRank);
        if(m_tournamentTree.root!= nullptr) 
        {
            m_highestRank = m_tournamentTree.root->m_maxRankAfterExtras;
        }
        else{
            m_highestRank=0;
        }
	}
	catch(...){return StatusType::ALLOCATION_ERROR;}
	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_newest_player(int teamId)
{
	if(teamId<=0){return StatusType::INVALID_INPUT;}
	Node<int,Team>* team=m_teamsTable.find(teamId);
	if(team==nullptr || team->Data.m_numOfPlayersInTeam==0){return StatusType::FAILURE;}
	try
	{
		NodeOnList<Contestant>* player=team->Data.m_playersList.tail;
		team->Data.removeFromTeam(player);
		int sumExtra=m_tournamentTree.findAndSum(team->Data.m_myTournamentTeam);
        m_tournamentTree.remove(team->Data.m_myTournamentTeam,sumExtra);
        team->Data.updatePowerAndRank(sumExtra);
        int newPower=team->Data.m_teamPower;
        int newRank=team->Data.m_teamRank;
        team->Data.m_myTournamentTeam.m_teamPower=newPower;
        team->Data.m_myTournamentTeam.m_teamRank=newRank;
        if(team->Data.m_numOfPlayersInTeam!=0) {
            m_tournamentTree.insert(newPower,team->Data.m_myTournamentTeam,newRank);
        }
        if(m_tournamentTree.root!= nullptr) {
            m_highestRank = m_tournamentTree.root->m_maxRankAfterExtras;
        }
        else{m_highestRank=0;}
		
	}
	catch(...){return StatusType::ALLOCATION_ERROR;}
	return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
	if(teamId1<=0||teamId2<=0 || teamId1==teamId2){return StatusType::INVALID_INPUT;}
	Node<int,Team>* team1=m_teamsTable.find(teamId1);
	Node<int,Team>* team2=m_teamsTable.find(teamId2);
	if(team1==nullptr || team2==nullptr || team1->Data.m_numOfPlayersInTeam==0 || team2->Data.m_numOfPlayersInTeam==0){return StatusType::FAILURE;}
	if(team1->Data.m_teamPower>team2->Data.m_teamPower)
	{
		return output_t<int>(updateWinningTeam(team1));
	}
	else if(team1->Data.m_teamPower<team2->Data.m_teamPower)
	{
		return output_t<int>(updateWinningTeam(team2));
	}
	else
	{
		if(teamId1<teamId2)
		{
			return output_t<int>(updateWinningTeam(team1));
		}
		else
		{
			return output_t<int>(updateWinningTeam(team2));
		}
	}
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    if(teamId<=0){return StatusType::INVALID_INPUT;}
	Node<int,Team>* team=m_teamsTable.find(teamId);
	if(team==nullptr){return StatusType::FAILURE;}
    int sumExtra=0;
    if(team->Data.m_teamCounter>1)
    {
        sumExtra=m_tournamentTree.findAndSum(team->Data.m_myTournamentTeam);
    }
	return output_t<int>(team->Data.m_numOfWins+sumExtra);
}

output_t<int> olympics_t::get_highest_ranked_team()
{
	if(m_numOfTeams==0)
	{
		return output_t<int>(-1);
	}
    if(m_tournamentTree.root== nullptr&&m_numOfTeams>0)
    {
        return output_t<int>(0);
    }
    return output_t<int>(m_highestRank);
}

StatusType olympics_t::unite_teams(int teamId1,int teamId2)
{
    if(teamId1<=0||teamId2<=0||teamId1==teamId2){return StatusType::INVALID_INPUT;}
    Node<int,Team>* team1=m_teamsTable.find(teamId1);
    Node<int,Team>* team2=m_teamsTable.find(teamId2);
    if(team1== nullptr||team2== nullptr){return StatusType::FAILURE;}
    if(team1->Data.m_numOfPlayersInTeam == 0 && team2->Data.m_numOfPlayersInTeam == 0)
    {
        m_teamsTable.remove(teamId2);
        m_numOfTeams--;
        return StatusType::SUCCESS;
    }
    team2->Data.updateIdBeforeUnite(team1->Data.m_teamCounter-1);
    int sumExtraTeam1=0;
    if(team1->Data.m_numOfPlayersInTeam>0)
    {
        sumExtraTeam1=m_tournamentTree.findAndSum(team1->Data.m_myTournamentTeam);//needed?
    }
    if(team1->Data.m_numOfPlayersInTeam>0) {
        m_tournamentTree.remove(team1->Data.m_myTournamentTeam,0);
    }
    if(team2->Data.m_numOfPlayersInTeam>0) {
        m_tournamentTree.remove(team2->Data.m_myTournamentTeam,0);// update it when the new remove in ready
    }
    team1->Data.makeUnited(team2->Data);
    team1->Data.m_teamCounter=team1->Data.m_teamCounter+team2->Data.m_teamCounter-1;
    m_teamsTable.remove(teamId2);
    m_numOfTeams--;
    team1->Data.updatePowerAndRank(sumExtraTeam1);
    int newPower=team1->Data.m_teamPower;
    int newRank=team1->Data.m_teamRank;
    team1->Data.m_myTournamentTeam.m_teamPower=newPower;
    team1->Data.m_myTournamentTeam.m_teamRank=newRank;
	m_tournamentTree.insert(newPower,team1->Data.m_myTournamentTeam,newRank);
	m_highestRank=m_tournamentTree.root->m_maxRankAfterExtras;
    return StatusType::SUCCESS;

}

Node<int,TeamInTournament>*  olympics_t::getHighestRankedTeam()
{
    Node<int,TeamInTournament>* temp = m_tournamentTree.root;
    int sumExtra=0;
    if(!temp)
    {
        return nullptr;
    }
    if(temp->m_myRank+temp->m_Extra==m_highestRank)
    {
        return m_tournamentTree.root;
    }
    while(((temp->Right!= nullptr)&&temp->Right->m_maxRankAfterExtras==m_highestRank)||((temp->Left!= nullptr)&& temp->Left->m_maxRankAfterExtras==m_highestRank))
    {
        temp->m_sumTil=sumExtra;
        sumExtra+=temp->m_Extra;
        if(temp->Right!= nullptr&&temp->Right->m_maxRankAfterExtras==m_highestRank)
        {
            temp=temp->Right;
        }
        else if(temp->Left!= nullptr) {
            temp = temp->Left;
        }
        else{return temp;}
    }
    temp->m_sumTil+=sumExtra;
    return temp;

}

void olympics_t::right_max(Node<int, TeamInTournament> *winner)
{
    int sumExtra=0,sumLeft=0,maxTemp=0,nodeNewRank=0;
    sumExtra=m_tournamentTree.findAndSum(winner->Data);
    while(winner!= nullptr)
    {
        if(winner->Left) {
            sumLeft = winner->Left->m_Extra + sumExtra + winner->Left->m_maxRankBeforeExtras;
            winner->Left->m_maxRankAfterExtras=std::max(winner->Left->m_maxRankAfterExtras,sumLeft);
        }
        nodeNewRank=winner->m_myRank+sumExtra;
        if(winner->Right) {
            winner->m_maxRankAfterExtras = std::max(winner->m_maxRankAfterExtras,std::max(winner->Right->m_maxRankAfterExtras,std::max(nodeNewRank, sumLeft)));
        }
        else{
            winner->m_maxRankAfterExtras=std::max(winner->m_maxRankAfterExtras,std::max(nodeNewRank,sumLeft));
        }
        maxTemp=std::max(winner->m_maxRankAfterExtras,maxTemp);
        sumExtra-=winner->m_Extra;
        sumLeft=0;
        winner=winner->Parent;
    }
    if(maxTemp>m_highestRank)
    {
        m_highestRank=maxTemp;
    }
}

void olympics_t::updateHighestRankedPath(Node<int, TeamInTournament> *highest)
{
    if(highest== nullptr)
    {
        return;
    }
    if(highest==m_tournamentTree.root)
    {
        return;
    }
    int sumExtra=m_tournamentTree.findAndSum(highest->Data);
    int UpdateRank=0;
    UpdateRank=sumExtra+highest->m_myRank;
    highest->m_maxRankAfterExtras=std::max(highest->m_maxRankAfterExtras,UpdateRank);
    if(highest->Left)
    {
        highest->Left->m_maxRankAfterExtras=std::max(highest->Left->m_maxRankAfterExtras,highest->m_maxRankAfterExtras);
    }
    if(highest->Right)
    {
        highest->Right->m_maxRankAfterExtras=std::max(highest->Right->m_maxRankAfterExtras,highest->m_maxRankAfterExtras);
    }
    highest=highest->Parent;
    while (highest!= nullptr)
    {
     if(highest->Left&&highest->Right)
     {
         highest->m_maxRankAfterExtras=std::max(highest->m_maxRankAfterExtras,std::max(highest->Right->m_maxRankAfterExtras,highest->Left->m_maxRankAfterExtras));
     }
     else if(highest->Left)
     {
         highest->m_maxRankAfterExtras=std::max(highest->m_maxRankAfterExtras,highest->Left->m_maxRankAfterExtras);
     }
     else if(highest->Right)
     {
         highest->m_maxRankAfterExtras=std::max(highest->m_maxRankAfterExtras,highest->Right->m_maxRankAfterExtras);
     }
     highest=highest->Parent;
    }

}

void olympics_t::stimulate_match(int i, Node<int, TeamInTournament> *winner, int HighBound)
{
    while(i>0)
    {
        Node<int,TeamInTournament>* leftBoundNode=m_tournamentTree.Select(HighBound-i);//select for input is the index of the item
        m_tournamentTree.addValue(winner->Data,1);//add value for Data
        m_tournamentTree.addValue(leftBoundNode->Data,-1);//add value for Data
        right_max(winner);
        i=i/2;
    }
}

output_t<int> olympics_t::play_tournament(int lowPower,int highPower)
{
    if(lowPower<=0||highPower<=0||highPower<=lowPower){return StatusType::INVALID_INPUT;}
    Node<int,TeamInTournament>* winner;
    lowPower=lowPower-1;
    Node<int,TeamInTournament>* temp;
    int highBoundNumOfItems=m_tournamentTree.findClosetFromRight(highPower,&winner);// findClosest to the min return numOfItems until him
    int lowBoundNumOfItems=m_tournamentTree.findClosetFromRight(lowPower, &temp);// findClosest to the max return numOfItems until him and update winner
    int i=highBoundNumOfItems-lowBoundNumOfItems;
    if(i==1)
    {
        return output_t<int>(winner->Data.m_teamId);
    }
    if(!IsPowerOf2(i)){return StatusType::FAILURE;}
    stimulate_match(i/2,winner,highBoundNumOfItems);
    updateHighestRankedPath(getHighestRankedTeam());
    return output_t<int>(winner->Data.m_teamId);
}
