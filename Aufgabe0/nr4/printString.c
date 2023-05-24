# include <stdio.h>

int printString() {
    char str[10000];//Strings sind CharArrays
    printf("Geben Sie was ein: ");
    fgets(str, 10000, stdin);
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ')
            printf("\n");
        else
            printf("%c", str[i]);
        i++;
    }
    return 0;
}