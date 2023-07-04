#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MIN_SAMPLES 100
#define MAX_SAMPLES 100000
#define MIN_VALUE 1
#define MAX_VALUE 100
#define NUM_FILES 500

int generate_data() {
   srand(time(NULL)); // Seed the random number generator

   for (int file_num = 1; file_num <= NUM_FILES; file_num++) {
      FILE* file;
      char filename[20];
      sprintf(filename, "data/data%d.txt", file_num);

      int num_samples = MIN_SAMPLES + (rand() % (MAX_SAMPLES - MIN_SAMPLES + 1));
      double x[MAX_SAMPLES];
      double y[MAX_SAMPLES];

      // Generate random X and Y values for each sample
      for (int i = 0; i < num_samples; i++) {
         x[i] = MIN_VALUE + (rand() % (MAX_VALUE - MIN_VALUE + 1));
         y[i] = MIN_VALUE + (rand() % (MAX_VALUE - MIN_VALUE + 1));
      }

      // Open the file to write the data
      file = fopen(filename, "w");
      if (file == NULL) {
         printf("Error opening the file '%s'.\n", filename);
         return 1;
      }

      // Write the number of samples and the X and Y values to the file
      fprintf(file, "%d\n", num_samples);
      for (int i = 0; i < num_samples; i++) {
         fprintf(file, "%.2f %.2f\n", x[i], y[i]);
      }

      // Close the file
      fclose(file);

      printf("File '%s' generated successfully.\n", filename);
   }

   return 0;
}

// Function to calculate the mean of an array of values
double mean(double* array, int size) {
   double sum = 0.0;
   for (int i = 0; i < size; i++) {
      sum += array[i];
   }
   return sum / size;
}

// Function to calculate the slope of the regression line
double calculate_slope(double* x, double* y, double x_mean, double y_mean, int size) {
   double numerator = 0.0;
   double denominator = 0.0;
   for (int i = 0; i < size; i++) {
      numerator += (x[i] - x_mean) * (y[i] - y_mean);
      denominator += (x[i] - x_mean) * (x[i] - x_mean);
   }
   return numerator / denominator;
}

// Function to calculate the y-intercept of the regression line
double calculate_intercept(double x_mean, double y_mean, double slope) {
   return y_mean - slope * x_mean;
}

// Function to perform linear regression
void linear_regression(double* x, double* y, int size) {
   double x_mean = mean(x, size);
   double y_mean = mean(y, size);

   double slope = calculate_slope(x, y, x_mean, y_mean, size);
   double intercept = calculate_intercept(x_mean, y_mean, slope);

   // printf("Regression line equation: y = %.2fx + %.2f\n", slope, intercept);
}

// Function to calculate the coefficients of the quadratic equation
void calculate_quadratic_coefficients(double* x, double* y, int size, double* a, double* b, double* c) {
   double sum_x = 0.0, sum_x2 = 0.0, sum_x3 = 0.0, sum_x4 = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2y = 0.0;

   for (int i = 0; i < size; i++) {
      double xi = x[i];
      double yi = y[i];
      double xi2 = xi * xi;
      double xi3 = xi2 * xi;
      double xi4 = xi2 * xi2;

      sum_x += xi;
      sum_x2 += xi2;
      sum_x3 += xi3;
      sum_x4 += xi4;
      sum_y += yi;
      sum_xy += xi * yi;
      sum_x2y += xi2 * yi;
   }

   double denominator = (size * sum_x2 * sum_x4 - sum_x2 * sum_x3 * sum_x2 - sum_x * sum_x * sum_x4 + sum_x * sum_x2 * sum_x3 + sum_x * sum_x * sum_x2 * sum_x2 - size * sum_x2 * sum_x2 * sum_x2);
   *a = (sum_y * sum_x2 * sum_x4 - sum_y * sum_x3 * sum_x2 - sum_xy * sum_x2 * sum_x2 + sum_xy * sum_x2 * sum_x3 + sum_x2y * sum_x * sum_x2 - sum_x2y * size * sum_x2 * sum_x2) / denominator;
   *b = (size * sum_xy * sum_x4 - sum_y * sum_x * sum_x4 - sum_x2y * sum_x3 + sum_x2 * sum_y * sum_x2 - sum_x2 * sum_xy * sum_x2 + sum_x * sum_x2y * sum_x2) / denominator;
   *c = (sum_x2 * sum_x2 * sum_x3 * sum_y - sum_x2 * sum_x2 * sum_x2 * sum_xy - sum_x * sum_x * sum_x2 * sum_x2 * sum_y + sum_x * sum_x * sum_x2 * sum_x2 * sum_xy + sum_x2 * sum_x * sum_x * sum_x2y - sum_x2 * sum_x * sum_x2 * sum_x2y) / denominator;
}

// Function to perform quadratic regression
void quadratic_regression(double* x, double* y, int size) {
   double a, b, c;
   calculate_quadratic_coefficients(x, y, size, &a, &b, &c);

   // printf("Quadratic regression equation: y = %.20fx^2 + %.20fx + %.20f\n", a, b, c);
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

   // printf("Cubic regression equation: y = %.20fx^3 + %.20fx^2 + %.20fx + %.20f\n", a, b, c, d);
}

int main() {
   clock_t t;
   double total_t = 0;
   double time = 0;

   // int g = generate_data();
   // if (g == 1) {
   //    printf("Generating File Failed!\n");
   //    return 1;
   // }

   for (int file_num = 1; file_num <= NUM_FILES; file_num++) {
      FILE* file;
      char filename[20];
      sprintf(filename, "data/data%d.txt", file_num);

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
   printf("TOTAL TIME: %.12fs\n", total_t);
   return 0;
}
