#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "SetLimits.h"
#include "SyntacticalAnalyzer.h"

int main (int argc, char * argv[])
{
	if (argc < 2)
	{
		printf ("format: proj1 <filename>\n");
		exit (1);
	}
	SetLimits ();
	
	//LexicalAnalyzer mainLex(argv[1]);
	
	/*
	string temp = argv[1];
	cout << temp << endl;
       
	int dotPos = temp.find(".");
	string fileName = temp.substr(0,dotPos); 
	cout << fileName << endl;
	ofstream listing,debugging;
	string lfile = fileName+".lst";
	string dfile = fileName+".dbg";
	listing.open(lfile.c_str());
	debugging.open(dfile.c_str());
	listing.close();
	debugging.close();
	*/
	SyntacticalAnalyzer parse (argv[1]);
	
	return 0;
}
