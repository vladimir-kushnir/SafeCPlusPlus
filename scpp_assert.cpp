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
