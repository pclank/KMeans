//*********************************************************
// Parallel Implementation of KMeans Algorithm Using OpenMP
//*********************************************************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>


// Definitions - Macros
#define Max 100         // Upper Absolute Value Limit of Vector Values
#define N 100000           // Number of Vectors
#define Nv 1000           // Number of Dimensions
#define Nc 100           // Number of Centroids
#define TermCond 0.000001   // Termination Condition

// Declare Arrays
int index_array[Nc];
float vectors[N][Nv];
float centroids[Nc][Nv];
float classes[N];       // Distance From Closest Centroid
int cluster[N];         // Which Cluster Every Vector Belongs To
int vector_num[Nc] = {};     // Number of Vectors for Every Cluster
float prev_error[N];    // Previous Distance for Comparison

// *******************************************************************
#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline", "unsafe-math-optimizations")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
//************************************************************


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
        temp = rand() % N;
//        printf("Printing Indices...\n");
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

        flag = 0;                               // Return Flag to 0
    }
}

//***************************************
// Function to Copy Errors Between Arrays
//***************************************

void copyErrors(void)
{
    for (int i = 0; i < N; i++)
    {
        prev_error[i] = classes[i];
    }
}

// Function of Parallel Version of createVectors
void createVectors2(void)
{
    #pragma omp parallel
    {
        srand((time(NULL)) ^ omp_get_thread_num());          // Create rand() Seed and Differentiate for Each Thread

//    printf("Printing Vectors...\n");

        int i, j;

        #pragma omp for private(i, j) ordered
        for (i = 0; i < N; i++)
        {
            #pragma omp simd
            for (j = 0; j < Nv; j++)
            {
                vectors[i][j] = (float) (((double) rand() - RAND_MAX / 2) / (double) RAND_MAX * Max);
//            printf("%f\n", vectors[i][j]);
            }
//        puts("\n");
        }
    }
}

// Function to Initialize Centroids
void initCentroids(void)
{
    // TODO: Put Index Generation Function Back
    generateIndices();                      // Run Function to Generate Indices

//    printf("Printing Centroids...\n");
    for (int i = 0; i < Nc; i++)            // Fill Centroids
    {
        for (int j = 0; j < Nv; j++)
        {
            centroids[i][j] = vectors[index_array[i]][j];
//            printf("%f\n", centroids[i][j]);
        }
    }

//    puts("\n");
}

// Function for Parallel Version of calcDistance()
void calcDistance2(void)
{
    int flag;                          // Flag Indicating Whether The Deepest Loop Has Ran Before for Every Vector

    for (int n = 0; n < Nc; n++)
    {
        vector_num[n] = 0;
    }

    float distance;
    int i, j, k;

    #pragma omp parallel for private(flag, i, j, k, distance) shared(classes, cluster) schedule(static) ordered      // Parallelize First 2 For-Loops
    for (i = 0; i < N; i++)                 // For Every Vector
    {
        flag = 0;
//        #pragma omp parallel for reduction(+:distance)
        for (j = 0; j < Nc; j++)            // From Every Centroid
        {
            distance = 0;                           // Distance to Zero for Every Centroid
            #pragma omp simd
            for (k = 0; k < Nv; k++)
            {
                distance += (vectors[i][k] - centroids[j][k]) * (vectors[i][k] - centroids[j][k]);                // Euclidean Distance Omitting Expensive Square Root Operation
            }

            if ((!flag) || (distance < classes[i]))  // Replace Min Distance and Cluster for Current Vector
            {
                    #pragma omp critical
                    {
                        classes[i] = distance;                  // Replace Min Distance

                        if (flag) {
                            vector_num[cluster[i]]--;               // Decrement Number of Vectors of the Cluster Vector i Used to Belong to
                        }

                        cluster[i] = j;                         // Replace Cluster

                        vector_num[j]++;                        // Increment Number of Vectors of the New Cluster Vector i Belongs to

                        flag = 1;                               // Set Flag to "Already Ran Deepest Loop"

//                printf("Printing Distance: %f\n", classes[i]);
                    }
            }
        }
    }

    #pragma omp barrier
}

// Function for Parallel Version of calcCentroids()
void calcCentroids2(void)
{
    for (int i = 0; i < Nc; i++)                // Reset centroid Values to 0
    {
        for (int j = 0; j < Nv; j++)
        {
            centroids[i][j] = 0;
        }
    }

    int cnt;
    int i, j, k;

    #pragma omp parallel for private(cnt, i, j, k) shared(vector_num, centroids) ordered
    for (i = 0; i < Nc; i++)                // For Each Cluster Calculate New Centroid Based on Median
    {
        cnt = 0;                                    // Variable to Stop Loop Early

        for (j = 0; j < N; j++)                 // Sum Up the Vectors Belonging to Cluster i, and Stop the Loop on vector_num[i] containing the Number of Vectors in Cluster i
        {
            if (cluster[j] == i)
            {
                #pragma omp simd
                for (k = 0; k < Nv; k++)
                {
                    centroids[i][k] += (float)((double)vectors[j][k] / (double)vector_num[i]);
                }

                cnt++;
                if (cnt == vector_num[i])               // Break Loop Early
                {
                    break;
                }
            }
        }
    }
}

// Function to Check Conditions of Termination
int checkCondition(void)
{
    int flag = 0;

    for (int i = 0; i < N; i++)
    {
        if (fabs(prev_error[i] - classes[i]) >= (double)TermCond)
        {
            flag = 1;

            break;
        }
    }

    return flag;
}

// Driver Function
int main(void)
{
    // TODO: Improve Driver Code.
    createVectors2();
    initCentroids();

    int cnt = 0;

    int condition = 1;

    calcDistance2();
    calcCentroids2();

    while (condition)
    {
        printf("Iteration %d...\n\n", cnt);

        copyErrors();
        calcDistance2();
        calcCentroids2();

        condition = checkCondition();
        cnt++;
    }

    printf("\n\nPrinting Number of Elements in Every Cluster...\n");
    for (int i = 0; i < Nc; i++)
    {
        printf("Elements in Cluster %d: %d\n", i, vector_num[i]);
    }

    return 0;
}
