#include "Savings.h"

#include <iostream>

// Constructor
Savings::Savings(unsigned number, const std::string& name,
				 unsigned balance)
	: Account(SAVINGS, number, name, balance)
	, mInterestRate(0.6)
	, mWithdrawalAmounts(4)
{
}

// Public Methods
	// Update Balance
void Savings::updateBalance()
{
	setBalance(unsigned(getBalance() + (mInterestRate * getBalance() / 100)));
}
	// Transaction
void Savings::transaction(const TransactionType& type)
{
	if (type == WITHDRAW) {
		if (mWithdrawalAmounts > 3) {
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
	// Inquiry
void Savings::inquiry() const
{
	using std::cout;
	using std::endl;

	Account::inquiry();
	cout << "Interest Rate " << mInterestRate << "%\n\n"
		<< "Remaining Withdrawals " << mWithdrawalAmounts << endl << endl;
	std::cin.get(); std::cin.get();
}