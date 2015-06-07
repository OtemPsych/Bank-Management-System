#include "Account.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

// Constructor 1
Account::Account(unsigned id, const IDType& type, const std::string& name, double balance)
	: mID(id)
	, mType(type, (type == CHECKING ? "Checking" : "Savings"))
	, mName(name)
	, mBalance(balance)
{
}

// Protected Methods
	// Read
const std::streampos& Account::read(const std::string& filename, std::streampos& pos)
{
	std::ifstream fin(filename, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		fin.seekg(pos);
		// ID
		fin.read(reinterpret_cast<char*>(&mID), sizeof mID);
		// Name
		unsigned nameLen;
		fin.read(reinterpret_cast<char*>(&nameLen), sizeof nameLen);
		mName.resize(nameLen);
		fin.read(const_cast<char*>(mName.c_str()), mName.size());
		// Balance
		fin.read(reinterpret_cast<char*>(&mBalance), sizeof mBalance);

		pos = fin.tellg();
		fin.close();
	}
	return pos;
}
	// Write
void Account::write(const std::string& filename)
{
	std::ofstream fout(filename, std::ios::out | std::ios::app | std::ios::binary);
	if (fout.is_open())
	{
		// Type
		unsigned typeSize = mType.second.size();
		fout.write(reinterpret_cast<char*>(&typeSize), sizeof typeSize);
		fout.write(const_cast<char*>(mType.second.c_str()), typeSize);
		// ID
		fout.write(reinterpret_cast<char*>(&mID), sizeof mID);
		// Name
		unsigned nameSize = mName.size();
		fout.write(reinterpret_cast<char*>(&nameSize), sizeof nameSize);
		fout.write(const_cast<char*>(mName.c_str()), nameSize);
		// Balance
		fout.write(reinterpret_cast<char*>(&mBalance), sizeof mBalance);

		fout.close();
	}
}
	// Operator =
Account* Account::operator=(std::unique_ptr<Account> acc)
{
	mType = acc->mType;
	mID = acc->mID;
	mName = acc->mName;
	mBalance = acc->mBalance;

	return this;
}

// Public Methods
	// Inquiry
void Account::inquiry()
{
	using std::cout;
	using std::endl;

	auto mode = [](std::ios::fmtflags s) { cout.setf(s, std::ios::adjustfield); };

	cout << "\n----ACCOUNT INQUIRY----\n\n";

	mode(std::ios::right);
	cout << std::setw(mFWidth) << "ID" << std::setw(3) << ' ';
	mode(std::ios::left);
	cout << mID << endl;

	mode(std::ios::right);
	cout << std::setw(mFWidth) << "Type" << std::setw(3) << ' ';
	mode(std::ios::left);
	cout << mType.second << endl;

	mode(std::ios::right);
	cout << std::setw(mFWidth) << "Name" << std::setw(3) << ' ';
	mode(std::ios::left);
	cout << mName << endl;

	mode(std::ios::right);
	cout << std::setw(mFWidth) << "Balance" << std::setw(3) << ' ';
	mode(std::ios::left);
	cout << mBalance << endl;
}
	// Transaction
void Account::transaction(const TransType& type)
{
	using std::cout;
	using std::cin;

	cout << "\n\n====TRANSACTION FORM====\n";
	inquiry();
	cout << std::endl;
	double amount;
	do {
		cout << (type == DEPOSIT ? "Deposit" : "Withdraw") << " Amount: ";
		if (!(cin >> amount)) {
			cin.clear();
			while (cin.get() != '\n')
				continue;
		}
	} while (amount < 0 || (type == WITHDRAW ? amount > mBalance : nullptr));
	type == DEPOSIT ? mBalance += amount : mBalance -= amount;
	cout << "\nTransaction Complete..\n";
	cin.get();
	cin.get();
}