// Inquisitor (Parsec Support tool)

#pragma warning (disable : 4710)

#include <stdio.h> // File IO
#include <stdlib.h> // Used for the Dynamic File Path
#include <string.h> 
#include <stdbool.h>

// Gets & prints out the Parsec Log
static bool getLog(const char *path)
{
    //Prep Path
    char logPath[256];
    snprintf(logPath, sizeof(logPath), "%s%s", path, "log.txt");
    
    // Open Log File for Reading
    FILE* fp = fopen(logPath,"r");
    
    // Check to make sure that the Parsec log opened correctly
    if(fp == NULL)
    {
        printf("No Log Found in %s\n", logPath);
        return false;
    }

    // Section header
    printf("------------------------------PARSEC LOG------------------------------\n");

    // Print file contents
    char c = fgetc(fp);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(fp);
    }
    
    printf("\n------------------------------END OF PARSEC LOG------------------------------\n\n");

    // Close the log file
    int d = fclose(fp);
    if(d != EOF){
        printf("Log File Closed Successfully \n");
    }else {
        printf("Log File Faild to Close \n");
    }
    return true;
}


// Gets & prints out the Parsec Config
static bool getConfig(const char *path)
{
    //Prep Path
    char configPath[256];
    snprintf(configPath, sizeof(configPath), "%s%s", path, "config.txt");
    
    // Open Log File for Reading
    FILE* fp = fopen(configPath,"r");
    
    // Check to make sure that the Parsec log opened correctly
    if(fp == NULL)
    {
        printf("No Config Found in %s\n", configPath);
        return false;
    }

    // Section header
    printf("------------------------------PARSEC CONFIG------------------------------\n");

    // Print file contents
    char c = fgetc(fp);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(fp);
    }
    
    printf("\n------------------------------END OF PARSEC CONFIG------------------------------\n\n");

    // Close the Config file
    int d = fclose(fp);
    if(d != EOF){
        printf("Config File Closed Successfully \n");
    }else {
        printf("Config File Faild to Close \n");
    }
    return true;
}

// Gets the System's CPU, GPU(s), Drivers, and What displays are connected to each
/*const char* getSysInfo(){
    
}*/


// Main Function of Inquisitor
int main()
{
    // Things to grab: Tracert, Log, System Info, ipconfig, Speed test?
    // char sysInfo[] = getSysInfo();

    // Prepare appData Path
    const char *userPath = getenv("USERPROFILE");
    char appdataPath[256];
    snprintf(appdataPath, sizeof(appdataPath), "%s%s", userPath, "/AppData/Roaming/Parsec/");

    // Prints log
    getLog(appdataPath);
    getConfig(appdataPath);

    // Get Log & Config from Programdata path
    getLog("C:/ProgramData/Parsec/");
    getConfig("C:/ProgramData/Parsec/");

    // Ending
    printf("Please select everything in this window by pressing Ctrl+A, then right click to copy it.\nHit 'ENTER' to exit'\n");
    getchar();
    return 0;
}
