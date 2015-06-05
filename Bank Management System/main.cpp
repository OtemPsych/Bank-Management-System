#include "Bank.h"

#include <iostream>

int main()
{
	using std::cout;
	using std::cin;

	Bank bank;
	short option;

	do {
		cout << "====BANK MANAGEMENT SYSTEM====\n\n";
		cout << "MAIN MENU\n\n"
			<< "1| Open Account\n\n"
			<< "2| Deposit\n\n"
			<< "3| Withdraw\n\n"
			<< "4| Account Inquiry\n\n"
			<< "5| All Account Inquiries\n\n"
			<< "6| Modify Account\n\n"
			<< "7| Close Account\n\n"
			<< "8| Exit\n\n";
		while (!(cout << "Select an option (1-8): ") || !(cin >> option) || option < 1 || option > 8) {
			cin.clear();
			while (cin.get() != '\n')
				continue;
		}
		system("cls");
		try {
			switch (option)
			{
			case 1:
				bank.openAccount();
				break;
			case 2:
				bank.findAccount()->transaction(Account::TransType::DEPOSIT);
				break;
			case 3:
				bank.findAccount()->transaction(Account::TransType::WITHDRAW);
				break;
			case 4:
				bank.findAccount()->inquiry();
				cin.get(); cin.get();
				break;
			case 5:
				bank.accountInquiries();
				break;
			case 6:
				bank.modifyAccount(bank.findAccount());
				break;
			case 7:
				bank.closeAccount(bank.findAccount());
			}
		}
		catch (const Bank::unfoundAccExc& e) {
			cout << e.what();
			cin.get(); cin.get();
		}
		system("cls");
	} while (option != 8);

	return 0;
}