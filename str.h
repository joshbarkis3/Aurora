#ifndef str_h
#define str_h

#include "common.h"
#include <string.h>	

/*
	wrappers for standard GNU/Linux string functions
	 > just so i dont need to think about it
*/

typedef struct {
	char* string;
	usize MaxSz;
	usize len;
} str;

void StrInit(str *s, char * string, usize MaxSize);
void StrCpy(str *dest, str *src);
bool StrCmp(str *str1, str *str2);
bool StrcCmp(str *str1, char* str2);
void StrCat(str *dest, str *src);
void StrChrCat(str *dest, char ch);

#endif