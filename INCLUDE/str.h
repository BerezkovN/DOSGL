#ifndef __STR_H
#define __STR_H

#include <string.h>

class String {
public:
   char* char_ptr;   // pointer to string contents
   int length;       // length of string in characters
   // three different constructors
   String(char *text);           // constructor using existing string
   String(int size = 80);        // creates default empty string
   String(String& Other_String); // for assignment from another

   ~String() {delete char_ptr;}; // inline destructor
   int Get_len (void);
   String operator+ (String& Arg);
};

String::String(char* text)
{
	length = strlen(text);  // get length of text
	char_ptr = new char[length + 1];
	strcpy(char_ptr, text);
};

String::String(int size)
{
	length = size;
	char_ptr = new char[length + 1];
	*char_ptr = '\0';
};

String::String(String& Other_String)
{
	length = Other_String.length;       // length of other string
	char_ptr = new char[length + 1];   // allocate the memory
	strcpy(char_ptr, Other_String.char_ptr); // copy the text
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