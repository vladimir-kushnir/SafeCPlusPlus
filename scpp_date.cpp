/*

 Safe C++, Or How to Avoid Most Common Mistakes in C++ Code
 by Vladimir Kushnir, (O’Reilly).

 Copyright 2012 Vladimir Kushnir, ISBN 9781449320935.
 
 If you feel your use of code examples falls outside fair use or the
 permission given above, feel free to contact us at permissions@oreilly.com.

*/

#include "scpp_date.hpp"

#include <string.h>  // strlen
#include <stdlib.h>  // atoi

namespace scpp {
Date::Date()
: date_(0)
{
#ifdef _DEBUG
	yyyymmdd_ = 0;
#endif
}

Date::Date(const char* str_date) {
	SCPP_ASSERT(str_date!=NULL, "Date(): string argument=0.")

	// must be mm/dd/yyyy, at least m/d/yyyy
	SCPP_TEST_ASSERT(strlen(str_date)>=8, "Bad Date input: '" << str_date << "'.")

	unsigned mm, dd=0, yyyy=0;

	mm = atoi(str_date);
	for(const char* p=str_date; (*p)!='\0'; ++p) {
		if(*p=='/') {
			if(dd==0)
				dd = atoi(p+1);
			else {
				yyyy = atoi(p+1);
				break;
			}
		}
	}

	SCPP_TEST_ASSERT(mm!=0 && dd!=0 && yyyy!=0, "Bad Date input '" << str_date << "', must be MM/DD/YYYY.");

	*this = Date(yyyy, mm, dd);
}

Date::Date(const std::string& str) {
	*this = Date(str.c_str());
}

Date::Date(unsigned yyyymmdd) {
	int yyyy = yyyymmdd / 10000;
	int mmdd = yyyymmdd - 10000 * yyyy;
	int mm = mmdd / 100;
	int dd = mmdd - 100 * mm;

	*this = Date(yyyy, mm, dd);
}

Date::Date(unsigned year, unsigned month, unsigned day) {
	SCPP_TEST_ASSERT(year>=1900, "Year must be >=1900.")
	SCPP_TEST_ASSERT(JAN<=month && month<=DEC, "Wrong month " << month << " must be 1..12.")
#ifdef SCPP_TEST_ASSERT_ON
	unsigned ml = MonthLength(month, year);
	SCPP_TEST_ASSERT(1<=day && day<=ml, "Wrong day: " << day << " must be 1.." << ml << ".");
#endif
	int n_years_before = year-1;
	date_ = 365*n_years_before 
		+ n_years_before/4 - n_years_before/100 + n_years_before/400
		+ day + NumberOfDaysBeforeMonth(month, year);

	SyncDebug(year, month, day);
}

unsigned Date::AsYYYYMMDD() const {
	unsigned y = Year();
	unsigned m = Month();
	unsigned d = Data() - Date(y, m, 1).Data() + 1;

	return y*10000 + m*100 + d;
}

bool Date::IsLeap(unsigned year) {
	if(year%4)
		return false;

	if(year%400 == 0)
		return true;

	if(year%100 == 0)
		return false;

	return true;
}

Date::DayOfWeekType Date::DayOfWeek() const {
	return (DayOfWeekType)(date_ % 7);
}

const char* Date::DayOfWeekStr() const {
	static const char* str_day_of_week[] = { 
		"Sunday", "Monday", "Tuesday", "Wednesday", 
		"Thursday", "Friday", "Saturday" };

	DayOfWeekType dow = DayOfWeek();
	return str_day_of_week[(unsigned)dow];	
}

// static
unsigned Date::MonthLength(unsigned month, unsigned year) {
	static int month_length[13] = { 0, 31,28,31,30,31,30,31,31,30,31,30,31 };
	SCPP_TEST_ASSERT(year>=1900, "Wrong year: " << year << ", must be >=1900.");
	SCPP_TEST_ASSERT(JAN <= month && month <= DEC, "Wrong month " << month);
	if(month == FEB && IsLeap(year))
		return 29;
	return month_length[month];
}

// static
unsigned Date::NumberOfDaysBeforeMonth(unsigned month, unsigned year) {
	static int days_before_month[12] = { 0, 31,59,90,120,151,181,212,243,273,304,334 };
	SCPP_TEST_ASSERT(year>=1900, "Wrong year: " << year << ", must be >=1900.");
	SCPP_TEST_ASSERT(JAN <= month && month <= DEC, "Wrong month " << month);
	unsigned days_before = days_before_month[month - 1];
	if (month >= MAR && IsLeap(year)) 
		++days_before;
	return days_before;
}
	
unsigned Date::Year() const {
	SCPP_TEST_ASSERT(IsValid(), "Date is not valid")

	unsigned y = Data() / 365;
	while(Date(y,1,1).Data() > Data())
		--y;
	return y;
}

unsigned Date::Month() const {
	SCPP_TEST_ASSERT(IsValid(), "Date is not valid")

	unsigned y = Year();
	Date endOfLastYear(y-1, DEC, 31);
	unsigned day = Data() - endOfLastYear.Data();
	for(unsigned m=JAN; m<=DEC; ++m)
	{
		unsigned ml = MonthLength(m, y);
		if(day <= ml)
			return m;
		day -= ml;
	}
	SCPP_ASSERT(false, "Fatal algorith error.")
	return 0;
}

unsigned Date::DayOfMonth() const {
	SCPP_TEST_ASSERT(IsValid(), "Date is not valid")

	unsigned y = Year();
	unsigned m = Month();
	unsigned d = Data() - Date(y, m, 1).Data() + 1;
	SCPP_TEST_ASSERT(d > 0 && d <= MonthLength(m,y),
		"Wrong day " << d << " of month " << m << " year " << y );
	return d;
}

char* Date::AsString(char* buffer,  unsigned bufLen, DateOutputFormat frmt) const {
	SCPP_TEST_ASSERT(IsValid(), "Date is not valid")
	SCPP_TEST_ASSERT(bufLen>=MIN_BUFFER_SIZE,
		"Buffer is too short: " << bufLen << " must be at least " << MIN_BUFFER_SIZE)

	unsigned y = Year();
	unsigned m = Month();
	unsigned d = Data() - Date(y, m, 1).Data() + 1;

	switch(frmt) {
		case FRMT_AMERICAN: 
			sprintf(buffer, "%02d/%02d/%04d", m, d, y);
			break;
	
		case FRMT_EUROPEAN: 
			sprintf(buffer, "%02d.%02d.%4d", m, d, y);
			break;

		default:
			SCPP_ASSERT(false, "Wrong output format " << frmt); 
	}

	return buffer;
}

std::string Date::AsString(DateOutputFormat frmt) const {
	char buffer[ 12 ];
	return AsString(buffer, sizeof(buffer), frmt);
}
} // namespace scpp

