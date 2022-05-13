#pragma once
#include "ElectionH.h"

namespace ElectionsDay
{
	class SimpleElectionRound : public Election
	{
	private:
		int simple_Round = 2;
	public:
		SimpleElectionRound(Date& date, int numOfDelegate);
		SimpleElectionRound(ifstream& inFile, Date& date, int numOfDelegate = 0) :Election(inFile, date) {}
		virtual ~SimpleElectionRound() {}

		virtual void updateListCitizen(List<Citizen>* lstVotes, List<District>& lstDistrict, int year);
		virtual void updateListDistrict(List<District>* lst, List<Faction>* lstF);
		virtual void addingFactionDelegate(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict);
		virtual void updateListFaction(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict);

		virtual Faction* get_Faction_With_Most_Delegates_Or_Votes() const;
		virtual void printResFaction(Faction* faction) const;
		virtual void ResDistrict() const;
	};
}