/*
 * thredex.c
 *
 *  Created on: Oct 10, 2014
 *      Author: vinay
 */
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define HORIZONTAL_CHECK 1 // check the sodoku horizontally
#define VERTICAL_CHECK 2
#define SUBMATIX_CHECK 3
#define NONNUMBER_FOUND 4

void *rowCheck();
void *colCheck();
void *matCheck();
void printUsage();

char matrix[9][9];
int flag = 0;

main(int argc, char *argv[])
{
	pthread_t row,col,mat;

int printNotValid(char *array, int type, int errorLocation);
		FILE *data;
	      if(argc != 2)
	      {
	    	  printUsage();
	    	  exit(1);
	      }
	      else
	    	  data = fopen(argv[1],"r");
	      int c;
	      int i = 0,j = 0;
	      while((c = getc(data)) != EOF)
	      {
	    	  if (c == ' ')
	    		  continue;
	    	  else if(c == '\n')
	    	  {
	    		  i = 0;
	    		  j++;
	    	  }
	    	  else matrix[j][i++] = c;

	      }

	/*printing the matrix*/

	printf("\n   -- Matrix -- \n");

	for(i = 0;i < 9;i++)
	{
		for(j = 0;j < 9;j++)
			printf("%c ",matrix[i][j]);
		printf("\n");
	}

	/* If the sudoku contains other than numbers, exit, printing error */
		    printf("\n");
		    for (i = 0; i < 9; i++)
		        {
		            for (j = 0; j < 9; j++)
		                {
		                    if ( (matrix[i][j] < '0') || (matrix[i][j] > '9'))
		                        {
		                            printf(" Error (%d,%d): Sudoku can contain only numbers \n",
		                                   i+1, j+1);
						exit(1);
		                        }
		                }
		        }

	pthread_create(&row,NULL,rowCheck,NULL); // thread for row check
	pthread_create(&col,NULL,colCheck,NULL); // thread for column check
	pthread_create(&mat,NULL,matCheck,NULL); // thrad for inner matrix check

			pthread_join(row,NULL);
		    pthread_join(col,NULL);
		    pthread_join(mat,NULL);

}
/*program which check the row lines*/
void *rowCheck()
{
	int i,j;
	char buffer[9];

	for (i = 0; i < 9; i++)
	        {
	            for (j = 0; j < 9; j++)
	                buffer[j] = matrix[i][j];

	            if (check(buffer) != 0)
	                printNotValid(buffer, HORIZONTAL_CHECK, i);
	        }
	pthread_exit(NULL);
	return 0;
}
/*program which checks column lines */
void *colCheck()
{
	int i,j;
	char buffer[9];

	 for (j = 0; j < 9; j++)
	        {
	            for (i = 0; i < 9; i++)
	                {
	                    buffer[i] = matrix[i][j];
	                }
	            if (check(buffer) != 0)
	                printNotValid(buffer, VERTICAL_CHECK, j);
	        }
		pthread_exit(NULL);
		return 0;
}

/*sub matrix check */
/*to calculate the sub matrix check*/
void *matCheck()
{
	int i,j;
	char buffer[9];
	int count = 0;
	    int icount = 0;
	    int jcount = 0;
	    int matcount = 0;

	    for (i = 0; i <= 6; i+= 3)
	        {
	            for (j = 0; j <= 6; j += 3)
	                {
	                    for (icount = 0, count = 0; icount < 3; icount++)
	                        {
	                            for (jcount = 0; jcount < 3; jcount++)
	                                {
	                                    buffer[count++] = matrix[i+icount][j+jcount];
	                                }

	                        }

	                    ++matcount;
	                    if (check(buffer) != 0)
	                        printNotValid(buffer, SUBMATIX_CHECK, matcount);
	                }
	        }

	    if (flag == 0)
	        printf("\n The Sudoku is correct :)");

			pthread_exit(NULL);
			return 0;
}

int check(char *array)
{
    int i, count;
    for (i = 0; i < 9; i++)
        {
            count = i;
            while (count < 9)
                {
                    if (array[i] == array[++count])
                        return 1;
                }
        }
    return 0;
}
/*printing the error if found in row,column,submatrix*/
int printNotValid(char *array, int type, int errorLocation)
{
    int i;
    flag = 1;
    if (type == HORIZONTAL_CHECK)
        {
            printf("\n Found error at row %d: \n\t", ++errorLocation);
            for (i = 0; i < 9; i++)
                printf("%c ", array[i]);
        }
    else if (type == VERTICAL_CHECK)
        {
            printf("\n Found error at column %d: \n", ++errorLocation);
            for (i = 0; i < 9; i++)
                printf("\n\t %c ", array[i]);
        }
    else if (type == SUBMATIX_CHECK)
        {
            printf("\n Found error at Submatrix %d: \n", errorLocation);
            for (i = 0; i < 9; i++)
                {
                    printf("%c ", array[i]);
                    if ((i == 2) || (i == 5))
                        printf("\n");
                }

        }
    return 0;
}
/*if no argument is passed to the program*/
void printUsage()
{
    printf(" File required to read sodoku\n");
}

