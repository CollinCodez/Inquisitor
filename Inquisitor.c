// Inquisitor (Parsec Support tool)

#include <stdio.h> // File IO
#include <stdlib.h> // Used for the Dynamic File Path
#include <string.h> 


/ /Gets & prints out the Parsec Log
static void getLog(const char *path)
{
    // Open Log File for Reading
    FILE* fp = fopen(path,"r");
    
    // Check to make sure that the Parsec log opened correctly
    if(fp == NULL)
    {
        printf("No Log Found in %s\n", path);
        return(-1);
    }

    //Section header
    printf("------------------------------PARSEC LOG------------------------------\n");

    // Print file contents
    char c = fgetc(fp);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(fp);
    }
    
    printf("\n------------------------------END OF PARSEC LOG------------------------------\n\n");

    //Close the log file
    fclose(FILE* fp);
}


// Gets the System's CPU, GPU(s), Drivers, and What displays are connected to each
/*const char* getSysInfo(){
    
}*/


// Main Function of Inquisitor
int main()
{
    // Things to grab: Tracert, Log, System Info, ipconfig, Speed test?
    // char sysInfo[] = getSysInfo();

    // Prints log
    getLog(strcat(getenv("USERPROFILE"),"/AppData/Roaming/Parsec/log.txt"));
    getLog("C:/ProgramData/Parsec/log.txt");

    // Ending
    printf("Please select everything in this window by pressing Ctrl+A, then right click to copy it.\nHit 'ENTER' to exit'\n");
    getchar();
    return 0;
}



