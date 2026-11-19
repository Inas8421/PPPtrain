#include "date.h"

vector<string> month_tbl = 
	{"Not a month", "January", "February", "March", "April", "May", "June", 
		"July", "August", "September", "October", "November", "December"};

int to_int(Month m)
{
	return static_cast<int>(m);
}

Month operator++(Month& m)
{
	m= (m == Month::dec) ? Month::jan : Month{to_int(m)+1};
	return m;
}

ostream& operator<<(ostream& os, Month m)
{
	return os << month_tbl[to_int(m)];
}

ostream& operator<<(ostream& os, Date d)
{
	return os << d.year() << '/'<< d.month() <<'/' << d.day();
}

int main()
try
{
	Date date {2026, Month::jun, 17};
	cout << date << '\n';
	date.add_day(2);
	cout << date << '\n';
}
catch(Date::Invalid)
{
	error("some(): invalid date");
}

