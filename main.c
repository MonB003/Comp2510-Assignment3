#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 100
#define BASE_TEN 10
#define VALID_NUM_ARGS 3
#define TWO 2
#define ONE 1

/*
 * Note: For this assignment, I used the merge sort algorithm. This algorithm involves recursion to split the laptop
 * array in halves, then sorts each half in ascending order, and merges them back together after.
 */


// Laptop struct
struct laptop {
    char brand[MAX_LENGTH];
    float price;
};


// Displays the sorted laptop array on the console.
void displayOnConsole(struct laptop sortedArray[], int productsPerPage) {
    for (int index = 0; index < productsPerPage; index++) {
        printf("%s\t\t%.2f\n", sortedArray[index].brand, sortedArray[index].price);
    }
}


// Adds remaining elements to the sorted laptop array. Either the left or right side will have remaining elements.
void addRemainingElements(struct laptop array[], const struct laptop leftSide[], int leftSideSize, const struct laptop rightSide[], int rightSideSize, int leftPos, int rightPos, int mergedPos) {
    while (leftPos < leftSideSize) {
        array[mergedPos] = leftSide[leftPos];
        leftPos++;
        mergedPos++;
    }

    while (rightPos < rightSideSize) {
        array[mergedPos] = rightSide[rightPos];
        rightPos++;
        mergedPos++;
    }
}


/*
 * Merges the left and right side arrays by comparing the prices.
 * After, it adds the remaining element from the side of the array that wasn't completely added.
 */
void merge(struct laptop array[], const struct laptop leftSide[], int leftSideSize, const struct laptop rightSide[], int rightSideSize) {
    int leftPos = 0, rightPos = 0, mergedPos = 0;

    while (leftPos < leftSideSize && rightPos < rightSideSize) {
        if (leftSide[leftPos].price <= rightSide[rightPos].price) {
            array[mergedPos] = leftSide[leftPos];
            leftPos++;
        } else {
            array[mergedPos] = rightSide[rightPos];
            rightPos++;
        }
        mergedPos++;
    }

    addRemainingElements(array, leftSide, leftSideSize, rightSide, rightSideSize, leftPos, rightPos, mergedPos);
}


/*
 * Sorts laptop array in ascending order based on price values. This uses the merge sort algorithm.
 * If number of array elements is less than 2 (0 or 1), it's already sorted, so this code is skipped.
 */
void mergeSortPrices(struct laptop array[], int arrSize) {
    if (arrSize >= TWO) {
        int middleIndex = arrSize / TWO;
        struct laptop leftSide[middleIndex];
        struct laptop rightSide[arrSize - middleIndex];
        for (int left = 0; left < middleIndex; left++) {
            leftSide[left] = array[left];
        }
        for (int right = middleIndex; right < arrSize; right++) {
            rightSide[right - middleIndex] = array[right];
        }

        mergeSortPrices(leftSide, middleIndex);
        mergeSortPrices(rightSide, arrSize - middleIndex);
        merge(array, leftSide, middleIndex, rightSide, arrSize - middleIndex);
    }
}


// Stores the value of a laptop's price from the text file.
void setPriceValue(char *priceStr, struct laptop *laptopArray, int *index) {
    float price = strtof(priceStr, NULL);
    laptopArray[*index].price = price;
    *index = *index + ONE;
}


/*
 * Creates an array to store structs and allocates the memory using malloc.
 * Reads each set of values from the text file and stores them in a laptop struct within the array.
 * Adjusts the size of the memory using reallocation each time a laptop is added.
 * After, it calls the methods to sort and print the number of laptops for a page.
 */
void allocateMemory(FILE *filePtr, int productsPerPage) {
    struct laptop *laptopArray = malloc(sizeof *laptopArray);
    if (laptopArray == NULL) {
        printf("No memory was allocated.\n");
    } else {
        int index = 0;
        char priceStr[MAX_LENGTH];

        while (fscanf(filePtr, "%s %s", laptopArray[index].brand, priceStr) != EOF) {
            setPriceValue(priceStr, laptopArray, &index);
            laptopArray = realloc(laptopArray, (index+ONE) * sizeof *laptopArray);
            if (laptopArray == NULL) {
                printf("No memory was allocated.\n");
                exit(0);
            }
        }
        mergeSortPrices(laptopArray, index);
        displayOnConsole(laptopArray, productsPerPage);
    }
    free(laptopArray);
}


/*
 * Checks if the file is null.
 * If not, it calls the method to read the file.
 */
void checkFile(char *fileName, int productsPerPage) {
    FILE *filePtr = fopen(fileName, "r");

    if (filePtr == NULL) {
        printf("Unable to open the file: %s", fileName);
    } else {
        allocateMemory(filePtr, productsPerPage);
    }

    fclose(filePtr);
}


/*
 * Checks the number of arguments passed.
 * If number of arguments is 3, it's valid (1 for current directory, 1 for file name, 1 for number of products/page).
 * If the number is valid, call the method to check if file can be opened.
 */
void callMethods(int argc, char* argv[]) {
    if(argc == VALID_NUM_ARGS) {
        char *fileName = argv[ONE];
        int productsPerPage = strtol(argv[TWO], NULL, BASE_TEN);
        if (productsPerPage != 0) {
            checkFile(fileName, productsPerPage);
        }
    } else {
        printf("Error: Number of arguments passed must be 3. Number of arguments you passed: %d.\n", argc);
    }
}


int main(int argc, char* argv[]) {
    callMethods(argc, argv);

    return 0;
}
