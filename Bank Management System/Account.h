#ifndef Account_H_
#define Account_H_

#include <string>
#include <vector>
#include <memory>

class Account
{
	friend class Bank;
protected:
	// Protected Enumerator
	enum IDType {CHECKING, SAVINGS};
private:
	// Private Data Members
	unsigned		 mID;
	std::pair<IDType,
		std::string> mType;
	std::string		 mName;
	double			 mBalance;

	unsigned short   mFWidth = 12;

protected:
	// Protected Methods
	virtual const std::streampos& read(const std::string& filename, std::streampos& pos) = 0;
	virtual void write(const std::string& filename) = 0;
	Account* operator=(std::unique_ptr<Account> acc);
	// Protected Setters-Getter
	inline void setType(const std::pair<IDType, std::string>& type) { mType = type; }
	inline unsigned short getFWidth() const { return mFWidth; }
public:
	// Constructors - Destructor
	Account() = default;
	Account(unsigned id, const IDType& type, const std::string& name, double balance);
	virtual ~Account() {};
	// Public Enumerator
	enum TransType {DEPOSIT, WITHDRAW};
	// Public Methods
	virtual void inquiry() const = 0;
	virtual void transaction(const TransType& type) = 0;
};
#endif