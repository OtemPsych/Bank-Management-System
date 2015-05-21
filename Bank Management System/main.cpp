#include "Bank.h"

#include <iostream>

int main()
{
	Bank bank;
	short option;

	do {
		std::cout << "====BANK MANAGEMENT SYSTEM====\n\n";
		std::cout << "MAIN MENU\n\n"
			<< "1| Open Account\n\n"
			<< "2| Deposit\n\n"
			<< "3| Withdraw\n\n"
			<< "4| Account Inquiry\n\n"
			<< "5| All Account Inquiries\n\n"
			<< "6| Modify Account\n\n"
			<< "7| Close Account\n\n"
			<< "8| Exit\n\n";
		while (!(std::cout << "Select an option (1-8): ") || !(std::cin >> option) || option < 1 || option > 8)
			while (std::cin.get() != '\n')
				std::cin.clear();
		system("cls");
		try {
			switch (option)
			{
			case 1:
				bank.openAccount();
				break;
			case 2:
				bank.findAccount()->transaction(Account::TransactionType::DEPOSIT);
				break;
			case 3:
				bank.findAccount()->transaction(Account::TransactionType::WITHDRAW);
				break;
			case 4:
				bank.findAccount()->inquiry();
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
		catch (const Bank::AccountError& e) {
			std::cout << e.what();
			std::cin.get(); std::cin.get();
		}
		system("cls");
	} while (option != 8);

	return 0;
}