#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

// Integrity check for day - how many days in each month.
// Without February with 29 days (one to 4 years).
const int daysAsMonth[13] = { -1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
using namespace std;

namespace ElectionsDay
{
	class Date
	{
	private:
		int day;
		int month;
		int year;
	public:
		Date(int day = 0, int month = 0, int year = 0);
		Date(ifstream& inFile);
		~Date() {}

		int getDay() const;
		int getMonth() const;
		int getYear() const;

		void setDay(int day);
		void setMonth(int month);
		void setYear(int year);

		friend ostream& operator<<(ostream& os, const Date& date);
		void save(ostream& outFile) const;
	};
}