#ifndef _Savings_H_
#define _Savings_H_

#include "Account.h"

class Savings : public Account
{
	friend class Bank;
private:
	// Private Data Members
	double	 mInterestRate;
	unsigned mWithdrawalAmounts;

protected:
	// Protected Methods
	const std::streampos& read(const std::string& filename, std::streampos& pos);
	void write(const std::string& filename);
	Account* operator=(std::unique_ptr<Account> acc);
public:
	// Constructors
	Savings() = default;
	Savings(unsigned id, const std::string& name, double balance);
	// Public Methods
	void inquiry();
	void transaction(const TransType& type);
};
#endif