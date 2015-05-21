#ifndef _Checking_H_
#define _Checking_H_

#include "Account.h"

class Checking :public Account
{
public:
	// Constructor
	Checking(unsigned number, const std::string& name = "Unknown",
			 unsigned balance = 0.0);
	// Public Methods
	void transaction(const TransactionType& type);
	void inquiry() const;
};
#endif