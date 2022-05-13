#pragma once
#include <iostream>
#include "CitizenH.h"
#include "DelegatesH.h"

namespace ElectionsDay
{
	class Faction
	{
	private:
		string name; // שם המפלגה
		static int counterFac; // מס' מפלגות
		int serialNum; // מס' סידורי
		Citizen* candidate; // מועמד המפלגה
		List<Delegates>* lst; // רשימת נציגים לכל מחוז
		int sumDelegate; // סכום הנציגים שהמחוז זכה בהם
		int sumVotes;
		bool checked = false;

	public:
		Faction() :candidate(nullptr), lst(new List<Delegates>()), serialNum(0), sumDelegate(0), sumVotes(0) {}
		Faction(const string name, Citizen& candidate, List<District>* lst);
		Faction(ifstream& inFile, List<Citizen>* lstOfCitizen, List<District>* lstOfDistrict);
		~Faction();
		const Faction& operator=(const Faction& other);

		int getID() const { return serialNum; }
		int getCandidateID() const { return candidate->getID(); }
		int getTotalVotesForFaction() const;
		int getSumVotes() const { return sumVotes; }
		int getSumDelegate() const { return sumDelegate; }
		string getNameFac() const { return name; }
		const string getNameCandidate() const { return candidate->getNameCit(); }
		List<Delegates>* getList() const { return lst; }
		bool getIfAlreadyChecked() const { return checked; }

		void setSumDelegate(int numDelegatePerDis) { sumDelegate += numDelegatePerDis; }
		void setSumVotes();
		void setCheck(bool _checked) { checked = _checked; }

		void PrintFactionPerDistrict() const;
		void PrintFactionDitails() const;
		void Print_Candidate() const;

		friend ostream& operator<<(ostream& os, const Faction& faction);
		void save(ofstream& outFile) const;
	};
}
