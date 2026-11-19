#include "PPPheaders.h"

enum class Month
{
	jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
};


class Date
{
	public:
		class Invalid { };

		Date(int y, Month m, int d);	//constructor: check for valid date and initialize
		
		void add_day(int n);		//increase the Date by n days

		Month month(){return m;}

		int day(){return d;}
			
		int year(){return y;}

	private:
		int y;				//year
		Month m;			//month
		int d;				//day
		
		bool is_valid();

};
