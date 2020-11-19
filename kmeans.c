//************************************
// First Iteration of KMeans Algorithm
//************************************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definitions - Macros
#define Max 10         // Upper Absolute Value Limit of Vector Values
#define N 10           // Number of Vectors
#define Nv 3           // Number of Dimensions
#define Nc 3            // Number of Centroids

// Declare Arrays
int index_array[Nc];
float vectors[N][Nv];
float centroids[Nc][Nv];

//**********************************************************
// Fix to Generate Random Sequence of Non-Repeating Integers
//**********************************************************

void generateIndices(void)
{
    srand(time(NULL));          // Create rand() Seed

    int temp;
    unsigned int flag = 0;                  // Duplicate Flag

    int i = 0;
    while (i != Nc)
    {
        temp = rand() % Nc;
//        printf("%d\n", temp);

        for (int j = 0; j < i; j++)
        {
            if (temp == index_array[j])             // Check if Duplicate
            {
                flag = 1;
                break;
            }
        }

        if (!flag)                              // If NOT Duplicate, Add to Array
        {
            index_array[i] = temp;

            i++;
        }

        flag = 0;
    }
}


// Function Creating N Random Vectors of Size Nv
void createVectors(void)
{
    srand(time(NULL));          // Create rand() Seed

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < Nv; j++)
        {
            vectors[i][j] = (float)(((double)rand() - RAND_MAX / 2) / (double)RAND_MAX * Max);
            printf("%f\n", vectors[i][j]);
        }
        puts("\n");
    }
}

// Function to Initialize Centroids
void initCentroids(void)
{

}

// Driver Function
int main(void)
{
    // TODO: Add Driver Code.
    createVectors();

    return 0;
}
