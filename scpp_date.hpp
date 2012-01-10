/*
 
 Safe C++, Or How to Avoid Most Common Mistakes in C++ Code
 by Vladimir Kushnir, (O’Reilly).

 Copyright 2012 Vladimir Kushnir, ISBN 9781449320935.

 If you feel your use of code examples falls outside fair use or the
 permission given above, feel free to contact us at permissions@oreilly.com.
 
*/

#ifndef __SCPP_DATE_HPP_INCLUDED__
#define __SCPP_DATE_HPP_INCLUDED__

#include <iostream>
#include <string>

#include "scpp_assert.hpp"
#include "scpp_types.hpp"

/* 
	Date class.
	Features:
		All date arithmetic operators and comparisons are provided.
		Date arithmetic is implemented as an integer arithmetic.
		No Y2K problems -- all years must be >= 1900.
		Default output format is American (MM/DD/YYYY).
		In debug one can see the date in debugger as yyyymmdd --
			just point your debugger to a yyyymmdd_ data member.

	No implicit type conversions are allowed.

*/
namespace scpp {
class Date {
public:
	// Creates an empty (invalid in terms of IsValid()) date.
	Date();

	// Input format: "mm/dd/yyyy".
	explicit Date(const char* str_date);

	// Same as above.
	explicit Date(const std::string& str_date);

	// Date from integer in the YYYYMMDD format, e.g. Dec. 26, 2011 is 20111226.
	explicit Date(unsigned yyyymmdd);

	// Year must be 4-digit,
	// month is 1-based, i.e. 1 .. 12, 
	// day is 1 .. MonthLength() <= 31
	Date(unsigned year, unsigned month, unsigned day);

	// Returns true if the date is not empty,
	// as is the case when it is created by the default constructor.
	// Most operations on invalid date are not allowed 
	// (will call error handler).
	bool IsValid() const { return date_!=0; }

	// Returns date in YYYYMMDD format, e.g. Dec. 26, 2011 is 20111226.
	unsigned AsYYYYMMDD() const;

	// 4-digit year.
	unsigned Year() const;

	enum { JAN=1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };
	// Returns month number JAN .. DEC, i.e. 1..12.
	unsigned Month() const;

	// Day of month 1 .. MonthLength() <= 31.
	unsigned DayOfMonth() const;

	static bool IsLeap(unsigned year);

	typedef enum { SUN, MON, TUE, WED, THU, FRI, SAT } DayOfWeekType;
	// Returns day of week SUN .. SAT.
	DayOfWeekType DayOfWeek() const;

	// "Sunday", "Monday" .. "Saturday".
	const char* DayOfWeekStr() const;

	int Data() const { return date_; }

	typedef enum { FRMT_AMERICAN,   // MM/DD/YYYY
			       FRMT_EUROPEAN	// MM.DD.YYYY
						// one can add formats in here if necessary.
			} DateOutputFormat; 

	enum { MIN_BUFFER_SIZE=11 };
	// The function prints a date into a user-provided buffer
	// and returns the same buffer.
	// Make sure the buffer size >= MIN_BUFFER_SIZE chars at least.
	char* AsString(char* buffer, unsigned bufLen,
			       DateOutputFormat frmt=FRMT_AMERICAN) const;

	// Same as above, but C++ style.
	std::string AsString(DateOutputFormat frmt=FRMT_AMERICAN) const;

	// Returns negative int, 0 or positive int in cases of *this<d, *this==d and *this>d.
	int CompareTo(const Date& d) const {
		SCPP_TEST_ASSERT(IsValid(), "Date is not valid")
		SCPP_TEST_ASSERT(d.IsValid(), "Date is not valid")
		
		return date_ - d.date_;
	}

    SCPP_DEFINE_COMPARISON_OPERATORS(Date)

	Date& operator ++ () {
		++date_;
		SyncDebug();
		return *this;
	}

	Date operator ++ (int) {
		Date copy(*this);
		++(*this);
		return copy;
	}

	Date& operator -- () {
		--date_;
		SyncDebug();
		return *this;
	}

	Date operator -- (int) {
		Date copy(*this);
		--(*this);
		return copy;
	}

	Date& operator += (int nDays) {
		date_ += nDays;
		SyncDebug();
		return *this;
	}

	Date& operator -= (int nDays) {
		(*this) += (-nDays);
		return *this;
	}

private:
	int date_; // number of days from A.D., i.e. 01/01/0001 is 1.

#ifdef _DEBUG
	int yyyymmdd_;
#endif

	void SyncDebug() {
#ifdef _DEBUG
		yyyymmdd_ = AsYYYYMMDD();
#endif
	}

void SyncDebug(unsigned year, unsigned month, unsigned day) {
#ifdef _DEBUG
		yyyymmdd_ = 10000*year + 100*month + day;
#endif
	}
	
	// Returns month's length in days, 
	// input: month = 1 .. 12
	static unsigned MonthLength(unsigned month, unsigned year);
	
	// Returns number of calendar days before beginning of the month,
	// e.g. for JAN - 0, 
	//      for FEB - 31,
	//      for MAR - 59 or 60 depending on the leap year.
	static unsigned NumberOfDaysBeforeMonth(unsigned month, unsigned year);
};
} // namespace scpp

inline std::ostream& operator<<(std::ostream& os, const scpp::Date& d) {
	char buffer[scpp::Date::MIN_BUFFER_SIZE];
	os << d.AsString(buffer, scpp::Date::MIN_BUFFER_SIZE);
	return os;
}

inline scpp::Date operator + (const scpp::Date& d, int nDays) {
	scpp::Date copy(d);
	return (copy += nDays);
}

inline scpp::Date operator - (const scpp::Date& d, int nDays) {
	scpp::Date copy(d);
	return (copy -= nDays);
}

inline int operator - (const scpp::Date& lhs, const scpp::Date& rhs) {
	return lhs.Data() - rhs.Data();
}
#endif // __SCPP_DATE_HPP_INCLUDED__
