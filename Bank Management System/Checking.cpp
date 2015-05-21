#include "Checking.h"

#include <iostream>

// Constructor
Checking::Checking(unsigned number, const std::string& name,
				   unsigned balance)
	: Account(CHECKING, number, name, balance)
{
}

// Public Methods
	// Transaction
void Checking::transaction(const TransactionType& type)
{
	Account::transaction(type);
}
	// Inquiry
void Checking::inquiry() const
{
	Account::inquiry();
	std::cin.get(); std::cin.get();
}