#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

/* define each move by the state achieved when applying it to a solved cube */
int moves[9][8] = {
  /* R | R2 | R' */
  {20, 1, 2, 8, 15, 5, 6, 19},
  {7, 1, 2, 4, 3, 5, 6, 0},
  {19, 1, 2, 15, 8, 5, 6, 20},

  /* U | U2 | U' */
  {3, 0, 1, 2, 4, 5, 6, 7},
  {2, 3, 0, 1, 4, 5, 6, 7},
  {1, 2, 3, 0, 4, 5, 6, 7},

  /* F | F2 | F' */
  {9, 21, 2, 3, 16, 12, 6, 7},
  {5, 4, 2, 3, 1, 0, 6, 7},
  {12, 16, 2, 3, 21, 9, 6, 7}
};

/********************************    
 * cubeEquals
 *     Determines whether two cubes share the same state.
 * 
 *     @return: 1 if equal, 0 if not equal.
*********************************/
int cubeEquals(int cube1[8], int cube2[8]) {
  for (int i = 0; i < 8; i++) {
    if (cube1[i] != cube2[i]) {
      return 0;
    }
  }

  return 1;
}

/********************************    
 * reset
 *     Resets a cube to the solved state {0, 1, 2, 3, 4, 5, 6, 7}.
*********************************/
void reset(int cube[8]) {
  for (int i = 0; i < 8; i++) {
    cube[i] = i;
  }
}

/********************************    
 * print
 *     Prints the current state of a cube.
*********************************/
void print(int cube[8]) {
  printf("{%d, %d, %d, %d, %d, %d, %d, %d}.\n", cube[0], cube[1], cube[2], cube[3], cube[4], cube[5], cube[6], cube[7]);
}

/********************************    
 * apply
 *     Applies a certain move to the cube.
*********************************/
void apply(int state[8], int m) {
  int temp3[8];
  memcpy(temp3, state, 32);

  for (int i = 0; i < 8; i++) {
    state[i] = ((temp3[moves[m][i] % 8] + 8 * (int)(moves[m][i] / 8)) % 24);
  }
}

/********************************    
 * applyAlg
 *     An optimised alternative to `apply` for multiple sequential moves.
*********************************/
void applyAlg(int state[8], int m[], int l) {
  int temp[8];

  for (int j = 0; j < l; j++) {
    memcpy(temp, state, 32);

    for (int i = 0; i < 8; i++) {
      state[i] = ((temp[moves[m[j]][i] % 8] + 8 * (int)(moves[m[j]][i] / 8)) % 24);
    }
  }
}

/********************************    
 * invert
 *     Inverts the moves applied to a cube.
 * 
 *     For example, {20, 1, 2, 8, 15, 5, 6, 19} will become {19, 1, 2, 15, 8, 5, 6, 20} (and vice
 *     versa) as the moves that result in these states are inverses.
*********************************/
void invert(int state[8]) {
  int temp[8];
  memcpy(temp, state, 32);

  for (int i = 0; i < 8; i++) {
    state[temp[i] % 8] = (i + 2 * (int)(temp[i] / 8) * 8) % 24;
  }
}

/********************************    
 * scramble
 *     Scrambles a cube with a specified array of moves.
 * 
 *     Note: prints the scramble and resulting state to the terminal.
*********************************/
void scramble(int state[8], int m[], int l) {
  for (int i = 0; i < l; i++) {
    switch(m[i]) {
      case 0:
        printf("R ");
        break;
      case 1:
        printf("R2 ");
        break;
      case 2:
        printf("R' ");
        break;
      case 3:
        printf("U ");
        break;
      case 4:
        printf("U2 ");
        break;
      case 5:
        printf("U' ");
        break;
      case 6:
        printf("F ");
        break;
      case 7:
        printf("F2 ");
        break;
      case 8:
        printf("F' ");
        break;
    }
  }

  applyAlg(state, m, l);
  invert(state);
}

/********************************    
 * findSolutionsInPlaceHelper
 *     A helper function for findSolutionsInPlace that keeps track of the moves applied up to
 *     the current point and returns when `limit` has been reached.
*********************************/
void findSolutionsInPlaceHelper(int state[8], int depth, int limit, int path[]) {
  if (depth == limit) {
    return;
  }

  int temp[8];
  memcpy(temp, state, 32);

  int solved[8] = {0, 1, 2, 3, 4, 5, 6, 7};

  for (int m = 0; m < 9; m++) {
    if ((int)(path[depth - 1] / 3) == (int)(m / 3)) {continue;}

    apply(state, m);
    path[depth] = m;

    if (cubeEquals(state, solved)) {
      for (int i = 0; i <= depth; i++) {
        switch(path[i]) {
          case 0:
            printf("R ");
            break;
          case 1:
            printf("R2 ");
            break;
          case 2:
            printf("R' ");
            break;
          case 3:
            printf("U ");
            break;
          case 4:
            printf("U2 ");
            break;
          case 5:
            printf("U' ");
            break;
          case 6:
            printf("F ");
            break;
          case 7:
            printf("F2 ");
            break;
          case 8:
            printf("F' ");
            break;
        }
      }
      printf("\n");
    }

    findSolutionsInPlaceHelper(state, depth + 1, limit, path);
    memcpy(state, temp, 32);
  }
}

/********************************    
 * findSolutionsInPlace
 *     Finds all solutions for a given state of length less than or equal to `limit`.
*********************************/
void findSolutionsInPlace(int state[8], int depth, int limit) {
  int path[limit];
  int temp[8];

  invert(state);
  memcpy(temp, state, 32);

  int solved[8] = {0, 1, 2, 3, 4, 5, 6, 7};

  for (int m = 0; m < 9; m++) {
    apply(state, m);
    path[depth] = m;

    if (cubeEquals(state, solved)) {
      for (int i = 0; i <= depth; i++) {
        switch(path[i]) {
          case 0:
            printf("R ");
            break;
          case 1:
            printf("R2 ");
            break;
          case 2:
            printf("R' ");
            break;
          case 3:
            printf("U ");
            break;
          case 4:
            printf("U2 ");
            break;
          case 5:
            printf("U' ");
            break;
          case 6:
            printf("F ");
            break;
          case 7:
            printf("F2 ");
            break;
          case 8:
            printf("F' ");
            break;
        }
      }
      printf("\n");
    }

    findSolutionsInPlaceHelper(state, depth + 1, limit, path);
    memcpy(state, temp, 32);
  }
}

/********************************    
 * moveStringToArray
 *     Converts a string of moves to the array format used by the solver functions.
 * 
 *     @return: A malloc'd int* whose first value is the number of moves translated.
*********************************/
int *moveStringToArray(char scr[]) {
  int arr[strlen(scr)];
  int j = 0;

  for (int i = 0; i <= strlen(scr); i++) {
    if (scr[i] == 'R') {arr[j] = 0; j++;}
    if (scr[i] == 'U') {arr[j] = 3; j++;}
    if (scr[i] == 'F') {arr[j] = 6; j++;}

    if (scr[i] == '2') {arr[j - 1] += 1;}
    if (scr[i] == '\'') {arr[j - 1] += 2;}

    if (scr[i] == ' ') {continue;}
  }

  int *ret = (int *)malloc((j + 1) * sizeof(int));
  ret[0] = j;

  for (int i = 1; i < j + 1; i++) {
    ret[i] = arr[i - 1];
  }

  return ret;
}

/********************************    
 * solutionsFromString
 *     The core function called from main which handles the necessary logic for finding solutions.
*********************************/
void solutionsFromString(char scr[]) {
  // convert the string into an array of move indices
  int* temp = moveStringToArray(scr);

  int alg[temp[0]];
  for (int i = 1; i <= temp[0]; i++) {
    alg[i - 1] = temp[i];
  }

  // create and scramble the cube
  int cube[8];
  reset(cube);
  scramble(cube, alg, temp[0]);

  // ensure given scramble was valid
  int solved[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  if (cubeEquals(cube, solved)) {
    exit(1);
  } else {
    print(cube);
  }

  // calculate and print solutions
  printf("-----\n");
  findSolutionsInPlace(cube, 0, 11);

  // free the allocated memory from `moveStringToArray`
  free(temp);
}

int main(int argc, char* argv[]) {
  if (argv[1] && !argv[2]) {
    solutionsFromString(argv[1]);
    return(0);
  } else {
    return(1);
  }
}
