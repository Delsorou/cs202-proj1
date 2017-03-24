/* -----------------------------------------------------------------------------

FILE:              account.ccp

DESCRIPTION:       Definition file for the Account structure

COMPILER:          gcc 5.4.0

NOTES:             None

MODIFICATION HISTORY:

Author                  Date               Version
---------------         ----------         --------------
Aaryna Irwin            2017-02-21         0.1 - setVar function 
Aaryna Irwin            2017-02-22         0.2 - remove setVar, use strcpy
Aaryna Irwin            2017-02-23         0.2.1 - ... put setVar back...
Aaryna Irwin            2017-02-26         0.3 - Added default constructor
Aaryna Irwin            2017-02-27         0.4 - << >> overloaded
Aaryna Irwin            2017-03-05         0.5 - Moved overloads to Aseek
                                           class to isolate db functions
Aaryna Irwin            2017-03-11         0.6 - Added overload for ssn/phone
Aaryna Irwin            2017-03-12         0.7 - Added input validation

----------------------------------------------------------------------------- */

#include "account.h"
#include <cctype>
#include <cstring>

/* -----------------------------------------------------------------------------
FUNCTION:          Account()
DESCRIPTION:       Default constructor initializing the Account structure
RETURNS:           N/A
NOTES:             None
----------------------------------------------------------------------------- */
Account::Account()
{
	mi = '*';
	const char* iname = "****************";
	setVar(lname, iname);
	setVar(fname, iname);
	setVar(ssn, "***********");
	setVar(phone, "*************");
	setVar(anum, "*****");
	setVar(pass, "******");
	balance = 0.0;
}

/* -----------------------------------------------------------------------------
FUNCTION:          setVar(char*, const char*)
DESCRIPTION:       Local version of strcpy()
RETURNS:           bool
NOTES:             Incorporates code for input validation
----------------------------------------------------------------------------- */
bool Account::setVar(char* o, const char* r)
{
	bool valid = true;
	int i = 0;

	do
	{
		valid = isalnum(r[i]);
		if (r[i] == '*' || r[i] == '\0')
			valid = true;

		*(o + i) = *(r + i);
	}while (*(r + i++) != '\0' && valid == true);
//	@@ Add code here to prevent o from changing if invalid, currently
//	is partially modified, result undefined, could screw up database
	return valid;
}

/* -----------------------------------------------------------------------------
FUNCTION:          setVar(char*, const char*, const char)
DESCRIPTION:       Local version of std::strcpy()
RETURNS:           void
NOTES:             Overloaded for ssn/phone formatting
----------------------------------------------------------------------------- */
bool Account::setVar(char* o, const char* r, const char t)
{
	bool valid = true;
	int i = 0, sb = 0;

	do
	{
		valid = isdigit(r[i]);
		if (r[i] == '*' || r[i] == '\0')
			valid = true;

		if (t == 's' && (i == 3 || i == 5))
		{
			*(o + i + sb) = '-';
			++sb;
		}
		else if (t == 'p' && (i == 0 || i == 3 || i == 6))
		{
			switch (i)
			{
				case 0:
					*(o + i + sb) = '(';
					break;
				case 3:
					*(o + i + sb) = ')';
					break;
				case 6:
					*(o + i + sb) = '-';
					break;
			}
			++sb;
		}
		*(o + i + sb) = *(r + i);
	}while (*(r + i++) != '\0' && valid == true);
//	@@ Add code here to prevent o from changing if invalid, currently
//	is partially modified, result undefined, could screw up database

	return valid;
}

/* -----------------------------------------------------------------------------
FUNCTION:          sMI(const char)
DESCRIPTION:       Sets the middle initial
RETURNS:           bool
NOTES:             None
----------------------------------------------------------------------------- */
bool Account::sMI(const char r)
{
	if (isalpha(r) || r == '*')
	{
		mi = r;
		return true;
	}
	else
		return false;
}
