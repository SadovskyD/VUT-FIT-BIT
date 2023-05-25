/*
    lab 5, graded program #1
    file name: xloginxx_lab5_1.c
    complete the program and submit to WIS
*/

#include<stdio.h>

int main() {
    // declare a 2-dimensional array (aka matrix) and initialize it
    int arr[5][5];
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            arr[i][j] = (i+1)*10 + (j+1);
        }
    }
    printf("\n");

    printf("matrix:\n");
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            printf("%d ", arr[j][i]);
        }
        printf("\n");
    }
   printf("elements above the main diagonal (upper triangular matrix):\n");
     for(int i = 0; i < 5; i++) {
        for(int j = i+1; j < 5; j++) {
    printf("%d ", arr[i][j]);

        }
        printf("\n");
    }

    return 0;
}
