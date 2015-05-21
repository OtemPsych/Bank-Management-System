#include "Bank.h"
#include "Checking.h"
#include "Savings.h"

#include <iostream>
#include <cctype>
#include <algorithm>

// Public Methods
	// Find Account
Account* Bank::findAccount() const
{
	using std::cout;
	using std::cin;

	cout << "====FIND ACCOUNT====\n\n";
	cout << "Number: ";
	unsigned number;
	while (!(cin >> number)) {
		while (cin.get() != '\n')
			cin.clear();
		cout << "Invalid Input\n";
	}

	for (auto& acc : mAccounts)
		if (acc->mNumber == number)
			return &(*acc);

	throw AccountError();
}
// Input Account Details
std::unique_ptr<Account> Bank::inputAccountDetails() const
{
	using std::cout;
	using std::cin;
	using std::endl;

	unsigned number;
	while (!(cout << "Number: ") || !(cin >> number) || number < 0)
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
	Account::Type type = (toupper(charType) == 'C' ? Account::Type::CHECKING
												   : Account::Type::SAVINGS);

	if (type == Account::Type::CHECKING) {
		std::unique_ptr<Checking> acc(new Checking(number, name, balance));
		return std::move(acc);
	}
	else {
		std::unique_ptr<Savings> acc(new Savings(number, name, balance));
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

	string No = "Number";
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
		if (to_string(acc->mNumber).size() > longestNo)
			longestNo = to_string(acc->mNumber).size();
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
	for (const auto& acc : mAccounts) {
		cout << acc->mNumber;
		cout.width(longestNo - to_string(acc->mNumber).size() + acc->mName.size() + spaceBetween);
		cout << acc->mName;
		cout.width(longestNa - acc->mName.size() + to_string(acc->mBalance).size() + spaceBetween);
		cout << acc->mBalance;
		cout.width(longestBa - to_string(acc->mBalance).size() + (acc->mType == Account::CHECKING 
																	   ? string("Checking").size() 
																	   : string("Savings").size()) + spaceBetween);
		cout << (acc->mType == Account::CHECKING ? "Checking" : "Savings") << endl;
	}
	std::cin.get(); std::cin.get();
}
	// Open Account
void Bank::openAccount()
{
	std::cout << "====OPEN ACCOUNT====\n\n";
	mAccounts.push_back(inputAccountDetails());
	
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
	if (number == pAcc->mNumber) {
		for (auto& ps = mAccounts.begin(); ps != mAccounts.end(); ps++)
			if (&(*ps)->mNumber == &pAcc->mNumber) {
				int pos = find(mAccounts.begin(), mAccounts.end(), *ps) - mAccounts.begin();
				mAccounts.erase(mAccounts.begin() + pos);
				break;
			}
		cout << "\nAccount successfully closed..\n";
	}
	cin.get(); cin.get();
}