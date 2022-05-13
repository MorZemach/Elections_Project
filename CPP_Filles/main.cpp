#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:6011)
#include "RegularElectionRoundH.h"
#include "SimpleElectionRoundH.h"
using namespace ElectionsDay;
using namespace std;

enum class ElectionMainMenu
{
	AddDistrict = 1, AddCitizen, AddFaction, AddDelegate, PrintDistrict, PrintCitizen, PrintFaction, Vote, PrintElectionRes,
	SaveElectionRound, LoadElectionRound, Exit
};

enum class ElectionRound
{
	REGULAR_ROUND = 1, SIMPLE_ROUND
};

enum class ElectionInitMenu
{
	Creat_New_Election_Round = 1, Load_Existing_Election_Round, Exit
};

void myTerminate()
{
	cout << "Error occurred." << endl;
	exit(1);
}

void is_empty_file(ifstream& pFile) // Check if the fill we want to load from is empty. 
{
	if (pFile.peek() == ifstream::traits_type::eof())
	{
		throw logic_error("The file is empty.");
	} 
}

void creatNewElectionRound(Date date, ElectionRound E_round);

using namespace ElectionsDay;

void ElectionDayMenu(Election* election) // Main Menu 
{
	int input;
	ElectionMainMenu option;
	bool resElectionDisplayed = false;

	while (true)
	{
		cout << "1. Adding a district" << endl;
		cout << "2. Adding a citizen" << endl;
		cout << "3. Adding a faction" << endl;
		cout << "4. Adding a citizen as a faction delegate" << endl;
		cout << "5. Presentation of all the districts" << endl;
		cout << "6. Presentation of all the citizens" << endl;
		cout << "7. Presentation of all the factions" << endl;
		cout << "8. vote" << endl;
		cout << "9. Presentation of the election results" << endl;
		cout << "10. Save Election Round" << endl;
		cout << "11. Load Election Round" << endl;
		cout << "12. Exit" << endl;
		cin >> input;

		option = static_cast<ElectionMainMenu>(input);

		switch (option)
		{
		case ElectionMainMenu::AddDistrict:
		{
			system("CLS");
			election->updateListDistrict(election->getListDistrict(), election->getListFaction());
			break;
		}
		case ElectionMainMenu::AddCitizen:
		{
			system("CLS");
			try
			{
				election->updateListCitizen(election->getListCitizen(), *(election->getListDistrict()), election->getDate()->getYear());
			}
			catch (logic_error& ex)
			{
				cout << "Error: " << ex.what() << endl << endl;
			}
			break;
		}
		case ElectionMainMenu::AddFaction:
		{
			system("CLS");
			try
			{
				election->updateListFaction(election->getListFaction(), election->getListCitizen(), election->getListDistrict());
			}
			catch (logic_error& ex)
			{
				cout << "Error: " << ex.what() << endl;
			}
			break;
		}
		case ElectionMainMenu::AddDelegate:
		{
			system("CLS");
			try
			{
				election->addingFactionDelegate(election->getListFaction(), election->getListCitizen(), election->getListDistrict());
				cout << endl;
			}
			catch (logic_error& ex)
			{
				cout << "Error: " << ex.what() << endl;
			}
			break;
		}
		case ElectionMainMenu::PrintDistrict:
		{
			system("CLS");
			election->getListDistrict()->PrintListData("District");
			break;
		}
		case ElectionMainMenu::PrintCitizen:
		{
			system("CLS");
			election->getListCitizen()->PrintListData("Citizen");
			break;
		}
		case ElectionMainMenu::PrintFaction:
		{
			system("CLS");
			election->getListFaction()->PrintListData("Faction");
			break;
		}
		case ElectionMainMenu::Vote:
		{
			if (election->getListFaction()->isEmptyLst())
			{
				cout << "There is no faction to vote for." << endl;
			}
			else
			{
				cout << "Please enter ID and the serial number of the faction that you choose to vote." << endl;
				int ID, serialNumber;
				cin >> ID >> serialNumber;

				Faction* currFaction = election->getListFaction()->findNode(serialNumber);
				Citizen* currCitizen = election->getListCitizen()->findNode(ID);

				try
				{
					if (currFaction == nullptr)
					{
						throw logic_error("The faction is not exist.");
					}
					if (currCitizen == nullptr)
					{
						throw logic_error("The citizen isn't allowed to vote.");
					}
					else
					{
						if (currCitizen->getIsVote() == true)
						{
							throw logic_error("The citizen is already voted.");
						}
						else
						{
							if (currCitizen->getIsVote() == true)
							{
								throw logic_error("The citizen is already voted.");
							}
							else
							{
								try
								{
									currFaction->getList()->findNode(currCitizen->getDistrictNum())->setVoteSum();
									currCitizen->setNumVotesDistrict();
									currFaction->setSumVotes();
									currCitizen->setIsVote();
								}
								catch (SettingException& ex)
								{
									cout << "Error: " << ex.what() << " citizen's vote." << endl;
								}
							}
						}
					}
				}
				catch (logic_error& ex)
				{
					cout << "Error: " << ex.what() << endl;
				}
			}
			break;
		}
		case ElectionMainMenu::PrintElectionRes:
		{
			system("CLS");
			if (resElectionDisplayed == false)
			{
				cout << *(election->getDate());
				election->ResElection(resElectionDisplayed);
				cout << endl;
				cout << "Thank you for your participation." << endl;
				cout << endl;
			}
			else
			{
				cout << "The election round has been restarted according to your choice." << endl << endl;

				if (typeid(*election) == typeid(RegularElectionRound))
				{
					ElectionRound E_round = static_cast<ElectionRound>(1);
					creatNewElectionRound(*(election->getDate()), E_round);
				}
				else
				{
					ElectionRound E_round = static_cast<ElectionRound>(2);
					creatNewElectionRound(*(election->getDate()), E_round);
				}
				return;
			}
			break;
		}
		case ElectionMainMenu::SaveElectionRound:
		{
			system("CLS");

			cout << "Please enter the name of the file that you want to save to: " << endl;
			string fileName;
			cin >> fileName;

			ofstream outFile(fileName, ios::binary | ios::trunc);

			if (!outFile)
			{
				cout << "The file is not open propertly." << endl;
				exit(-1);
			}

			try
			{
				if (typeid(*election) == typeid(RegularElectionRound))
					election->save(outFile, 1);

				else // simple election round
					election->save(outFile, 2);
			}
			catch (SaveException& ex)
			{
				cout << "Error: " << ex.what() << endl;
				break;
			}

			cout << "The file saved successfully." << endl << endl;

			outFile.close();
			break;
		}
		case ElectionMainMenu::LoadElectionRound:
		{
			system("CLS");

			cout << "Please enter the name of the file that you want to load from: " << endl;
			string nameFile;
			cin.ignore();
			getline(cin, nameFile);

			ifstream inFile(nameFile, ios::binary);

			try
			{
				is_empty_file(inFile);
				int type;
				inFile.read(rcastc(&type), sizeof(int));
				ElectionRound E_round = static_cast<ElectionRound>(type);

				Date* date = new Date(inFile);

				switch (E_round)
				{
				case ElectionRound::REGULAR_ROUND:
				{
					if (election != nullptr)
					{
						RegularElectionRound R_ElectionRound(inFile, *date);
						cout << "The file loaded successfully." << endl << endl;
						ElectionDayMenu(&R_ElectionRound);
						return;
					}
					break;
				}
				case ElectionRound::SIMPLE_ROUND:
				{
					if (election != nullptr)
					{
						SimpleElectionRound S_ElectionRound(inFile, *date);
						cout << "The file loaded successfully." << endl << endl;
						ElectionDayMenu(&S_ElectionRound);
						return;
					}
					break;
				}
				default:
					cout << "The save type (simple or regular election round) at the file is illegal," <<
						" hence it doesn't load the file." << endl;
					return;
				}
				inFile.close();
			}
			catch (logic_error& ex)
			{
				cout << "Error: " << ex.what() << endl;
			}
			break;
		}
		case ElectionMainMenu::Exit:
		{
			system("CLS");
			return;
		}
		default:
			system("CLS");
			cout << "Error: invalid option" << endl << endl;
			break;
		}
	}
}

void creatNewElectionRound(Date date, ElectionRound E_round) // Create a new election round.
{
	bool expectedInput = false;
	int choice;

	while (expectedInput == false)
	{
		switch (E_round)
		{
		case ElectionRound::REGULAR_ROUND:
		{
			RegularElectionRound R_ElectionRound(date);
			ElectionDayMenu(&R_ElectionRound);
			expectedInput = true;
			break;
		}
		case ElectionRound::SIMPLE_ROUND:
		{
			int numOfDelegate;
			bool flag = false;
			while (!flag)
			{
				cout << "Please enter the number of delegates at the state:" << endl;
				cin >> numOfDelegate;
				if (numOfDelegate > 0)
					flag = true;
				else
					cout << "Number of delegates is illegal, it has to be a positive number." << endl;
			}
			SimpleElectionRound S_ElectionRound(date, numOfDelegate);
			ElectionDayMenu(&S_ElectionRound);
			expectedInput = true;
			break;
		}
		default:
			cout << "Your choice isn't match. Please try again." << endl;
			break;
		}
	}
}


void Load_Election_Round(ifstream& inFile) // load election round from a given fill.
{
	int type;
	inFile.read(rcastc(&type), sizeof(int));

	ElectionRound E_round = static_cast<ElectionRound>(type);
	Date* date = new Date(inFile);

	switch (E_round)
	{
	case ElectionRound::REGULAR_ROUND:
	{
		RegularElectionRound R_ElectionRound(inFile, *date);
		cout << "The file loaded successfully." << endl << endl;
		ElectionDayMenu(&R_ElectionRound);
		break;
	}
	case ElectionRound::SIMPLE_ROUND:
	{
		SimpleElectionRound S_ElectionRound(inFile, *date);
		cout << "The file loaded successfully." << endl << endl;
		ElectionDayMenu(&S_ElectionRound);
		break;
	}
	default:
		cout << "The save type (simple or regular election round) at the file is illegal," <<
			" hence it doesn't load the file." << endl;
		return;
	}
}

void InitElectionMenu()
{
	int choice;
	ElectionInitMenu option;

	cout << "Welcome to the Election Day! Please follow the instructions." << endl << endl;
	cout << "First, Please choose one of the following option below:" << endl;
	cout << "1. Create new election round" << endl;
	cout << "2. Lode an existing election round" << endl;
	cout << "3. Exit" << endl;

	cin >> choice;
	option = static_cast<ElectionInitMenu>(choice);

	while (true)
	{
		switch (option)
		{
		case ElectionInitMenu::Creat_New_Election_Round:
		{

			bool expectedInput = false;
			int day, month, year;
			Date electionDate;

			while (!expectedInput)
			{
				try
				{
					cout << "Please enter the date of the election. Enter 3 numbers: day, month and year without any symbols." << endl;
					cin >> day;
					cin >> month;
					cin >> year;
					electionDate.setMonth(month);
					electionDate.setDay(day);
					electionDate.setYear(year);
					expectedInput = true;
				}
				catch (logic_error& ex)
				{
					cout << "Error: " << ex.what() << endl;
				}
				catch (out_of_range& ex)
				{
					cout << "Error: " << ex.what() << endl;
				}
			}

			expectedInput = false;

			while (!expectedInput)
			{
				try
				{
					cout << "Enter 1 for regular election round." << endl;
					cout << "Enter 2 for simple election round." << endl;

					cin >> choice;
					if (choice == 1 || choice == 2)
					{
						expectedInput = true;
					}
					else
					{
						throw logic_error("Your choice must be the number 1 or 2.");
					}
				}
				catch (logic_error& ex)
				{
					cout << "Error: " << ex.what() << endl;
				}
			}

			ElectionRound E_round = static_cast<ElectionRound>(choice);

			creatNewElectionRound(electionDate, E_round);
			return;
		}
		case ElectionInitMenu::Load_Existing_Election_Round:
		{
			cout << "Please enter the name of the file that you want to load from: " << endl;
			string nameFile;
			cin.ignore();
			getline(cin, nameFile);

			ifstream inFile(nameFile, ios::binary);
			try
			{
				is_empty_file(inFile);
				Load_Election_Round(inFile);
			}
			catch (logic_error& ex)
			{
				cout << "Error: " << ex.what() << endl;
			}

			inFile.close();
			return;
		}
		case ElectionInitMenu::Exit:
			return;
		default:
			cout << "Error: invalid option." << endl << endl;
			break;
		}
	}
}


void main()
{
	set_terminate(myTerminate);
	InitElectionMenu(); // The initial menu displayed to the user.
	cout << "Goodbye..." << endl; // The user chooses to exit the program.
}