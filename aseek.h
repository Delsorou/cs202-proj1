#ifndef ASEEK_H
#define ASEEK_H
/* -----------------------------------------------------------------------------

FILE:              aseek.h

DESCRIPTION:       Specification file for the Aseek class

COMPILER:          gcc 5.4.0

NOTES:             This class exists to encapsulate database functions for
                   the Account structure

MODIFICATION HISTORY:

Author                  Date               Version
---------------         ----------         --------------
Aaryna Irwin            2017-03-05         0.1 - Initial version

----------------------------------------------------------------------------- */

#include "account.h"
#include <fstream>

// Structure database field size (CHANGE WITH CAUTION - CAN CORRUPT DATABASE)
#define A_FIELD 20

/* -----------------------------------------------------------------------------
CLASS:             Aseek
DESCRIPTION:       Class to encapsulate database formatting functions for
                   Account structure
NOTES:             None
----------------------------------------------------------------------------- */
class Aseek : public Account
{
	// Default private member variable to hold this record's stream position
	std::streampos loc;

	// Private member function prototypes
	std::streampos seekRec(const char*, std::fstream&);
public:
	// Default constructor
	Aseek(const char*, std::fstream&);

	// Accessor
	std::streampos gLoc()
		{ return loc; };

	// Mutator
	void sLoc(const char* anum, std::fstream& db)
		{ loc = seekRec(anum, db); };

	// Structure operator overload prototypes - for formatting
	friend std::ostream& operator<<(std::ostream&, const Aseek&);
	friend std::istream& operator>>(std::istream&, Aseek&);
};
#endif // ASEEK_H
