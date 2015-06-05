#include "Savings.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>

// Constructor
Savings::Savings(unsigned id, const std::string& name, double balance)
	: Account(id, SAVINGS, name, balance)
	, mInterestRate(0.6)
	, mWithdrawalAmounts(4)
{
}

// Protected Methods
	// Read
const std::streampos& Savings::read(const std::string& filename, std::streampos& pos)
{
	std::ifstream fin(filename, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		std::pair<IDType, std::string> type;
		type.first = SAVINGS;
		type.second = "Savings";
		setType(type);
		pos = Account::read(filename, pos);

		fin.seekg(pos);
		// Interest Rate
		fin.read(reinterpret_cast<char*>(&mInterestRate), sizeof mInterestRate);
		// Withdrawal Amounts
		fin.read(reinterpret_cast<char*>(&mWithdrawalAmounts), sizeof mWithdrawalAmounts);

		pos = fin.tellg();
		fin.close();
	}
	return pos;
}
	// Write
void Savings::write(const std::string& filename)
{
	Account::write(filename);

	std::ofstream fout(filename, std::ios::out | std::ios::app | std::ios::binary);
	if (fout.is_open())
	{
		// Interest Rate
		fout.write(reinterpret_cast<char*>(&mInterestRate), sizeof mInterestRate);
		// Withdrawal Amounts
		fout.write(reinterpret_cast<char*>(&mWithdrawalAmounts), sizeof mWithdrawalAmounts);

		fout.close();
	}
}

// Public Methods
	// Inquiry
void Savings::inquiry() const
{
	using std::cout;
	using std::endl;

	cout.setf(std::ios::left, std::ios::adjustfield);

	Account::inquiry();
	cout << std::setw(getFWidth()) << "Interest Rate" << mInterestRate << endl
		<< std::setw(getFWidth()) << "Withdrawals" << mWithdrawalAmounts << endl;
}
	// Transaction
void Savings::transaction(const TransType& type)
{
	if (type == WITHDRAW) {
		if (mWithdrawalAmounts > 0) {
			Account::transaction(type);
			mWithdrawalAmounts--;
		}
		else
			std::cout << "\nReached maximum withdrawals for this month.\n\n";
		std::cin.get(); std::cin.get();
	}
	else
		Account::transaction(type);
}