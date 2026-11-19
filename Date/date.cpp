#include "date.h"

Date::Date(int yy, Month mm, int dd)
	:y{yy}, m{mm}, d{dd}
{
	if(!is_valid())
	{
		throw Invalid{};
	}
}


void Date::add_day(int n)
{
	
}

bool Date::is_valid()
{
	return true;
}
