/*

 Safe C++, Or How to Avoid Most Common Mistakes in C++ Code
 by Vladimir Kushnir, (OÕReilly).

 Copyright 2012 Vladimir Kushnir, ISBN 9781449320935.

 If you feel your use of code examples falls outside fair use or the
 permission given above, feel free to contact us at permissions@oreilly.com.

*/

#ifndef __SCPP_REFCOUNTPTR_HPP_INCLUDED__
#define __SCPP_REFCOUNTPTR_HPP_INCLUDED__

#include "scpp_assert.hpp"

namespace scpp {

// Reference-counting pointer.  Takes ownership of an object.  Can be copied.
template <typename T>
class RefCountPtr {
  public:

	explicit RefCountPtr(T* p = NULL) {
		Create(p);	
	}

	RefCountPtr(const RefCountPtr<T>& rhs) {
		Copy(rhs);					
	}

	RefCountPtr<T>& operator=(const RefCountPtr<T>& rhs) {
		if(ptr_ != rhs.ptr_) {
			Kill();
			Copy(rhs);
		}
									
		return *this;
	}

	RefCountPtr<T>& operator=(T* p) {						
		if(ptr_ != p) {
			Kill();
			Create(p);
		}
							
		return *this;
	}

	~RefCountPtr() {							
		Kill();
	}

	T* Get() const { return ptr_; }

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
	int*	count_;

	void Create(T* p) {
		ptr_ = p;
		if(ptr_ != NULL) {
			count_ = new int;
			*count_ = 1;
		} else {
			count_ = NULL;
		}
	}

	void Copy(const RefCountPtr<T>& rhs) {
		ptr_ = rhs.ptr_;
		count_ = rhs.count_;
		if(count_ != NULL)
			++(*count_);
	}

	void Kill() {
		if(count_ != NULL) {
			if(--(*count_) == 0) {
				delete ptr_;
				delete count_;
			}
		}
	}

};
} // namespace scpp

#endif // __SCPP_REFCOUNTPTR_HPP_INCLUDED__
