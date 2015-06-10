#include "Bank.h"
#include "Checking.h"
#include "Savings.h"

#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <iomanip>

// Constructor
Bank::Bank()
	: mAccFilename("Bank Accounts.dat")
	, mTimeFilename("Time Passed.dat")
{
	// Accounts File
	try {
		std::ifstream fin(mAccFilename, std::ios::in | std::ios::binary);
		std::streampos pos = 0;
		while (fin.is_open())
		{
			fin.seekg(pos);
			unsigned typeLen;
			fin.read(reinterpret_cast<char*>(&typeLen), sizeof typeLen);
			std::string typeString;
			typeString.resize(typeLen);
			fin.read(const_cast<char*>(typeString.c_str()), typeString.size());
			if (typeString == "Checking") {
				std::unique_ptr<Account> acc(new Checking());
				pos = acc->read(mAccFilename, fin.tellg());
				mAccounts.push_back(std::move(acc));
			}
			else if (typeString == "Savings") {
				std::unique_ptr<Account> acc(new Savings());
				pos = acc->read(mAccFilename, fin.tellg());
				mAccounts.push_back(std::move(acc));
			}
			if (fin.eof())
				fin.close();
		}
	}
	catch (std::bad_alloc e) {
		std::ofstream fout(mAccFilename, std::ios::out | std::ios::app | std::ios::binary);
		if (fout.is_open())
			fout.close();
	}
	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout << std::fixed << std::setprecision(2);

	// Time Passed File
	std::ifstream fTimeIn(mTimeFilename, std::ios::in | std::ios::binary);
	if (fTimeIn.is_open())
	{
		fTimeIn.read(reinterpret_cast<char*>(&mTimePassed), sizeof mTimePassed);
		mTimePassed.first += 4;
		if (mTimePassed.second < (mTimePassed.second + mTimePassed.first / 12))
			mNewYear = true;
		updateSavingsAccounts();
		mTimePassed.second += mTimePassed.first / 12;
		mTimePassed.first = mTimePassed.first % 12;

		fTimeIn.close();
	}
	std::ofstream fTimeOut(mTimeFilename, std::ios::out | std::ios::binary);
	if (fTimeOut.is_open())
	{
		fTimeOut.write(reinterpret_cast<char*>(&mTimePassed), sizeof mTimePassed);
		fTimeOut.close();
	}
}

// Public Methods
	// Find Account
Account* Bank::findAccount() const
{
	using std::cout;
	using std::cin;

	cout << "====FIND ACCOUNT====\n\n";
	std::string id = "\n";
	do {
		cout << "ID Number: ";
		if (cin.peek() != '\n')
			getline(cin, id);
		else {
			cin.clear();
			while (cin.get() != '\n')
				continue;
			continue;
		}
	} while (!std::all_of(id.begin(), id.end(), ::isdigit));

	for (auto& acc : mAccounts)
		if (acc->mID == std::stoi(id))
			return &(*acc);

	throw unfoundAccExc();
}
	// Input Account Details
std::unique_ptr<Account> Bank::inputAccountDetails() const
{
	using std::cout;
	using std::cin;
	using std::endl;

	std::string id = "\n";
	do {
		cout << "ID: ";
		if (cin.peek() != '\n')
			getline(cin, id);
		else {
			cin.clear();
			while (cin.get() != '\n')
				continue;
			continue;
		}
	} while (!std::all_of(id.begin(), id.end(), ::isdigit));
	cout << endl;

	std::string name = "\n";
	do {
		cout << "Name: ";
		if (cin.peek() != '\n')
			getline(cin, name);
		else {
			cin.clear();
			while (cin.get() != '\n')
				continue;
			continue;
		}
	} while (std::any_of(name.begin(), name.end(), ::isdigit) || std::any_of(name.begin(), name.end(), ::ispunct));
	cout << endl;

	double balance;
	do {
		cout << "Balance: ";
		if (!(cin >> balance)) {
			cin.clear();
			while (cin.get() != '\n')
				continue;
		}
	} while (balance < 0);
	cout << endl;

	char charType;
	while (!(cout << "(C - Checking, S - Savings)\nType: ") || !(cin >> charType) || (toupper(charType) != 'C'
		&& toupper(charType) != 'S'))
		while (cin.get() != '\n')
			cin.clear();
	Account::IDType type = (toupper(charType) == 'C' ? Account::IDType::CHECKING
		: Account::IDType::SAVINGS);

	if (type == Account::IDType::CHECKING) {
		std::unique_ptr<Checking> acc(new Checking(stoi(id), name, balance));
		return std::move(acc);
	}
	else {
		std::unique_ptr<Savings> acc(new Savings(stoi(id), name, balance));
		return std::move(acc);
	}
}
	// Account Inquiries
void Bank::accountInquiries() const
{
	using std::cout;
	using std::endl;
	using std::string;
	using std::to_string;
	using std::setw;

	cout.setf(std::ios::left, std::ios::adjustfield);

	cout << "====ACCOUNT INQURIES====\n\n";

	Savings* ps;
	bool castable;
	string No = "ID",
		   Na = "Name",
		   Ba = "Balance",
		   Ty = "Type",
		   IR = "Interest",
		   Wi = "Withdrawals";
	unsigned short longestNo = No.size(),
				   longestNa = Na.size(),
				   longestBa = Ba.size(),
				   longestTy = Ty.size(),
				   longestIR = IR.size(),
				   longestWi = Wi.size();
	const unsigned short spaceBetween = 4;

	// Set Longest Strings
	for (const auto& acc : mAccounts) {
		if (to_string(acc->mID).size() > longestNo)
			longestNo = to_string(acc->mID).size();
		if (acc->mName.size() > longestNa)
			longestNa = acc->mName.size();
		if (to_string(acc->mBalance).size() - 4 > longestBa)
			longestBa = to_string(acc->mBalance).size() - 4;
		if (acc->mType.second.size() > longestTy)
			longestTy = acc->mType.second.size();

		castable = false;
		if (ps = dynamic_cast<Savings*>(&(*acc)))
			castable = true;

		if (castable && to_string(ps->mInterestRate).size() > longestIR)
			longestIR = to_string(ps->mInterestRate).size();
		if (castable && to_string(ps->mWithdrawalAmounts).size() > longestWi)
			longestWi = to_string(ps->mWithdrawalAmounts).size();
	}
	// Types and Separators
	cout << string(longestNo + longestNa + longestBa + longestTy 
		+ longestIR + longestWi + spaceBetween * 5, '=') << endl;
	cout << setw(longestNo + spaceBetween) << No
		<< setw(longestNa + spaceBetween) << Na
		<< setw(longestTy + spaceBetween) << Ty
		<< setw(longestBa + spaceBetween) << Ba
		<< setw(longestIR + spaceBetween) << IR
		<< Wi << endl;
	cout << string(longestNo + longestNa + longestBa + longestTy
		+ longestIR + longestWi + spaceBetween * 5, '=') << endl;
	// Display
	for (const auto& acc : mAccounts) {
		cout << setw(longestNo + spaceBetween) << acc->mID
			<< setw(longestNa + spaceBetween) << acc->mName
			<< setw(longestTy + spaceBetween) << acc->mType.second
			<< setw(longestBa + spaceBetween) << acc->mBalance;

		castable = false;
		if (ps = dynamic_cast<Savings*>(&(*acc)))
			castable = true;

		if (castable) {
			cout << std::setprecision(1) << ps->mInterestRate
				<< setw(longestIR+1 + spaceBetween - (to_string(ps->mInterestRate).size() - 4)) << "%"
				<< setw(longestWi + spaceBetween) << ps->mWithdrawalAmounts;
		}
		cout << endl;
		cout << std::fixed << std::setprecision(2);
	}
	cout << endl;
}
	// Open Account
void Bank::openAccount()
{
	std::cout << "====OPEN ACCOUNT====\n\n";

	auto acc = inputAccountDetails();
	acc->write(mAccFilename);
	mAccounts.push_back(std::move(acc));

	std::cout << "\n\nAccount successfully opened..\n";
	std::cin.get();
}
	// Modify Account
void Bank::modifyAccount(Account* pAcc)
{
	system("cls");
	std::cout << "====MODIFY ACCOUNT====\n\n";
	pAcc->inquiry();
	std::cout << "----NEW DETAILS----\n\n";
	*pAcc = inputAccountDetails();

	std::cout << "\nAccount successfully modified..\n";
	std::cin.get();
}
	// Close Account
void Bank::closeAccount(Account* pAcc)
{
	using std::cout;
	using std::cin;

	system("cls");
	std::cout << "====CLOSE ACCOUNT====\n\n";
	pAcc->inquiry();
	unsigned number;
	while (!(cout << "\nReenter ID to confirm: ") || !(cin >> number))
		while (cin.get() != '\n')
			cin.clear();
	if (number == pAcc->mID) {
		for (auto& ps = mAccounts.begin(); ps != mAccounts.end(); ps++)
			if (&(*ps)->mID == &pAcc->mID) {
				int pos = find(mAccounts.begin(), mAccounts.end(), *ps) - mAccounts.begin();
				mAccounts.erase(mAccounts.begin() + pos);
				break;
			}
		cout << "\nAccount successfully closed..\n";
	}
	cin.get();
}
	// Update Savings Accounts
void Bank::updateSavingsAccounts()
{
	Savings* ps;
	for (const auto& acc : mAccounts)
		if (ps = dynamic_cast<Savings*>(&(*acc))) {
			ps->mWithdrawalAmounts = 4;
			if (mNewYear)
				ps->mBalance += ps->mBalance * ps->mInterestRate / 100;
		}
}
	// Recreate File
void Bank::recreateFile()
{
	std::ofstream fout(mAccFilename, std::ios::out | std::ios::binary);
	if (fout.is_open())
		for (const auto& x : mAccounts)
			x->write(mAccFilename);
}