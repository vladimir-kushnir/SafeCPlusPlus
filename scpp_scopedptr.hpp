/*

 Safe C++, Or How to Avoid Most Common Mistakes in C++ Code
 by Vladimir Kushnir, (OÕReilly).

 Copyright 2012 Vladimir Kushnir, ISBN 9781449320935.

 If you feel your use of code examples falls outside fair use or the
 permission given above, feel free to contact us at permissions@oreilly.com.
 
*/

#ifndef __SCPP_SCOPEDPTR_HPP_INCLUDED__
#define __SCPP_SCOPEDPTR_HPP_INCLUDED__

#include "scpp_assert.hpp"

namespace scpp {

// Scoped pointer, takes ownership of an object, could not be copied.
template <typename T>
class ScopedPtr {
  public:

	explicit ScopedPtr(T* p = NULL)
	: ptr_(p) {
	}

	ScopedPtr<T>& operator=(T* p) {						
		if(ptr_ != p)
		{
			delete ptr_;
			ptr_ = p;
		}
							
		return *this;
	}

	~ScopedPtr() {							
		delete ptr_;
	}

	T* Get() const {						
		return ptr_;
	}

	T* operator->() const
	{						
		SCPP_TEST_ASSERT(ptr_ != NULL, "Attempt to use operator -> on NULL pointer.");
		return ptr_;
	}

	T& operator* () const { 
		SCPP_TEST_ASSERT(ptr_ != NULL, "Attempt to use operator * on NULL pointer.");
		return *ptr_;
	}
	
	// Release ownership of the object to the caller.
	T* Release() {
		T* p = ptr_;
		ptr_ = NULL;
		return p;
	}

private:
	T*	ptr_;

	// Copy is prohibited:
	ScopedPtr(const ScopedPtr<T>& rhs);
	ScopedPtr<T>& operator=(const ScopedPtr<T>& rhs);
};

} // namespace scpp

#endif // __SCPP_SCOPEDPTR_HPP_INCLUDED__
