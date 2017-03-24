/* -----------------------------------------------------------------------------

FILE:              bankacct.cpp

DESCRIPTION:       Source file for S2017 CS202 Project 1: Bank Account

COMPILER:          gcc 5.4.0

NOTES:             None

MODIFICATION HISTORY:

Author                  Date               Version
---------------         ----------         --------------
Aaryna Irwin            2017-02-22         0.1 - Initial version, test stuff
Aaryna Irwin            2017-02-23         0.2 - Began file output testing 
Aaryna Irwin            2017-02-25         0.2.5 - File output test complete
Aaryna Irwin            2017-02-26         0.2.7 - Deposit and withdrawal test
                                           0.3 - Remove test functionality,
										   implement cmd-ln argument validation
										   0.4 - Implement record seeking
										   0.5 - Implement stream operations
Aaryna Irwin            2017-02-27         0.5.5 - Debugging seekRec()
Aaryna Irwin            2017-02-28         0.5.7 - Encapsulate seekRec in Aseek 
Aaryna Irwin            2017-03-07         0.6 - Added c-l parsing func.
Aaryna Irwin            2017-03-09         0.7 - Added c-l execution func.

----------------------------------------------------------------------------- */

#include "account.h"
#include "aseek.h"
#include <sysexits.h>
#include <iostream>
#include <cstring>
#include <vector>

// Command-line switch character
#define CLSW '/'
// Valid command-line argument list
#define ARGS "AaCcDdFfIiLlMmNnPpRrSsTtWw" 

// Command-line argument datatype
struct Arg
{
	char cmd;
	char dat[A_FIELD];
};

// Function prototypes
void dispHelp();
bool valCmd(const int, const char* [], std::vector<Arg>&, std::fstream&);
bool valAcc(std::vector<Arg>&, Account[2], std::fstream&);
void procDat(std::vector<Arg>&, Account[2], std::fstream&);
void dispMen(std::fstream&);
void makeNew(std::fstream&);
void writeRpt(std::fstream&);
void depMen(std::fstream&);
void witMen(std::fstream&);
void disAcc(std::fstream&);
void transAcc(std::fstream&);
void closeAcc(std::fstream&);
	
/* -----------------------------------------------------------------------------
FUNCTION:          main(int, char* [])
DESCRIPTION:       It functions, so others don't have to 
RETURNS:           int 
NOTES:             None
----------------------------------------------------------------------------- */
int main(const int argc, const char* argv[])
{	
	// Command-line arguments
	std::vector<Arg> args;

	// Database file - default is "database.ask"
	std::fstream dbFile;

	// Active account(s) info
	Account b[2];

	// If there are any valid command-line arguments
	if (argc > 1)
	{
		if (valCmd(argc, argv, args, dbFile))
		{
			// Default database file check
			if (!dbFile.is_open())
			{
				dbFile.open("database.ask",
						std::ios_base::in | std::ios_base::out);
			}
			// If it's valid, procDat argument!
			if (valAcc(args, b, dbFile))
				procDat(args, b, dbFile);
		}
		else
		{
			std::cout << "Invalid argument - aborting." << std::endl;
			return EX_USAGE;
		}
	}
	if (!dbFile.is_open())
		dbFile.open("database.ask",	std::ios_base::in | std::ios_base::out);
	if (argc == 1)
		dispMen(dbFile);
	dbFile.close();

	return 0;
}

/* -----------------------------------------------------------------------------
FUNCTION:          valCmd(const int, const char* [], vector<Arg>&, fstream&)
DESCRIPTION:       Validates and organizes command-line arguments
RETURNS:           bool
NOTES:             None
----------------------------------------------------------------------------- */
bool valCmd(const int argc, const char* argv[],
		std::vector<Arg>& args, std::fstream& database)
{
	// Run flag
	bool run = true;

	// Command-line argument storage using Arg datatype
	Arg argbuf = {'*', "*****"};

	// Command-line mode minimal requirements - at least one anum/pass needed
	int anum = 0, pass = 0;

	// Validate command-line arguments
	for (int i = 1; i < argc; ++i)
	{
		// Verify command-line argument switch
		if (*argv[i] == CLSW)
		{	
			// Discriminate special cases
			switch (argv[i][1])
			{
				// Help menu
				case '?':
//					dispHelp();
					break;
				case 'R':
				case 'r':
//					writeRpt(database);
					break;
				// User-specified database file (only one allowed)
				case 'O':
				case 'o':
					if (!database.is_open())
					{
						database.open(&argv[i][2],
								std::ios_base::in | std::ios_base::out);
					}
					else
						run = false;
					break;
				// Increment anum count
				case 'A':
				case 'a':
					++anum;
					break;
				// Increment pass count
				case 'P':
				case 'p':
					++pass;
					break;
			}
			// Ensure command is valid and push it to the vector if so
			if (strchr(ARGS, argv[i][1]) != nullptr)
			{
				argbuf.cmd = argv[i][1];
				strcpy(argbuf.dat, &argv[i][2]);
				args.push_back(argbuf);
			}
			else
				run = false;
		}
	}
	// If the requirements are not met, signal to abort
	if (anum == 0 || anum > 2 || anum != pass)
		run = false;

	// Return run flag
	return run;
}

/* -----------------------------------------------------------------------------
FUNCTION:          valAcc(vector<Arg>&, Account[2], fstream&)
DESCRIPTION:       Pulls and validates account numbers and passwords
RETURNS:           bool
NOTES:             None
----------------------------------------------------------------------------- */
bool valAcc(std::vector<Arg>& args,	Account b[2], std::fstream& db)
{
	// Password validation flag
	bool valid = true;

	// Pull account info from arguments
	for (Arg i : args)
	{
		switch (i.cmd)
		{
			case 'A':
			case 'a':
				if (!strcmp(b[0].gANum(), "*****"))
					b[0].sANum(i.dat);
				else
					b[1].sANum(i.dat);
				break;
			case 'P':
			case 'p':
				if (!strcmp(b[0].gPass(), "******"))
					b[0].sPass(i.dat);
				else
					b[1].sPass(i.dat);
				break;
		}
	}

	// Access active account(s) - void sAcct is first empty or append
	Aseek fAcct(b[0].gANum(), db);
	db >> fAcct;
	Aseek sAcct(b[1].gANum(), db);
	db >> sAcct;
	
	// Validate passwords
	if ((strcmp(b[0].gPass(), fAcct.gPass())) ||
				(strcmp(b[1].gPass(), sAcct.gPass())))
		valid = false;

	return valid;
}

/* -----------------------------------------------------------------------------
FUNCTION:          procDat(vector<Arg>&, Acccount[2], fstream&)
DESCRIPTION:       Processes program command functions
RETURNS:           Void function
NOTES:             None
----------------------------------------------------------------------------- */
void procDat(std::vector<Arg>& args, Account b[2], std::fstream& db)
{
	// Open active accounts
	Aseek fAcct(b[0].gANum(), db);
	db >> fAcct;
	Aseek sAcct(b[1].gANum(), db);
	db >> sAcct;

	for (Arg i : args)
	{
		switch (i.cmd)
		{
			case 'I':
			case 'i':
				std::cout << fAcct;
				break;
			case 'T':
			case 't':
				if (fAcct.withdraw(strtod(i.dat, nullptr)))
					sAcct.deposit(strtod(i.dat, nullptr));
				else
					std::cout << "Insufficient funds!" << std::endl;
				break;
			case 'D':
			case 'd':
				fAcct.deposit(strtod(i.dat, nullptr));
				break;
			case 'W':
			case 'w':
				if (!fAcct.withdraw(strtod(i.dat, nullptr)))
					std::cout << "Insufficient funds!" << std::endl;
				break;
			case 'S':
			case 's':
				if (!fAcct.sSSN(i.dat))
					std::cout << "Invalid input!" << std::endl;
				break;
			case 'M':
			case 'm':
				if (!fAcct.sMI(*i.dat))
					std::cout << "Invalid input!" << std::endl;
				break;
			case 'L':
			case 'l':
				if (!fAcct.sLName(i.dat))
					std::cout << "Invalid input!" << std::endl;
				break;
			case 'F':
			case 'f':
				if (!fAcct.sFName(i.dat))
					std::cout << "Invalid input!" << std::endl;
				break;
			case 'C':
			case 'c':
				if (!fAcct.sPass(i.dat))
					std::cout << "Invalid input!" << std::endl;
				break;
			case 'N':
			case 'n':
				if (!fAcct.sPhone(i.dat))
					std::cout << "Invalid input!" << std::endl;
		}
	}

	// Write active accounts
	db << fAcct << sAcct;
}

/* -----------------------------------------------------------------------------
FUNCTION:          dispMen(fstream&)
DESCRIPTION:       Displays main menu for interactive mode
RETURNS:           Void function
NOTES:             None
----------------------------------------------------------------------------- */
void dispMen(std::fstream& file)
{
	bool run = true;
	do
	{
		system("clear");
		std::cout << "---------------------------------------------------------\n";
		std::cout << "	FIDUCIARY TRUST INSTITUTION OF FINANCE INC.\n";
		std::cout << "---------------------------------------------------------\n";
		std::cout << std::endl;
		std::cout << " a. Create a new account\n";
		std::cout << " b. Deposit funds\n";
		std::cout << " c. Withdraw funds\n";
		std::cout << " d. Display account info\n";
 		std::cout << " e. Transfer funds\n";
		std::cout << " f. Close an account\n";
		std::cout << " g. Write a report file\n";
		std::cout << " x. Exit\n";
		std::cout << std::endl;
		std::cout << "What would you like to do? :";
		
		char choice = '*';
		choice = std::cin.get();
	
		switch (choice)
		{
			case 'a':
				makeNew(file);
				break;
			case 'b':
//				depMen(file);
				break;
			case 'c':
//				witMen(file);
				break;
			case 'd':
//				disAcc(file);
				break;
			case 'e':
//				transAcc(file);
				break;
			case 'f':
//				closeAcc(file);
				break;
			case 'g':
//				writeRpt(file);
				break;
			case 'x':
				run = false;
				break;
			default:
				std::cout << "Invalid selection!\n";
				std::cin.get();
				break;
		}
	}while (run);
}

void makeNew(std::fstream& file)
{
	std::cin.ignore(1);
	Aseek account("*****", file);
	char buffer[A_FIELD];

	// Get last name
	std::cout << "Enter last name: ";
	do
	{
		std::cin.getline(buffer, 16);
	}while (!account.sLName(buffer));

	// Get first name
	std::cout << "Enter first name: ";
	do
	{
		std::cin.getline(buffer, 16);
	}while (!account.sFName(buffer));

	// Get middle initial
	std::cout << "Enter middle initial: ";
	char b = 0;
	do
	{
		b = std::cin.get();
		std::cin.ignore(1);
	}while (!account.sMI(b));
	
	// Get ssn
	std::cout << "Enter ssn(unformatted numeric): ";
	do
	{
		std::cin.getline(buffer, 9);
	}while (!account.sSSN(buffer));

	// Get phone 
	std::cout << "Enter phone(unformatted numeric): ";
	do
	{
		std::cin.getline(buffer, 10);
	}while (!account.sPhone(buffer));

	// Get pass
	std::cout << "Enter password: ";
	do
	{
		std::cin.getline(buffer, 6);
	}while (!account.sPass(buffer));

	// Get unique anum
	std::cout << "Enter unique account number: ";

	bool exists = true;
	do
	{
		std::cin.getline(buffer, 5);
		Aseek test(buffer, file);
		if (!strcmp("*****", test.gANum()))
				exists = false;
	}while (!account.sANum(buffer) && exists);
	file << account;
}
//void writeRpt(std::fstream&);
//void dispHelp();

