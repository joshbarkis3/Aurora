#include "common.h"
#include "tok.h"

char* readFile(const char* filename);

int main(i32 argc, char** argv) {
	char* code = readFile("./tests/test.aur");

	// for(i32 i = 0; i <= strlen(code); i++) {
	// 	switch (code[i]) {
	// 	case '\0':
	// 		puts("EOF");
	// 	}
	// }

	Tokenizer(code, strlen(code));

	// printf("%zu\n", strlen(code));

	// printf("%s\n", code);
	return 0;
}

char* readFile(const char* filename) {
	FILE *file = fopen(filename, "r");
	if(file == NULL) {
		fprintf(stderr, "Error Reading File. %s\n", filename);
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	usize fileSize = ftell(file);
	rewind(file);
	
	char* fileContents = (char*)malloc(fileSize + 1);
	if (fileContents == NULL) {
		fprintf(stderr, "File memory allocation failed.\n");
		fclose(file);
		exit(1);
	}

	usize bytesRead = fread(fileContents, 1, fileSize, file);
	if(bytesRead != fileSize) {
		fprintf(stderr, "Error Reading file.\nbytesRead : %zu\nfileSize  : %zu\n", bytesRead, fileSize);
		free(fileContents);
		fclose(file);
		exit(1);
	}

	// printf("%zu ", bytesRead);
	// printf("%zu ", fileSize);

	fileContents[fileSize] = '\0';
	fclose(file);

	return fileContents;
}
