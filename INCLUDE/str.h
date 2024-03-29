/*/// str.h
///	Provides simple string class
///
/// Written by Nikita Berezkov
//*/

#ifndef __STR_H
#define __STR_H

#include <string.h>

class String {
public:
   char* char_ptr;   // pointer to string contents
   int length;       // length of string in characters

   String(char *text);           
   String(int size = 80);       
   String(String& Other_String); 

   ~String() {delete[] char_ptr;}; 
   int Get_len (void);
   String operator+ (String& Arg);
};

String::String(char* text)
{
	length = strlen(text);  
	char_ptr = new char[length + 1];
	strcpy(char_ptr, text);
};

/***
 *	Creates empty string
 */
String::String(int size)
{
	length = size;
	char_ptr = new char[length + 1];
	*char_ptr = '\0';
};

String::String(String& Other_String)
{
	length = Other_String.length;       
	char_ptr = new char[length + 1]; 
	strcpy(char_ptr, Other_String.char_ptr); 
};

String String::operator+ (String& Arg)
{
	String Temp(length + Arg.length);
	strcpy(Temp.char_ptr, char_ptr);
	strcat(Temp.char_ptr, Arg.char_ptr);
	return Temp;
}

int String::Get_len(void)
{
	return (length);
};

#endif