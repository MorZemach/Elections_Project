#pragma once
#include "CitizenH.h"
#include "DistrictH.h"

using namespace std;

namespace ElectionsDay
{
	class Delegates
	{
	private:
		List<Citizen>* lstDelegate;
		int serialNumDistrict;
		int voteSum;

	public:
		Delegates(int serialNum, int voteSum = 0, List<Citizen>* lst = new List<Citizen>())
			:serialNumDistrict(serialNum), voteSum(voteSum), lstDelegate(lst) {}
		~Delegates() {}
		int getID() const { return this->serialNumDistrict; }
		int getVoteSum() const { return this->voteSum; }
		float getPrecentPerFaction(int numOfVotesAtDis) const;
		List<Citizen>* getlst() const { return this->lstDelegate; }
		void setSerialNumDistrict(int serialNumDistrict);
		void setVoteSum();
		void printLstDelegate(int numOfDelegate) const;
		friend ostream& operator<<(ostream& os, const Node<Citizen>& delegate);
		void save(ofstream& outFile) const;
	};
}