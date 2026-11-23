#include "PPPheaders.h"

struct Year
{
	int y;
};

enum class Month
{
	jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
};


class Date
{
	public:
		class Invalid { };

		Date(Year y, Month m, int d);	//constructor: check for valid date and initialize
		
		void add_day(int n);		//increase the Date by n days

		Month month()const{return m;}

		int day()const{return d;}
			
		Year year()const{return y;}

	private:
		Year y;				//year
		Month m;			//month
		int d;				//day
		
		bool is_valid();

};
