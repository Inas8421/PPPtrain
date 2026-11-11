#include "PPPheaders.h"

class Token
{
	public:
		char kind;						//what kind of token
		double value;						//for numbers: a value
		string name;						//for Variables: name
		Token():kind{0}{}					//default constructor
		Token(char ch):kind{ch}{}				//initialize kind with ch	
		Token(char ch, double val):kind{ch}, value{val}{}	//initialize kind and value
		Token(char ch, string n):kind{ch}, name{n}{}		//initialize kind and name
};


class Token_stream
{
	public:
		Token_stream();		//make a Token_stream that reads from cin
		Token get();		//get a Token
		void putback(Token t);	//put a Token back
		void ignore(char c);	//discard characters up to and including a c
	private:
		bool full = false;	//is there a Token in buffer?
		Token buffer;		//here is where we keep a Token put back
};

