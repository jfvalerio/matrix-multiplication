/*
 	Valerio, Jandrel Ian F.
	CMSC 125 - T1L
	February 28, 2019
	Matrix Multiplication
*/

#include <stdio.h>    // Input and Output
#include <stdlib.h>   // Memory Allocations
#include <pthread.h>  // Threads

#define SUCCESS 0

/* Argument Structure to Pass to Threads */
typedef struct ARG {
	int Arow;
	int Bcol;
	int ans;
} args;

/* Global Variables */
int **A;
int **B;
int numRowA, numRowB;
int numColA, numColB;
int numOp;

/* Function to perform by each Thread */
void * matrixMultiplication(void * arguments) {
	args * temp;
	temp = (args *) arguments;
	int * temp2 = (int *)malloc(sizeof(int) * numColA);
	int i;

	// Multiplying partners in the matrix
	for(i = 0; i < numColA; i++) {
		temp2[i] = A[temp->Arow][i] * B[i][temp->Bcol];
	}

	// Adding all products
	for(i = 0; i < numColA; i++) {
		temp->ans += temp2[i];
	}

	// Returning the sum of all products
	pthread_exit(&(temp->ans));
}

/* Main Function */
int main(){
	args *arguments;
	FILE *fp;
	int numOfThreads = 0;
	int counter = 0;
	int i, j, k;

	// Opens the file for reading
	fp = fopen("matrix.in", "r");
	if(fp!=NULL){

		//read file here
		//you can use fscanf for reading the first and second lines
		//check if the size is invalid, i.e. colA != rowB
		//read the matrix by repeated fscanf to an integer, then getchar until the matrix is filled or the end-of-file is reached

		// Gets the number of operations input
		fscanf(fp, "%d", &numOp);

		// Loops the program until numOps is exhausted
		while(numOp != 0) {

			// Prints the current iteration of the program
			counter++;
			printf("\n(%d)", counter);

			// Gets the input for Matrix A and initializes it
			fscanf(fp, "%d %d", &numRowA, &numColA);
			A = (int **)malloc(sizeof(int*) * numRowA);
			for(i = 0; i < numRowA; i++)
				A[i] = (int *)malloc(sizeof(int) * numColA);
			for(i = 0; i < numRowA; i++) {
				for(j = 0; j < numColA; j++) {
					fscanf(fp, "%d", &(A[i][j]));
				}
			}

			// Gets the input for Matrix B and initializes it
			fscanf(fp, "%d %d", &numRowB, &numColB);
			B = (int **)malloc(sizeof(int*) * numRowB);
			for(i = 0; i < numRowB; i++)
				B[i] = (int *)malloc(sizeof(int) * numColB);
			for(i = 0; i < numRowB; i++) {
				for(j = 0; j < numColB; j++) {
					fscanf(fp, "%d", &(B[i][j]));
				}
			}

			//print Matrix A
			printf("\nMatrix A (%d x %d)\n", numRowA, numColB);
			for(i = 0; i < numRowA; i++) {
				for(j = 0; j < numColA; j++) {
					printf("%d ", A[i][j]);
				}
				printf("\n");
			}

			//print Matrix B
			printf("\nMatrix B (%d x %d)\n", numRowB, numColB);
			for(i = 0; i < numRowB; i++) {
				for(j = 0; j < numColB; j++) {
					printf("%d ", B[i][j]);
				}
				printf("\n");
			}

			// Checks if the 2 Matrices are not valid for Matrix Multiplication
			if(numRowB != numColA) {
				printf("\nError. Invalid Input for Matrix Multiplication.\n");

				// Frees the allocated memories
				for(i = 0; i < numRowA; i++)
				    free(A[i]);
				free(A);
				for(i = 0; i < numRowB; i++)
				    free(B[i]);
				free(B);

				// Continue the program and check other input
				numOp--;
				continue;
			}

			//create your threads here. Pass to the thread the row of A and the column of B they need to check.
			numOfThreads = numRowA * numColB;
			arguments = (args *)malloc(sizeof(args) * numOfThreads);
			pthread_t tid[numOfThreads];
			int *num[numOfThreads];
			int row = 0;
			int col = 0;

			// Creating the threads
			for(i = 0; i < numOfThreads; i++) {
				if(i % numColB == 0 && i != 0) {
					row++;
					col = 0;
				}
				arguments[i].Arow = row;
				arguments[i].Bcol = col;
				arguments[i].ans = 0;
				pthread_create(&tid[i], NULL, matrixMultiplication, (void *) &arguments[i]);
				col++;
			}

			//join your threads here
			for(i = 0; i < numOfThreads; i++) {
				pthread_join(tid[i], (void ** )&num[i]);
			}

			//manage the return values of the threads here
			//print the solution here
			printf("\nProduct of Matrix A and Matrix B (%d x %d)\n", numRowA, numColB);
			for(i = 0; i < numOfThreads; i++) {
				if(i % numColB == 0 && i != 0)
					printf("\n");
				printf("%d ", *((int *)num[i]));
			}

			printf("\n");
			numOp--;

			/* Frees the allocated memories */
			for(i = 0; i < numRowA; i++)
			    free(A[i]);
			free(A);
			for(i = 0; i < numRowB; i++)
			    free(B[i]);
			free(B);
			free(arguments);
		}

		// Closes the file
		printf("\n");
		fclose(fp);

	/* Error message when the file is not found */
	}else{
		printf("Error. File not found!\n");
	}

	// Terminating the program
	return SUCCESS;
}
