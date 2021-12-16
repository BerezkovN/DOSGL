/*/// str.h
///	Клас для роботи зі строками
//*/

#ifndef __STR_H
#define __STR_H

#include <string.h>

class String {
public:
   char* char_ptr;   // Вказівник на масив з символами
   int length;       // Довжина строки

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
 *	Створює пусту сроку з довжиної size
 */
String::String(int size)
{
	length = size;
	char_ptr = new char[length + 1];
	*char_ptr = '\0';
};

/***
 *	Копіює значення іншої строки
 */
String::String(String& Other_String)
{
	length = Other_String.length;       
	char_ptr = new char[length + 1]; 
	strcpy(char_ptr, Other_String.char_ptr); 
};

/***
 *	З'єднює строки
 */
String String::operator+ (String& Arg)
{
	String Temp(length + Arg.length);
	strcpy(Temp.char_ptr, char_ptr);
	strcat(Temp.char_ptr, Arg.char_ptr);
	return Temp;
}

/***
 *	Повертає довжину строки
 */
int String::Get_len(void)
{
	return (length);
};

#endif