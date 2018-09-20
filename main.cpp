#include <stdio.h>
#include <stdlib.h>
#include "MazeParams.h"
#include "Recursion.h"
#include <time.h>
#include <assert.h>


int maze[MATRIX_SIZE][MATRIX_SIZE];
#define WALL (1 << 30)
#define MARK (1 << 29)
#define MASK (((-1) ^ WALL) ^ MARK)
int start_col; // the starting column for the maze;

struct MartianSolution {
    int goal;
    Martian solution;
};

//for nicks = 2, dodeks = 5
MartianSolution solns[16] = {
	{0, {0, 0, 0}},
	{1, {1, 0, 0}},
	{2, {0, 1, 0}},
	{3, {1, 1, 0}},
	{4, {0, 2, 0}},
	{5, {0, 0, 1}},
	{6, {1, 0, 1}},
	{7, {0, 1, 1}},
	{8, {1, 1, 1}},
	{9, {0, 2, 1}},
	{10, {0, 0, 2}},
	{11, {1, 0, 2}},
	{12, {0, 1, 2}},
	{13, {1, 1, 2}},
	{14, {0, 2, 2}},
	{15, {0, 0, 3}}
};

//for nicks = 4, dodeks = 9
MartianSolution solnz[17] = {
	{0, {0, 0, 0}},
	{1, {1, 0, 0}},
	{2, {2, 0, 0}},
	{3, {3, 0, 0}},
	{4, {0, 1, 0}},
	{5, {1, 1, 0}},
	{6, {2, 1, 0}},
	{7, {3, 1, 0}},
	{8, {0, 2, 0}},
	{9, {0, 0, 1}},
	{10, {1, 0, 1}},
	{11, {2, 0, 1}},
	{12, {0, 3, 0}},
	{13, {0, 1, 1}},
	{14, {1, 1, 1}},
	{15, {2, 1, 1}},
	{16, {0, 4, 0}}
};

//for nicks = 5, dodeks = 6
MartianSolution solnzzz[27] = {
	{0, {0,0,0}},
	{1, {1,0,0}},
	{2, {2,0,0}},
	{3, {3,0,0}},
	{4, {4,0,0}},
	{5, {0,1,0}},
	{6, {0,0,1}},
	{7, {1,0,1}},
	{8, {2,0,1}},
	{9, {3,0,1}},
	{10, {0,2,0}},
	{11, {0,1,1}},
	{12, {0,0,2}},
	{13, {1,0,2}},
	{14, {2,0,2}},
	{15, {0,3,0}},
	{16, {0,2,1}},
	{17, {0,1,2}},
	{18, {0,0,3}},
	{19, {1,0,3}},
	{20, {0,4,0}},
	{21, {0,3,1}},
	{22, {0,2,2}},
	{23, {0,1,3}},
	{24, {0,0,4}},
	{25, {1,0,4}},
	{25, {0,5,0}}
};

void printMaze(void) {
	unsigned r, c;
	for (r = 0; r < MATRIX_SIZE; r += 1) {
		for (c = 0; c < MATRIX_SIZE; c += 1) {
			switch (maze[r][c] ) {
			case 0:
				putchar(' ');
				break;
			case 1:	
				putchar('#');
				break;
			case 2: // bread crumb
				putchar('o');
				break;
			default: // error!
				putchar('@');
				break;			
			}
		}
		putchar('\n');
	}
}
 	
void printCodedMaze(void) {
	unsigned r, c;
	for (r = 0; r < MATRIX_SIZE; r += 1) {
		for (c = 0; c < MATRIX_SIZE; c += 1) {
			switch(maze[r][c] & WALL) {
			case WALL: 
				putchar('#');
				break;
			case 0:
				putchar(' ');
				break;
			}
		}
		putchar('\n');
	}
}

enum Directions {
	UP,
	DOWN, 
	LEFT,
	RIGHT
};

void interpretDir(int* prow, int* pcol, int dir) {
	switch (dir) {
		case UP: *prow = *prow - 1; break;
		case DOWN: *prow = *prow + 1; break;
		case LEFT: *pcol = *pcol - 1; break;
		case RIGHT: *pcol = *pcol + 1; break;
	}
}

void clearWall(int row, int col, int dir) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	interpretDir(&r, &c, dir);
	maze[r][c] &= ~WALL;
}

void markCell(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	
	maze[r][c] |= MARK;
}

int isMarked(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	
	return (maze[r][c] & MARK);
}


/*
 * return an integer that uniquely identifies each cell
 */
int cellID(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	return r * MATRIX_SIZE + c;
}

/*
 * reverse the cellID to compute the row and col numbers
 */
void findCell(int cell, int* prow, int* pcol) {
	int r = cell / MATRIX_SIZE;
	int c = cell % MATRIX_SIZE;
	*prow = r / 2;
	*pcol = c / 2;
}

/*
 * store some annotation information in a cell 
 * (the annotation must be an int less than 25 bits (i.e., smaller than 32 million)
 */
void annotateCell(int row, int col, int annotation) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	
	maze[r][c] &= ~MASK;
	maze[r][c] |= annotation;
}

int annotation(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	
	return maze[r][c] & MASK;
}

void clearCellMark(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	
	maze[r][c] &= ~MARK;
}

void clearCell(int row, int col) {
	int r = row * 2 + 1;
	int c = col * 2 + 1;
	
	maze[r][c] = 0;
}

void makeAllWalls(void) {
	unsigned r, c;
	unsigned row, col;
	for (r = 0; r < MATRIX_SIZE; r += 1) {
		for (c = 0; c < MATRIX_SIZE; c += 1) {
			maze[r][c] = WALL;
		}
	}
	for (row = 0; row < MAZE_SIZE; row += 1) {
		for (col = 0; col < MAZE_SIZE; col += 1) {
			clearCell(row, col);
		}
	}
}

/*
 * this function makes a random maze with exactly one path between
 * any two points in the maze
 *
 * If you're curious about the algorithm, come talk to me.  It's not very
 * complicated (although the code might be confusing)
 */
void makeMaze() {
	int num_visited = 1;
	int first;
	int finish_col;
	makeAllWalls();
	markCell(0, 0);  // mark the first cell as visited
	
	/* add the first cell (row 0, col 0) to the linked list of visited cells */
	first = cellID(0, 0);
	annotateCell(0, 0, 0);
	srand(time(NULL)); 
	
	while(num_visited < MAZE_SIZE * MAZE_SIZE) {
		int visit = rand() % num_visited;
		int cell = first; 
		int row, col;
		int d;
		int nrow, ncol;

		findCell(cell, &row, &col);
		while (visit > 0) {
			cell = annotation(row, col);
			findCell(cell, &row, &col);
			visit -= 1;
		}
		d = rand() % 4;
		nrow = row; // row of neighbor cell
		ncol = col; // col of neighbor cell
		interpretDir(&nrow, &ncol, d);
		if (nrow >= 0 && nrow < MAZE_SIZE
			&& ncol >= 0 && ncol < MAZE_SIZE
			&& !isMarked(nrow, ncol)) {
			clearWall(row, col, d);
			num_visited += 1;
			markCell(nrow, ncol);
			annotateCell(nrow, ncol, first);
			first = cellID(nrow, ncol);	
		}
	}
	
	start_col = rand() % MAZE_SIZE;
	start_col = 2 * start_col + 1;
	maze[0][start_col] &= ~WALL;
	finish_col = rand() % MAZE_SIZE;
	maze[MATRIX_SIZE - 1][2 * finish_col + 1] &= WALL;
}


/*
 * recode the maze so that all walls are exactly 1 and all 
 * other cells are exactly zero
 */
void recodeMaze(void) {
	int r, c;
	for (r = 0; r < MATRIX_SIZE; r += 1) {
		for (c = 0; c < MATRIX_SIZE; c += 1) {
			maze[r][c] = ((maze[r][c] & WALL) == WALL);
		}
	}
}

int smallestElement(int arr[], int length) {
	int smallest = 2147483647;
	for (int i = 0; i < length; i++) {
		if (arr[i] < smallest)
			smallest = arr[i];
	}
	return smallest;
}

void testSmallestAlpha() {
	int length = (rand() % 1000 + 1);
	int arr[length];
	for(int j = 0; j < 10; j++){
		srand(j);
		for(int i = 0; i < length; i++) {
			arr[i] = rand() % 100 + 1;
		}
		assert(smallestElement(arr, length) == minRec1(arr, length));
		assert(smallestElement(arr, length) == minRec2(arr, length));
	}
	printf("You passed all the minRec1 and minRec2 tests\n");
}
	

int main(void) {
	const int magic_number = time(NULL);

	/* test min */
	printf("Starting min tests:\n");
	int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	printf("the smallest of the first 10 natural numbers is: %d\n", minIt(a, 10));
	printf("the smallest of the first 10 natural numbers is: %d\n", minRec1(a, 10));
	printf("the smallest of the first 10 natural numbers is: %d\n", minRec2(a, 10));
	a[3] = -1;
	printf("now the smallest is %d\n", minIt(a, 10));
	printf("now the smallest is %d\n", minRec1(a, 10));
	printf("now the smallest is %d\n", minRec2(a, 10));

	int b[] = {6, 7, 9, 10, 13, -5, -7, -4, 8};
	printf("the smallest number here is %d, and it should be -7\n", minIt(b, 9));
	printf("the smallest number here is %d, and it should be -7\n", minRec1(b, 9));
	printf("the smallest number here is %d, and it should be -7\n", minRec2(b, 9));
	b[6] = -10;
	printf("the smallest number is now %d, and it should be -10\n", minIt(b, 9));
	printf("the smallest number is now %d, and it should be -10\n", minRec1(b, 9));
	printf("the smallest number is now %d, and it should be -10\n", minRec2(b, 9));
	b[6] = 10;
	printf("the smallest number is now %d, and it should be -5\n", minIt(b, 9));
	printf("the smallest number is now %d, and it should be -5\n", minRec1(b, 9));
	printf("the smallest number is now %d, and it should be -5\n", minRec2(b, 9));
	
	int c[] = {0, 1, 2, 3, 4, 5, 6, 6, 7, 7, 6, 9, 10, 17, 13};
	printf("the smallest number is %d, and it should be 0\n", minIt(c, 15));
	printf("the smallest number is %d, and it should be 0\n", minRec1(c, 15));
	printf("the smallest number is %d, and it should be 0\n", minRec2(c, 15));
	c[14] = -1;
	printf("the smallest number is %d, and it should be -1\n", minIt(c, 15));
	printf("the smallest number is %d, and it should be -1\n", minRec1(c, 15));
	printf("the smallest number is %d, and it should be -1\n", minRec2(c, 15));	

	testSmallestAlpha();

	/* test sqrt */
	printf("the sqrt of 25 is %g\n", sqrtIt(25.0, 0, 25.0));
	printf("the sqrt of 26 is %g\n", sqrtIt(26.0, 0, 26.0));
	printf("the sqrt of 2 is %g\n", sqrtIt(2.0, 0, 2.0));
	printf("the sqrt of 25 is %g\n", sqrtRec(25.0, 0, 25.0));
	printf("the sqrt of 26 is %g\n", sqrtRec(26.0, 0, 26.0));
	printf("the sqrt of 2 is %g\n", sqrtRec(2.0, 0, 2.0));
	printf("the sqrt of 4 is %g\n", sqrtRec(4.0, 0, 4.0));
	printf("the sqrt of 10 is %g\n", sqrtRec(10.0, 0, 10.0));
	printf("the sqrt of 237 is %g\n", sqrtRec(237.0, 0, 237.0));
	printf("the sqrt of 16 is %g\n", sqrtRec(16.0, 0, 16.0));
	printf("the sqrt of 9 is %g\n", sqrtRec(9.0, 0, 9.0));
	printf("the sqrt of 36 is %g\n", sqrtRec(36.0, 0, 36.0));
	printf("the sqrt of 49 is %g\n", sqrtRec(49.0, 0, 49.0));
	printf("the sqrt of 64 is %g\n", sqrtRec(64.0, 0, 64.0));
	printf("the sqrt of 81 is %g\n", sqrtRec(81.0, 0, 81.0));
	printf("the sqrt of 100 is %g\n", sqrtRec(100.0, 0, 100.0));
	printf("the sqrt of 121 is %g\n", sqrtRec(121.0, 0, 121.0));
	printf("the sqrt of 144 is %g\n", sqrtRec(144.0, 0, 144.0));
	printf("the sqrt of 169 is %g\n", sqrtRec(169.0, 0, 169.0));
	printf("the sqrt of 1 is %g\n", sqrtRec(1.0, 0, 1.0));
	printf("the sqrt of 577 is %g\n", sqrtRec(577.0, 0, 577.0));
	printf("the sqrt of 0.25 is %g\n", sqrtRec(0.25, 0, 1.0));
	printf("the sqrt of 0.375 is %g\n", sqrtRec(0.375, 0, 1.0));
	printf("the sqrt of 0.5 is %g\n", sqrtRec(0.5, 0, 1.0));
	printf("the sqrt of 0.7 is %g\n", sqrtRec(0.7, 0, 1.0));
	printf("the sqrt of 0.9 is %g\n", sqrtRec(0.9, 0, 1.0));
	printf("the sqrt of 160000 is %g\n", sqrtRec(160000.0, 0, 160000.0));

	/* test strCompare */
	printf("Starting strCompare tests:\n");
	char* s1; char* s2;
	s1 = "apple"; s2 = "apricot";
	if (strCompare(s1, s2) < 0) { 
		printf("\"%s\" is less than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be less than \"%s\"\n", s1, s2);
	}
			
	s1 = "Apple"; s2 = "apple";
	if (strCompare(s1, s2) < 0) { 
		printf("\"%s\" is less than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be less than \"%s\"\n", s1, s2);
	}

	s1 = "baby boy"; s2 = "banana";
	if (strCompare(s1, s2) < 0) { 
		printf("\"%s\" is less than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be less than \"%s\"\n", s1, s2);
	}

	s1 = "a rather silly string"; s2 = "a rather silly string";
	if (strCompare(s1, s2) == 0) { 
		printf("\"%s\" is equal to \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be equal to \"%s\"\n", s1, s2);
	}

	s1 = "12345"; s2 = "12345";
	if (strCompare(s1, s2) == 0) { 
		printf("\"%s\" is equal to \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be equal to \"%s\"\n", s1, s2);
	}

	s1 = "Numbers: 12345"; s2 = "12345";
	if (strCompare(s1, s2) > 0) { 
		printf("\"%s\" is greater than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be greater than \"%s\"\n", s1, s2);
	}

	s1 = "Texas"; s2 = "California";
	if (strCompare(s1, s2) > 0) { 
		printf("\"%s\" is greater than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be greater than \"%s\"\n", s1, s2);
	}

	s1 = "apple"; s2 = "applebottomjeans";
	if (strCompare(s1, s2) < 0) {
		printf("\"%s\" is less than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be less than \"%s\"\n", s1, s2);
	}

	s1 = " apple"; s2 = "apple";
	if (strCompare(s1, s2) < 0) {
		printf("\"%s\" is less than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be less than \"%s\"\n", s1, s2);
	}

	/* test strCompare2 */
	printf("Starting strCompare2 tests:\n");
	s1 = "apple"; s2 = "Apricot";
	if (strCompare2(s1, s2) < 0) { 
		printf("\"%s\" is less than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be less than \"%s\"\n", s1, s2);
	}

	s1 = "Batman!"; s2 = "bat man";
	if (strCompare2(s1, s2) == 0) { 
		printf("\"%s\" is equal to \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be equal to \"%s\"\n", s1, s2);
	}

	s1 = "OMG, WTF?"; s2 = "oh my goodness, what the heck?";
	if (strCompare2(s1, s2) > 0) { 
		printf("\"%s\" is greater than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be greater than \"%s\"\n", s1, s2);
	}

	s1 = " ZZZ"; s2 = "aaa";
	if (strCompare2(s1, s2) > 0) {
		printf("\"%s\" is greater than \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be equal to \"%s\"\n", s1, s2);
	}

	s1 = "applebottomjeans"; s2 = "apple bottom JEANS!";
	if (strCompare2(s1, s2) == 0) { 
		printf("\"%s\" is equal to \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be equal to \"%s\"\n", s1, s2);
	}

	s1 = "o-07253-032- 0]r 3-21[].';';.';.;'';.';a.';346';.2n5';2.5g21.e'12;.56;'";
	s2 = "orange";
	if (strCompare2(s1, s2) == 0) { 
		printf("\"%s\" is equal to \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be equal to \"%s\"\n", s1, s2);
	}

	s1 = "orange";
	s2 = "o-07253-032- 0]r 3-21[].';';.';.;'';.';a.';346';.2n5';2.5g21.e'12;.56;'";
	if (strCompare2(s1, s2) == 0) { 
		printf("\"%s\" is equal to \"%s\", very good\n", s1, s2);
	} else {
		printf("oops, \"%s\" should be equal to \"%s\"\n", s1, s2);
	}

	/* test maze */		
	srand(magic_number);
	makeMaze();
	recodeMaze();
	for (int i = 0; i < MATRIX_SIZE; i++) {
		maze[MATRIX_SIZE - 1][i] = 1;
	}
	printf("recursive solution to the maze\n");
	printMaze();
	printf("\n\n\n");
	solveMazeRec(0, start_col);	
	printMaze();
	printf("\n\n\n");
	
	/*printf("iterative solution to the maze\n");
	srand(magic_number);
	makeMaze();
	recodeMaze();
	solveMazeIt(0, start_col);
	printMaze();*/


	/* test Martian */
	Martian change1 = change(15);
	printf("change 1 should be 0d, 3n, 0p and is: %dd %dn %dp\n", change1.dodeks, change1.nicks, change1.pennies);

	Martian change2 = change(0);
	printf("change 2 should be 0d, 0n, 0p and is: %dd %dn %dp\n", change2.dodeks, change2.nicks, change2.pennies);

	Martian change3 = change(17);
	printf("change 3 should be 1d, 1n, 0p and is: %dd %dn %dp\n", change3.dodeks, change3.nicks, change3.pennies);

	Martian change4 = change(25);
	printf("change 4 should be 2d, 0n, 1p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);

	printf("\nWhen nick = 2, dodek = 5:");
	for (int i = 0; i < 16; i++) {
		change4 = change(solns[i].goal, 2, 5);
		printf("\n%d cents would be %dd, %dn, %dp and is: %dd %dn %dp\n", i, solns[i].solution.dodeks, solns[i].solution.nicks, solns[i].solution.pennies, change4.dodeks, change4.nicks, change4.pennies);
	}

	printf("\nWhen nick = 4, dodek = 9");
	for (int i = 0; i < 17; i++) {
		change4 = change(solnz[i].goal, 4, 9);
		printf("\n%d cents would be %dd, %dn, %dp and is: %dd %dn %dp\n", i, solnz[i].solution.dodeks, solnz[i].solution.nicks, solnz[i].solution.pennies, change4.dodeks, change4.nicks, change4.pennies);
	}

	printf("\nWhen nick = 5, dodek = 6");
	for (int i = 0; i < 27; i++) {
		change4 = change(solnzzz[i].goal, 5, 6);
		printf("\n%d cents would be %dd, %dn, %dp and is: %dd %dn %dp\n", solnzzz[i].goal, solnzzz[i].solution.dodeks, solnzzz[i].solution.nicks, solnzzz[i].solution.pennies, change4.dodeks, change4.nicks, change4.pennies);
	}

	/* A very simple and obvious test of the general form of Martian
	 * be sure and test your solution more thoroughly!!!! */
	change4 = change(25, 5, 12);
	printf("change 4 should be 2d, 0n, 1p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);

	change4 = change(90, 10, 11);
    printf("change 4 should be 0d, 9n, 0p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);
    change4 = change(88, 10, 11);
    printf("change 4 should be 8d, 0n, 0p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);
    change4 = change(15, 5, 6);
    printf("change 4 should be 0d, 3n, 0p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);
    change4 = change(174, 75, 100);
    printf("change 4 should be 0d, 2n, 24p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);
    change4 = change(150, 5, 12);
    printf("change 4 should be 12d, 1n, 1p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);
    change4 = change(300, 5, 12);
    printf("change 4 should be 25d, 0n, 0p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);
    change4 = change(600, 5, 12);
    printf("change 4 should be 50d, 0n, 0p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);
    change4 = change(30000, 5, 7);
    printf("change 4 should be 4285d, 1n, 0p and is: %dd %dn %dp (efficiency test)\n", change4.dodeks, change4.nicks, change4.pennies);
    change4 = change(30000, 5, 3457);
    printf("change 4 should be 8d, 468n, 4p and is: %dd %dn %dp\n", change4.dodeks, change4.nicks, change4.pennies);
}
