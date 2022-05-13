#define _CRT_SECURE_NO_WARNINGS
#include "FactionH.h"
#include "DelegatesH.h"

namespace ElectionsDay
{
//-----------------------------------------------------------------------------------------------//
	int Faction::counterFac = 0;

	Faction::Faction(const string name, Citizen& candidate, List<District>* lstD) // Faction Constructor
	{
		try
		{
			this->name = name;
			this->candidate = &candidate;
			this->serialNum = counterFac + 1;
			counterFac++;

			lst = new List<Delegates>();

			if (lstD->isEmptyLst()) // check if the list of ths district is empty. 
			{
				List<Delegates>(); // Default Constructor.
			}
			else
			{
				List<District>::Iterator currDistrict = lstD->begin();

				while (currDistrict != lstD->end()) // running over list district
				{
					Delegates* currNode = new Delegates(currDistrict->getID());
					lst->Insert(*currNode);// Insert the current district from the districts list into the current list. 
					currDistrict++;
				}
			}
		}
		catch (bad_alloc& ex) // Memory allocation failed.
		{
			cout << "Error: " << ex.what() << endl;
		}
	}
//-----------------------------------------------------------------------------------------------//
	Faction::Faction(ifstream& inFile, List<Citizen>* lstOfCitizen, List<District>* lstOfDistrict) // load the current Faction from a given fill. 
	{
		int lenName, IDofCandidate, sizeOfList, currDistrictSerialNum, voteSum;
		inFile.read(rcastc(&lenName), sizeof(int));
		name.resize(lenName + 1);
		inFile.read(&name[0], sizeof(char) * lenName); // faction name
		name[lenName] = '\0';
		inFile.read(rcastc(&counterFac), sizeof(int)); // static counter (for serial number)
		inFile.read(rcastc(&serialNum), sizeof(int)); // serial number of faction
		inFile.read(rcastc(&IDofCandidate), sizeof(int)); // ID of candidate - it will find it after the load
		inFile.read(rcastc(&sumDelegate), sizeof(int)); // how many delegates the faction got
		inFile.read(rcastc(&sumVotes), sizeof(int)); // how many votes the faction got
		candidate = lstOfCitizen->findNode(IDofCandidate); // find the candidate of the faction.
		inFile.read(rcastc(&sizeOfList), sizeof(int)); // load the size of the current list.

		lst = new List<Delegates>();
		List<Citizen>* currDelegateList = nullptr;

		for (int i = 0; i < sizeOfList; i++)
		{
			inFile.read(rcastc(&currDistrictSerialNum), sizeof(int)); 
			inFile.read(rcastc(&voteSum), sizeof(int)); 
			currDelegateList = new List<Citizen>(inFile, lstOfDistrict); 
			Delegates* newNode = new Delegates(currDistrictSerialNum, voteSum, currDelegateList);
			lst->Insert(*newNode); 
		}
	}
//-----------------------------------------------------------------------------------------------//
	Faction::~Faction()  // Faction Destructor.
	{
		if (lst != nullptr)
		{
			delete lst;
		}
	}
//-----------------------------------------------------------------------------------------------//
	const Faction& Faction::operator=(const Faction& other) // Placement opertor
	{
		if (this != &other) // Avoid self-placement
		{
			try
			{
				this->name = other.name;
				this->serialNum = other.serialNum;
				this->candidate = other.candidate;
				this->checked = other.checked;
				this->sumDelegate = other.sumDelegate;
				this->sumVotes = other.sumVotes;
				this->lst = other.lst;
				this->counterFac = other.counterFac;
			}
			catch (bad_alloc& ex) // Memory allocation failed.
			{
				cout << "Error: " << ex.what() << endl;
			}
		}
		return *this;
	}
//-----------------------------------------------------------------------------------------------//
	void Faction::PrintFactionPerDistrict() const
		 // Over the list of delegates in each district at distirict list, and print how many delegate faction has from this district 
		 // and the delegates details.
	{
		List<Delegates>::Iterator currLstDelegate = lst->begin();
		int j;
		while (currLstDelegate!= lst->end())
		{
			j = 1;
			if (currLstDelegate->getlst()->getSize() != 0)
			{
				cout << "The faction has " << currLstDelegate->getlst()->getSize() << " delegates:" << endl;
				for(List<Citizen>::Iterator itr = currLstDelegate->getlst()->begin()
					; itr != currLstDelegate->getlst()->end(); ++itr)
				{
					cout << j++ << ". Name: " << itr->getNameCit() << " ID: " << itr->getID() << 
						", BirthYear: " << itr->getBirthYear() << endl;
				}
			}
			else
			{
				cout << "The faction dose not have a delegates in district: ." << currLstDelegate->getID() << endl;
			}
			currLstDelegate++;
		}
	}
//-----------------------------------------------------------------------------------------------//
	void Faction::PrintFactionDitails() const  // print faction details.
	{
		cout << "Serial Number: " << serialNum
			<< ", Name: " << name << endl;
		cout << "Candidate's details: " << endl;
		Print_Candidate();
		PrintFactionPerDistrict();
	}
//-----------------------------------------------------------------------------------------------//
	void Faction::Print_Candidate() const // print candidate details.
	{
		cout << "Name: " << candidate->getNameCit() << ", ID: " << candidate->getID() << ", BirthYear: "
			<< candidate->getBirthYear() << ", District: " << candidate->getDistrictNum() << endl;
	}
//-----------------------------------------------------------------------------------------------//
	int Faction::getTotalVotesForFaction() const
		// Calculation of the number of votes the current faction received in the election day. 
	{
		int sumVotes = 0;
		List<Delegates>::Iterator currLstDelegate = lst->begin();

		while (currLstDelegate!=lst->end())
		{
			sumVotes += currLstDelegate->getVoteSum();
			currLstDelegate++;
		}
		return sumVotes;
	}
	
//-----------------------------------------------------------------------------------------------//
	void Faction::setSumVotes() // Update the number of citizen who voted to this list. 
	{
		int temp = sumVotes;
		sumVotes++;
		if (sumVotes != temp + 1)
		{
			throw SettingException();
		}
	}
//-----------------------------------------------------------------------------------------------//
	ostream& operator<<(ostream& os, const Faction& faction) // Print Operator. 
	{
		faction.PrintFactionDitails();
		return os;
	}
//-----------------------------------------------------------------------------------------------//
	void Faction::save(ofstream& outFile) const // This function saves the faction current faction to a given file.
	{
		int lenName = name.size();
		int IDofCandidate = candidate->getID();
		outFile.write(rcastcc(&lenName), sizeof(int)); // Save faction name length.
		outFile.write(rcastcc(&name[0]), sizeof(char) * lenName); // Save faction name. 
		outFile.write(rcastcc(&counterFac), sizeof(int));
		outFile.write(rcastcc(&serialNum), sizeof(int)); // Save the serial number of the faction. 
		outFile.write(rcastcc(&IDofCandidate), sizeof(int)); // Save the ID of faction candidate.
		outFile.write(rcastcc(&sumDelegate), sizeof(int)); // Save how many delegates the faction got.
		outFile.write(rcastcc(&sumVotes), sizeof(int)); // Save how many votes the faction got.

		if (!outFile.good())
		{
			throw SaveException();
		}

		try
		{
			lst->save(outFile); // Save & check list of district + the lists of delegates in each district at the list.
		}
		catch (SaveException& ex)
		{
			throw;
		}
	}
//-----------------------------------------------------------------------------------------------//
}