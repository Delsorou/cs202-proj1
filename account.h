#ifndef ACCOUNT_H
#define ACCOUNT_H
/* -----------------------------------------------------------------------------

FILE:              account.h

DESCRIPTION:       Specification file for the Account structure

COMPILER:          gcc 5.4.0

NOTES:             None

MODIFICATION HISTORY

Author                  Date               Version
---------------         ----------         --------------
Aaryna Irwin            2017-02-17         0.1 - Initial version
Aaryna Irwin            2017-02-18         0.2 - Added include guard/comments
Aaryna Irwin            2017-02-21         0.3 - Added acc/mut functions
Aaryna Irwin            2017-02-22         0.3.1 - All acc/mut, custom strcpy
Aaryna Irwin            2017-03-05         0.5 - Operator overloads moved
                                           to Aseek class for db interface
Aaryna Irwin            2017-03-13         0.6 - Modified for input validation

----------------------------------------------------------------------------- */

struct Account
{
protected:
	// Account holder's name
	char lname[17],
		fname[17],
		mi;

	// Account holder's personal info (ssn/phone#)
	char ssn[12],
		phone[14];

	// Current account balance
	double balance;

	// Account number and password
	char anum[6],
		pass[7];

	// Protected member functions
	bool setVar(char*, const char*);
	bool setVar(char*, const char*, const char); 

public:
	// Default constructor
	Account();

	// Structure accessor functions
	const char gMI() const
   		{ return mi; };
	const char* gLName() const
   		{ return lname; };
	const char* gFName() const
   		{ return fname; };
	const char* gSSN() const
   		{ return ssn; };
	const char* gPhone() const
   		{ return phone; };
	const char* gANum() const
   		{ return anum; };
	const char* gPass() const
   		{ return pass; };
	const double gBalance() const
   		/* You must */ { return balance; }; // to the Force.

	// Structure mutator functions
	bool sMI(const char);
	bool sLName(const char* r)
   		{ return setVar(lname, r); };
	bool sFName(const char* r)
   		{ return setVar(fname, r); };
	bool sSSN(const char* r)
   		{ return setVar(ssn, r, 's'); };
	bool sPhone(const char* r)
   		{ return setVar(phone, r, 'p'); };
	bool sANum(const char* r)
   		{ return setVar(anum, r); };
	bool sPass(const char* r)
   		{ return setVar(pass, r); };
	bool sBalance(const double r)
		{ return (r > 0) && (balance = r) ? true : false; };
	bool deposit(const double r)
   		{ return (r > 0) && (balance += r) ? true : false; };
	bool withdraw(const double r)
		{ return (balance >= r) && (balance -= r) ? true : false; }; 
};
#endif // ACCOUNT_H
