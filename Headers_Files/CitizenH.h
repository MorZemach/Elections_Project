#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "DistrictH.h"

using namespace std;

namespace ElectionsDay
{
	class Citizen
	{
	private:
		string name;
		int ID;
		int birthYear; // Did the citizen *can* choose according to his age? - check at the main.
		bool isVote; // Did the citizen choose to vote? true OR false. - check at the voting.
		District* district;

	public:
		Citizen(const string name, int ID, int birthYear, District& district);
		Citizen(ifstream& inFile, List<District>* lst);
		~Citizen() {};
		Citizen(const Citizen& other);
		Citizen() :ID(0), birthYear(0), isVote(false), district(nullptr) {}

		const Citizen& operator=(const Citizen& other);
		const string getNameCit() const { return name; }
		int getID() const { return ID; }
		int getBirthYear() const { return birthYear; }
		int getDistrictNum() const { return this->district->getID(); }
		bool getIsVote() const { return isVote; } 

		void setIsVote();
		void setNumVotesDistrict();
		void setNumCanVotesDis();

		void PrintCitizen(bool isDelegate) const;
		friend ostream& operator<<(ostream& os, const Citizen& citizen);
		void save(ofstream& outFile) const;

	};
}
