#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <locale.h>
#include <errno.h>

int errno;

void rprintf(char tekst[256]){
    printf("\033[0;31m");
    printf("%s \n",tekst);
    printf("\033[0m");
}

int ReverseFile(char file[]){
    FILE *txt = fopen(file,"r");
    int linesAmounts = 0;
    int i = 0;

    if(txt == NULL){
        printf("\033[0;31m");
        perror ("Failed open file");
        printf( "Error number: %d\n", errno );
        printf("\033[0m");
        return -1;
    }

    while (feof(txt)!= 1){
        if(fgetc(txt)=='\n'){
            linesAmounts++;
        }
    }


    fclose(txt);

    char **revFile;
    revFile=(char**)malloc(linesAmounts*sizeof(char*));
    for(i=0; i<=linesAmounts; i++){
        revFile[i]=(char*)malloc(256*sizeof(char));
    }



    fopen(file,"r");
    i = 0;
    while (feof(txt)!= 1) {
        fgets(revFile[i], 256, txt);
        i++;
    }



    for(i=linesAmounts-1;i>=0;i--){
        printf("%s",revFile[i]);
    }

    free(revFile);
    fclose(txt);
    return 0;
}

int Tac(int argc, char *argv[]){


    if(argc<3){
        rprintf("too few arguments");
        return -1;
    }

    if(argc>3){
        rprintf("too many arguments");
        return -1;
    }

    if(fopen(argv[1],"r")==NULL){
        printf("\033[0;31m");
        perror ("Failed open file1");
        printf( "Error number: %d\n", errno );
        printf("\033[0m");
        return  -1;
    }
    if(fopen(argv[2],"r")==NULL){
        printf("\033[0;31m");
        perror ("Failed open file2");
        printf( "Error number: %d\n", errno );
        printf("\033[0m");
        return  -1;
    }


    if(ReverseFile(argv[1]) != 0){
        return -1;
    };
    if(ReverseFile(argv[2]) != 0){
        return -1;
    };

    return 0;
}

int SerialPaste(int argc, char *argv[]) {
    if (argc < 5) {
        rprintf("too few arguments");
        return -1;
    }
    if (argc > 6) {
        rprintf("too many arguments");
        return -1;
    }
    if (*argv[4] != '>') {
        rprintf("no redirection, use '>'");
        return -1;
    }


    FILE *txt1 = fopen(argv[2], "r");
    FILE *txt2 = fopen(argv[3], "r");
    FILE *target = fopen(argv[5], "w");

    if (txt1 == NULL) {
        printf("\033[0;31m");
        perror("Failed open file1");
        printf("Error number: %d\n", errno);
        printf("\033[0m");
        return -1;
    }
    if (txt2 == NULL) {
        printf("\033[0;31m");
        perror("Failed open file2");
        printf("Error number: %d\n", errno);
        printf("\033[0m");
        return -1;
    }
    if (target == NULL) {
        printf("\033[0;31m");
        perror("Failed open/create target file");
        printf(" Error number: %d\n", errno);
        printf("\033[0m");
        return -1;
    }

    char line1[256];
    char line2[256];

    while (feof(txt1) != 1) {
        if (fgets(line1, 256, txt1) != NULL) {
            line1[strlen(line1) - 1] = ' ';
            fprintf(target, "%s \t", line1);
        }
    }

    fclose(txt1);
    fprintf(target, "\n");

    while (feof(txt2) != 1) {
        if (fgets(line2, 256, txt2) != NULL) {
            line2[strlen(line2) - 1] = ' ';
            fprintf(target, "%s \t", line2);
        }

    }
        fclose(txt2);
        fclose(target);
        return 0;

    }

int NormalPaste(int argc, char *argv[]){
        if(argc<5){
            rprintf("too few arguments");
            return -1;
        }
        if(argc>5){
            rprintf("too many arguments");
            return -1;
        }
        if(*argv[3]!='>'){
            rprintf("no redirection, use '>'");
            return -1;
        }


        FILE *txt1 = fopen(argv[1],"r");
        FILE *txt2 = fopen(argv[2],"r");
        FILE *target = fopen(argv[4],"w");

        if(txt1 == NULL){
            printf("\033[0;31m");
            perror ("Failed open file1");
            printf( "Error number: %d\n", errno );
            printf("\033[0m");
            return -1;
        }
        if(txt2 == NULL){
            printf("\033[0;31m");
            perror ("Failed open file2");
            printf( "Error number: %d\n", errno );
            printf("\033[0m");
            return -1;
        }
        if(target == NULL){
            printf("\033[0;31m");
            perror ("Failed open/create target file");
            printf( " Error number: %d\n", errno );
            printf("\033[0m");
            return -1;
        }

        char line1[256];
        char line2[256];



        while (feof(txt1)!= 1 || feof(txt2)!=1) {

            if (fgets(line1, 256, txt1) != NULL) {
                line1[strlen(line1) - 1] = ' ';
                fprintf(target, "%s", line1);
            }
            if (fgets(line2, 256, txt2) != NULL) {
                line2[strlen(line2) - 1] = ' ';
                fprintf(target,"\t%s", line2);
            }
            fprintf(target, "\n");
        }
        fclose(txt1);
        fclose(txt2);
        fclose(target);
        return 0;
}

void PasteControll(int argc, char *argv[]){
    if(strcmp(argv[1],"-s")==0){
        SerialPaste(argc, argv);
    }
    else{
        NormalPaste(argc, argv);
    }
}

void help(){
    printf("JPS \nBy Krzysztof Szostak \n");
    printf("Własne implementacje komend: \n");
    printf(">paste: (available argument: -s) \n");
    printf(">tac: \n");
}

int ChangeDirectory(int argc, char *argv[]){
    if(argc<2){
        rprintf("too few arguments \n");
        return 0;
    }
    else {
        chdir(argv[1]);
    }
    return 0;
}

int BuiltInCommand(int argc, char *argv[]){
    if(strcmp(argv[0],"exit") == 0){
        exit(0);
    }
    if(strcmp(argv[0],"help") == 0){
        help();
        return 0;
    }
    if(strcmp(argv[0],"cd") == 0){
        ChangeDirectory(argc, argv);
    }
    if(strcmp(argv[0],"paste") == 0){
        PasteControll(argc, argv);
    }
    if(strcmp(argv[0],"tac") == 0){
        Tac(argc, argv);
    }
    return 0;
}

int LinuxCommand(char *argv[]){

    pid_t pid = fork();

    if(pid == 0) {
        execvp(argv[0],argv);
        perror("execvp failed");
        printf("Error number: %d\n", errno );
        exit(-1);
    }
    else if(pid > 0){
        int result = 0;
        wait(&result);
        return 0;
    }

    perror("Problems with fork");
    printf("%d",errno);
    return 0;
}

void CommandAnalize(char command[]) {

    int i = 0;
    int j = 0;
    int controll = 0;
    size_t eoa = strlen(command);
    int toknumber = 0;
    char token[10][256] = {'\0'};

    if(command[0] == ' '){
        controll = 1;
    }

    for(i = controll; i<eoa; i++) {
        if (command[i]==' ') {
            if(command[i+1]==' '){
                i++;
            }
            toknumber++;
            j = 0;
            i++;
        }
        token[toknumber][j] = command[i];
        j++;
    }

    char **argv;
    argv=(char**)malloc(toknumber*sizeof(char*));
    for(i=0; i<=toknumber+1; i++){
        argv[i]=(char*)malloc(256*sizeof(char));
    }
    for(i=0;i<=toknumber;i++){
        argv[i] = token[i];
    }
    argv[toknumber+1] = NULL;


    if(strcmp(argv[0],"cd") == 0 || strcmp(argv[0],"exit") == 0 || strcmp(argv[0],"help") == 0 || strcmp(argv[0],"paste") == 0 || strcmp(argv[0],"tac") == 0){
        BuiltInCommand(toknumber+1, argv);
    }
    else{
        LinuxCommand(argv);
    }
    free(argv);

}

int main(int argc, char *argv[]) {
    char path[256];

    while (1) {
        char command[1000] = {'\0'};
        getcwd(path, 256);
        printf("\033[1;32m");
        printf("[%s]@%s $ ", path,getenv("USER"));
        printf("\033[0m");


        fgets(command,100,stdin);
        command[strlen(command)-1] = '\0';
        CommandAnalize(command);
    }
}
