#include "../include/text.h"

struct termios original_termios; // original state of command line 
/*** Terminal settings ***/
void death(const char *s){
    perror(s);
    exit(1);

}
void disableRawMode(){ // disable after done writing
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&original_termios) == -1){
        death("tcsetattr");
    }
    
}

int enableRawMode(){
    /*We want to change our terminal settings so we can start displaying real time feed
    This func makes it so we can see what is being typed in our terminal. B/c we arent editing our text from CL

    Basics : 
    Our terminal settings are in a struct which we need to modify and update for changes to go through
    refrence : https://man7.org/linux/man-pages/man3/termios.3.html 
    */
 
    if(tcgetattr(STDIN_FILENO,&original_termios) ==-1){death("tcgetattr");}

    atexit(disableRawMode);

    struct termios raw = original_termios;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_iflag &= ~(BRKINT|ICRNL|ISTRIP|ICRNL|IXON); // ctrl-q,s disable flow  
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON| ISIG| IEXTEN); // disables echo mode / ability to see text in terminal /ctrl-v, ...
    raw.c_cflag |= ~(CS8);
    raw.c_cc[VMIN] = 0; 
    raw.c_cc[VTIME] = 1;
    
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw) == -1 ){death("tcsetattr");} // modifes with updated vals or error
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
    while(1){ 

        read_c = '\0';
        if(read(STDIN_FILENO, &read_c, 1) == -1 && errno != EAGAIN){death("read");}

        if(iscntrl(read_c)){
            printf("%d\r\n",read_c);//tester
        }
        else{
         //   fputc(read_c,file_pointer); // use putc because we used char to consume a char to read for end condition which we need to add back/ have it included in string
            printf("%d, ('%c')\r\n",read_c,read_c);//tester
        }
        if(read_c == '~'){break;}
       
        
    }

    fclose(file_pointer);
    return 0;
}


