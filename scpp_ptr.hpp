/*

 Safe C++, Or How to Avoid Most Common Mistakes in C++ Code
 by Vladimir Kushnir, (OÕReilly).

 Copyright 2012 Vladimir Kushnir, ISBN 9781449320935.

 If you feel your use of code examples falls outside fair use or the
 permission given above, feel free to contact us at permissions@oreilly.com.
 
*/

#ifndef __SCPP_PTR_HPP_INCLUDED__
#define __SCPP_PTR_HPP_INCLUDED__

#include "scpp_assert.hpp"

namespace scpp {

// Template pointer, does not take ownership of an object.
template <typename T>
class Ptr {
  public:

	explicit Ptr(T* p = NULL)
	: ptr_(p) {
	}

	T* Get() const {						
		return ptr_;
	}

	Ptr<T>& operator=(T* p) {
		ptr_ = p;
		return *this;
	}

	T* operator->() const {						
		SCPP_TEST_ASSERT(ptr_ != NULL, "Attempt to use operator -> on NULL pointer.");
		return ptr_;
	}

	T& operator* () const { 
		SCPP_TEST_ASSERT(ptr_ != NULL, "Attempt to use operator * on NULL pointer.");
		return *ptr_;
	}

private:
	T*	ptr_;
};

} // namespace scpp

#endif // __SCPP_PTR_HPP_INCLUDED__
