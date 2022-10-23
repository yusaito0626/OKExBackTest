#include "calendar.h"

Calendar* calendar = Calendar::GetInstance();

Calendar::Calendar()
{

}

void Calendar::Initialize(std::string CalendarFile)
{
	cld = new std::map<int, Date>();
	std::ifstream ifs(CalendarFile);
	std::string line;
	while (std::getline(ifs, line))
	{
		std::stringstream ss(line);
		std::string elem;
		int i = 0;
		Date dt;
		while (std::getline(ss, elem, ','))
		{
			switch (i)
			{
			case 0:
				dt.iday = stoi(elem);
				break;
			case 1:
				dt.weekday = stoi(elem);
				break;
			case 2:
				dt.strday = elem;
				break;
			case 3:
				dt.nextday = elem;
				break;
			case 4:
				dt.TKHoliday = stoi(elem);
				break;
			case 5:
				dt.LDHoliday = stoi(elem);
				break;
			case 6:
				dt.NYHoliday = stoi(elem);
				break;
			case 7:
				dt.SummerTime = stoi(elem);
				break;
			default:
				break;
			}
			++i;
		}
		cld->emplace(dt.iday, dt);
	}
	CurrentDate = cld->end();
}

std::map<int, Date>::iterator Calendar::GetDay(int base, int offset)
{
	std::map<int, Date>::iterator it = cld->find(base);
	if (it == cld->end())
	{
		return cld->end();
	}
	else
	{
		if (offset > 0)
		{
			for (int i = 0; i < offset; ++i)
			{
				if (it == cld->end())
				{
					return cld->end();
				}
				++it;
			}
			return it;
		}
		else if (offset < 0)
		{
			for (int i = 0; i < -offset; ++i)
			{
				if (it == cld->begin())
				{
					return cld->end();
				}
				--it;
			}
			return it;
		}
		else
		{
			return it;
		}
	}
}

std::map<int, Date>::iterator Calendar::end(void)
{
	return cld->end();
}