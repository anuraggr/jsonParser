#include<stdio.h>
#include<stdlib.h>

int main(int agrc, char *argv[]){
	if(argc < 1){
		printf("Usage: jsonparser <file> ...\n", argv[0]);
		return -1;
	}

	FILE* file = fopen(argv[0], "r");

	if(file == NULL){
		printf("Error: Failed to read json file");
		return -1;
	}

	//move file pos indicator to the end, find what byte currently siting on, move back to front 
	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* jsonBuffer = malloc(length + 1);
	//read 1 at a time for length time
	fread(jsonBuffer, 1, length, file);
	jsonBuffer[length] = '\0';

	fclose(file);

	TokenList tokens = tokenize(jsonBuffer);
	//then parse

	free(jsonBuffer);
	return 0;
}
