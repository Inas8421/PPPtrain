#include "PPPheaders.h"

class Date
{
	public:
		class Invalid { };

		Date(int y, int m, int d);	//constructor: check for valid date and initialize
		
		void add_day(int n);		//increase the Date by n days

		int month(){return m;}

		int day(){return d;}
			
		int year(){return y;}

	private:
		int y;				//year
		int m;				//month
		int d;				//day
		
		bool is_valid();

};
