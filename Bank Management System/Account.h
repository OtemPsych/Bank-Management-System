#ifndef _Account_H_
#define _Account_H_

#include "Bank.h"

#include <string>
#include <memory>

class Account
{
	friend class Bank;
protected:
	// Protected Enumerator
	enum Type { CHECKING, SAVINGS };
private:
	// Private Data Members
	Type		mType;
	unsigned	mNumber;
	std::string mName;
	unsigned		mBalance;

protected:
	// Protected Methods
	unsigned getBalance() const { return mBalance; }
	void setBalance(unsigned balance) { mBalance = balance; }
public:
	// Constructor - Destructor
	Account(const Type& type, unsigned number = 0, const std::string& name = "Unknown",
			 unsigned balance = 0.0);
	virtual ~Account() {}
	// Public Enumerator
	enum TransactionType { DEPOSIT, WITHDRAW };
	// Public Methods
	virtual void transaction(const TransactionType& type) = 0;
	virtual void inquiry() const = 0;
	Account* operator=(std::unique_ptr<Account> acc);
};
#endif