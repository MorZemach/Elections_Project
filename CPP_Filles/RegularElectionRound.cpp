#define _CRT_SECURE_NO_WARNINGS
#include "RegularElectionRoundH.h"

namespace ElectionsDay
{
	//-----------------------------------------------------------------------------------------------//
	void RegularElectionRound::updateListCitizen(List<Citizen>* lstCitizen, List<District>& lstDistrict, int Electionyear)
		// Update the List of citizen by adding a new citizen. 
	{
		try
		{
			string nameTemp;
			nameTemp.resize(MAX_NAME_LEN);
			int ID, birthYear, districtSerialNum;
			bool isInputValid = false;

			while (!isInputValid)
			{
				try
				{
					cout << "Please enter the name of the citizen." << endl;
					cin.ignore();
					getline(cin, nameTemp);
					cout << "Please enter the ID, year of birth and his district number of the citizen." << endl;
					cin >> ID >> birthYear >> districtSerialNum;
					checkCitizen(nameTemp, ID, birthYear, Electionyear, districtSerialNum);
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
			if (lstDistrict.findNode(districtSerialNum) == nullptr)
			{
				throw logic_error("This district is not exist at the state.");
			}

			if (lstCitizen->findNode(ID) != nullptr)
			{
				throw logic_error("This citizen is already at the list.");
			}
			
			string name = nameTemp;

			Citizen* citizen = new Citizen(name, ID, birthYear, *(lstDistrict.findNode(districtSerialNum)));

			if ((Electionyear - birthYear) >= 18)  // Checks whether the age of the citizen is over the age of 18 according to the date of the current election round.
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
	//-----------------------------------------------------------------------------------------------//
	void RegularElectionRound::updateListDistrict(List<District>* lst, List<Faction>* lstF)
		// Update the List of district by adding a new district. 
	{
		string nameTemp;
		nameTemp.resize(MAX_NAME_LEN);
		int numDelegates, type;
		bool isInputVaild = false;

		while (!isInputVaild)
		{
			cout << "Please enter first the type of the district, 0 for united district, 1 for divided district." << endl;
			cin >> type;
			if (type == 0 || type == 1)
			{
				isInputVaild = true;
			}
			else
			{
				cout << "Your choice is illegal." << endl;
			}
		}

		isInputVaild = false;

		while (!isInputVaild)
		{
			try
			{
				cout << "Please enter the name of the district and how many delegates belong it." << endl;
				getchar();
				nameTemp.clear();
				nameTemp.resize(MAX_NAME_LEN);
				numDelegates = InitNameAndNumber(nameTemp);
				checkDistrictAndFaction(nameTemp, numDelegates);
				isInputVaild = true;
			}
			catch (bad_array_new_length& ex)
			{
				cout << "Error: District name length must be bigger than 0. Please enter again." << endl;
			}
			catch (logic_error& ex)
			{
				cout << "Error: District " << ex.what() << " Please enter again." << endl;
			}
		}

		string name = nameTemp;
	
		District* district = new District(name, numDelegates, type);
		lst->Insert(*district);

		List<Faction>::Iterator temp = lstF->begin();

		while (temp!=lstF->end())
		{
			List<Faction>::Iterator currFaction = temp;
			List<Delegates>* currDD = currFaction->getList();
			currDD->setSize();
			Delegates newNode(district->getID());
			currDD->Insert(newNode);
			temp++;
		}
	}
	//-----------------------------------------------------------------------------------------------//
	void RegularElectionRound::addingFactionDelegate(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict)
		// Adding new faction delegate. 
	{
		cout << "Please enter ID, serial number of the faction and serial number of the district that the delegate will belong to." << endl;
		int delegateID, factionNum, districtNum;
		cin >> delegateID >> factionNum >> districtNum;
		Citizen* delegate(lstCitizen->findNode(delegateID));
		Faction* factionD(lst->findNode(factionNum));
		District* districtD(lstDistrict->findNode(districtNum));

		if (factionD == nullptr)
		{
			cout << "The faction is not found." << endl;
			return;
		}
		else // The faction is found.
		{
			if (districtD == nullptr)
			{
				cout << "The district is not found." << endl;
				return;
			}
			else // The faction and the district has found.
			{
				if (delegate == nullptr)
				{
					cout << "The delegate is not belong to the state." << endl;
					return;
				}
				else if (delegate->getID() == factionD->getCandidateID())
				{
					cout << "The delegate is already the candidate of this faction." << endl;
					return;
				}

				else // The faction, delegate and the district has found.
				{
					List<Delegates>::Iterator currLstDelegate = factionD->getList()->begin();
					List<Faction>::Iterator currFaction = lst->begin();

					while (currFaction!= lst->end())
					{
						List<Delegates>::Iterator currLstDelegate = currFaction->getList()->begin();
						while (currLstDelegate->getID() != districtD->getID())
						{
							currLstDelegate++;
						}
						if (currLstDelegate->getlst()->findNode(delegateID) != nullptr)
						{
							cout << "This delegate is already a delegate at another faction." << endl;
							return;
						}
						else if (currFaction->getCandidateID() == delegateID)
						{
							cout << "The delegate is already a candidate of other faction." << endl;
							return;
						}
						currFaction++;
					}
					while (currLstDelegate->getID() != districtD->getID())
					{
						currLstDelegate++;
					}
					if (currLstDelegate->getlst()->findNode(delegateID) !=nullptr)
					{
						cout << "This delegate is already at the delegates list." << endl;
						return;
					}
					currLstDelegate->getlst()->Insert(*delegate);

				}
			}
		}
	}
	//-----------------------------------------------------------------------------------------------//
	void RegularElectionRound::updateListFaction(List<Faction>* lst, List<Citizen>* lstCitizen, List<District>* lstDistrict)
		// Update the List of faction by adding a new faction. 
	{
		string nameTemp;
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
			cout << "This candidate is not belong to the state. He can not be Prime Minister." << endl;
			return;
		}

		List<Faction>::Iterator currFaction = lst->begin();

		while (currFaction!=lst->end())
		{
			if (candidate->getID() == currFaction->getCandidateID())
			{
				cout << "This candidate is already a candidate for another faction. He can not be candidate for this faction also." << endl;
				return;
			}
			currFaction++;
		}

		Faction* faction = new Faction(name, *candidate, lstDistrict);
		lst->Insert(*faction);
	}
	//-----------------------------------------------------------------------------------------------//
	Faction* RegularElectionRound::get_Faction_With_Most_Delegates_Or_Votes() const  // Finding ths faction with the most delegates / votes the faction received in fuction list. 
	{
		List<Faction>::Iterator currFaction = lstFaction->begin();
		List<Faction>::Iterator currMaxFaction = nullptr;

		Faction* res = nullptr;
		int maxDelegate = -1;

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
			if (currMaxFaction->getSumDelegate() > maxDelegate && currMaxFaction->getIfAlreadyChecked() == false)
			{
				maxDelegate = currMaxFaction->getSumDelegate();
				res = lstFaction->findNode(currMaxFaction->getID());;
			}
			currMaxFaction++;
		}
		res->setCheck(true);
		return res;
	}
	//-----------------------------------------------------------------------------------------------//
	void RegularElectionRound::printResFaction(Faction* faction) const
		// Print the results for each Faction.
	{
		cout << "The candidate of faction " << "'" << faction->getNameFac() << "'" << " is: ";
		faction->Print_Candidate();
		cout << "The faction has got: " << faction->getSumDelegate() << " delegates, and total "
			<< faction->getSumVotes() << " votes." << endl;
	}
	//-----------------------------------------------------------------------------------------------//
	int RegularElectionRound::Find_Max_At_Array(vector<int>* arr, int& numOfDelegate) const // Finding ths faction with the most delegates the faction received in a given array. 
	{
		int index = 1, curr = 1;
		if (arr != nullptr && lstFaction->getSize() > 1)
		{
			for (vector<int>::iterator itr = arr->begin() + 1; itr!=arr->end(); ++itr, ++curr)
			{
				if ((*itr)!=-1)
				{
					index = curr;
					break;
				}
			}

			for (vector<int>::iterator itr = arr->begin() + index; itr != arr->end(); ++itr, ++curr)
			{
				if (*(arr->begin()+index) < *(itr))
				{
					index = curr;
				}
			}
		}
		numOfDelegate = *(arr->begin() + index);
		*(arr->begin() + index) = -1;
		return index;
	}
	//-----------------------------------------------------------------------------------------------//
	void RegularElectionRound::Print_Candidates_Divided_District(vector<int> arr) const
		// Print the faction candidates in descending order according to the number of delegates they received.
	{

		for (vector<int>::iterator itr = arr.begin() + 1; itr != arr.end(); ++itr)
		{
			int numOfDelegate = 0;
			int index = Find_Max_At_Array(&arr, numOfDelegate);
			if (numOfDelegate > 0)
			{
				Faction* currFaction = lstFaction->findNode(index);
				cout << "The candidate of faction " << "'" << currFaction->getNameFac() << "'" << " is: ";
				currFaction->Print_Candidate();
				cout << "The faction has got: " << numOfDelegate << " delegates." << endl;
			}
		}
	}
	//-----------------------------------------------------------------------------------------------//
	void RegularElectionRound::ResDistrict() const
	{
		// Go through all the state districts and print the results for each of them.

		try
		{
			List<District>::Iterator currDistrict = this->lstDistrict->begin();

			while (currDistrict!= this->lstDistrict->end())
			{
				vector<int> arrayNumOfDelegate;
				arrayNumOfDelegate.resize(lstFaction->getSize() + 1);
				arrayNumOfDelegate[0] = -1;

				cout << *(currDistrict); // prints district detailes. 

				Faction* winFaction = nullptr;
				int typeDistrict = currDistrict->getDistrictType();

				if (typeDistrict == 0)
				{
					winFaction = Winning_Faction_Per_District(currDistrict->getID());
					cout << "The candidate of district " << "'" << currDistrict->getNameDis() << "'" << " is: ";
					winFaction->Print_Candidate();
				}

				if (currDistrict->getNumOfVotes() != 0) // Checking if someone voted at this district. 
				{
					float precentdis = currDistrict->getPrecentVoted();
					cout << fixed << setprecision(4); // print 4 digits after the decimal point.
					cout << "Percent votes at the district is: " << precentdis << "%." << endl;

					List<Faction>::Iterator currFac = lstFaction->begin();
					while (currFac != lstFaction->end())
					{
						int numOfVotesAtDis = currDistrict->getNumOfVotes();
						Delegates* currFacDis = currFac->getList()->findNode(currDistrict->getID());

						cout << fixed << setprecision(4);
						cout << "The faction: " << "'" << currFac->getNameFac() << "'" <<
							" received " << currFacDis->getPrecentPerFaction(numOfVotesAtDis) << "%. " <<
							"Total " << currFacDis->getVoteSum() << " of votes from this district." << endl;
						int numOfDelegatePerFac = currDistrict->howManyDelegateDistrict(currDistrict->getNumDelegate(),
							currDistrict->getNumOfVotes(), currFacDis->getVoteSum());

						if (typeDistrict == 0)
						{
							if (lstFaction->findNode(currFac->getID()) == winFaction)
								currFac->setSumDelegate(currDistrict->getNumDelegate());
						}
						else
						{
							if (currFac->getID() < 0 || currFac->getID() > lstFaction->getSize())
							{
								throw out_of_range("Attempt to access elements out of defined range.");
							}
							else
							{
								arrayNumOfDelegate[currFac->getID()] = numOfDelegatePerFac;
							}
							currFac->setSumDelegate(numOfDelegatePerFac);
						}

						if (numOfDelegatePerFac == 0)
						{
							cout << "The faction has got 0 delegates from this district." << endl;
						}
						else
						{
							currFacDis->printLstDelegate(numOfDelegatePerFac);
						}
						
						currFac++;
					}
				}
				else
				{
					cout << "No one has votes yet at this disrict." << endl;
				}

				if (typeDistrict == 1)
				{
					Print_Candidates_Divided_District(arrayNumOfDelegate);
				}
				currDistrict++;
				cout << endl;
			}
		}
		catch (CalculationException& ex)
		{
			cout << "Error: " << ex.what() << endl;
		}
		catch (bad_alloc& ex) // Memory allocation failed.
		{
			cout << "Error: " << ex.what() << endl;
		}
	}
	//-----------------------------------------------------------------------------------------------//
}