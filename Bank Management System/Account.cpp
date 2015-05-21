#include "Account.h"

#include <iostream>

// Constructor
Account::Account(const Type& type, unsigned number, const std::string& name,
				 unsigned balance)
	: mType(type)
	, mNumber(number)
	, mName(name)
	, mBalance(balance)
{
}

// Public Methods
	// Transaction
void Account::transaction(const TransactionType& type)
{
	using std::cout;
	using std::cin;

	cout << "\n\n====TRANSACTION FORM====\n\n";
	inquiry();
	cout << (type == DEPOSIT ? "Deposit" : "Withdraw") << " Amount: ";
	unsigned amount;
	while (!(cin >> amount) || amount < 0 || (type == WITHDRAW ? amount > mBalance : nullptr)) {
		while (cin.get() != '\n')
			cin.clear();
		cout << "Invalid Input\n";
	}
	type == DEPOSIT ? mBalance += amount
		: mBalance -= amount;
}
	// Inquiry
void Account::inquiry() const
{
	using std::cout;
	using std::endl;

	cout << "----ACCOUNT DETAILS----\n\n";
	cout << "Number " << mNumber
		<< "\n\nName " << mName
		<< "\n\nBalance " << mBalance
		<< "\n\nType " << (mType == CHECKING ? "Checking" : "Savings") << endl << endl;
}
	// Operator =
Account* Account::operator=(std::unique_ptr<Account> acc)
{
	mType = acc->mType;
	mNumber = acc->mNumber;
	mName = acc->mName;
	mBalance = acc->mBalance;

	return this;
}