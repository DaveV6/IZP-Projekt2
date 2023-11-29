#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

enum command{progName, command, r, c, txtFile};
enum commandExecute{cHelp, cTest, cLpath, cRpath, cShortest};
enum direction{R, U, L, D, CONTROL};
enum border{left, right, horizontal};

#define RPATH -1
#define LPATH 1

typedef struct {
	int rows;
  	int cols;
  	unsigned char *cells;
} Map;

Map *mapConstruct(int rows, int cols);
void mapDeconstruct(Map *map);
bool isBorder(Map *map, int r, int c, int border);
int parseArgs(int argc, char* argv[], int* startR, int* startC);
Map *readMapFromFile(char *filename);
FILE* fileOpen(char *file);
//int checkMaze(Map *map);
bool isExit(Map *map, int r, int c, int direction);
int start_border(Map *map, int r, int c, int leftright);
void changePos(int* r, int* c, int direction);
int changeDir(Map *map, int r, int c, int leftright, int direction);

int main(int argc, char* argv[]){

	int startR = 0;
	int startC = 0;
	int direction = -1;
	int args = parseArgs(argc, argv, &startR, &startC);

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

				if(map == NULL){
					fprintf(stdout, "Invalid\n");
				}else{
					fprintf(stdout, "Valid\n");
				}

				break;

			case cLpath:

				direction = start_border(map, startR, startC, LPATH);
				
				fprintf(stdout, "%d,%d\n", startR, startC);
				
				while(!isExit(map, startR, startC, direction)){
					
					direction = changeDir(map, startR, startC, LPATH, direction);
				
					if(!isExit(map, startR, startC, direction)){
						changePos(&startR, &startC, direction);
						fprintf(stdout, "%d,%d\n", startR, startC);
					}
				}

				break;

			case cRpath:
				direction = start_border(map, startR, startC, RPATH);
				
				fprintf(stdout, "%d,%d\n", startR, startC);

				while(!isExit(map, startR, startC, direction)){
					
					direction = changeDir(map, startR, startC, RPATH, direction);
				
					if(!isExit(map, startR, startC, direction)){
						changePos(&startR, &startC, direction);
						fprintf(stdout, "%d,%d\n", startR, startC);
					}
				}

				break;

		}

	}
	
	if(map != NULL){
		mapDeconstruct(map);
		map = NULL;
	}

	return 0;

}

int changeDir(Map *map, int r, int c, int leftright, int direction){

	direction += leftright;

	if(direction == CONTROL){
		direction = R;
	}
	if(direction == -1){
		direction = D;
	}

	while(isBorder(map, r, c, direction)){
		direction -= leftright;

		if(direction == CONTROL){
			direction =  R;
		}
		if(direction == -1){
			direction =  D;
		}

	}

	return direction;

}

int start_border(Map *map, int r, int c, int leftright){

	if(leftright == LPATH){
        if(c == 1 && (r % 2) != 0){
			return D;
		}
		if(c == 1 && (r % 2) == 0){
			return R;
		}
		if(c == map->cols && (r % 2) != 0){
			return L;
		}
		if(c == map->cols && (r % 2) == 0){
			return U;
		}
		if(r == 1){
			return D;
		}
		if(r == map->rows){
			return U;
		}
    }

    if(leftright == RPATH){
        if(c == 1 && (r % 2) != 0){
			return R;
		}
		if(c == 1 && (r % 2) == 0){
			return R;
		}
		if(c == map->cols && (r % 2) != 0){
			return L;
		}
		if(c == map->cols && (r % 2) == 0){
			return L;
		}
		if(r == 1){
			return D;
		}
		if(r == map->rows){
			return U;
		}
    }

    return -1;

}

int checkMaze(Map *map){

	for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols - 1; j++) {
            if (isBorder(map, i, j, right) != isBorder(map, i, j + 1, left)) {
                return 0;
            }
        }
    }

    return 1;

}

void changePos(int* r, int* c, int direction){

	if(direction == L){
		(*c)--;
	}
	if(direction == R){
		(*c)++;
	}
	if(direction == U){
		(*r)--;
	}
	if(direction == D){
		(*r)++;
	}

}

bool isExit(Map *map, int r, int c, int direction){

	if(r == 1 && !isBorder(map, r, c, direction) && direction == U){
		return true;
	}
	if(r == map->rows && !isBorder(map, r, c, direction) && direction == D){
		return true;
	}
	if(c == 1 && !isBorder(map, r, c, direction) && direction == L){
		return true;
	}
	if(c == map->cols && !isBorder(map, r, c, direction) && direction == R){
		return true;
	}

	return false;
}

Map* mapConstruct(int rows, int cols){
 	Map *tempMap = malloc(sizeof(Map));

	if(tempMap != NULL){
		tempMap->cols = cols;
		tempMap->rows = rows;
		tempMap->cells = malloc((cols * rows) * sizeof(unsigned char));

		if(tempMap->cells == NULL){
			fprintf(stderr, "ERROR: MALLOC FAILED!");
		}

	}

	return tempMap;
} 

void mapDeconstruct(Map *map){

  	free(map->cells);
	map->cells = NULL;
	free(map);

}

bool isBorder(Map *map, int r, int c, int border){

    switch (border)
    {
    case R:
        border = right;
        break;
    case L:
        border = left;
        break;
    case U:
        if ((r+c) % 2 == 0){
			border = horizontal;
		}
        else{
			return true;
		}
        break;
    case D:
        if ((r+c) % 2 != 0){
			border = horizontal;
		}
        else{
			return true;
		}
        break;
    }
    int index = (r - 1) * map->cols + c - 1;
    int value = map->cells[index] - '0';

    return (value & (1 << border)) != 0;
}

int parseArgs(int argc, char* argv[], int* startR, int* startC){

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
			
			*startR = strtol(argv[r], NULL, 10);
			*startC = strtol(argv[c], NULL, 10);

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
	int count = 0;
   	fgetc(f);

    for (int i = 0; i < rows * cols; i++){
        char cells;
		
        while (isspace(cells = fgetc(f))){
        }

        if (cells == EOF){
            mapDeconstruct(map);
            fclose(f);
            return NULL;
        }
		if(cells < '0' || cells > '7'){
			return NULL;
		}

		count++;
        map->cells[i] = cells;
    }

	if(count != map->rows*map->cols){
		return NULL;
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
