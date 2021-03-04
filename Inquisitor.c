//Inquisitor (Parsec Support tool)

#include <stdio.h> //File IO
#include <stdlib.h> //Used for the Dynamic File Path
#include <string.h> 


//Gets & prints out the Parsec Log
int getLog(){
    //Open Log File for Reading
    FILE* fp = fopen(strcat(getenv("USERPROFILE"),"/AppData/Roaming/Parsec/log.txt"),"r");
    
    //Check to make sure that the Parsec log oppened correctly
    if(fp == NULL) {
        printf("No Log Found \n");
        return(-1);
    }

    //Section header
    printf("------------------------------PARSEC LOG------------------------------\n");

    char c;
    c = (char)fgetc(fp);
    while (c != EOF){
        printf("%c", c);
        c = fgetc(fp);
    }
    
    printf("\n------------------------------END OF PARSEC LOG------------------------------\n\n");

    int fclose(FILE* fp);//Close the log file
}


//Gets the System's CPU, GPU(s), Drivers, and What displays are connected to each
/*const char* getSysInfo(){
    
}*/


//Main Function of Inquisitor
int main()
{
    //Things to grab: Tracert, Log, System Info, ipconfig, Speed test?
    //char sysInfo[] = getSysInfo();

    //Prints log
    int logOutput = getLog();

    //Ending
    printf("Please select everything in this window by pressing Ctrl+A, then right click to copy it.\nHit 'ENTER' to exit'\n");
    fflush(stdout);
    (void)getchar();
    return 0;
}



