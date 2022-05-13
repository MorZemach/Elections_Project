#pragma once
#include "ElectionH.h"


namespace ElectionsDay
{
	class RegularElectionRound : public Election
	{
	private:
		int regular_Round = 1;
	public:
		RegularElectionRound(Date& date) :Election(date) {}
		RegularElectionRound(ifstream& inFile, Date& date) :Election(inFile, date) {}
		virtual ~RegularElectionRound() {}

		virtual void updateListCitizen(List<Citizen>* lstCitizen, List<District>& lstDistrict, int year);
		virtual void updateListDistrict(List<District>* lst, List<Faction>* lstF);
		virtual void addingFactionDelegate(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict);
		virtual void updateListFaction(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict);

		virtual Faction* get_Faction_With_Most_Delegates_Or_Votes() const;
		virtual void printResFaction(Faction* faction) const;
		virtual void ResDistrict() const;

		int Find_Max_At_Array(vector<int>* arr, int& numOfDelegate) const;
		void Print_Candidates_Divided_District(vector<int> arr) const;
	};
}
