#define _CRT_SECURE_NO_WARNINGS
#include "SimpleElectionRoundH.h"

namespace ElectionsDay
{
//-----------------------------------------------------------------------------------------------//
	SimpleElectionRound::SimpleElectionRound(Date& date, int numOfDelegate) // Simple election Round Constructor.
		:Election(date)
	{
		try
		{
			District* district = new District("State", numOfDelegate);
			lstDistrict->Insert(*district);
		}
		catch (bad_alloc& ex) // Memory allocation failed.
		{
			cout << "Error: " << ex.what() << endl;
		}
	}
//-----------------------------------------------------------------------------------------------//
	void SimpleElectionRound::updateListCitizen(List<Citizen>* lstCitizen, List<District>& lstDistrict, int Electionyear)
		// Update the List of citizen by adding a new citizen. 
	{
		try
		{
			string nameTemp;
			int ID, birthYear;
			bool isInputValid = false;
			while (!isInputValid)
			{
				try
				{
					cout << "Please enter the name of the citizen." << endl;
					cin.ignore();
					getline(cin, nameTemp);
					cout << "Please enter ID and year of birth of the citizen." << endl;
					cin >> ID >> birthYear;
					checkCitizen(nameTemp, ID, birthYear, Electionyear, 1);
					isInputValid = true;
				}
				catch (bad_array_new_length& ex)
				{
					cout << "Error: " << "The length of citizen's name must be a positive number, Which does not exceed the maximum value specified in the program." << endl;
				}
				catch (logic_error& ex)
				{
					cout << "Error: " << ex.what() << endl;
				}
			}

			if (lstCitizen->findNode(ID) != nullptr)
			{
				throw logic_error("This citizen is already at the list.");
			}

			string name = nameTemp;
			
			Citizen* citizen = new Citizen(name, ID, birthYear, *(lstDistrict.begin()));
			if ((Electionyear - birthYear) >= 18) // Checks whether the age of the citizen is over the age of 18 according to the date of the current election round.
			{
				lstCitizen->Insert(*citizen);
				citizen->setNumCanVotesDis(); // update the number of citizens from the current citizen's district that can vote.
			}
			else
			{
				throw logic_error("Citizen is under 18, he can not participate in the election round.");
			}
		}

		catch (bad_alloc& ex) // Memory allocation failed.
		{
			cout << "Error: " << ex.what() << endl;
		}
		catch (SettingException& ex)
		{
			cout << "Error: " << ex.what() << " the number of citizens that can vote at this current citizen's district." << endl;
		}

		system("CLS");
	}
////-----------------------------------------------------------------------------------------------//
	void SimpleElectionRound::updateListDistrict(List<District>* lst, List<Faction>* lstF)
		// Since there is no district at the state (Except for the divided district which is hidden from the eyes of the user), we will print an arror message.
	{
		cout << "Notice - All the citizens belong to the state. There are no districts at all." << endl;
	}
////-----------------------------------------------------------------------------------------------//
	void SimpleElectionRound::addingFactionDelegate(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict)
		// Adding new faction delegate. 
	{
		cout << "Please enter ID and serial number of the faction that the delegate is belong to." << endl;
		int delegateID, factionNum;
		cin >> delegateID >> factionNum;
		Citizen* delegate(lstCitizen->findNode(delegateID));
		Faction* factionD(lst->findNode(factionNum));

		if (factionD == nullptr)
		{
			throw logic_error("The faction is not found.");
		}
		else // The faction is found.
		{
			if (delegate == nullptr)
			{
				throw logic_error("The delegate is not belong to the state.");
			}
			else if (delegate->getID() == factionD->getCandidateID())
			{
				throw logic_error("The delegate is already the candidate of this faction.");
			}
			else
			{
				List<Delegates>::Iterator currLstDelegate = factionD->getList()->begin();
				List<Faction>::Iterator currFaction = lst->begin();

				while (currFaction != lst->end())
				{
					List<Delegates>::Iterator currLstDelegate = currFaction->getList()->begin();

					if (currLstDelegate->getlst()->findNode(delegateID) != nullptr)
					{
						throw logic_error("This delegate is already a delegate at other faction.");
					}
					else if (currFaction->getCandidateID() == delegateID)
					{
						throw logic_error("The delegate is already a candidate of other faction.");
					}
					currFaction++;
				}
				if (currLstDelegate->getlst()->findNode(delegateID) != nullptr)
				{
					throw logic_error("This delegate is already at the delegates list.");
				}
				currLstDelegate->getlst()->Insert(*delegate);
			}
		}
	}
////-----------------------------------------------------------------------------------------------//
	void SimpleElectionRound::updateListFaction(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict)
		// Update the List of faction by adding a new faction. 
	{
		string nameTemp;
		nameTemp.resize(MAX_NAME_LEN);
		int candidateID;
		bool isRightInput = false;

		while (!isRightInput)
		{
			try
			{
				cout << "Please enter the name of the faction and ID of the candidate." << endl;
				getchar();
				nameTemp.clear();
				nameTemp.resize(MAX_NAME_LEN);
				candidateID = InitNameAndNumber(nameTemp);
				checkDistrictAndFaction(nameTemp, candidateID);
				isRightInput = true;
			}
			catch (bad_array_new_length& ex)
			{
				cout << "Error: Faction name length must be bigger than 0. Please enter again." << endl;
			}
			catch (logic_error& ex)
			{
				cout << "Error: Faction " << ex.what() << " Please enter again." << endl;
			}
		}

		string name = nameTemp;
		Citizen* candidate(lstCitizen->findNode(candidateID));

		if (candidate == nullptr)
		{
			throw logic_error("This candidate is not belong to the state. He can not be Prime Minister.");
		}

		List<Faction>::Iterator currFaction = lst->begin();

		while (currFaction != lst->end())
		{
			if (candidate->getID() == currFaction->getCandidateID())
			{
				throw logic_error("This candidate is already a candidate for another faction. He can not be candidate for this faction also.");
			}
			currFaction++;
		}

		Faction* faction = new Faction(name, *candidate, lstDistrict);
		lst->Insert(*faction);
	}
////-----------------------------------------------------------------------------------------------//
	Faction* SimpleElectionRound::get_Faction_With_Most_Delegates_Or_Votes() const
		// Finding ths faction with the most delegates / votes the faction received in fuction list. 
	{
		List<Faction>::Iterator currFaction = lstFaction->begin();

		List<Faction>::Iterator currMaxFaction = nullptr;

		Faction* res = nullptr;
		int maxVotes = -1;

		while (currFaction != lstFaction->end())
		{
			if (currFaction->getIfAlreadyChecked() == false)
			{
				currMaxFaction = currFaction;
				break;
			}
			currFaction++;
		}

		while (currMaxFaction != lstFaction->end())
		{
			if (currMaxFaction->getSumVotes() > maxVotes && currMaxFaction->getIfAlreadyChecked() == false)
			{
				maxVotes = currMaxFaction->getSumVotes();
				res = lstFaction->findNode(currMaxFaction->getID());
			}
			currMaxFaction++;
		}
		res->setCheck(true);
		return res;
	}
////-----------------------------------------------------------------------------------------------//
	void SimpleElectionRound::printResFaction(Faction* faction) const // print the results of the election day per faction. 
	{
		int numOfVotesAtDis = this->lstDistrict->begin()->getNumOfVotes();

		cout << "The candidate of faction " << "'" << faction->getNameFac() << "'" << " is: ";
		faction->Print_Candidate();

		cout << fixed << setprecision(4);
		cout << "The faction has got: " << faction->getSumDelegate() << " delegates, vote precent: "
			<< faction->getList()->begin()->getPrecentPerFaction(numOfVotesAtDis) << "%, and total " << faction->getSumVotes() << " votes." << endl;

		faction->getList()->begin()->printLstDelegate(faction->getSumDelegate());
	}
////-----------------------------------------------------------------------------------------------//
	void SimpleElectionRound::ResDistrict() const // print the results of the election day at the state. 
	{
		try
		{
			List<District>::Iterator currDistrict = this->lstDistrict->begin();

			cout << *(currDistrict); // prints district detailes.

			if (currDistrict->getNumOfVotes() != 0) // Checking if someone voted at this district. 
			{
				float precentdis = currDistrict->getPrecentVoted();
				cout << fixed << setprecision(4); // print 4 digits after the decimal point.
				cout << "Percent votes at the district is: " << precentdis << "%." << endl;

				List<Faction>::Iterator currFac = lstFaction->begin();

				while (currFac != lstFaction->end())
				{ 
					List<Delegates>::Iterator currFacDis = currFac->getList()->begin();

					int numOfDelegatePerFac = currDistrict->howManyDelegateDistrict(currDistrict->getNumDelegate(),
						currDistrict->getNumOfVotes(), currFacDis->getVoteSum());

					currFac->setSumDelegate(numOfDelegatePerFac);

					currFac++;
				}
			}
			else
			{
				cout << "No one has votes yet at this disrict." << endl;
			}
		}
		catch (CalculationException& ex)
		{
			cout << "Errow: " << ex.what() << endl;
		}
		catch (bad_alloc& ex) // Memory allocation failed.
		{
			cout << "Error: " << ex.what() << endl;
		}
	}
//-----------------------------------------------------------------------------------------------//
}