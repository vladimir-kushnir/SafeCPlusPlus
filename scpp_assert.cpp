/*

 Safe C++, Or How to Avoid Most Common Mistakes in C++ Code
 by Vladimir Kushnir, (O’Reilly).
 
 Copyright 2012 Vladimir Kushnir, ISBN 9781449320935.
 
 If you feel your use of code examples falls outside fair use or the
 permission given above, feel free to contact us at permissions@oreilly.com.
 
*/

#include "scpp_assert.hpp"

#include <iostream>	// cerr, endl, flush
#include <stdlib.h>	// exit()


using namespace std;

#ifdef SCPP_THROW_EXCEPTION_ON_BUG
namespace scpp {
	ScppAssertFailedException::ScppAssertFailedException(const char* file_name,
														 unsigned line_number,
														 const char* message) {
		ostringstream s;
		s << "SCPP assertion failed with message '" << message
		  << "' in file " << file_name << " #" << line_number;
		
		what_ = s.str();
	}
}
#endif

void SCPP_AssertErrorHandler(const char* file_name,
							 unsigned line_number,
							 const char* message) {
	// This is a good place to put your debug breakpoint:
	// You can also add writing of the same info into a log file if appropriate.
	
#ifdef SCPP_THROW_EXCEPTION_ON_BUG
	throw scpp::ScppAssertFailedException(file_name, line_number, message);
#else
	cerr << message << " in file " << file_name << " #" << line_number << endl << flush;
	// Terminate application
	exit(1);
#endif
}
