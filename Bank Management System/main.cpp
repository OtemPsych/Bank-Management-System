#include "Bank.h"

#include <iostream>
#include <algorithm>

int main()
{
	using std::cout;
	using std::cin;
	using std::stoi;

	Bank bank;
	std::string option = "\n";

	do {
		cout << "====BANK MANAGEMENT SYSTEM====\t\tTime Passed: " 
			<< bank.getTimePassed().second << " years, " << bank.getTimePassed().first << " months\n\n";
		cout << "MAIN MENU\n\n"
			<< "1| Open Account\n\n"
			<< "2| Deposit\n\n"
			<< "3| Withdraw\n\n"
			<< "4| Account Inquiry\n\n"
			<< "5| All Account Inquiries\n\n"
			<< "6| Modify Account\n\n"
			<< "7| Close Account\n\n"
			<< "8| Exit\n\n";
		do {
			cout << "Select an option (1-8): ";
			if (cin.peek() != '\n')
				getline(cin, option);
			else {
				cin.clear();
				while (cin.get() != '\n')
					continue;
				continue;
			}
		} while (!std::all_of(option.begin(), option.end(), ::isdigit)
			|| stoi(option) < 1 || stoi(option) > 8);

		system("cls");
		try {
			switch (stoi(option))
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
				break;
			case 5:
				bank.accountInquiries();
				break;
			case 6:
				bank.modifyAccount(bank.findAccount());
				break;
			case 7:
				bank.closeAccount(bank.findAccount());
				break;
			}
			if (stoi(option) != 8 && stoi(option) != 2) {
				cin.clear();
				while (cin.get() != '\n')
					continue;
			}
		}
		catch (Bank::unfoundAccExc& e) {
			cout << e.what();
			cin.get();
		}
		system("cls");
	} while (stoi(option) != 8);

	bank.recreateFile();
	return 0;
}