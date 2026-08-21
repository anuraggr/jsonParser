#include<stdio.h>
#include<stdlib.h>
#include "tokenizer.h"
#include "parser.h"

int main(int argc, char *argv[]){
	if(argc < 1){
		printf("Usage: jsonparser <file> ...\n");
		return -1;
	}

	FILE* file = fopen(argv[1], "r");

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
	
	JsonNode* root = parse(&tokens);

	freeTokenList(&tokens);
	free(jsonBuffer);

	if(root){
        	printf("Successfully parsed JSON:\n");
        	printJsonNode(root, 0);
    	} 
	else{
        	printf("Failed to parse JSON.\n");
	}

	freeJsonNode(root);
	return 0;
}
