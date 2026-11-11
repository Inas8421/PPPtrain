/*
	Simple calculator

	This program implements a basic expression calculator.
	Input from cin; output to cout.
	The grammar for input is:

	Calculation:
		Statement
		Print
		Quit
		Calculation Statement
	Statement:
		Declaration
		Expression
	Declaration:
		"let" Name "=" Expression
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

#include "token.h"

//-----------------------------------------------------------------------

constexpr char number = '8';	//t.kind == number means that t is a number Token
constexpr char quit = 'q';	//t.kind == quit means that t is a quit Token
constexpr char print = ';';	//t.kind == print means that t is a print Token
constexpr char let = 'L';	//declaration token
constexpr char name = 'a';	//name token

constexpr string declkey = "let";//declaration keyword;
constexpr string prompt = "> ";
constexpr string result = "= ";	//used to indicate that what follows is a result

//-----------------------------------------------------------------------

class Variable
{
	public:
		string name;
		double value;
};

//-----------------------------------------------------------------

double expression(Token_stream& ts);	//read and evaluate an Expression

//----------------------------------------------------------

vector<Variable> var_table;	//provides storage of variables

//----------------------------------------------------------------------

//Search Variable for a given name by looking elements one after another
double get_value(string s)
{
	for(const Variable& v : var_table)
		if(v.name == s)
			return v.value;

	//TODO:compilation warning here, because of no return value
	error("trying to read undefined variable ", s);
} 

//----------------------------------------------------------------------

//Set new value for Variable
void set_value(string s, double d)
{
	for(Variable& v : var_table)
		if(v.name == s)
		{
			v.value = d;
			return;
		}

	error("trying to write undefined variable", s);
}

//----------------------------------------------------------------------

//Is Variable already in var_table?
bool is_declared(string var)
{
	for(const Variable& v : var_table)
		if(v.name == var)
			return true;

	return false;
}

//----------------------------------------------------------------------

//Add new Variable to var_table
double define_name(string var, double val)
{
	if(is_declared(var))
		error(var, " declared twice");

	var_table.push_back(Variable{var, val});
	return val;
}

//----------------------------------------------------------------------

//Deal with numbers and parentheses
double primary(Token_stream& ts) 
{
	Token t = ts.get();
	switch(t.kind)
	{
		case '(':	//handle '(' expression ')'
		{
			double d = expression(ts);
			t = ts.get();
			if(t.kind != ')')
				error("')' expected !");
			return d;
		}
		case number:
			return t.value;		//return the number's value
		case '-':
			return - primary(ts);
		case '+':
			return primary(ts);
		case name:
			return get_value(t.name);
		default:
			error("primary expected !");
			//TODO:compilation warning here, because of no return value
	}
}

//------------------------------------------------------------

//Deal with *, /, and %
double term(Token_stream& ts)
{
	double left = primary(ts);
	Token t = ts.get();	//get the next token from token stream
	while(true)
	{
		switch(t.kind)
		{
			case '*':
				left *= primary(ts);
				t = ts.get();
				break;
			case '/':
			{
				double d = primary(ts);
				if(d == 0)
					error("division by zero !");
				left /= d;
				t = ts.get();
				break;
			}
			case '%':
			{
				double d = primary(ts);
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
double expression(Token_stream& ts)
{
	double left = term(ts);	//read and evaluate a Term
	Token t = ts.get();	//get the next token
	while(true)
	{
		switch(t.kind)
		{
			case '+':
				left += term(ts);	//evaluate term and add
				t = ts.get();
				break;
			case '-':
				left -= term(ts);	//evaluate term and substract
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
void clean_up_mess(Token_stream& ts)
{
	ts.ignore(print);
}

//----------------------------------------------------------------

double statement(Token_stream& ts);

//----------------------------------------------------------------

//Calculation loop
void calculate(Token_stream& ts)
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
		cout << result << statement(ts) << '\n';
	}
	catch(exception& e)
	{
		cerr << "calculate() error: " << e.what() << '\n';
		clean_up_mess(ts);
	}
}

//---------------------------------------------------------------------

double declaration(Token_stream& ts);

//----------------------------------------------------------------------

double define_name(string var, double val);

//----------------------------------------------------------------------
//asume we have seen "let"
//handle: name = expression
//declare a variable called "name" with the initial value "expression"
double declaration(Token_stream& ts)
{
	Token t = ts.get();
	if(t.kind != name)
		error("name expected in declaration");

	Token t2 = ts.get();
	if(t2.kind != '=')
		error("= missing in declaration of ", t.name);

	double d = expression(ts);
	define_name(t.name, d);
	
	return d;
}

//----------------------------------------------------------------------

double statement(Token_stream& ts)
{
	Token t = ts.get();
	switch(t.kind)
	{
		case let:
			return declaration(ts);
		default:
			ts.putback(t);
			return expression(ts);
	}
}

//----------------------------------------------------------------------

int main()
try
{
	//predefined names
	define_name("pi", 3.1415926535);
	define_name("e", 2.7182818284);

	Token_stream ts;

	calculate(ts);
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
