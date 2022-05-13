#define _CRT_SECURE_NO_WARNINGS
#include "ElectionH.h"

namespace ElectionsDay
{
	//-----------------------------------------------------------------------------------------------//
	Election::Election(ifstream& inFile, Date& date) // load the current election from a given fill.
	{
		electionDate = &date;
		lstDistrict = new List<District>(inFile);
		lstCitizen = new List<Citizen>(inFile, lstDistrict);
		lstFaction = new List<Faction>(inFile, lstCitizen, lstDistrict);
	}
	//-----------------------------------------------------------------------------------------------//
	Election::~Election() // Election Destructor.
	{
		if (lstDistrict != nullptr)
		{
			delete lstDistrict;
		}
		if (lstFaction != nullptr)
		{
			delete lstFaction;
		}
		if (lstCitizen != nullptr)
		{
			delete lstCitizen;
		}
	}
//-----------------------------------------------------------------------------------------------//
	Faction* Election::Winning_Faction_Per_District(int serialDisNum) const // Finding the wining faction for each district at the state.
	{
		List<Faction>::Iterator currFaction = lstFaction->begin();
		Delegates* currDisPerFac = currFaction->getList()->findNode(serialDisNum);

		List<Faction>::Iterator winFaction;

		int maxVote;
	
		if (currDisPerFac != nullptr)
		{
			maxVote = currDisPerFac->getVoteSum();
			winFaction = currFaction;
		}
		else
		{
			return nullptr;
		}

		while (currFaction!= lstFaction->end())
		{
			currDisPerFac = currFaction->getList()->findNode(serialDisNum);
			if (currDisPerFac->getVoteSum() > maxVote)
			{
				maxVote = currDisPerFac->getVoteSum();
				winFaction = currFaction;
			}
			currFaction++;
		}

		return lstFaction->findNode(winFaction->getID());
	}
//-----------------------------------------------------------------------------------------------//
	void Election::ResElection(bool& resElectionDisplayed) const // Prints election results. 
	{
		List<Citizen>::Iterator currCitizen = lstCitizen->begin();
		bool someoneVoted = false;

		while (!someoneVoted && currCitizen!=nullptr)
		{
			if (currCitizen->getIsVote() == true)
			{
				someoneVoted = true;
				resElectionDisplayed = true;
			}
			currCitizen++;
		}

		if (someoneVoted == false)
		{
			cout << "No one has voted yet." << endl;
			resElectionDisplayed = false;
			return;
		}

		ResDistrict();
		List<Faction>::Iterator curr = lstFaction->begin();

		while (curr!= lstFaction->end())
		{
			printResFaction(get_Faction_With_Most_Delegates_Or_Votes());
			curr++;
		}
	}
//-----------------------------------------------------------------------------------------------//
	void Election::checkDistrictAndFaction(string name, int num)
			// check if the length of the faction name & the length of the district name is bigger than 0 and the number the fuction got is an positive number.
		// if so, it is an proper input.
	{
		if (name.length() <=0 || name.length() >= MAX_NAME_LEN)
		{
			throw bad_array_new_length();
		}
		if (num <= 0)
		{
			throw logic_error("serial number must be a positive number.");
		}
	}
//-----------------------------------------------------------------------------------------------//
	void Election::checkCitizen(string name, int ID, int birthYear, int year, int serialNumDistrict)
			// check if the Id of the citizen has 9 digit, his birth year id a positive number and the length of the name if bigger than 0.
			 // if so, it is an proper input.
	{
		int count = 0;
		int temp = ID;

		while (temp > 0)
		{
			temp /= 10;
			count++;
		}

		if (count != 9)
		{
			throw logic_error("Citizen's ID does not consist of 9 digits.");
		}

		if (birthYear > year)
		{
			throw logic_error("Citizen's year of birth can not be bigger than the year of the current election round.");
		}
		
		if (birthYear <= 0)
		{
			throw logic_error("Citizen's year of birth must be a positive number.");
		}
		if (serialNumDistrict <= 0)
		{
			throw logic_error("The serial number of the district the citizen is belong to must be a positive number.");
		}

		if (name.size() <= 0 || name.size() >= MAX_NAME_LEN)
		{
			throw bad_array_new_length();
		}
	}
//			
//-----------------------------------------------------------------------------------------------//
	void Election::isDigit(char ch) // Check if the given char is a digit. 
	{
		if (ch < '0' || ch > '9')
		{
			throw "The given char is not a digit.";
		}
	}
//-----------------------------------------------------------------------------------------------//
	int Election::InitNameAndNumber(string& str)
	{
		string name;
		name.resize(MAX_NAME_LEN);
		getline(cin, name);
		int len = name.size() - 1;
		int num = 0, count = 1, i = 0;
		string& temp = str;	
		char ch = name.at(0);
		bool isADigit = false; 

		while (ch != '\n' && isADigit == false && i < len)
		{
			try
			{
				isDigit(ch);
				isADigit = true;
			}
			catch (const char* msg)
			{
				temp[i] = ch;
				ch = name.at(i + 1);
				i++;
			}
		}
		try
		{
			isDigit(ch);
			if (i > len)
			{
				throw out_of_range("Attempt to access elements out of defined range.");
			}
			temp[i] = '\0';
			temp.resize(i);
			int firstDigit = ch - '0';
			if (i < len)
			{
				for (int k = len; k > i; k--)
				{
					num += (name.at(k) - '0') * count;
					count *= 10;
				}
				num += (firstDigit * count);
			}
			else
				num = firstDigit;

			if (i > 2)
			{
				if (temp[i - 1] == '-')
				{
					temp[i - 2] = '\0';
					return (-1 * num);
				}
				else
					temp[i - 1] = '\0';
			}

			return num;
		}
		catch (const char* msg)
		{
			if (i == len)
			{
				temp[i] = ch;
				temp[i + 1] = '\0';
				temp.resize(i+1);
				cin >> num;
				getchar(); // "eat" the enter.
				return num;
			}
		}
	}

//-----------------------------------------------------------------------------------------------//
	void Election::save(ofstream& outFile, int roundType) const // Save the election round in to a given file.
	{
		try // check if the save action successfully.
		{
			outFile.write(rcastcc(&roundType), sizeof(int)); // save election round type.
			electionDate->save(outFile); // Save the election date & check if the save action successfully.
			lstDistrict->save(outFile); // Save the list of the districts at the state & check if the save action successfully.
			lstCitizen->save(outFile); // Save the list of the citizen that can vote (over 18) at the state & check if the save action successfully.
			lstFaction->save(outFile); // Save the list of the facion at the state & check if the save action successfully.
		}
		catch (SaveException& ex)
		{
			throw;
		}
	}
	//-----------------------------------------------------------------------------------------------//
}