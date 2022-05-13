#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <typeinfo>
#include <fstream>
#include "Exception.h"
#include "List.h"

//#include "List_District_Loader.h"

using namespace std;

namespace ElectionsDay
{
	class District
	{
	private:
		string name; // district name. 
		static int counter; // will help us to update seriel number.
		int serialNum; // serial number of the district. 
		int numDelegate; // how many delegate this district has.
		int numCanVotes; // how many citizens can vote.
		int numVotes; // how many citizens voted (to all factions). 
		int type; // the type of the district - united or divided. 

	public:

		District(const string name, int numDelegate = 0, int type = 1);
		District();
		District(District& other);
		District(ifstream& inFile);
		~District() {};

		const District& operator=(const District& other);
		string getNameDis() const { return name; };
		int getID() const { return serialNum; }
		int getNumDelegate() const { return numDelegate; }
		float getPrecentVoted() const;
		int getNumOfVotes() const { return numVotes; }
		int getDistrictType() { return type; }
		void setNumVotes();
		void setCanVotes();

		int howManyDelegateDistrict(int numOfDelegate, int numVoted, int numVotPerFac) const;
		void PrintDistrict() const;
		friend ostream& operator<<(ostream& os, const District& district);

		void save(ofstream& outFile) const;
		/*static List<District>* Load_List(ifstream& inFile);*/
	};
}