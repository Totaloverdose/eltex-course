#include <stdio.h>

#define N 5

void print_array(int[]);
void print_matrix(int[][N]);
void transform_matrix_simple(int[][N]);
void transform_array_reversed(int[]);
void transform_matrix_triangular(int[][N]);
void transform_matrix_snail(int[][N]);


int main() {
  int matrix[N][N];
  int array[N];

  transform_matrix_simple(matrix);
  print_matrix(matrix);

  transform_array_reversed(array);
  print_array(array);
  
  transform_matrix_triangular(matrix);
  print_matrix(matrix);

  transform_matrix_snail(matrix);
  print_matrix(matrix);
}

void print_array(int array[]) {
  for (int i = 0; i < N; i++) {
    printf("%3d ", array[i]);
  }
  printf("\n\n");
}

void print_matrix(int matrix[][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%3d ", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void transform_matrix_simple(int matrix[][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = i * N + j;
    }
  }
}

void transform_array_reversed(int array[]) {
  for (int i = 1; i <= N; i++) {
    array[N - i] = i;
  }
}

void transform_matrix_triangular(int matrix[][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i + j < N - 1) {
        matrix[i][j] = 0;
      } else {
        matrix[i][j] = 1;
      }
    }
  }
}

void transform_matrix_snail (int matrix[][N]) {
  // Where the start and the end of the first sequential row or column
  // from top, right, left, bottom sides respectively
  int start = 0;
  int end = N - 1;
  // This keeps the current number in the snail
  int accumulator = 1;
  // Counters in loops
  int i = 0;
  int j = 0;

  // Fill sequences from left to right
  while (end - start >= 0) {
    int n = 0;
    
    for (j = start; j <= end; j++) {
      matrix[i][j] = accumulator;
      accumulator++;
    }

    start++;
    end--;
    i++;
    
    n = end - start + 1;
    accumulator += (n + 1) * 3 - 1;
  }

  start = 1;
  end = N - 1;
  accumulator = N + 1;
  j = N - 1;
  // Fill sequences from top to bottom
  while (end - start >= 0) {
    int n = 0;

    for (i = start; i <= end; i++) {
      matrix[i][j] = accumulator;
      accumulator++;
    }

    start++;
    end--;
    j--;
    
    n = end - start + 1;
    accumulator += (n + 1) * 3 + 2 - 1;
  }

  start = N - 2;
  end = 0;
  accumulator = N * 2;
  i = N - 1;
  // Fill sequences from right to left
  while (start - end >= 0) {
    int n = 0;

    for (j = start; j >= end; j--) {
      matrix[i][j] = accumulator;
      accumulator++;
    }

    start--;
    end++;
    i--;
    
    n = start - end + 1;
    accumulator += (n + 1) * 3 - 1;
  }
  
  start = N - 2;
  end = 1;
  accumulator = N * 3 - 1;
  j = 0;
  // Fill sequences from bottom to top
  while (start - end >= 0) {
    int n = 0;

    for (i = start; i >= end; i--) {
      matrix[i][j] = accumulator;
      accumulator++;
    }

    start--;
    end++;
    j++;
    
    n = start - end + 1;
    accumulator += (n + 1) * 3 + 2 - 1;
  }
}
