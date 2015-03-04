/*
  James Foster
  3/3/15
  CS 460
*/

#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include "LexicalAnalyzer.h"


using namespace std;

//Might not need
string valid = ".;+-/*%=<>&^|!~();?:,"; 

//Might not need
static string token_names[] = {"IDENT_T","NUMLIT_T","INTTYPE_T","DBLTYPE_T","MAIN_T","IF_T","ELSE_T","WHILE_T","VOID_T","PLUS_T","MINUS_T","DIV_T","MULT_T","MOD_T","MINUSMINUS_T","PLUSPLUS_T","ASSIGN_T","EQUALTO_T","NOTEQ_T","GT_T","LT_T","GTE_T","LTE_T","LOGAND_T","LOGOR_T","NOT_T","AND_T","OR_T","XOR_T","SHIFTL_T","SHIFTR_T","TILDA_T","LPAREN_T","RPAREN_T","LBRACE_T","RBRACE_T","SEMI_T","COMMA_T","ERROR_T","EOF_T","QUEST_T","COLON_T"};

//DFA
int tokenTable[100][100] = 
  {
    //error state 100
    {},//0
    {-1,-1,-1,2,6,10,12,14,16,24,30,20,36,40,44,46,48,50,52,53,54,55,56,62,100},//1
    {3,3,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,100},//2
    {103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,100},//3
    {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,100},//4
    {104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,100},//5
    {7,7,7,8,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,100},//6
    {105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,100},//7
    {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,100},//8
    {106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,100},//9
    {11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,100},//10
    {107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,100}, //11
    {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,100},//12
    {108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,100},//13
    {15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,100},//14
    {109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,100},//15
    {17,17,17,17,17,17,17,18,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,100},//16
    {110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,100},//17
    {19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,100},//18
    {111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,100},//19
    {21,21,21,21,21,21,21,22,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,100},//20
    {112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,100},//21
    {23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,100},//22
    {113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,100},//23
    {25,25,25,25,25,25,25,28,26,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,100},//24
    {114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,100},//25
    {27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,100},//26
    {115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,100},//27
    {29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,100},//28
    {116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,100},//29
    {31,31,31,31,31,31,31,32,31,34,31,31,31,31,31,31,31,31,31,31,31,31,31,31,100},//30
    {117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,100},//31
    {33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,100},//32
    {118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,100},//33
    {35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,100},//34
    {119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,100},//35
    {37,37,37,37,37,37,37,37,37,37,37,38,37,37,37,37,37,37,37,37,37,37,37,37,100},//36
    {120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,100},//37
    {39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,100},//38
    {121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,100},//39
    {41,41,41,41,41,41,41,41,41,41,41,41,42,41,41,41,41,41,41,41,41,41,41,41,100},//40
    {122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,100},//41
    {43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,100},//42
    {123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,100},//43
    {45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,100},//44
    {124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,100},//45
    {47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,100},//46
    {125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,100},//47
    {49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,100},//48
    {126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,100},//49
    {51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,100},//50
    {127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,100},//51
    {57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,100},//52
    {58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,100},//53
    {59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,100},//54
    {60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,100},//55
    {61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,100},//56
    {128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,100},//57
    {129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,100},//58
    {130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,130,100},//59
    {131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,100},//60
    {132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,100},//61
    {63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,100},//62
    {133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,133,100},//63
    //64
  };

//Constructor 
LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
  //setup input stream
  input.open(filename);

  string temp = filename;
  
  totalLines =   std::count(std::istreambuf_iterator<char>(input), 
			    std::istreambuf_iterator<char>(), '\n');
  input.clear();
  input.seekg(0, ios::beg);

  //  cout << "Total Lines: " << totalLines << endl;
  int dotPos = temp.find(".");
  string rootName = temp.substr(0,dotPos);
  
  string lfile = rootName+".lst";
  string dfile = rootName+".dbg";
  listing.open(lfile.c_str());
  debug.open(dfile.c_str());
  
  position = -1;
  statePos.first = 1;
  statePos.second = 1;

  cout << "Input file: " << temp << endl;
  listing << "Input file: " << temp << "\n";
  debug << "Input file: " << temp << "\n";
  tokenFound = false;
  errorCount = 0;
  lineNum = 0;
  
}

//Close all files
LexicalAnalyzer::~LexicalAnalyzer ()
{
  listing.close();
  debug.close();
  input.close();
}

//Get token from line
token_type LexicalAnalyzer::get_token ()
{
  //cout << "in loop" << endl;
  //cout << statePos.first << " " << statePos.second << endl;
  
  if(position == -1 || position >= line.length())
    {
      
      lineNum++;
      if(lineNum <= totalLines){
	getline(input,line);
	debug << lineNum << ": " << line << endl;
	listing << lineNum << ": " << line << endl;
	position = 0;
      }
      else
	return EOF_T;
    }
  //something
  string otherThing;
  lexemeTemp = "";
  while(tokenTable[statePos.first][statePos.second] < 100 && position < line.length())
    {
      //cout << "pos " << position << " " << line[position] << endl;
      if(line[position] == '+')
	{
	  //cout << "PLUS" << endl;
	  if(statePos.first == 1)
	    statePos.first = 2;
	  else if(statePos.first == 2)
	    statePos.first = 4;
	  else
	      helper();
	}
      else if(line[position] == '-')
	{
	  //cout << "MINUS" << endl;
	  if(statePos.first == 1)
            statePos.first = 6;
          else if(statePos.first == 6)
            statePos.first = 8;
	  else
	    helper();
	}
      else if(line[position] == '/')
	{
	  if(statePos.first == 1)
	    statePos.first = 10;
	  else 
	    helper();
	}
      else if(line[position] == '*')
	{
	  if(statePos.first == 1)
            statePos.first = 12;
	  else
	    helper();
	}
      else if(line[position] == '%')
	{
	  if(statePos.first == 1)
            statePos.first = 14;
	  else 
	    helper();
	}
      else if(line[position] == '=')
	{
	  if(statePos.first == 1)
            statePos.first = 16;
          else if(statePos.first == 16)
            statePos.first = 18;
	  else if(statePos.first == 20)
	    statePos.first = 22;
	  else if(statePos.first == 30)
	    statePos.first = 32;
	  else 
	    helper();
	}
      else if(line[position] == '>')
	{
	  if(statePos.first == 1)
            statePos.first = 24;
          else if(statePos.first == 24)
            statePos.first = 28;
	  else
	    helper();
	}
      else if(line[position] == '<')
	{
	  if(statePos.first == 1)
            statePos.first = 30;
          else if(statePos.first == 30)
            statePos.first = 34;
	  else
	    helper();
	}
      else if(line[position] == '!')
	{
	  if(statePos.first == 1)
            statePos.first = 20;
	  else
	    helper();
	}
      else if(line[position] == '&')
	{
	  if(statePos.first == 1)
            statePos.first = 36;
          else if(statePos.first == 36)
            statePos.first = 38;
	  else
	    helper();
	}
      else if(line[position] == '|')
	{
	  if(statePos.first == 1)
            statePos.first = 40;
          else if(statePos.first == 40)
            statePos.first = 42;
	  else
	    helper();
	}
      else if(line[position] == '^')
	{
	  if(statePos.first == 1)
	    statePos.first = 44;
	  else
	    helper();
	}
      else if(line[position] == '~')
	{
	  if(statePos.first == 1)
	    statePos.first == 46;
	  else
	    helper();
	}
      else if(line[position] == '(')
	{
	  if(statePos.first == 1)
	    statePos.first = 48;
	  else
	    helper();
	}
      else if(line[position] == ')')
	{
	  if(statePos.first == 1)
	    statePos.first = 50;
	  else
	    helper();
	}
      else if(line[position] == '{')
	{
	  if(statePos.first == 1)
	    statePos.first = 52;
	  else
	    helper();
	}
      else if(line[position] == '}')
	{
	  if(statePos.first == 1)
	    statePos.first = 53;
	  else 
	    helper();
	}
      else if(line[position] == ';')
	{
	  if(statePos.first == 1)
	    statePos.first = 54;
	  else
	    helper();
	}
      else if(line[position] == ',')
	{
	  if(statePos.first == 1)
	    statePos.first = 55;
	  else 
	    helper();
	}
      else if(line[position] == '?')
	{
	  if(statePos.first == 1)
	    statePos.first = 56;
	  else
	    helper();
	}
      else if(line[position] == ':')
	{
	  if(statePos.first == 1)
	    statePos.first = 62;
	  else
	    helper();
	}
      else if(isalpha(line[position]) || line[position] == '_')
	{
	  if(statePos.first == 1){
	    otherThing = "";
	    for(;isalpha(line[position]) || line[position] == '_' &&position < line.length();position++)
	      {
		otherThing+=line[position];	
	      } 
	    //cout << "ALPHA " << otherThing << endl;
	    lexemeTemp = otherThing;
	    //helper();
	    //options
	    if(otherThing == "int"){
	      writeHelper("INTTYPE_T");
	      return INTTYPE_T;
	    }
	    else if(otherThing == "float"){
	      writeHelper("DBLTYPE_T");
	      return DBLTYPE_T;
	    }
	    else if(otherThing == "main"){
	      writeHelper("MAIN_T");
	      return MAIN_T;
	    }
	    else if(otherThing == "if"){
	      writeHelper("IF_T");
	      return IF_T;
	    }
	    else if(otherThing == "else"){
	      writeHelper("ELSE_T");
	      return ELSE_T;
	    }
	    else if(otherThing == "while"){
	      writeHelper("WHILE_T");
	      return WHILE_T;
	    }
	    else if(otherThing == "void"){
	      writeHelper("VOID_T");
	      return VOID_T;
	    }
	    else{
	      writeHelper("IDENT_T");
	      return IDENT_T;
	    }
	  }
	  else
	    helper();
	}
      else if(isdigit(line[position]) || line[position] == '.')
	{
	  
	  if(statePos.first == 1)     
	    {
	      otherThing = "";
	      //stringstream ss;
	      if(line[position] == '.')
		{
		  otherThing+=line[position];
		  position++;
		  for(;isdigit(line[position]) && position < line.length();position++)
		    {
		      if(line[position] != '.')
			otherThing+=line[position];
		      else
			{
			  lexemeTemp = otherThing;
			  helper();
			  writeHelper("NUMLIT_T");
			  return NUMLIT_T;
			}
		    }
		}
	      else if(isdigit(line[position]))
		{
		  bool period = false;
		  for(;isdigit(line[position]) && position < line.length() && !period;position++)
		    {
		      otherThing+=line[position];
		      if(line[position] == '.')
			period = true;
		    }
		  for(;isdigit(line[position]) && position < line.length();position++)
		    {
		      if(line[position] != '.')
			otherThing+=line[position];
		      else{
			lexemeTemp = otherThing;
			helper();
			writeHelper("NUMLIT_T");
			return NUMLIT_T;
		      }
		    }
		}
	      lexemeTemp = otherThing;
	      //cout << "Num " << otherThing << endl;
	      writeHelper("NUMLIT_T");
	      return NUMLIT_T;
	      
	    }
	  else
	    helper();
	    
	}
      else if(line[position] == '@')
	{
	  errorCount++;
	  lexemeTemp = line[position];
	  report_error(""+line[position]);
	}
      else if(line[position] == ' ')
	{
	  //do nothing
	}
      else{
	errorCount++;
	lexemeTemp = line[position];
	report_error(""+line[position]);
	//cout << "ERROR" << endl;
	statePos.first = 1;
	statePos.second = 24;
	//helper();
	position++;
	return ERROR_T;
      }
      lexemeTemp+=line[position];
      
      position++;	
      
      
    }
  if(lexemeTemp.length() >= 1)
   lexemeTemp.resize(lexemeTemp.length()-1);
  if(position == line.length()-1){
    if(lineNum == totalLines){
      cout << errorCount << " errors found in input file" << endl;
      debug << errorCount << " errors found in input file" << endl;      
      listing << errorCount << " errors found in input file" << endl;

      return EOF_T;
    }
    else
      helper();
  }
  //cout << "Outside" << endl;
  

  //statePos.first = 1;
  //statePos.second = 1;

  int cellTemp = tokenTable[statePos.first][statePos.second];
  //cout << "Cell Temp " << statePos.first << " " << statePos.second << " " << cellTemp << endl;

  statePos.first = 1;
  statePos.second = 1;

  
  
  switch(cellTemp)
    {
    case 100:
      writeHelper("ERROR_T");
      return ERROR_T;
    case 103:
      writeHelper("PLUS_T");
      return PLUS_T;
    case 104:
      writeHelper("PLUSPLUS_T");
      return PLUSPLUS_T;
    case 105:
      writeHelper("MINUS_T");
      return MINUS_T;
    case 106:
      writeHelper("MINUSMINUS_T");
      return MINUSMINUS_T;
    case 107:
      writeHelper("DIV_T");
      return DIV_T;
    case 108:
      writeHelper("MULT_T");
      return MULT_T;
    case 109:
      writeHelper("MOD_T");
      return MOD_T;
    case 110:
      writeHelper("ASSIGN_T");
      return ASSIGN_T;
    case 111:
      writeHelper("EQUALTO_T");
      return EQUALTO_T;
    case 112:
      writeHelper("NOT_T");
      return NOT_T;//case 113
    case 114:
      writeHelper("GT_T");
      return GT_T;//case 115,116
    case 117:
      writeHelper("LT_T");
      return LT_T;
    case 118:
      writeHelper("LTE_T");
      return LTE_T;
    case 119:
      writeHelper("SHIFTL_T");
      return SHIFTL_T;
    case 120:
      writeHelper("AND_T");
      return AND_T;//case 121
    case 122:
      writeHelper("OR_T");
      return OR_T;//case 123
    case 124:
      writeHelper("XOR_T");
      return XOR_T;
    case 125:
      writeHelper("TILDA_T");
      return TILDA_T;
    case 126:
      writeHelper("LPAREN_T");
      return LPAREN_T;
    case 127:
      writeHelper("RPAREN_T");
      return RPAREN_T;
    case 128:
      writeHelper("LBRACE_T");
      return LBRACE_T;
    case 129:
      writeHelper("RBRACE_T");
      return RBRACE_T;
    case 130:
      writeHelper("SEMI_T");
      return SEMI_T;
    case 131:
      writeHelper("COMMA_T");
      return COMMA_T;
    case 132:
      writeHelper("QUEST_T");
      return QUEST_T;
    case 133:
      writeHelper("COLON_T");
      return COLON_T;
      //default:
      //return IDENT_T;
    }
  //  if(input.eof())
  //return EOF_T;


}

//Get most recently found lexeme
string LexicalAnalyzer::get_lexeme () const
{
  return lexemeTemp;
}

//Output error
void LexicalAnalyzer::report_error (const string & msg)
{
  listing << "Error at " << lineNum << ',' << position << ':' <<
    "Invalid character found:" << lexemeTemp << "\n";
  //listing << temp << "\n";
  debug << "Error at " << lineNum << ',' << position << ':' <<
    "Invalid character found:" << lexemeTemp << "\n";

}

//Helps with DFA
void LexicalAnalyzer::helper()
{
  statePos.first = tokenTable[statePos.first][statePos.second];
  position--;
  //  tokenFound = true;
}

//Writes to debug file
void LexicalAnalyzer::writeHelper(string token)
{
  debug << token << "\t\t\t" << lexemeTemp << "\n"; 
}
