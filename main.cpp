#include <iostream>
#include "Aufgabe0/nr3/print1x1.c"
#include "Aufgabe0/nr4/printString.c"
#include "Aufgabe1/simple-fork.c"
#include "Aufgabe1/fork-example.c"
#include "Aufgabe1/nChildprocess.c"
#include "Aufgabe1/forknr2c.c"
#include "Aufgabe2/example/fileReader.c"
#include "Aufgabe2/example/aufgabe3.c"


int main(int argc, char* argv[]) {
    //int argc, char* argv[]
    //char* argv[2];
    ///home/user/Documents/BetriebsystemeLabor/Aufgabe2/example/example.bmp"
    //argv[1] = "example.bmp";
    //argv[2] = "4";

    //char dateiName[sizeof (argv[0])];
    //Programmname./main ist der erste Parameter 0 ter
    gerneratePicture(argv);
    //printf("%s \n %s", argv[1], argv[2]);
   return 0;
}

