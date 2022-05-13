#define _CRT_SECURE_NO_WARNINGS
#include "DelegatesH.h"

namespace ElectionsDay
{
	//-----------------------------------------------------------------------------------------------//
	void Delegates::setSerialNumDistrict(int serialNumDistrict)
		// Update serial number of the node to be the number the fuction got as input. 
	{
		try
		{
			if (serialNumDistrict > 0)
			{
				this->serialNumDistrict = serialNumDistrict;
			}
			else
			{
				throw logic_error("The serial number of the district must be a positive number.");
			}
		}
		catch (logic_error& ex)
		{
			cout << "Error: " << ex.what() << endl;
		}
	}
	//-----------------------------------------------------------------------------------------------//
	void Delegates::setVoteSum()
		// Update number of citizens vote to this faction from current district. 
	{
		try
		{
			int temp = this->voteSum;
			voteSum++;
			if (temp + 1 != voteSum)
				throw SettingException();
		}
		catch (SettingException& ex)
		{
			cout << "Error: " << ex.what() << " thr number of citizens that vote to this faction from current district." << endl;
		}
	}
//-----------------------------------------------------------------------------------------------//
	void Delegates::printLstDelegate(int numOfDelegate) const
		// print delegates at this faction from this district.
	{
		if (this->getlst()->isEmptyLst()) // Check if the list is empty. 
		{
			cout << "There are no delegate at this faction from this district." << endl;
		}
		else
		{
			cout << "The delegates from this faction are: " << endl;
			if (this->getlst()->getSize() < numOfDelegate)
			{
				numOfDelegate = this->getlst()->getSize();
				cout << "Notice!" << endl << "The faction doesn't have enough delegates for the current district. " <<
					"It recived only " << numOfDelegate << " delegates." << endl;
			}
			List<Citizen>::Iterator currDelegate = this->lstDelegate->begin();
			int j = 1;
			for (int i = 0; i < numOfDelegate; i++)
			{
				cout << j++ << ". Name: " << currDelegate->getNameCit() <<
					" ID: " << currDelegate->getID() <<
					", Year of Birth: " << currDelegate->getBirthYear() << endl;
				currDelegate++;
			}
		}
	}
	//-----------------------------------------------------------------------------------------------//
	float Delegates::getPrecentPerFaction(int numOfVotesAtDis) const
		// Calculate the precent votes the faction got at this current district.
	{
		float res;

		if (numOfVotesAtDis != 0) // Avoid division at zero.
		{
			res = (this->getVoteSum() * 100);
			res /= numOfVotesAtDis;
			return res;
		}
		else
		{
			throw CalculationException();
		}
	}
	//-----------------------------------------------------------------------------------------------//
	ostream& operator<<(ostream& os, const Node<Citizen>& delegate) // Print Operator
	{
		delegate.getData()->PrintCitizen(true);
		return os;
	}
	//-----------------------------------------------------------------------------------------------//
	void Delegates::save(ofstream& outFile) const // This function save the current node to a given file.
	{
		outFile.write(rcastcc(&serialNumDistrict), sizeof(int)); // Save serial number of the district.
		outFile.write(rcastcc(&voteSum), sizeof(int));
		if (!outFile.good())// Check if the number save successfully.
			throw SaveException();
		try
		{
			lstDelegate->save(outFile); // Save the list of the delegate from the current district in the curren faction date
			// & check if the save action successfully.
		}
		catch (SaveException& ex)
		{
			throw;
		}
	}
	//-----------------------------------------------------------------------------------------------//
}