#include "str.h"


/* Initialize a str object */
void StrInit(str *s, char *string, usize MaxSize) {
	s->len = strlen(string);
	s->MaxSz = (MaxSize >= s->len) ? MaxSize : s->len;
	s->string = malloc(s->MaxSz + 1);
	if(s->string == NULL) {
		fprintf(stderr, "%s\n", "failed to allocate string");
	}
	strcpy(s->string, string);
	if(s->string == NULL) {
		fprintf(stderr, "%s\n", "failed to move string");
	}

	if(s->string[s->len] != EOF) {
		s->string[s->len] = '\0';
	}
}

/* Extends a string */
void StrExtend(str *s) {
	i32 oldCapa = s->MaxSz;
	s->MaxSz = GROW_CAPACITY(oldCapa);
	s->string = GROW_ARR(char, s->string, oldCapa, s->MaxSz);
}

/*
Copies the contents of string src to string dest overriding string src's contents;
if the dest's MaxSz is less than src's MaxSz; then ret
*/
void StrCpy(str *dest, str *src) {
	
	if(dest->MaxSz < src->MaxSz) {
		// ^ i can't figure whether to use both MaxSzs or MaxSz and len
		// fprintf(stderr, "Destination string cannot hold src string\n");
		// exit(-2);
		StrExtend(dest);
		StrCpy(dest, src);
		return;
	}
	strcpy(dest->string, src->string);
	if (dest->string == NULL) {
		fprintf(stderr, "failed to transfer strings\n");
		exit(-2);
	}
}


/* Compares two strings, returns true if same; false if not */
bool StrCmp(str *str1, str *str2) {
	return ((strcmp(str1->string, str2->string) == 0) ? true : false);
}

/* Compares two strings, one str obj and a char*, returns true if same; false if not */
bool StrcCmp(str *str1, char* str2) {
	return ((strcmp(str1->string, str2) == 0) ? true : false);
}

/* Concatenates str src to str dest. if the maxsz of dest cannot hold the size of str src, return */
void StrCat(str *dest, str *src) {
	if(src->MaxSz + src->len < dest->MaxSz) {
		dest->len += src->len;
		strcat(dest->string, src->string);
	} else {
		// puts("dest size cannot bear the src size");
		StrExtend(dest);
		StrCat(dest, src);
	}

	return;

	/*
		--- Basic StrCat Fn ---
		i32 destPos = dest->len;
		i32 srcPos = 0;
		while(src->string[srcPos] != '\0' || destPos <= dest->MaxSz) {
			dest->string[destPos] = src->string[srcPos];
			destPos++;
			srcPos++;
		}

	*/
}

/* Concatenates a character to a string. If it cannot hold the character, return */
void StrChrCat(str *dest, char ch) {
	/* Concats a Chr to a given Str */
	if(dest->len + 1 <= dest->MaxSz) {
		dest->string[dest->len] = ch;
		dest->len++;
		dest->string[dest->len] = '\0';
	} else {
		// puts("dest size cannot bear the char");
		StrExtend(dest);
		StrChrCat(dest, ch);
		return;
	}
}

// i32 main() {
// 	str s1, s2;

// 	StrInit(&s1, "He", 3);
// 	StrInit(&s2, "World", 5);

// 	printf("string 1: %s, %zu, %zu\n", s1.string, s1.MaxSz, s1.len);
// 	printf("string 2: %s, %zu, %zu\n", s2.string, s2.MaxSz, s2.len);

// 	// StrCpy(&s1, &s2);
// 	// printf("string 1: %s, %zu, %zu\n", s1.string, s1.MaxSz, s1.len);
// 	// printf("string 2: %s, %zu, %zu\n", s2.string, s2.MaxSz, s2.len);

// 	// StrCat(&s1, &s2);
// 	// printf("string 1: %s, %zu, %zu\n", s1.string, s1.MaxSz, s1.len);
// 	// printf("string 2: %s, %zu, %zu\n", s2.string, s2.MaxSz, s2.len);

// 	// str s3, s4;
// 	// StrInit(&s3, "What", 5);
// 	// StrInit(&s4, "Waht", 5);
// 	// if(StrCmp(&s3, &s4)) {
// 	// 	puts("True");
// 	// } else {
// 	// 	puts("False");
// 	// }

// 	// str s5;
// 	// StrInit(&s5, "", 1);
// 	// StrChrCat(&s5, 'a');
// 	// StrChrCat(&s5, 'a');
// 	// printf("string 5: %s, %zu, %zu\n", s5.string, s5.MaxSz, s5.len);

// 	return 0;
// }
