#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

enum Command {
    PROG_NAME, COMMAND, R, C, TXT_FILE
};

enum CommandExecute {
    C_HELP, C_TEST, C_LPATH, C_RPATH, C_SHORTEST
};

enum Direction {
    R_DIRECTION, U_DIRECTION, L_DIRECTION, D_DIRECTION, CONTROL_DIRECTION
};

enum Border {
    LEFT_BORDER, RIGHT_BORDER, HORIZONTAL_BORDER
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
int path(Map *map, int r, int c, int direction, int leftright);

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

    if (args != -1) {

        switch (args) {

            case C_HELP:
                fprintf(stdout, "THE GOAL OF THE PROGRAM IS TO EXIT THE MAZE\n");
                break;

            case C_TEST:
                fprintf(stdout, map ? "Valid\n" : "Invalid\n");
                break;

            case C_LPATH:
                direction = startBorder(map, startR, startC, L_PATH);
                path(map, startR, startC, direction, L_PATH);
                break;

            case C_RPATH:
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

int path(Map *map, int r, int c, int direction, int leftright) {
	fprintf(stdout, "%d,%d\n", r, c);

	while (!isExit(map, r, c, direction)) {
		direction = changeDir(map, r, c, leftright, direction);

		if (!isExit(map, r, c, direction)) {
			changePos(&r, &c, direction);
			fprintf(stdout, "%d,%d\n", r, c);
		}
	}

	return r;
	return c;
}

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

int startBorder(Map *map, int r, int c, int leftright) {
    if (leftright == L_PATH) {
        if (c == 1 && (r % 2) != 0) {
            return D_DIRECTION;
        }
        if (c == 1 && (r % 2) == 0) {
            return R_DIRECTION;
        }
        if (c == map->cols && (r % 2) != 0) {
            return L_DIRECTION;
        }
        if (c == map->cols && (r % 2) == 0) {
            return U_DIRECTION;
        }
        if (r == 1) {
            return D_DIRECTION;
        }
        if (r == map->rows) {
            return U_DIRECTION;
        }
    } else if (leftright == R_PATH) {
        if (c == 1 && (r % 2) != 0) {
            return R_DIRECTION;
        }
        if (c == 1 && (r % 2) == 0) {
            return R_DIRECTION;
        }
        if (c == map->cols && (r % 2) != 0) {
            return L_DIRECTION;
        }
        if (c == map->cols && (r % 2) == 0) {
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

int checkMaze(Map *map) {
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols - 1; j++) {
            if (isBorder(map, i, j, RIGHT_BORDER) != isBorder(map, i, j + 1, LEFT_BORDER)) {
                return 0;
            }
            if (isBorder(map, i + 1, j, HORIZONTAL_BORDER) != isBorder(map, i, j, HORIZONTAL_BORDER)) {
                return 0;
            }
        }
    }
    return 1;
}

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

void mapDeconstruct(Map *map) {
    free(map->cells);
    free(map);
}

bool isBorder(Map *map, int r, int c, int border) {
    int index = (r - 1) * map->cols + c - 1;
    int val = map->cells[index] - '0';

    switch (border) {
        case R_DIRECTION:
            border = RIGHT_BORDER;
            break;
        case L_DIRECTION:
            border = LEFT_BORDER;
            break;
        case U_DIRECTION:
            if ((r + c) % 2 == 0) {
                border = HORIZONTAL_BORDER;
            } else {
                return true;
            }
            break;
        case D_DIRECTION:
            if ((r + c) % 2 != 0) {
                border = HORIZONTAL_BORDER;
            } else {
                return true;
            }
            break;
    }

    return (val & (1 << border)) != 0;
}

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

FILE *fileOpen(char *file) {
    FILE *f = fopen(file, "r");

    if (f == NULL) {
        fprintf(stderr, "ERROR: FILE FAILED TO OPEN! TRY AGAIN!\n");
    }

    return f;
}
