#include "common.h"
#include "tok.h"

char* readFile(const char* filename);

int main(i32 argc, char** argv) {
	char* code = NULL;

	if(argc < 2) {
		printf("usage: ./aurora <filename>.aur [options] [outname]\n");
		exit(-1);
	}

	if(strstr(argv[1], ".aur") == NULL) {
		fprintf(stderr, "Error filetype: incorrect filetype.\n");
		exit(-1);
	} else {
		code = readFile("./tests/test.aur");
	}

	vectk tokens = Tokenizer(code, strlen(code));

	// if -t is on the third position print tokens & exit
	// short circuit protects from Memory access violation
	if((argc >= 3) && (strcmp(argv[2], "-t") == 0)) {
		printTokens(&tokens);
		goto end;
	}

	// parsing

	

	end:
	free(code);
	freeVec(&tokens);

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
