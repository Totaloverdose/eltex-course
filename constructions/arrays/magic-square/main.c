#include <stdio.h>

#define N 7

int main() {
  if (0 == N % 2) {
    printf("Can't build magic square for N = %d. Please, make sure that you choose an odd N\n", N);
    return 0;
  }

  const int SWT_SIZE = 2 * N - 1;
  const int SWT_CENTER =  SWT_SIZE / 2;
  const int TERRACE_SIZE = N / 2;
  const int SQUARE_CENTER = N / 2;
  int square[N][N];
  int square_with_terraces[SWT_SIZE][SWT_SIZE];

  // Fill sqare with terraces with zeros. This is for readability
  for (int i = 0; i < SWT_SIZE; i++) {
    for (int j = 0; j < SWT_SIZE; j++) {
      square_with_terraces[i][j] = 0;
    }
  }

  // Diagonal filling
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      square_with_terraces[i + SWT_CENTER - j][i + j] = i * N + j + 1;
    }
  }

  // How diagonal filling worked
  for (int i = 0; i < SWT_SIZE; i++) {
    for (int j = 0; j < SWT_SIZE; j++) {
      printf("%2d  ", square_with_terraces[i][j]);
    }
    printf("\n\n");
  }

  // Fill magic square without terraces
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      square[i][j] = square_with_terraces[i + TERRACE_SIZE][j + TERRACE_SIZE];
    }
  }

  // Fill magic square from terraces
  for (int i = 0; i < TERRACE_SIZE; i++) {
    for (int j = SQUARE_CENTER - i; j <= SQUARE_CENTER + i; j += 2) {
      // Top terrace
      square[N - TERRACE_SIZE + i][j] = square_with_terraces[i][j + TERRACE_SIZE];
      // Bottom terrace
      square[TERRACE_SIZE - i - 1][j] = square_with_terraces[SWT_SIZE - i - 1][j + TERRACE_SIZE];
      // Left terrace
      square[j][N - TERRACE_SIZE + i] = square_with_terraces[j + TERRACE_SIZE][i];
      // Right terrace
      square[j][TERRACE_SIZE - i - 1] = square_with_terraces[j + TERRACE_SIZE][SWT_SIZE - i - 1];
    }
  }
  
  // Print magic square
  printf("\n\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%2d  ", square[i][j]);
    }
    printf("\n\n");
  }
}
