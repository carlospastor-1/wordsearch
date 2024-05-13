#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
int bSize;
int reached_index = false;

void printPuzzle(char** arr) {
    for (int i = 0; i < bSize;i++) {
        for (int j = 0; j < bSize;j++) {
            printf("%c ", *(*(arr+i)+j) );
        }
        printf("\n");
    }
    printf("\n");
}


char uppercase(char word) {
    if (word >= 97 && word <= 122) {//checks if 'word' is a lowercase letter
        word = word-32;
    }
    return word;
}

bool search(char** arr, char* word, int row, int column, int index, int size, int** result) {
    bool North = true, West = true, East = true, South = true, NorthEast = true, NorthWest = true, SouthEast = true, SouthWest = true;
    if (index == size) {
        reached_index = true;
        return true;
    }
    if (row < 0 || column < 0 || row == bSize || column == bSize ) {
        return false;
    }
    if (*(*(arr+row)+column) == (uppercase(*(word+index)))) {
        if ((*(*(result+row)+column)) == 0) { // Not visited yet
            (*(*(result+row)+column)) = index + 1;
        }
        else {
            *(*(result+row)+column) = (*(*(result+row)+column) * 10) + (index + 1);//already visited
        }
        NorthWest = search(arr, word, row-1, column-1, index+1,size, result);
        if (!NorthWest) {
            North = search(arr, word, row-1, column, index+1, size, result);
        } 
        if (!North) {
            NorthEast = search(arr, word, row-1, column+1, index+1, size, result);
        }
        if (!NorthEast) {
            West = search(arr, word, row, column-1, index+1, size, result);
        }
        if (!West) {
            East = search(arr, word, row, column+1, index+1, size, result);
        }
        if (!East) {
            SouthWest = search(arr, word, row+1, column-1,index+1, size, result);
        }
        if (!SouthWest) {
            South = search(arr, word, row+1, column, index+1, size, result);
        }
        if (!South) {
            SouthEast = search(arr, word, row+1, column+1, index+1, size, result);
        }
        if (!North && !West && !East && !South && !NorthEast && !NorthWest && !SouthEast && !SouthWest) {
            if (*(*(result+row)+column) != 0) {
                *(*(result+row)+column) = (*(*(result+row)+column) / 10);
                if (*(*(result+row)+column) != 0) {
                    *(*(result+row)+column) = (*(*(result+row)+column) - (index+1));
                }
            }
        }
        return (North || West || East || South || NorthEast || NorthWest || SouthEast || SouthWest);
    }
    return false;
}

int length(int result) {//finds amount of digits in 'result'
    if (result == 0) {
        return 1;
    }
    int length = 0;
    while (result != 0) {
        result = result / 10;
        length++;
    }
    return length;
}

void searchPuzzle(char** arr, char* word) {
    int ** result = (int**) malloc(bSize * sizeof(int*));//
    for (int i = 0; i < bSize;i++) {
        *(result+i) = (int*) malloc(bSize * sizeof(int));
    }
    for (int i = 0; i < bSize;i++) {
        for (int j = 0; j < bSize;j++) {
            *(*(result+i)+j) = 0;
        }
    }
    int size = strlen(word);
    int index = 0;
    bool final;
    for (int i = 0 ; i < bSize;i++) {
        for (int j = 0; j < bSize;j++) {
            if (*(*(arr+i)+j) == uppercase(*(word))) {
                final = search(arr, word, i, j, index, size, result);
            }
        }
    }
    if (!reached_index) {
        printf("Word not found!\n");
    }
    else {
        printf("Word found!\nPrinting the search path:\n");
    
    for (int i = 0; i < bSize;i++) {
        for (int j = 0; j < bSize;j++) {
            int temp = length(*(*(result+i)+j));
            printf("%d", *(*(result+i)+j));
            // printf("%d\n", temp);
            for (int i = temp; i <= 8;i++) {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
    }
}


// Main function, DO NOT MODIFY    
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;


    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }


    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
   
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));


    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);


    printf("Enter the word to search: ");
    scanf("%s", word);
   
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
   
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
   
    return 0;
}