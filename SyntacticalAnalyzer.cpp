#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token_type t;
	while (t != EOF_T)
	  {
	    t = lex->get_token();
	  }
	cout << endl;
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}
