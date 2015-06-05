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
	: mFilename("Bank Accounts.dat")
{
	std::ifstream fin(mFilename, std::ios::in | std::ios::binary);
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
			pos = acc->read(mFilename, fin.tellg());
			mAccounts.push_back(std::move(acc));
		}
		else if (typeString == "Savings") {
			std::unique_ptr<Account> acc(new Savings());
			pos = acc->read(mFilename, fin.tellg());
			mAccounts.push_back(std::move(acc));
		}
		if (fin.eof())
			fin.close();
	}
}

// Public Methods
// Find Account
Account* Bank::findAccount() const
{
	using std::cout;
	using std::cin;

	cout << "====FIND ACCOUNT====\n\n";
	cout << "Number: ";
	unsigned id;
	while (!(cin >> id)) {
		cin.clear();
		while (cin.get() != '\n')
			continue;
		cout << "Invalid Input\n";
	}

	for (auto& acc : mAccounts)
		if (acc->mID == id)
			return &(*acc);

	throw unfoundAccExc();
}
// Input Account Details
std::unique_ptr<Account> Bank::inputAccountDetails() const
{
	using std::cout;
	using std::cin;
	using std::endl;

	unsigned id;
	while (!(cout << "ID: ") || !(cin >> id) || id < 0)
		while (cin.get() != '\n')
			cin.clear();
	cout << endl;

	std::string name;
	cin.get();
	do {
		cout << "Name: ";
		getline(cin, name);
	} while (std::any_of(name.begin(), name.end(), ::isdigit) || std::any_of(name.begin(), name.end(), ::ispunct));
	cout << endl;

	unsigned balance;
	while (!(cout << "Balance: ") || !(cin >> balance) || balance < 0)
		while (cin.get() != '\n')
			cin.clear();
	cout << endl;

	char charType;
	while (!(cout << "(C - Checking, S - Savings)\nType: ") || !(cin >> charType) || (toupper(charType) != 'C'
		&& toupper(charType) != 'S'))
		while (cin.get() != '\n')
			cin.clear();
	Account::IDType type = (toupper(charType) == 'C' ? Account::IDType::CHECKING
		: Account::IDType::SAVINGS);

	if (type == Account::IDType::CHECKING) {
		std::unique_ptr<Checking> acc(new Checking(id, name, balance));
		return std::move(acc);
	}
	else {
		std::unique_ptr<Savings> acc(new Savings(id, name, balance));
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

	cout << "====ACCOUNT INQURIES====\n\n";

	string No = "ID";
	string Na = "Name";
	string Ba = "Balance";
	string Ty = "Type";
	unsigned short longestNo = No.size();
	unsigned short longestNa = Na.size();
	unsigned short longestBa = Ba.size();
	const unsigned short longestTy = string("Checking").size();
	const unsigned short spaceBetween = 4;

	// Set Longest Strings
	for (const auto& acc : mAccounts) {
		if (to_string(acc->mID).size() > longestNo)
			longestNo = to_string(acc->mID).size();
		if (acc->mName.size() > longestNa)
			longestNa = acc->mName.size();
		if (to_string(acc->mBalance).size() > longestBa)
			longestBa = to_string(acc->mBalance).size();
	}
	// Types and Separators
	cout << string(longestNo + longestNa + longestBa + longestTy + spaceBetween * 3, '=');
	cout << endl << No;
	cout.width(longestNo - No.size() + Na.size() + spaceBetween);
	cout << Na;
	cout.width(longestNa - Na.size() + Ba.size() + spaceBetween);
	cout << Ba;
	cout.width(longestBa - Ba.size() + Ty.size() + spaceBetween);
	cout << Ty << endl;
	cout << string(longestNo + longestNa + longestBa + longestTy + spaceBetween * 3, '=');
	cout << endl;
	// Display
	cout.setf(std::ios::left, std::ios::adjustfield);
	for (const auto& acc : mAccounts) {
		cout << std::setw(longestNo + spaceBetween) << acc->mID;
		cout << std::setw(longestNa + spaceBetween) << acc->mName;
		cout << std::setw(longestBa + spaceBetween) << acc->mBalance;
		cout << (acc->mType.first == Account::CHECKING ? "Checking" : "Savings") << endl;
	}
	std::cin.get(); std::cin.get();
}
// Open Account
void Bank::openAccount()
{
	std::cout << "====OPEN ACCOUNT====\n\n";

	auto acc = inputAccountDetails();
	acc->write(mFilename);
	mAccounts.push_back(std::move(acc));

	std::cout << "\nAccount successfully opened..\n";
	std::cin.get(); std::cin.get();
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
	std::cin.get(); std::cin.get();
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
	while (!(cout << "Reenter Account Number to confirm: ") || !(cin >> number))
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
	cin.get(); cin.get();
}