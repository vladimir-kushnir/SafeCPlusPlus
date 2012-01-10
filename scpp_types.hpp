/*

 Safe C++, Or How to Avoid Most Common Mistakes in C++ Code
 by Vladimir Kushnir, (OÕReilly).
 
 Copyright 2012 Vladimir Kushnir, ISBN 9781449320935.

 If you feel your use of code examples falls outside fair use or the
 permission given above, feel free to contact us at permissions@oreilly.com.

*/

#ifndef __SCPP_TYPES_HPP_INCLUDED__
#define __SCPP_TYPES_HPP_INCLUDED__

#include <ostream>
#include "scpp_assert.hpp"

// Template wrapper around a built-in type T.
// Behaves exactly as T, except initialized by default to 0.
template<typename T>
class TNumber {
public:
	TNumber(const T& x=0)
		: data_(x) {
	}

	operator T () const { return data_; }

	TNumber& operator = (const T& x) {
		data_ = x;
		return *this;
	}

	// postfix operator x++
	TNumber operator ++ (int) {
		TNumber<T> copy(*this);
		++data_;
		return copy;
	}

	// prefix operator ++x
	TNumber& operator ++ () {
		++data_;
		return *this;
	}

	TNumber& operator += (T x) {
		data_ += x;
		return *this;
	}

	TNumber& operator -= (T x) {
		data_ -= x;
		return *this;
	}

	TNumber& operator *= (T x) {
		data_ *= x;
		return *this;
	}

	TNumber& operator /= (T x) {
		SCPP_TEST_ASSERT(x!=0, "Attempt to divide by 0");
		data_ /= x;
		return *this;
	}

	T operator / (T x) 
	{
		SCPP_TEST_ASSERT(x!=0, "Attempt to divide by 0");
		return data_ / x;
	}

private:
	T data_;
};

typedef long long int64;
typedef unsigned long long unsigned64;

typedef		TNumber<int>		Int;
typedef		TNumber<unsigned>		Unsigned;
typedef		TNumber<int64>		Int64;
typedef		TNumber<unsigned64>	Unsigned64;
typedef		TNumber<float>		Float;
typedef		TNumber<double>		Double;
typedef		TNumber<char>		Char;

class Bool {
public:
	Bool(bool x=false)
		: data_(x)
	{}

	operator bool () const { return data_; }
	Bool& operator = (bool x) {
		data_ = x;
		return *this;
	}

	Bool& operator &= (bool x) {
		data_ &= x;
		return *this;
	}

	Bool& operator |= (bool x) {
		data_ |= x;
		return *this;
	}

private:
	bool data_;
};

inline
std::ostream& operator << (std::ostream& os, Bool b) {
	if(b)
		os << "True";
	else
		os << "False";
	return os;
}

#define SCPP_DEFINE_COMPARISON_OPERATORS(Class) \
	bool operator < (const Class& that) const { return CompareTo(that) < 0; } \
	bool operator > (const Class& that) const { return CompareTo(that) > 0; } \
	bool operator ==(const Class& that) const { return CompareTo(that) ==0; } \
	bool operator <=(const Class& that) const { return CompareTo(that) <=0; } \
	bool operator >=(const Class& that) const { return CompareTo(that) >=0; } \
	bool operator !=(const Class& that) const { return CompareTo(that) !=0; } 


#endif // __SCPP_TYPES_HPP_INCLUDED__