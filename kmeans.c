//************************************
// First Iteration of KMeans Algorithm
//************************************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Definitions - Macros
#define Max 10         // Upper Absolute Value Limit of Vector Values
#define N 5           // Number of Vectors
#define Nv 1           // Number of Dimensions
#define Nc 2            // Number of Centroids

// Declare Arrays
int index_array[Nc];
float vectors[N][Nv];
float centroids[Nc][Nv];
float classes[N];

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
        printf("Printing Indices...\n");
        printf("%d\n", temp);

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

        flag = 0;                               // Return Flag to 0
    }
}


// Function Creating N Random Vectors of Size Nv
void createVectors(void)
{
    srand(time(NULL));          // Create rand() Seed

    printf("Printing Vectors...\n");
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
    generateIndices();                      // Run Function to Generate Indices

    printf("Printing Centroids...\n");
    for (int i = 0; i < Nc; i++)            // Fill Centroids
    {
        for (int j = 0; j < Nv; j++)
        {
            centroids[i][j] = vectors[index_array[i]][j];
            printf("%f\n", centroids[i][j]);
        }
    }

    puts("\n");
}

// Function to Calculate Minimum Euclidean Distance (Omitting Square Root) of Every Vector From Every Centroid
void calcDistance(void)
{
    unsigned int flag;                          // Flag Indicating Whether The Deepest Loop Has Ran Before for Every Vector

    float distance;
    for (int i = 0; i < N; i++)                 // For Every Vector
    {
        flag = 0;
        for (int j = 0; j < Nc; j++)            // From Every Centroid
        {
            distance = 0;                           // Distance to Zero for Every Centroid
            for (int k = 0; k < Nv; k++)
            {
                distance += (float)sqrtf(vectors[i][k] - centroids[j][k]);     // Euclidean Distance Omitting Expensive Square Root Operation
            }

            if ((!flag) || (distance < classes[i]))  // Replace Min Distance for Current Vector
            {
                classes[i] = distance;
                flag = 1;

                printf("Printing Distance: %f\n", classes[i]);
            }
        }
    }
}

// Driver Function
int main(void)
{
    // TODO: Improve Driver Code.
    createVectors();
    initCentroids();
    calcDistance();

    return 0;
}
