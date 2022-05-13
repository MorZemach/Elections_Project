#pragma once

#include <iostream>
using namespace std;

namespace ElectionsDay
{
	class CalculationException : public exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "Division by zero.";
		}
	};

	class SettingException : public exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "Failed to update";
		}
	};

	class SaveException : public exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "Failed save to the file.";
		}
	};

	class LoadException : public exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "Failed load from the file.";
		}
	};
}


