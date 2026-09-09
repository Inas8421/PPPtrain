#include "PPPheaders.h"

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

Token get_token()
{
	char ch;
	cin >> ch;

	switch(ch)
	{
		//not yet case ';': //for "print"
		//not yet case 'q': //for "quit"
		case '(':
		case ')':
		case '+':
		case '-':
		case '*':
		case '/':
			return Token(ch); //let each character represent itself
		case '.':
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
			double val;		 
			cin >> val;		//read a floating-point number
			return Token('8', val);	//let '8' represent "a number"
		}
		default:
			error("Bad token");
		//compilation warning here, because of no return value
	}
}

//----------------------------------------------------------
double expression();	//read and evaluate an Expression
//----------------------------------------------------------
double term();		//read and evaluate a Term
//----------------------------------------------------------
double primary()	//read and evaluate a Primary 
{
	Token t = get_token();
	switch(t.kind)
	{
		case '(':	//handle '(' expression ')'
		{
			double d = expression();
			t = get_token();
			if(t.kind != ')')
				error("')' expected !");
			return d;
		}
		case '8':	// we use '8' to represent number
			return t.value;
		default:
			error("primary expected !");
			//compilation warning here, because of no return value
	}
}

double term()
{
	double left = primary();
	Token t = get_token();	//get the next token
	while(true)
	{
		switch(t.kind)
		{
			case '*':
				left *= primary();
				t = get_token();
				break;
			case '/':
			{
				double d = primary();
				if(d == 0)
					error("division by zero !");
				left /= d;
				t = get_token();
				break;
			}
			default:
				return left;
		}
	}	
}

double expression()
{
	double left = term();	//read and evaluate a Term
	Token t = get_token();	//get the next token
	while(true)
	{
		switch(t.kind)
		{
			case '+':
				left += term();	//evaluate term and add
				t = get_token();
				break;
			case '-':
				left -= term();	//evaluate term and substract
				t = get_token();
				break;
			default:
				return left;	//finally: no more + or -: return the answer
		}
	}

	return left;
}

int main()
try
{
	while(cin)
		cout << "=" << expression() << '\n';	
}
catch(exception& e)
{
	cerr << e.what() << '\n';
	return 1;
}
catch(...)
{
	cerr << "exception \n";
	return 2;
}
