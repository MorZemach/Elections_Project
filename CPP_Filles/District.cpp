#define _CRT_SECURE_NO_WARNINGS
#include "DistrictH.h"

namespace ElectionsDay
{
//-----------------------------------------------------------------------------------------------//
	int District::counter = 0;

	District::District(const string name, int numDelegate, int _type) // District Constructor
		:numCanVotes(0), numVotes(0), numDelegate(numDelegate), type(_type)
	{
		try
		{
			this->name = name;
			this->serialNum = counter + 1;
			counter++; // Update the static counter for the next serial number.
		}
		catch (bad_alloc& ex) // Memory allocation failed.
		{
			cout << "Error: " << ex.what() << endl;
		}
	}
//-----------------------------------------------------------------------------------------------//
	District::District() // Default Constructor
	{
		this->numDelegate = 0;
		this->serialNum = 0;
		this->numVotes = 0;
		this->numCanVotes = 0;
	}
//-----------------------------------------------------------------------------------------------//
	District::District(District& other) // Copy Constructor
	{
		try
		{
			this->name = other.name;
			this->numVotes = other.numVotes;
			this->serialNum = other.serialNum;
			this->numDelegate = other.numDelegate;
			this->numCanVotes = other.numCanVotes;
		}
		catch (bad_alloc& ex) // Memory allocation failed.
		{
			cout << "Error: " << ex.what() << endl;
		}
	}
//-----------------------------------------------------------------------------------------------//
	const District& District::operator=(const District& other)  // Placement opertor
	{
		if (this != &other)  // Avoid self-placement
		{
			try
			{
				this->name = other.name;
				this->serialNum = other.serialNum;
				this->numDelegate = other.numDelegate;
				this->numVotes = other.numVotes;
				this->numCanVotes = other.numCanVotes;
			}
			catch (bad_alloc& ex) // Memory allocation failed.
			{
				cout << "Error: " << ex.what() << endl;
			}
		}
		return *this;
	}
//-----------------------------------------------------------------------------------------------//
	District::District(ifstream& inFile) // load the current district into a given fill.
	{
		int lenName;
		inFile.read(rcastc(&lenName), sizeof(int));
		if (!inFile.good())
			LoadException();
		name.resize(lenName + 1);
		inFile.read(rcastc(&name[0]), sizeof(char) * lenName);
		name[lenName] = '\0';
		inFile.read(rcastc(&counter), sizeof(int));
		inFile.read(rcastc(&serialNum), sizeof(int));  // load serial number.
		inFile.read(rcastc(&numDelegate), sizeof(int)); // load number of delegate.
		inFile.read(rcastc(&numCanVotes), sizeof(int)); // load number of citizen that can votes.
		inFile.read(rcastc(&numVotes), sizeof(int)); // load number of citizen that already voted.
		inFile.read(rcastc(&type), sizeof(int));  // load type - united or divided.
		if (!inFile.good())
			LoadException();
	}
//-----------------------------------------------------------------------------------------------//
	void District::setNumVotes() // update the number of people that *voted* at the district.  
	{
		int temp = numVotes;
		numVotes++;
		if (temp + 1 != numVotes)
		{
			throw SettingException(); // check if update is Succeeded.
		}
	}
//-----------------------------------------------------------------------------------------------//
	void District::setCanVotes() // update the number of people that can vote at the district.  
	{
		int temp = numCanVotes;
		numCanVotes++;
		if (temp + 1 != numCanVotes) // check if update is Succeeded.
		{
			throw SettingException();
		}
	}
	//-----------------------------------------------------------------------------------------------//
	float District::getPrecentVoted() const // Calculation of the percentage of votes in the district.
	{
		if (numCanVotes != 0)  // Avoid division at zero.
		{
			return (numVotes * 100) / numCanVotes;
		}
		else
		{
			throw CalculationException();
		}
	}
	//-----------------------------------------------------------------------------------------------//
	void District::PrintDistrict() const // print the current district. 
	{
		/////////////////////OUTPUT/////////////////////////
		//Serial number: ______, Name _______.
		//Gives _______ deligates.
		/////////////////////OUTPUT/////////////////////////

		cout << "The type of the district is: ";
		if (type == 0)
		{
			cout << "United." << endl;
		}
		else if (type == 1)
		{
			cout << "Divided." << endl;
		}
		cout << "Serial number: " << serialNum <<
			", Name: " << name << "." << endl << "Gives " << numDelegate << " deligates." << endl << endl;
	}
	//-----------------------------------------------------------------------------------------------//
	int District::howManyDelegateDistrict(int numOfDelegate, int numVoted, int numVotPerFac) const
		// Calculate the number of representatives that the district will give to a given party, 
		// based on the number of people who voted in the district,
		// the number of people who voted for that party from the relevant district
		// and the number of representatives of the district.
	{
		float res=0, decpart;
		int intpart;
		if (numVoted != 0) // Avoid division at zero.
		{
			res = (numVotPerFac * numOfDelegate);
			res /= numVoted;
			intpart = static_cast<int>(res);
			decpart = res - intpart;
			if (res >= 1 && decpart == 0.5)
			{
				return res + 1;
			}
			else if (res < 1 && decpart == 0.5)
			{
				return res;
			}

			if (decpart > 0.5)
			{
				return res + 1;
			}
			else
			{
				return res;
			}
		}
		else
		{
			throw CalculationException();
		}
	}
	//-----------------------------------------------------------------------------------------------//
	ostream& operator<<(ostream& os, const District& district) // Print Operator.  
	{
		district.PrintDistrict();
		return os;
	}
	//-----------------------------------------------------------------------------------------------//
	void District::save(ofstream& outFile) const // This function saves a district to a given file.
	{
		int lenName = name.size();
		outFile.write(rcastcc(&lenName), sizeof(int));// save the size of the name of the district, to the given file.
		outFile.write(rcastcc(&name[0]), sizeof(char)* lenName); // save the name of the district, to the given file.
		outFile.write(rcastcc(&counter), sizeof(int));
		outFile.write(rcastcc(&serialNum), sizeof(int)); // save the serial number of the district, to the given file.
		outFile.write(rcastcc(&numDelegate), sizeof(int)); // save the number of delegate, the district has, to the given file.
		outFile.write(rcastcc(&numCanVotes), sizeof(int)); // save the type of the district- unites or divided, to the given file.
		outFile.write(rcastcc(&numVotes), sizeof(int)); // save how many people voted at the current district, to the given file. 
		outFile.write(rcastcc(&type), sizeof(int)); // save the type of the district- unites or divided, to the given file. 

		if (!outFile.good()) // Checks whether the save was performed successfully.
			throw SaveException();
	}
//-----------------------------------------------------------------------------------------------//
}
