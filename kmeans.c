//************************************
// First Iteration of KMeans Algorithm
//************************************

#include <stdio.h>
#include <stdlib.h>

// Definitions - Macros
#define Max 100         // Upper Absolute Value Limit of Vector Values
#define N 10           // Number of Vectors
#define Nv 10           // Number of Dimensions
#define Nc 5            // Number of Clusters

// Declare Arrays
float vectors[N][Nv];

// Function Creating N Random Vectors of Size Nv
void createVectors(void)
{
    for (int i = 0; i < N - 1; i++)
    {
        for (int j = 0; j < Nv - 1; j++)
        {
            vectors[i][j] = (float)(((double)rand() - RAND_MAX / 2) / (double)RAND_MAX * Max);
//            printf("%f\n", vectors[i][j]);
        }
    }
}

// Driver Function
int main(void)
{
    // TODO: Add Driver Code.
    createVectors();

    return 0;
}
