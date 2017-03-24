/* -----------------------------------------------------------------------------

FILE:              aseek.ccp

DESCRIPTION:       Definition file for the Aseek class

COMPILER:          gcc 5.4.0

NOTES:             This class exists to encapsulate database functions for
				   the Account structure

MODIFICATION HISTORY:

Author                  Date               Version
---------------         ----------         --------------
Aaryna Irwin            2017-03-05         0.1 - Initial version
Aaryna Irwin            2017-03-10         0.2 - Added clears and cycle seeks

----------------------------------------------------------------------------- */

#include "aseek.h"
#include <iomanip>
#include <fstream>
#include <cstring>

/* -----------------------------------------------------------------------------
FUNCTION:          Aseek()
DESCRIPTION:       Default constructor for Aseek class
RETURNS:           N/A
NOTES:             None
----------------------------------------------------------------------------- */
Aseek::Aseek(const char* num, std::fstream& acc)
{
	acc.clear();
	loc = seekRec(num, acc);
}

/* -----------------------------------------------------------------------------
FUNCTION:          seekRec(istream&, const char*)
DESCRIPTION:       Seeks records in account database
RETURNS:           streampos
NOTES:             Throws exception if not found 
----------------------------------------------------------------------------- */
std::streampos Aseek::seekRec(const char* anum, std::fstream& db)
{
	int size = (A_FIELD + 1) * 9;
	int setback = (A_FIELD + 1) * 3;
	char buffer[A_FIELD];

	for (std::size_t i = 0; db; ++i)	
	{
		db.seekg((size - setback) + size * i);
		db.getline(buffer, 6, ' ');
		db.seekg(-6, std::ios_base::cur);
		if (!strcmp(buffer, anum))
			break;
	}

	if (db)
	{
		db.seekg(-(A_FIELD + 1) * 6, std::ios_base::cur);
		return db.tellg();
	}
	else
	{
		db.clear();
		db.seekg(0, std::ios_base::end);
		return db.tellg();
	}
}

/* -----------------------------------------------------------------------------
FUNCTION:          operator<<(ostream&, const Aseek&)
DESCRIPTION:       Overloaded stream insertion operator
RETURNS:           ostream&
NOTES:             None
----------------------------------------------------------------------------- */
std::ostream& operator<<(std::ostream& strm, const Aseek& acct)
{
	strm.seekp(acct.loc);
	strm.clear();
	strm << std::left << std::setw(A_FIELD) << acct.lname << std::endl;
	strm << std::setw(A_FIELD) << acct.fname << std::endl;
	strm << std::setw(A_FIELD) << acct.mi << std::endl;
	strm << std::setw(A_FIELD) << acct.ssn << std::endl;
	strm << std::setw(A_FIELD) << acct.phone << std::endl;
	strm << std::setw(A_FIELD) << acct.balance << std::endl;
	strm << std::setw(A_FIELD) << acct.anum << std::endl;
	strm << std::setw(A_FIELD) << acct.pass << std::endl;
	strm << std::setw(A_FIELD) << ' ' << std::endl;
	strm.seekp(acct.loc);
	return strm;
}

/* -----------------------------------------------------------------------------
FUNCTION:          operator>>(istream&, Aseek&)
DESCRIPTION:       Overloaded stream extraction operator
RETURNS:           istream&
NOTES:             None
----------------------------------------------------------------------------- */
std::istream& operator>>(std::istream& strm, Aseek& acct)
{
	strm.seekg(acct.loc);
	strm.clear();
	strm >> acct.lname >> acct.fname >> acct.mi >> acct.ssn >>
		acct.phone >> acct.balance >> acct.anum >> acct.pass;
	strm.seekg(acct.loc);
	return strm;
}
