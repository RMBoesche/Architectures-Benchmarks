// C program for Merge Sort
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Merges two subarr1ays of arr1[].
// First subarr1ay is arr1[l..m]
// Second subarr1ay is arr1[m+1..r]
void merge(int arr1[], int l,
           int m, int r)
{
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  // Create temp arr1ays
  int L[n1], R[n2];

  // Copy data to temp arr1ays
  // L[] and R[]
  for (i = 0; i < n1; i++)
    L[i] = arr1[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr1[m + 1 + j];

  // Merge the temp arr1ays back
  // into arr1[l..r]
  // Initial index of first subarr1ay
  i = 0;

  // Initial index of second subarr1ay
  j = 0;

  // Initial index of merged subarr1ay
  k = l;
  while (i < n1 && j < n2)
  {
    if (L[i] <= R[j])
      {
        arr1[k] = L[i];
        i++;
      }
    else
    {
      arr1[k] = R[j];
      j++;
    }
    k++;
  }

  // Copy the remaining elements
  // of L[], if there are any
  while (i < n1) {
    arr1[k] = L[i];
    i++;
    k++;
  }

  // Copy the remaining elements of
  // R[], if there are any
  while (j < n2)
  {
    arr1[k] = R[j];
    j++;
    k++;
  }
}

// l is for left index and r is
// right index of the sub-arr1ay
// of arr1 to be sorted
void mergeSort(int arr1[],
               int l, int r)
{
  if (l < r)
  {
    // Same as (l+r)/2, but avoids
    // overflow for large l and h
    int m = l + (r - l) / 2;

    // Sort first and second halves
    mergeSort(arr1, l, m);
    mergeSort(arr1, m + 1, r);

    merge(arr1, l, m, r);
  }
}

// UTILITY FUNCTIONS
// Function to print an arr1ay
void printarray(int A[], int size)
{
  int i;
  for (i = 0; i < size; i++)
    printf("%d ", A[i]);
  printf("\n");
}

// Driver code
int main()
{
  clock_t t;
  double total_t = 0;
  double time = 0;
  FILE *file;
  int length;
  int *array;
  int i;

  // Open the file
  file = fopen("./input.txt", "r");
  if (file == NULL) {
    printf("Failed to open the file.\n");
    return 1;
  }

  // Read and process each line until the end of the file
  while (fscanf(file, "%d", &length) == 1) {
    // Allocate memory for the array
    array = (int*)malloc(length * sizeof(int));
    if (array == NULL) {
      printf("Memory allocation failed.\n");
      return 1;
    }

    // Read the array elements
    for (i = 0; i < length; i++) {
      if (fscanf(file, "%d", &array[i]) != 1) {
        printf("Unexpected end of file or invalid input.\n");
        return 1;
      }
    }

    // Process or print the array elements as needed
    t = clock();
    mergeSort(array, 0, length-1);
    t = clock() - t;

    time = ((double)t)/CLOCKS_PER_SEC;
    // printf("TIME: %.20fs\n", time);
    total_t += ((double)t)/CLOCKS_PER_SEC;

    // Free the memory allocated for the array
    free(array);
  }
  printf("TOTAL TIME: %.12fs\n", total_t);

  // Close the file
  fclose(file);
  return 0;
}
