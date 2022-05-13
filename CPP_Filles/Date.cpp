#define _CRT_SECURE_NO_WARNINGS
#include "DateH.h"
#include "Exception.h"

namespace ElectionsDay
{
	//-----------------------------------------------------------------------------------------------//
	Date::Date(int day, int month, int year) // Date Constructor.
	{
		this->month = month;
		this->day = day;
		this->year = year;
	}
	//-----------------------------------------------------------------------------------------------//
	Date::Date(ifstream& inFile) // load election date into a given fill.
	{
		inFile.read(rcastc(&day), sizeof(int)); // load election day.
		inFile.read(rcastc(&month), sizeof(int)); // load election month.
		inFile.read(rcastc(&year), sizeof(int)); // load election day.
	}
	//-----------------------------------------------------------------------------------------------//
	int Date::getDay() const // return the day of this date.
	{
		return this->day;
	}
	//-----------------------------------------------------------------------------------------------//
	int Date::getMonth() const // return the month of this date.
	{
		return this->month;
	}
	//-----------------------------------------------------------------------------------------------//
	int Date::getYear() const // return the year of this date.
	{
		return this->year;
	}
	//-----------------------------------------------------------------------------------------------//
	void Date::setDay(int day) // update the day of this date.
	{
		if (month < 0 || month > 12)
		{
			throw out_of_range("Attempt to access elements out of defined range.");
		}
		else
		{
			if (day <= daysAsMonth[month]) // Checks if the input is correct.
			{
				this->day = day;
			}
			else
			{
				throw logic_error("There is no such day in the current month."); // wrong input.
			}
		}
	}
	//-----------------------------------------------------------------------------------------------//
	void Date::setMonth(int month) // update the month of this date.
	{
		if (month > 0 && month < 13) // Checks if the input is correct.
		{
			this->month = month;
		}
		else
		{
			throw logic_error("Month must be between: 1 to 12."); // wrong input.
		}
	}
	//-----------------------------------------------------------------------------------------------//
	void Date::setYear(int year) // update the year of this date. 
	{
		if (year > 0) // Checks if the input is correct.
		{
			this->year = year;
		}
		else
		{
			throw logic_error("Year must be a positive number."); // wrong input.
		}
	}
	//-----------------------------------------------------------------------------------------------//
	ostream& operator<<(ostream& os, const Date& date) // print election date into the given file.
	{
		os << "Election date is: " << date.getDay() << "/" << date.getMonth() << "/" << date.getYear() << endl;
		return os;
	}
	//-----------------------------------------------------------------------------------------------//
	void Date::save(ostream& outFile) const // This function saves the election date to a given file.
	{
		try
		{
			outFile.write(rcastcc(&day), sizeof(int)); // save the day from the date to the given file.
			outFile.write(rcastcc(&month), sizeof(int));// save the month from the date to the given file. 
			outFile.write(rcastcc(&year), sizeof(int)); // save the year from the date to the given file. 
			if (!outFile.good()) // Checks whether the save was performed successfully.
				throw SaveException();
		}
		catch (SaveException& ex)
		{
			throw;
		}
	}
	//-----------------------------------------------------------------------------------------------//
}