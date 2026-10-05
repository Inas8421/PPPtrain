/*
	Simple calculator

	This program implements a basic expression calculator.
	Input from cin; output to cout.
	The grammar for input is:

	Statement:
		Expression
		Print
		Quit
	Print:
		";"
	Quit:
		"q"
	Expression:
		Term
		Expression "+" Term
		Expression "-" Term
	Term:
		Primary
		Term "*" Primary
		Term "/" Primary
		Term "%" Primary
	Primary:
		Number
		"(" Expression ")"
		"-" Primary
		"+" Primary
	Number:
		floating-point-literal

	Input comes from cin through the Token_stream called ts.
*/

#include "PPPheaders.h"

//-----------------------------------------------------------------------

constexpr char number = '8';	//t.kind == number means that t is a number Token
constexpr char quit = 'q';	//t.kind == quit means that t is a quit Token
constexpr char print = ';';	//t.kind == print means that t is a print Token
constexpr string prompt = "> ";
constexpr string result = "= ";	//used to indicate that what follows is a result

//-----------------------------------------------------------------------

class Token
{
	public:
		char kind;			// what kind of token
		double value;			// for numbers: a value
		Token(char ch)			// make a Token from a char
			:kind(ch), value(0){}
		Token(char ch, double val)	//make a Token from a char and a double
			:kind(ch), value(val){}
};

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

//The constructor just sets full to indicate that the buffer is empty
Token_stream::Token_stream()
	:full(false), buffer(0)	//no Token in buffer
{}

//--------------------------------------------------------------------------

//The putback() member function puts its argument back into the Token_stream's buffer
void Token_stream::putback(Token t)
{
	if(full)
		error("putback() into a full buffer");

	buffer = t;		//copy t to buffer
	full = true;		//buffer is now full
}

//--------------------------------------------------------------------------

//The ignore(char c) member function discards characters up to and including a c
void Token_stream::ignore(char c)		//c represents the kind of Token
{
	if(full && c == buffer.kind)		//first look in buffer
	{
		full = false;
		return;
	}

	full = false;

	//now search input:
	char ch = 0;
	while(cin >> ch)
		if(ch == c)
			return;	
}

//--------------------------------------------------------------------------

//The get() member function
Token Token_stream::get()
{
	if(full)	//do we already have a Token ready?
	{
		//remove Token from buffer
		full = false;
		return buffer;
	}

	char ch;
	if(!(cin >> ch))	//note that >> skips whitespace (space, newline, tab
		error("no input");
	switch(ch)
	{
		case print: //for "print"
		case quit: //for "quit"
		case '(':
		case ')':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			return Token{ch}; //let each character represent itself
		case '.':	//a floating-point literal can start with a dot
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			cin.putback(ch);	//put digit back into the input stream
			double val=0;		 
			cin >> val;		//read a floating-point number
			return Token{number, val};
		}
		default:
			error("Bad token");
		//compilation warning here, because of no return value
	}
}

//-----------------------------------------------------------------

Token_stream ts;	//provides get() and putback()

//----------------------------------------------------------

double expression();	//read and evaluate an Expression

//----------------------------------------------------------

//double term();		//read and evaluate a Term

//----------------------------------------------------------

//Deal with numbers and parentheses
double primary() 
{
	Token t = ts.get();
	switch(t.kind)
	{
		case '(':	//handle '(' expression ')'
		{
			double d = expression();
			t = ts.get();
			if(t.kind != ')')
				error("')' expected !");
			return d;
		}
		case number:
			return t.value;		//return the number's value
		case '-':
			return - primary();
		case '+':
			return primary();
		default:
			error("primary expected !");
			//compilation warning here, because of no return value
	}
}

//------------------------------------------------------------

//Deal with *, /, and %
double term()
{
	double left = primary();
	Token t = ts.get();	//get the next token from token stream
	while(true)
	{
		switch(t.kind)
		{
			case '*':
				left *= primary();
				t = ts.get();
				break;
			case '/':
			{
				double d = primary();
				if(d == 0)
					error("division by zero !");
				left /= d;
				t = ts.get();
				break;
			}
			case '%':
			{
				double d = primary();
				if(d == 0)
					error("%: division by zero !");
				left = fmod(left, d);
				t = ts.get();
				
//				int i1 = narrow<int>(left);
//				int i2 = narrow<int>(primary());
//				if(i2 == 0)
//					error("%: division by zero !");
//				left = i1%i2;
//				t = ts.get();
				break;
			}
			default:
				ts.putback(t);	//put t back into the token stream
				return left;
		}
	}	
}

//--------------------------------------------------------------

//Deal with + and -
double expression()
{
	double left = term();	//read and evaluate a Term
	Token t = ts.get();	//get the next token
	while(true)
	{
		switch(t.kind)
		{
			case '+':
				left += term();	//evaluate term and add
				t = ts.get();
				break;
			case '-':
				left -= term();	//evaluate term and substract
				t = ts.get();
				break;
			default:
				ts.putback(t);	//put t back into the token stream
				return left;	//finally: no more + or -: return the answer
		}
	}

	return left;
}

//----------------------------------------------------------------

//Error handling
void clean_up_mess()
{
	ts.ignore(print);
}

//----------------------------------------------------------------

//Calculation loop
void calculate()
{
	while(cin)
	try
	{
		cout << prompt;		//print promt
		Token t = ts.get();
		while(t.kind == print)
			t = ts.get();	//eat ';'
		if(t.kind == quit)	//'q' for quit
			 return;
		ts.putback(t);
		cout << result << expression() << '\n';
	}
	catch(exception& e)
	{
		cerr << "calculate() error: " << e.what() << '\n';
		clean_up_mess();
	}
}

//----------------------------------------------------------------

int main()
try
{
	calculate();
	return 0;
}
catch(exception& e)
{
	cerr << "error: " << e.what() << '\n';
	return 1;
}
catch(...)
{
	cerr << "Oops: unknown exception!\n";
	return 2;
}
