#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum command{progName, cmd, r, c, text};
enum commandExecute{cHelp, cTest, cLpath, cRpath, cShortest};

typedef struct {
	int rows;
  	int cols;
  	unsigned char *cells;
} Map;

int power(int base, int exponent) {
    
	int	result = 1;

	for(int i = 0; i < exponent; i++){
		result*=base;
	}

	return result;

}

int parseArgs(int argc, char* argv[]){

	switch(argc){

		case 2:
			
			if(!strcmp(argv[cmd], "--help")){
				fprintf(stdout, "KOKOT CHUDACKY\n");
				return cHelp;
			}
			
			break;

		case 3:
			
			if(!strcmp(argv[cmd], "--test")){
				fprintf(stdout, "VALID\n");
				return cTest;
			}

			break;

		case 5:
			
			if(!strcmp(argv[cmd], "--lpath")){
				fprintf(stdout, "lavy pes\n");
				return cLpath;
			}
			
			if(!strcmp(argv[cmd], "--rpath")){
				fprintf(stdout, "pravy pes\n");
				return cRpath;
			}

			break;
	}

	return 0;

}

FILE* fileOpen(char *file){

	//printf("%s \n", file);
	FILE *f = fopen(file, "r");

	if(f != NULL){
		return f;
	}
	else{
		fprintf(stderr, "FILE FAILED TO OPEN! TRY AGAIN!\n");
		return NULL;
	}

}

Map* mapConstruct(int rows, int cols)
{
 	Map *tempMap = malloc(sizeof(Map));
	tempMap->cols = cols;
	tempMap->rows = rows;
	tempMap->cells = malloc((cols * rows) * sizeof(unsigned char));

	if(tempMap == NULL){
		fprintf(stderr, "ERROR: MALLOC FAILED!");
	}
	if(tempMap->cells == NULL){
		fprintf(stderr, "ERROR: MALLOC FAILED!");
	}

	return tempMap;
} 

void mapDeconstruct(Map *map){

  	free(map->cells);
	free(map);

}

bool isBorder(Map *map, int r, int c, int border){

	int index = (r - 1) * map->cols + c;

	if((map->cells[index] & power(2, border)) == power(2, border)){
		return true;
	}
	else{
		return false;
	}

}

//bool startBorder(Map *map, int r, int c, int leftright){}

int main(int argc, char* argv[]){

	Map *map = mapConstruct(6,7);
	int x = power(2, 2);
	printf("%d\n", x);
	FILE *file = fileOpen(argv[argc - 1]);

	if(file != NULL){
		printf("kar\n");
		fclose(file);
	}
	parseArgs(argc, argv);
	mapDeconstruct(map);

}