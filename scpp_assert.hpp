/*

 Safe C++, Or How to Avoid Most Common Mistakes in C++ Code
 by Vladimir Kushnir, (OÕReilly).
 
 Copyright 2012 Vladimir Kushnir, ISBN 9781449320935.
 
 If you feel your use of code examples falls outside fair use or the
 permission given above, feel free to contact us at permissions@oreilly.com.
 
*/

#ifndef __SCPP_ASSERT_HPP_INCLUDED__
#define __SCPP_ASSERT_HPP_INCLUDED__

#include <sstream> // ostringstream

#ifdef SCPP_THROW_EXCEPTION_ON_BUG
#include <exception>

namespace scpp {
// This exception is thrown when the sanity checks defined below fail,
// and #ifdef SCPP_THROW_EXCEPTION_ON_BUG.
class ScppAssertFailedException : public std::exception {
 public:
	ScppAssertFailedException(const char* file_name,
							  unsigned line_number,
							  const char* message);
	
	virtual const char* what() const throw () { return what_.c_str(); }
							  
	virtual ~ScppAssertFailedException() throw () {}
 private:
  std::string what_;
};
} // namespace scpp
#endif

void SCPP_AssertErrorHandler(const char* file_name,
							 unsigned line_number,
							 const char* message);

// Permanent sanity check macro.
#define SCPP_ASSERT(condition, msg)                 \
    if(!(condition)) {                              \
        std::ostringstream s;                       \
        s << msg;                                   \
        SCPP_AssertErrorHandler(                    \
            __FILE__, __LINE__, s.str().c_str() );  \
	}

#ifdef _DEBUG
#	define SCPP_TEST_ASSERT_ON
#endif

// Temporary (for testing only) sanity check macro
#ifdef SCPP_TEST_ASSERT_ON
#	define SCPP_TEST_ASSERT(condition,msg) SCPP_ASSERT(condition, msg)
#else
#	define SCPP_TEST_ASSERT(condition,msg) // do nothing
#endif

#endif // __SCPP_ASSERT_HPP_INCLUDED__
