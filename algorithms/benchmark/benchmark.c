#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MIN_SAMPLES 100
#define MAX_SAMPLES 100000
#define MIN_VALUE 1
#define MAX_VALUE 100
#define NUM_FILES 500
#define ROUNDS 5

float calcInvSqRoot( float n ) {

  const float threehalfs = 1.5F;
  float y = n;

  long i = * ( long * ) &y;

  i = 0x5f3759df - ( i >> 1 );
  y = * ( float * ) &i;

  y = y * ( threehalfs - ( (n * 0.5F) * y * y ) );

  return y;
}

double inverse_sqrt() {
  int i = 0;
  int n = 1;
  clock_t t;
  double total_t = 0;

  t = clock();
  for (i=0;i<25000000;i++) {
    calcInvSqRoot(n);
    n = n*2;
  }
  t = clock() - t;

  total_t = ((double)t)/CLOCKS_PER_SEC;
  // printf("inverse_sqrt: %.12fs\n", total_t);
  return total_t;

}

// Function to calculate the coefficients of the cubic equation
void calculate_cubic_coefficients(float* x, float* y, int size, double* a, double* b, double* c, double* d) {
   double sum_x = 0.0, sum_x2 = 0.0, sum_x3 = 0.0, sum_x4 = 0.0, sum_x5 = 0.0, sum_x6 = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2y = 0.0, sum_x3y = 0.0;

   for (int i = 0; i < size; i++) {
      double xi = x[i];
      double yi = y[i];
      double xi2 = xi * xi;
      double xi3 = xi2 * xi;
      double xi4 = xi2 * xi2;
      double xi5 = xi2 * xi3;
      double xi6 = xi3 * xi3;

      sum_x += xi;
      sum_x2 += xi2;
      sum_x3 += xi3;
      sum_x4 += xi4;
      sum_x5 += xi5;
      sum_x6 += xi6;
      sum_y += yi;
      sum_xy += xi * yi;
      sum_x2y += xi2 * yi;
      sum_x3y += xi3 * yi;
   }

   double denominator = (size * sum_x2 * sum_x4 * sum_x6 - sum_x2 * sum_x3 * sum_x2 * sum_x5 - sum_x * sum_x * sum_x4 * sum_x6 + sum_x * sum_x2 * sum_x3 * sum_x5 + sum_x * sum_x * sum_x2 * sum_x2 * sum_x4 - size * sum_x2 * sum_x2 * sum_x2 * sum_x2 * sum_x2);
   *a = (sum_y * sum_x2 * sum_x4 * sum_x6 - sum_y * sum_x3 * sum_x2 * sum_x5 - sum_xy * sum_x2 * sum_x2 * sum_x4 + sum_xy * sum_x2 * sum_x3 * sum_x3 + sum_x2y * sum_x * sum_x2 * sum_x4 - sum_x2y * size * sum_x2 * sum_x2 * sum_x2 * sum_x2) / denominator;
   *b = (size * sum_xy * sum_x4 * sum_x6 - sum_y * sum_x * sum_x4 * sum_x6 - sum_x2y * sum_x3 * sum_x3 + sum_x2 * sum_y * sum_x2 * sum_x4 - sum_x2 * sum_xy * sum_x2 * sum_x2 * sum_x2 + sum_x * sum_x2y * sum_x2 * sum_x2 * sum_x2) / denominator;
   *c = (size * sum_x2y * sum_x6 - sum_y * sum_x3 * sum_x6 - sum_x2 * sum_x2 * sum_x5 + sum_x2 * sum_x3 * sum_x4 + sum_x * sum_x * sum_x5 - sum_x * sum_x2y * sum_x4) / denominator;
   *d = (sum_x2 * sum_x2 * sum_x3 * sum_x6 * sum_y - sum_x2 * sum_x2 * sum_x2 * sum_x5 * sum_xy - sum_x * sum_x * sum_x4 * sum_x6 * sum_y + sum_x * sum_x * sum_x2 * sum_x5 * sum_xy + sum_x2 * sum_x * sum_x * sum_x5 * sum_x2y - sum_x2 * sum_x * sum_x2 * sum_x4 * sum_x3y) / denominator;
}

// Function to perform cubic regression
void cubic_regression(float* x, float* y, int size) {
   double a, b, c, d;
   calculate_cubic_coefficients(x, y, size, &a, &b, &c, &d);
}

double linear_regression() {
   clock_t t;
   double total_t = 0;
   double time = 0;

   for (int file_num = 1; file_num <= NUM_FILES; file_num++) {
      FILE* file;
      char filename[50];
      sprintf(filename, "../linear_regression/data/data%d.txt", file_num);

      file = fopen(filename, "r");
      if (file == NULL) {
         printf("Error opening the file '%s'.\n", filename);
         return 1;
      }

      int num_samples;
      fscanf(file, "%d", &num_samples);
      if (num_samples > MAX_SAMPLES) {
         printf("Maximum number of samples exceeded.\n");
         fclose(file);
         return 1;
      }

      float x[MAX_SAMPLES];
      float y[MAX_SAMPLES];

      for (int i = 0; i < num_samples; i++) {
         fscanf(file, "%f %f", &x[i], &y[i]);
      }

      fclose(file);

      // printf("File '%s' data:\n", filename);
      t = clock();
      cubic_regression(x, y, num_samples);
      t = clock() - t;

      time = ((double)t)/CLOCKS_PER_SEC;
      // printf("TIME: %.20fs\n", time);
      total_t += ((double)t)/CLOCKS_PER_SEC;
      // printf("\n");
   }
   // printf("linear_regression: %.12fs\n", total_t);
  return total_t;
}

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
double merge_sort()
{
  clock_t t;
  double total_t = 0;
  double time = 0;
  FILE *file;
  int length;
  int *array;
  int i;

  // Open the file
  file = fopen("../merge_sort/input.txt", "r");
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
  // printf("merge_sort: %.12fs\n", total_t);

  // Close the file
  fclose(file);
  return total_t;
}

int main() {
  int i = 0;
  double t_inv_sqrt = 0;
  double t_lin_regr = 0;
  double t_merge_st = 0;

  for (i=0;i<ROUNDS;i++){
    t_inv_sqrt += inverse_sqrt();
    t_lin_regr += linear_regression();
    t_merge_st += merge_sort();
  }

  printf("inverse_sqrt: %.12fs\n", t_inv_sqrt/ROUNDS);
  printf("linear_regression: %.12fs\n", t_lin_regr/ROUNDS);
  printf("merge_sort: %.12fs\n", t_merge_st/ROUNDS);
  return 0;
}
