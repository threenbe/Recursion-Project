/*
 * Project5.c
 *
 * Name: Raiyan Chowdhury
 * TA: Ka Tai
 * Section Time: Class Tues/Thurs 3:30-5:00, TA section Mon 2:00-3:00
 *
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "MazeParams.h"
#include "Recursion.h"

/***************************minIt***************************
 * returns the smallest of the elements in array x[]
 * INPUTS: array x[], number of elements n
 * OUTPUTS: smallest element 
 */
int minIt(int x[], int n) {
    int min = x[0];

    for(int i = 1; i < n; i++) {
        if(x[i] < min) min = x[i];
    }

    return min;
}

/***************************minRec1***************************
 * returns the smallest of the elements in array x[]
 * uses recursion with n-1 decomposition
 * INPUTS: array x[], number of elements n
 * OUTPUTS: smallest element 
 */
int minRec1(int x[], int n) {
    if(n == 1) {
        return x[0];
    }

    else {
        int min = minRec1(x+1, n-1);
        if(min < x[0]) return min;
        else return x[0];
    }
}

/***************************minRec2***************************
 * returns the smallest of the elements in array x[]
 * uses recursion with n/2 decomposition
 * INPUTS: array x[], number of elements n
 * OUTPUTS: smallest element
 */
int minRec2(int x[], int n) {
    if(n == 1) {
        return x[n-1];
    }
    else if(n == 2) {
        if(x[n-1] < x[n-2]) return x[n-1];
        else return x[n-2];
    }

    else {
    //keep splitting array into halves until comparison becomes trivial
        int min1 = minRec2(x, n/2);//lower half of array
        int min2 = minRec2(x + (n/2), n - (n/2));//upper half

        if(min1 < min2) return min1;
        else return min2;
    }
}


/***************************sqrtIt***************************
 * calculates and returns the square root of x with 15 decimal digits of accuracy
 * (iterative version)
 * INPUTS: x, two guesses that are smaller and larger than the real sqrt
 * OUTPUTS: square root of x
 */
double sqrtIt(double x, double low_guess, double high_guess) {
    double guess = (low_guess + high_guess)/2;
    //if |guess^2 - x| < diff, our guess is close enough
    double diff = 0.000000000000001;
    
    while (guess*guess != x && high_guess - low_guess > diff) {
        //keep guessing
        if (guess*guess > x) {
            high_guess = guess;
            guess = (low_guess + high_guess)/2;
        }
        else if (guess*guess < x) {
            low_guess = guess;
            guess = (low_guess + high_guess)/2;
        }
    }
    return guess;
}

/***************************sqrtRec***************************
 * calculates and returns the square root of x with 15 decimal digits of accuracy
 * (recursion version)
 * INPUTS: x, two guesses that are smaller and larger than the real sqrt
 * OUTPUTS: square root of x
 */
double sqrtRec(double x, double low_guess, double high_guess) {
    double guess = (low_guess + high_guess)/2;
    //if |guess^2 - x| < diff, our guess is close enough
    double diff = 0.000000000000001;

    if (guess*guess == x || high_guess - low_guess < diff)
        return guess;//sqrt found, or close enough

    //if we reach here, keep guessing
    else if (guess*guess > x) 
        return sqrtRec(x, low_guess, guess);
    else if (guess*guess < x) 
        return sqrtRec(x, guess, high_guess);
}


/***************************strCompare***************************
 * this function is a string comparison function that uses ASCII
 * values to compare two strings
 * INPUTS: two character strings
 * OUTPUTS:
 * returns -1 if str1 is less than str2
 * returns 0 if the two strings are equal
 * returns +1 if str1 is greater than str2
 */

int strCompare(char* str1, char* str2) {
    if (str1[0] < str2[0]) return -1;
    else if (str1[0] > str2[0]) return 1;

    //keeps calling itself until ends of both strings are reached (equal)
    //or until a difference is finally found (return -1 or 1)
    else {
        if (str1[0] == '\0') return 0;
        else return strCompare(str1+1, str2+1);
    }
}

/***************************whatLetter***************************
 * if c is not a letter return -1
 * if c is a letter, return the position of c in the alphabet 
 * where 'A' (and 'a') are position 1 and 'Z' (and 'z') are position 26
 *
 * This code is rather ugly as I'm exploiting some detailed knowledge of the ASCII table
 */
int whatLetter(char c) {
    if (c < 'A') return -1;
    if (c > 'z') return -1;
    if (c > 'Z' && c < 'a') return -1;
    return c & ~32 - 64;
}

/***************************whatLetter2***************************
 * a modified version of the whatLetter function that returns a 0
 * if passed a null character--apart from that it is the same,
 * return -1 if c is not a letter, else return c's position in the alphabet
 */
int whatLetter2(char c) {
    if (c == '\0') return 0;//returns 0 if null
    if (c < 'A') return -1;
    if (c > 'z') return -1;
    if (c > 'Z' && c < 'a') return -1;
    return c & ~32 - 64;
}

/***************************strCompare2***************************
 * this function performs a string comparison that ignores any non-letter
 * characters as well as the case of the letters
 * INPUTS: two character strings
 * OUTPUTS:
 * returns -1 if str1 is less than str2
 * returns 0 if the two strings are equal
 * returns +1 if str1 is greater than str2
 */

int strCompare2(char* str1, char* str2) {
    //if the current character in either string is not a letter, skip it
    if (whatLetter2(str1[0]) == -1 && whatLetter2(str2[0]) == -1)
        return strCompare2(str1+1, str2+1);

    else if (whatLetter2(str1[0]) == -1)
        return strCompare2(str1+1, str2);

    else if (whatLetter2(str2[0]) == -1)
        return strCompare2(str1, str2+1);

    //returns -1 or 1 when difference is found
    else if (whatLetter2(str1[0]) < whatLetter2(str2[0])) 
        return -1;
    else if (whatLetter2(str1[0]) > whatLetter2(str2[0])) 
        return 1;

    //keeps calling itself until ends of both strings are reached (equal)
    //or until a difference is found (return -1 or 1)
    else if (whatLetter2(str1[0]) == whatLetter2(str2[0])) {
        if (str1[0] == '\0') return 0;
        else return strCompare2(str1+1, str2+1);
    }
}


/***************************solveMazeRec***************************
 * this function solves a maze surrounded on all four sides by walls, with
 * one entry point in the top row and one exit point in the bottom row
 * INPUTS: inital row/column numbers
 * OUTPUT: true/false values that indicate whether or not we're on the correct
 * path to the exit or not
 */

int solveMazeRec(int row, int col) {
    //when exit point is reached:
    if (row == MATRIX_SIZE - 1 && maze[row][col] == 0) {
        maze[row][col] = 2;
        return 1;//success!
    }
    //when in any invalid square (wall or breadcrumb from before):
    else if (maze[row][col] != 0) {
        return 0;//go back
    }

    //when in any valid square sans the exit:
    else {
        maze[row][col] = 2;

        if (solveMazeRec(row+1, col))//try going down
            return 1;
        else if (solveMazeRec(row-1, col))//try going up
            return 1;
        else if (solveMazeRec(row, col+1))//try going right
            return 1;
        else if (solveMazeRec(row, col-1))//try going left
            return 1;
        else {//can't go anywhere from current square
            maze[row][col] = 0;
            return 0;//go back
        }
    }
}


/**********************
 * adjacentCell and isOK are functions provided to help you write
 * solveMazeIt()
 */

/*
 * OK, we're currently at maze[row][col] and we're considering moving
 * in direction dir.  
 * Set trow and tcol (local variables inside the calling function)
 * to the row and column that we would move to IF we moved in
 * that direction
 *
 * For example, there are two good ways to use this function.
 * 1. to actually move one step in a direction use:
 *       adjacentCell(row, col, dir, &row, &col);
 *    That will set row and col to new values.  The new values will
 *    be one square away from the old values.
 *
 * 2. to set trow and tcol to a square that is adjacent to row and col use:
 *       adjacentCell(row, col, dir, &trow, &tcol);
 *    That will not change row and col, but will change trow and tcol.
 *    This is useful if you aren't sure if you can actually move in this 
 *    direction yet (e.g., maze[trow][tcol] may be a wall!).  So, you set
 *    trow and tcol, and then check to see if it's OK to move there
 */
void adjacentCell(int row, int col, int dir, int* trow, int* tcol) {
    *trow = row;
    *tcol = col;
    switch(dir) {
    case 0: // UP
        *trow = *trow - 1;
        break;
    case 1: // RIGHT
        *tcol = *tcol + 1;
        break;
    case 2: // DOWN
        *trow = *trow + 1;
        break;
    case 3: // LEFT
        *tcol = *tcol - 1;
        break;
    }
}


/* 
 * return false if there is a wall in the square for row and col
 * return true if it's not a wall.
 */
int isOK(int row, int col) {
    return (row > 0 && row < MATRIX_SIZE
        && col > 0 && col < MATRIX_SIZE
        && maze[row][col] != 1);
}

/*
 * return the value of the direction that is one turn to the right
 */
int turnRight(int dir) {
    return (dir + 1) % 4;
}

/*
 * return the value of the direction that is one turn to the left
 */
int turnLeft(int dir) {
    return (dir + 3) % 4;
}

/*

 */
void solveMazeIt(int row, int col) {
    int dir = 2; // 0 is up, 1 is right, 2 is down, 3 is left.
    maze[row][col] = 2; // drop a bread crumb in the starting square
    while (row < MATRIX_SIZE - 1) { // the exit is the only open square 
                                    // in the last row

        /* the rest of this loop is yours */

    }
}

/***************************addMartian***************************
 * this is a helper function used for the change() function
 * this function adds together two Martian structs
 * INPUTS: two Martian structs
 * OUTPUTS: the sum of the input structs
 */
Martian addMartian (Martian coins, Martian coins_temp) {
    coins.dodeks += coins_temp.dodeks;
    coins.nicks += coins_temp.nicks;
    coins.pennies += coins_temp.pennies;
    return coins;
}

/***************************change***************************
 * this function dispenses change in the form of pennies, nicks, and dodeks
 * it must dispense the smallest number of coins possible
 * pennies are worth 1 cent, nicks and dodeks have variable values
 * dodek_val > nick_val > 1 must hold true, however
 * INPUTS: the number of cents for which change is requested,
 * and the value of nicks and dodeks
 * OUTPUTS: a Martian struct containing the smallest penny, nick, and dodek 
 * combination that adds up to "cents"
 */
Martian change(int cents, int nick_val, int dodek_val) {
    //this function has 3 Martian structs so that it go through
    //all possible combinations that add up to the cents given
    Martian coins1 = {0}, coins2 = {0}, coins3 = {0};
    Martian coins1_temp, coins2_temp;

    if (cents == 0) 
        return coins1;//if cents == 0, a struct with all 0s is returned

    else if (cents >= dodek_val) {
        //try starting off with 1 dodek, then call the function again until cents == 0
        coins1.dodeks += 1;
        coins1_temp = change(cents - dodek_val, nick_val, dodek_val);
        //add up the results
        coins1 = addMartian(coins1, coins1_temp);

        //this time try nicks--no need to add nicks one by one since since the only other
        //coin left is the penny
        coins2.nicks += (cents/nick_val);
        coins2_temp = change(cents - ((cents/nick_val)*nick_val), nick_val, dodek_val);
        coins2 = addMartian(coins2, coins2_temp);

        coins3.pennies += cents;
    }
    else if (cents >= nick_val) {
        //if it's not possible to add a dodek because cents < dodek_val,
        //then fill coins1 with all negatives so that we can ignore it later
        coins1.dodeks = coins1.nicks = coins1.pennies = -1;

        coins2.nicks += (cents/nick_val);
        coins2_temp = change(cents - ((cents/nick_val)*nick_val), nick_val, dodek_val);
        coins2 = addMartian(coins2, coins2_temp);

        coins3.pennies += cents;
    }
    else {
        coins1.dodeks = coins1.nicks = coins1.pennies = -1;
        coins2.dodeks = coins2.nicks = coins2.pennies = -1;
        coins3.pennies += cents;
    }

    //compare the total # of coins in each combination
    int coins1_total = coins1.dodeks + coins1.nicks + coins1.pennies;
    int coins2_total = coins2.dodeks + coins2.nicks + coins2.pennies;
    int coins3_total = coins3.dodeks + coins3.nicks + coins3.pennies;

    //returns the coins struct with the smallest number of coins, unless the total is negative
    //if more than one struct happens to have the smallest number, it just returns one of them
    if ((coins1_total < coins2_total || coins1_total == coins2_total) && coins1_total >= 0) {
        if (coins1_total < coins3_total || coins1_total == coins3_total)
            return coins1;
        else
            return coins3;
    }
    else if ((coins2_total < coins3_total || coins2_total == coins3_total) && coins2_total >= 0) {
        return coins2;
    }
    else {
        return coins3;
    }
}

/****************************change***************************
 * this version of the change function is the specific case
 * where nicks are 5 cents and dodeks are 12 cents
 * this simply calls the general change function,
 * with 5 and 12 for the nick_val and dodek_val inputs, respectively
 * INPUTS: the number of cents for which change must be returned
 * OUTPUTS: a Martian struct with the smallest number of coins that
 * adds up to the amount of change requested
 */
Martian change(int cents) {
    return change(cents, 5, 12);
}

/* 
 * without using recursion, solve the Martian change problem described above.
 * it's not too bad for the specific case of nick_value = 5 and dodek_value = 12
 */
Martian changeIt(int cents) {
    return Martian{}; // delete this line, it's broken. Then write the function properly!
}

/*
 * However, I don't have a solution for the general case. In fact, I consider 
 * the general problem to "challenging" (i.e, an exercise in futility). 
 * But, if you like banging your head against the wall for zero credit
 * this is the problem for you!
 */
Martian changeIt(int cents, int nick_value, int dodek_value) {
    return Martian{}; // delete this line, it's broken. Then write the function properly!
}
