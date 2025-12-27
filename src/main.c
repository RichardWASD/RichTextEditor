#include "../include/text.h"

struct termios original_termios;

void disableRawMode(){ // disable after done writing
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&original_termios);
    
}

int enableRawMode(){
    /*We want to change our terminal settings so we can start displaying real time feed */
 
    tcgetattr(STDIN_FILENO,&original_termios);
    atexit(disableRawMode);

    struct termios raw = original_termios;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO); // disables echo mode / ability to see text in terminal
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw); // modifes with updated vals
    
    return 0;
}


int main (){
    enableRawMode();

    FILE *file_pointer ; 
    char input_text[1000]; // TODO: OUR Input should be able to write the entire file/ Large or no limit 
    char outfile_name[50];
    char ch; 



    printf("Rich Editor!\n");
    printf("Please Enter The Name of You're New File and End it with a '.txt': ");
    scanf("%s",outfile_name);

    int c; 
    while((c = getchar()) != '\n' && c != EOF);// gets rid of trailing \n that would stay in the buffer

    file_pointer = fopen(outfile_name,"w"); // Pointer creates the output file 

    printf("To stop writing to file press '~' key\n");

    if(file_pointer == NULL){
        exit(1);
    }

    char read_c ;
    while(read(STDIN_FILENO, &read_c, 1) == 1 && read_c != '~'){ // Todo: make condition for EOF reached.

        fputc(read_c,file_pointer); // use putc because we used char to consume a char to read for end condition which we need to add back/ have it included in string
        
    }

    fclose(file_pointer);
    return 0;
}


