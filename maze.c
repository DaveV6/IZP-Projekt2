#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum command{starter, cmd, r, c, text};
enum commandExecute{cHelp, cTest, cLpath, cRpath, cShortest};

typedef struct {
	int rows;
  	int cols;
  	unsigned char *cells;
} Map;

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

	printf("%s \n", file);
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

	return tempMap;
} 

void mapDeconstruct(Map *map){

  	free(map->cells);
	free(map);

}

//bool isBorder(Map *map, int r, int c, int border){}

//bool startBorder(Map *map, int r, int c, int leftright){}

int main(int argc, char* argv[]){

	Map *map = mapConstruct(6, 7);
	parseArgs(argc, argv);
	fileOpen(argv[argc-1]);
	mapDeconstruct(map);

}