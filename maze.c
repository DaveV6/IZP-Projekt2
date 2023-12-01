/** 
 *  IZP PROJECT 2 - working with data structures
 *  Author: David Bujzas, xbujzad00@stud.fit.vutbr.cz 
 *  Date: 20.11.2023 
 *  Note: The program takes in arguments from the user and uses them to seek paths out of the maze using the right or left hand rule
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

enum Command {
    PROG_NAME, COMMAND, R, C, TXT_FILE //arguments
};

enum CommandExecute {
    C_HELP, C_TEST, C_LPATH, C_RPATH, C_SHORTEST //execution of arguments
};

enum Direction {
    R_DIRECTION, U_DIRECTION, L_DIRECTION, D_DIRECTION, CONTROL_DIRECTION //directions
};

enum Border {
    LEFT_BORDER, RIGHT_BORDER, HORIZONTAL_BORDER //borders
};

#define R_PATH -1
#define L_PATH 1

typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

Map *mapConstruct(int rows, int cols);
void mapDeconstruct(Map *map);
bool isBorder(Map *map, int r, int c, int border);
int parseArgs(int argc, char *argv[], int *startR, int *startC);
Map *readMapFromFile(char *filename);
FILE *fileOpen(char *file);
bool isExit(Map *map, int r, int c, int direction);
int startBorder(Map *map, int r, int c, int leftright);
void changePos(int *r, int *c, int direction);
int changeDir(Map *map, int r, int c, int leftright, int direction);
void path(Map *map, int r, int c, int direction, int leftright);
bool checkMaze(Map *map);

int main(int argc, char *argv[]) {
    int startR = 0;
    int startC = 0;
    int direction = -1;
    int args = parseArgs(argc, argv, &startR, &startC);

    if (argc < 2) {
        fprintf(stderr, "ERROR: ENTER MORE ARGUMENTS!\n");
        return -1;
    }

	if (argc > 5) {
		fprintf(stderr, "ERROR: TOO MANY ARGUMENTS!\n");
		return -1;
	}
    
    Map *map = readMapFromFile(argv[argc - 1]);

    //if statement which checks if the user input the correct arguments
    if (args != -1) {
        //switching arguments between different cases
        switch (args) {

            case C_HELP: //if the user inputs --help the program outputs how to correctly use the program
                fprintf(stdout, "This program solves a maze which the user inputs from a .txt file using the following arguments:\n--help\n--test maze.txt - the argument tests if the .txt file is valid\n--lpath r c maze.txt - starts the path seeking sequence using the left hand rule\n--rpath r c maze.txt - starts the path seeking sequence using the right hand rule\n");
                break;

            case C_TEST: //if the map is NULL it is Invalid
                if(map == NULL || (map != NULL && checkMaze(map) == false)) {
                    fprintf(stdout, "Invalid\n");
                }
                else {
                    fprintf(stdout, "Valid\n");
                }
                break;

            case C_LPATH: //the program checks if the map is NULL, if it isn't it starts the path seeking sequence for the left hand rule
                if(map == NULL) {
                    fprintf(stderr, "FAILED TO READ MAP INCORRECT ARGUMENTS!\n");
                    break;
                }
                direction = startBorder(map, startR, startC, L_PATH);
                path(map, startR, startC, direction, L_PATH);
                break;

            case C_RPATH: //the program checks if the map is NULL, if it isn't it starts the path seeking sequence for the right hand rule
                if(map == NULL) {
                    fprintf(stderr, "FAILED TO READ MAP INCORRECT ARGUMENTS!\n");
                    break;
                }
                direction = startBorder(map, startR, startC, R_PATH);
				path(map, startR, startC, direction, R_PATH);
                break;
        }
    }

    if (map != NULL) {
        mapDeconstruct(map);
    }

    return 0;
}

/// @brief the function moves us from the entrance to the exit
/// @param map the map of the matrix
/// @param r the starting row
/// @param c the starting column
/// @param direction the direction in which we are starting
/// @param leftright left or  right path
/// @return it will print the left or right path depending on user input
void path(Map *map, int r, int c, int direction, int leftright) {
    if((r == 1 && isExit(map, r, c, U_DIRECTION)) || (r == map->rows && isExit(map, r, c, D_DIRECTION)) || (c == 1 && isExit(map, r, c, L_DIRECTION)) || (c == map->cols && isExit(map, r, c, R_DIRECTION))) {
        fprintf(stdout, "%d,%d\n", r, c);

        while (!isExit(map, r, c, direction)) {
            direction = changeDir(map, r, c, leftright, direction);

            if (!isExit(map, r, c, direction)) {
                changePos(&r, &c, direction);
                fprintf(stdout, "%d,%d\n", r, c);
            }
        }
    }
    else {
        fprintf(stderr, "YOU CANNOT SEEK A PATH WHEN STARTING INSIDE THE MAZE! START AT THE ENTRANCE!\n");
    }
	
}

bool checkMaze(Map *map) {
    for (int i = 1; i <= map->rows; i++) {
        for (int j = 1; j < map->cols; j++) {
            if (isBorder(map, i, j, R_DIRECTION) != isBorder(map, i, j + 1, L_DIRECTION)) {
                return false;
            }
        }
    }
    for (int i = 2; i <= map->rows; i++) {
        for (int j = 1; j <= map->cols; j++) {
            if (isBorder(map, i, j, U_DIRECTION) != isBorder(map, i - 1, j, D_DIRECTION)) {
                return false;
            }   
        }
    }
    return true;
}

/// @brief the function changes the direction we are facing at
/// @param map the map of the matrix
/// @param r the row we are located at
/// @param c the column we are located at
/// @param leftright left or right path
/// @param direction the direction we are facing in
/// @return it returns the changed direction
int changeDir(Map *map, int r, int c, int leftright, int direction) {
    direction += leftright;

    if (direction == CONTROL_DIRECTION) {
        direction = R_DIRECTION;
    }
    if (direction == -1) {
        direction = D_DIRECTION;
    }

    while (isBorder(map, r, c, direction)) {
        direction -= leftright;

        if (direction == CONTROL_DIRECTION) {
            direction = R_DIRECTION;
        }
        if (direction == -1) {
            direction = D_DIRECTION;
        }
    }

    return direction;
}

/// @brief the functions figures out our starting direction depending on which cell we are starting on
/// @param map the map of the matrix
/// @param r the starting row
/// @param c the starting column
/// @param leftright left or right path
/// @return the function returns a direction which we should face
int startBorder(Map *map, int r, int c, int leftright) {
    int odd = r % 2; 

    if (leftright == L_PATH) {
        if (c == 1 && odd != 0) {
            return D_DIRECTION;
        }
        if (c == 1 && odd == 0) {
            return R_DIRECTION;
        }
        if (c == map->cols && odd != 0) {
            return L_DIRECTION;
        }
        if (c == map->cols && odd == 0) {
            return U_DIRECTION;
        }
        if (r == 1) {
            return D_DIRECTION;
        }
        if (r == map->rows) {
            return U_DIRECTION;
        }
    } 
    else if (leftright == R_PATH) {
        if (c == 1 && odd != 0) {
            return R_DIRECTION;
        }
        if (c == 1 && odd == 0) {
            return R_DIRECTION;
        }
        if (c == map->cols && odd != 0) {
            return L_DIRECTION;
        }
        if (c == map->cols && odd == 0) {
            return L_DIRECTION;
        }
        if (r == 1) {
            return D_DIRECTION;
        }
        if (r == map->rows) {
            return U_DIRECTION;
        }
    }

    return -1;
}

/// @brief the function changes the position we are located at
/// @param r the row we are located at
/// @param c the column we are located at
/// @param direction the current direction
void changePos(int *r, int *c, int direction) {
    if (direction == L_DIRECTION) {
        (*c)--;
    } else if (direction == R_DIRECTION) {
        (*c)++;
    } else if (direction == U_DIRECTION) {
        (*r)--;
    } else if (direction == D_DIRECTION) {
        (*r)++;
    }
}

/// @brief the function checks if the current cell is an exit
/// @param map the map of the matrix
/// @param r the row for which we check the exit
/// @param c the col for which we check the exit
/// @param direction the direction in which we check the exit 
/// @return the function returns true if the cell is an exit and false if it is not
bool isExit(Map *map, int r, int c, int direction) {
    if (r == 1 && !isBorder(map, r, c, direction) && direction == U_DIRECTION) {
        return true;
    }
    if (r == map->rows && !isBorder(map, r, c, direction) && direction == D_DIRECTION) {
        return true;
    }
    if (c == 1 && !isBorder(map, r, c, direction) && direction == L_DIRECTION) {
        return true;
    }
    if (c == map->cols && !isBorder(map, r, c, direction) && direction == R_DIRECTION) {
        return true;
    }
    return false;
}


/// @brief the function allocates memory for the map
/// @param rows the amount of rows the matrix has
/// @param cols the amount of columns the matrix has
/// @return map
Map *mapConstruct(int rows, int cols) {
    Map *tempMap = malloc(sizeof(Map));

    if (tempMap != NULL) {
        tempMap->cols = cols;
        tempMap->rows = rows;
        tempMap->cells = malloc((cols * rows) * sizeof(unsigned char));

        if (tempMap->cells == NULL) {
            fprintf(stderr, "ERROR: MALLOC FAILED!\n");
            free(tempMap);
            return NULL;
        }
    }

    return tempMap;
}

/// @brief the functions frees the allocated memory
/// @param map the map of the matrix
void mapDeconstruct(Map *map) {
    free(map->cells);
    free(map);
}

/// @brief the function checks if there is a border in each direction using left shift
/// @param map the map of the matrix
/// @param r the row for which we check if it has a border
/// @param c the column for which we check if it has a border
/// @param border the border for which we check if it is true
/// @return if the border is present and the function returns true otherwise it returns false.
bool isBorder(Map *map, int r, int c, int border) {
    int index = (r - 1) * map->cols + c - 1;
    int val = map->cells[index] - '0';
    int parity = (r+c) % 2;

    switch (border) {
        case R_DIRECTION:
            border = RIGHT_BORDER;
            break;
        case L_DIRECTION:
            border = LEFT_BORDER;
            break;
        case D_DIRECTION:
            if (parity != 0) {
                border = HORIZONTAL_BORDER;
            } 
            else {
                return true;
            }
            break;
        case U_DIRECTION:
            if (parity == 0) {
                border = HORIZONTAL_BORDER;
            } 
            else {
                return true;
            }
            break;
    }

    return (val & (1 << border)) != 0;
}

/// @brief the function parses arguments and switches them depending on how many the user inputs
/// @param argc count of arguments
/// @param argv array of arguments
/// @param startR starting row which the user inputs
/// @param startC starting column which the user inputs
/// @return the return depends on which case the user triggers
int parseArgs(int argc, char *argv[], int *startR, int *startC) {
    switch (argc) {
        case 2:
            if (!strcmp(argv[COMMAND], "--help")) {
                return C_HELP;
            }
            break;

        case 3:
            if (!strcmp(argv[COMMAND], "--test")) {
                return C_TEST;
            }
            break;

        case 5:
            *startR = strtol(argv[R], NULL, 10);
            *startC = strtol(argv[C], NULL, 10);

            if (!strcmp(argv[COMMAND], "--lpath")) {
                return C_LPATH;
            }

            if (!strcmp(argv[COMMAND], "--rpath")) {
                return C_RPATH;
            }
            break;
    }

    return -1;
}

/// @brief the functions reads the matrix from the .txt file and assigns values to rows, columns and cells
/// @param filename 
/// @return map
Map *readMapFromFile(char *filename) {
    FILE *f = fileOpen(filename);

    if (!f) {
        return NULL;
    }

    int rows, cols;
    if (fscanf(f, "%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "ERROR: FAILED TO READ ROWS AND COLUMNS FROM FILE\n");
        fclose(f);
        return NULL;
    }

    if(rows < 0 || cols < 0){
        fprintf(stderr, "NEGATIVE NUMBERS IN .TXT FILE!\n");
        fclose(f);
        return NULL;
    }

    Map *map = mapConstruct(rows, cols);
    if (!map) {
        fclose(f);
        return NULL;
    }

    int count = 0;
    fgetc(f);

    for (int i = 0; i < rows * cols; i++) {
        char cells;

        while (isspace(cells = fgetc(f))) {
        }

        if (cells == EOF) {
            mapDeconstruct(map);
            fclose(f);
            return NULL;
        }

        if (cells < '0' || cells > '7') {
            mapDeconstruct(map);
            fclose(f);
            return NULL;
        }

        count++;
        map->cells[i] = cells;
    }

    if (count != map->rows * map->cols) {
        mapDeconstruct(map);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return map;
}

/// @brief the functions opens the .txt file from user input
/// @param file the file from user input
/// @return file
FILE *fileOpen(char *file) {
    FILE *f = fopen(file, "r");
    return f;
}
