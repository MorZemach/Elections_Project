#pragma once
#pragma warning(disable:6385)
#pragma warning(disable:6386)
#pragma warning(disable:6001)
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "FactionH.h"
#include "DateH.h"

const int MAX_NAME_LEN = 50;

using namespace std;

namespace ElectionsDay
{
	class Election
	{
	protected:
		Date* electionDate;
		List<District>* lstDistrict;
		List<Citizen>* lstCitizen; // list of all the citizens *can* votes at the election - according birthYear & the year of the election. 
		List<Faction>* lstFaction;

	public:

		///////////////  Constructor & Destructor  ///////////////

		Election(Date & date) : electionDate(&date), lstCitizen(new List<Citizen>()),
			lstDistrict(new List<District>()), lstFaction(new List<Faction>()) {}
	
		Election(ifstream& inFile, Date& date); 
		virtual ~Election();

		///////////////  Getters & Setters  ///////////////

		List<Citizen>* getListCitizen() const { return lstCitizen; }
		List<District>* getListDistrict() const { return lstDistrict; }
		List<Faction>* getListFaction() const { return lstFaction; }
		Date* getDate() const { return electionDate; }

		///////////////   Elecation Functions  ///////////////

		Faction* Winning_Faction_Per_District(int serialDisNum) const;
		void ResElection(bool& resElectionDisplayed) const;
		virtual void ResDistrict() const = 0;
		virtual Faction* get_Faction_With_Most_Delegates_Or_Votes() const = 0;
		virtual void printResFaction(Faction* faction) const = 0;

		///////////////  Check Functions  ///////////////

		void checkCitizen(string name, int ID, int birthYear, int year, int serialNumDistrict);
		void checkDistrictAndFaction(string name, int num);
		void isDigit(char ch);
		int InitNameAndNumber(string& str);

		///////////////  Menu Functions  ///////////////

		virtual void updateListDistrict(List<District>* lst, List<Faction>* lstF) = 0;
		virtual void updateListCitizen(List<Citizen>* lstCitizen, List<District>& lstDistrict, int year) = 0;
		virtual void addingFactionDelegate(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict) = 0;
		virtual void updateListFaction(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict) = 0;
		void save(ofstream& outFile, int roundType) const;

	};
}