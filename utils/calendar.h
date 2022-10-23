#pragma once

#include "includes.h"

struct Date
{
	int iday;
	std::string strday;
	std::string nextday;
	int weekday;
	bool TKHoliday;
	bool LDHoliday;
	bool NYHoliday;
	bool SummerTime;

	bool operator==(const Date& obj) { return this->iday == obj.iday; };
};

class Calendar
{
	Calendar();
	Calendar(const Calendar&) {};
	Calendar& operator=(const Calendar&) {};
	std::map<int, Date>* cld;//date,businessday
public:
	void Initialize(std::string CalendarFile);
	std::map<int, Date>::iterator GetDay(int base, int offset);
	std::map<int, Date>::iterator end(void);
	int GetBusinessDay(int base, int offset);

	std::map<int, Date>::iterator CurrentDate;

	static Calendar* GetInstance(void)
	{
		static Calendar* Singleton;
		if (!Singleton)
		{
			Singleton = new Calendar();
			return Singleton;
		}
		else
		{
			return Singleton;
		}
	}
};

extern Calendar* calendar;


