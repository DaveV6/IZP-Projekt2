#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

enum command{progName, command, r, c, txtFile};
enum commandExecute{cHelp, cTest, cLpath, cRpath, cShortest};
enum direction{R, U, L, D};
enum border{left, top, right, bottom};

typedef struct {
	int rows;
  	int cols;
  	unsigned char *cells;
} Map;

Map *mapConstruct(int rows, int cols);
void mapDeconstruct(Map *map);
int power(int base, int exponent);
bool isBorder(Map *map, int r, int c, int border);
int parseArgs(int argc, char* argv[]);
Map *readMapFromFile(char *filename);
FILE* fileOpen(char *file);
bool checkMaze(Map *map);

//bool startBorder(Map *map, int r, int c, int leftright){}

int main(int argc, char* argv[]){

	int args = parseArgs(argc, argv);

	if(argc < 2){
		fprintf(stderr, "ERROR: ENTER MORE ARGUMENTS!\n");
	}


	Map *map = readMapFromFile(argv[argc-1]);

	if(args != -1){
		
		switch(args){

			case cHelp:

				fprintf(stdout, "THE GOAL OF THE PROGRAM IS TO EXIT THE MAZE\n");
				break;

			case cTest:

				checkMaze(map);
				if(checkMaze == true){
					fprintf(stdout, "VALID");
				}
				else{
					fprintf(stderr, "INVALID");
				}
				break;

			case cLpath:

				fprintf(stdout, "LEFT PATH\n");
				break;

			case cRpath:

				fprintf(stdout, "RIGHT PATH\n");
				break;

		}

	}
	mapDeconstruct(map);

	return 0;

}

bool checkMaze(Map *map){

	int rows, cols;
	rows = map->rows;
	cols = map->cols;

	if(isBorder())

}

int changePos(Map *map, int r, int c){

	

}

bool isExit(Map *map, int r, int c){

	if(r == 1 || c == 1 || r == map->rows || c == map->cols){
		if(!isBorder(map, r, c, left) && (c < 1)){
			return true;
		}
		if(!isBorder(map, r, c, top) && (r < 1)){
			return true;
		}
		if(!isBorder(map, r, c, right) && (c > map->cols)){
			return true;
		}
		if(!isBorder(map, r, c, bottom) && (r > map->rows)){
			return true;
		}
	}

	return false;

}

int power(int base, int exponent) {
    
	int	result = 1;

	for(int i = 0; i < exponent; i++){
		result*=base;
	}

	return result;

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

int parseArgs(int argc, char* argv[]){

	switch(argc){

		case 2:
			
			if(!strcmp(argv[command], "--help")){
				return cHelp;
			}
			
			break;

		case 3:
			
			if(!strcmp(argv[command], "--test")){
				return cTest;
			}

			break;

		case 5:
			
			if(!strcmp(argv[command], "--lpath")){
				return cLpath;
			}
			
			if(!strcmp(argv[command], "--rpath")){
				return cRpath;
			}

			break;
	}

	return -1;

}

Map *readMapFromFile(char *filename){

    FILE *f = fileOpen(filename);
    if (!f){
        return NULL;
    }

    int rows, cols;
    if (fscanf(f, "%d %d", &rows, &cols) != 2){
        fprintf(stderr, "ERROR: FAILED TO READ ROWS AND COLUMNS FROM FILE\n");
        return NULL;
    }

    Map *map = mapConstruct(rows, cols);

   fgetc(f);

    for (int i = 0; i < rows * cols; i++){
        char cells;
        while (isspace(cells = fgetc(f))){
        }

        if (cells == EOF){
            fprintf(stderr, "ERROR: UNEXPECTED END OF FILE\n");
            mapDeconstruct(map);
            fclose(f);
            return NULL;
        }

        map->cells[i] = cells;
    }

    fclose(f);
    return map;
}

FILE* fileOpen(char *file){

	FILE *f = fopen(file, "r");

	if(f != NULL){
		return f;
	}
	else{
		fprintf(stderr, "ERROR: FILE FAILED TO OPEN! TRY AGAIN!\n");
		return NULL;
	}

}
