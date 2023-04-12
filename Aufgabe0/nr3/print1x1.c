# include <stdio.h>

    void array1x1(){
        int i;
        int j;
        int array [10][10];

        // fill 1x1 array
        for (i = 0; i<10; i++){
            for (j = 0; j<10; j++){
                array[i][j] = (i + 1) * (j + 1) ;
            }
        }
        // print 1x1 array
        for (i = 0; i<10; i++) {
            for (j = 0; j < 10; j++) {
                printf("%d ", array[i][j]);
            }
            printf("\n");
        }
    }