#ifndef _Checking_H_
#define _Checking_H_

#include "Account.h"

#include <iostream>

class Checking :public Account
{
protected:
	// Protected Methods
	const std::streampos& read(const std::string& filename, std::streampos& pos);
	inline void write(const std::string& filename) { Account::write(filename); }
public:
	// Constructors
	Checking() = default;
	Checking(unsigned id, const std::string& name, double balance)
		: Account(id, IDType::CHECKING, name, balance) {}
	// Public Methods
	inline void inquiry() const { Account::inquiry(); }
	inline void transaction(const TransType& type) { Account::transaction(type); }
};
#endif