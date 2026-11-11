#include "token.h"

constexpr char number = '8';	//t.kind == number means that t is a number Token
constexpr char quit = 'q';	//t.kind == quit means that t is a quit Token
constexpr char print = ';';	//t.kind == print means that t is a print Token
constexpr char let = 'L';	//declaration token
constexpr char name = 'a';	//name token

constexpr string declkey = "let";//declaration keyword;

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
		case '=':	//possible
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
			if(isalpha(ch))
			{
				string s;
				s+=ch;
				while(cin.get(ch) && (isalpha(ch) || isdigit(ch)))
					s+=ch;

				cin.putback(ch);
				if(s == declkey)
					return Token{let};	//declaration keyword
				return Token{name, s};
			}			

			//TODO:compilation warning here, because of no return value
			error("Bad token");
		}
}
