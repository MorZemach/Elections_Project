#define _CRT_SECURE_NO_WARNINGS
#include "CitizenH.h"

namespace ElectionsDay
{
//-----------------------------------------------------------------------------------------------//
	Citizen::Citizen(const string name, int ID, int birthYear, District& district) // Citizen Constructor
		:isVote(false)
	{
		try
		{
			this->name = name;
			this->ID = ID;
			this->birthYear = birthYear;
			this->district = &district;
		}
		catch (bad_alloc& ex) // Memory allocation failed.
		{
			cout << "Error: " << ex.what() << endl;
		}
	}
//-----------------------------------------------------------------------------------------------//
	const Citizen& Citizen::operator=(const Citizen& other) // Placement opertor
	{
		if (this != &other) // Avoid self-placement
		{
			try
			{
				this->name = other.name; 
				this->ID = other.ID;
				this->district = other.district;
				this->birthYear = other.birthYear;
				this->isVote = other.isVote;
			}
			catch (bad_alloc& ex) // Memory allocation failed.
			{
				cout << "Error: " << ex.what() << endl;
			}
		}
		return *this;
	}
//-----------------------------------------------------------------------------------------------//
	Citizen::Citizen(ifstream& inFile, List<District>* lst) // load the current citizen into a given fill.
	{
		int lenName, serialNumDistrict;
		inFile.read(rcastc(&lenName), sizeof(int));
		name.resize(lenName + 1);
		inFile.read(rcastc(&name[0]), sizeof(char)* lenName);
		name[lenName] = '\0';
		inFile.read(rcastc(&ID), sizeof(int)); // load citizen ID.
		inFile.read(rcastc(&birthYear), sizeof(int)); // load citizen year of birth.
		inFile.read(rcastc(&isVote), sizeof(bool)); // load citizen voting status.
		inFile.read(rcastc(&serialNumDistrict), sizeof(int)); // load the serial number od the district that the citizen is belong to.

		District* district = lst->findNode(serialNumDistrict); // base the serial number, finding the citizen district. 

		if (district) // the district is found.
		{
			this->district = district;
		}
		else
		{
			cout << "There is a problem with loading the serial number of the district that this citizen is belong to." <<
				" The district is not found." << endl;
			return;
		}
	}
//-----------------------------------------------------------------------------------------------//
	Citizen::Citizen(const Citizen& other) // Copy Constructor
	{
		try
		{
			this->name = other.name;
			this->ID = other.ID;
			this->birthYear = other.birthYear;
			this->isVote = other.isVote;
			this->district = other.district;
		}
		catch (bad_alloc& ex) // Memory allocation failed.
		{
			cout << "Error: " << ex.what() << endl;
		}
	}
//-----------------------------------------------------------------------------------------------//
	void Citizen::setIsVote() // update citizen's voting status.
	{
		this->isVote = true;
		if (this->isVote == false) // The set function isn't scceeded.
		{
			throw SettingException();
		}
	}
//-----------------------------------------------------------------------------------------------//
	void Citizen::setNumVotesDistrict() // when the current citizen vote, it will update the number of citizen from the district that voted.
	{
		try
		{
			this->district->setNumVotes();
		}
		catch (SettingException& ex)
		{
			throw;
		}
	}
//-----------------------------------------------------------------------------------------------//
	void Citizen::setNumCanVotesDis() // when the current citizen is over 18, it will update the number of citizen from the district that can vote.
	{
		try
		{
			this->district->setCanVotes();
		}
		catch (SettingException& ex)
		{
			throw;
		}
	}
//-----------------------------------------------------------------------------------------------//
	void Citizen::PrintCitizen(bool isDelegate) const // print citisen date into the screen.
	{
		/////////////////////OUTPUT/////////////////////////
		//Name: __, ID: __, year of birth: ___.
		//The citizen belongs to district number _.
		/////////////////////OUTPUT/////////////////////////

		cout << "Name: " << name << ", ID: " << ID << ", year of birth: " << birthYear << "." << endl;
		if (isDelegate == false)
			cout << "The citizen belongs to district number " << district->getID() << "." << endl;
	}
//-----------------------------------------------------------------------------------------------//
	ostream& operator<<(ostream& os, const Citizen& citizen) // print citizen date into the screen or the fill. Depends on the input.
	{
		citizen.PrintCitizen(false);
		return os;
	}
//-----------------------------------------------------------------------------------------------//
	void Citizen::save(ofstream& outFile) const // This function saves citizen from the state to a given file.
	{
		int lenName = name.size(); 
		int serialNumOfDistrict = district->getID();
		outFile.write(rcastcc(&lenName), sizeof(int)); // save the size of the name of the citizen.
		outFile.write(rcastcc(&name[0]), sizeof(char) * lenName); // save the name of the citizen.
		outFile.write(rcastcc(&ID), sizeof(int)); // save the ID of the citizen.
		outFile.write(rcastcc(&birthYear), sizeof(int)); // save the year of birth of the citizen.
		outFile.write(rcastcc(&isVote), sizeof(bool)); // save citizen's voting status. If he voted, he would save 'true'. Otherwise he will save 'false'.
		outFile.write(rcastcc(&serialNumOfDistrict), sizeof(int)); // save the serial number od the district that the the citizen is belong to.

		if (!outFile.good()) // Checks whether the save was performed successfully.
			throw SaveException();
	}
//-----------------------------------------------------------------------------------------------//
}