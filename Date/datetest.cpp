#include "date.h"

ostream& operator<<(ostream& os, Date d)
{
	return os << d.year() << '/'<< d.month() <<'/' << d.day();
}

void some(int m, int d)
try
{
	Date date {2026, m, d};
	cout << date << '\n';
	date.add_day(2);
}
catch(Date::Invalid)
{
	error("some(): invalid date");
}

int main()
{
	some(2, 27);
}
