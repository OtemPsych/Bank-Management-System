#include "Checking.h"

// Protected Method
	// Read
const std::streampos& Checking::read(const std::string& filename, std::streampos& pos)
{ 
	std::pair<IDType, std::string> type{ CHECKING, "Checking" };
	setType(type);
	pos = Account::read(filename, pos);
	return pos;
}