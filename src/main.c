#include "../include/text.h"
#include <sys/ioctl.h>
struct editorConfig{
    int screenRows;
    int screenCols;
    struct termios original_termios; // original state of command line
 };
 struct editorConfig E;

/*** Terminal settings ***/
void death(const char *s){
    write(STDIN_FILENO, "\x1b[2J",4);  
    write(STDIN_FILENO, "\x1b[H", 3); 
    perror(s);
    exit(1);

}
void disableRawMode(){ // disable after done writing
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&E.original_termios) == -1){
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
 
    if(tcgetattr(STDIN_FILENO,&E.original_termios) ==-1){death("tcgetattr");}

    atexit(disableRawMode);

    struct termios raw = E.original_termios;
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

char editorReadKey(){ // wait for keypress then deal with it later
    char c;
    int nread;
    while((nread == read(STDIN_FILENO, &c, 1) ) != 1 ){
        if(nread == -1 && errno != EAGAIN){
           death("read"); 
        }
    }
    return c;
}

/*** Input ***/
void editorProccessKey(){ // wait for keypress deal with later
    char c = editorReadKey();

    switch (c){
        case CTRL_KEY('q'):
        write(STDIN_FILENO, "\x1b[2J",4);  
        write(STDIN_FILENO, "\x1b[H", 3); 
        exit(0);
        break;
    }
}

int getCursorPos(int *rows, int *cols){
    char buff[32];
    unsigned int i = 0;
    if(write(STDIN_FILENO, "\x1b[6n", 4) != 4){ return -1;}

    printf("\r\n");
    char c;
    while(i < sizeof(buff)-1){
        if(read(STDIN_FILENO, &c, 1) != 1){ break; }
        if(buff[i] == 'R'){break;}
        i++;
    }
    buff[i] = '\0';
    if(buff[0]!= '\x1b' || buff[1] != -1){return -1;}
    if(sscanf(&buff[2] , "%d;%d", rows, cols) != 2){return -1;}

    return 0;
}

int getWindowSize(int *rows, int *cols){
    struct winsize ws;
    
    if(1 ||ioctl(STDIN_FILENO, TIOCGWINSZ , &ws) == -1 || ws.ws_col == 0){
        if(write(STDIN_FILENO, "\x1b[999C\x1b[999B" ,12) != 12){ // moves cursor bottom right 
            return -1;
        }
        return getCursorPos(rows,cols);
    }
    else{
        *rows = ws.ws_row; 
        *cols = ws.ws_col;

        return 0;
    }
}

void initEditor(){

    if(getWindowSize(&E.screenRows, &E.screenCols) == -1){
        death("GettingWindowSize");
    }
}

/*** Outputs ***/
void editorDrawRows (){
    int y; 
    for(y = 0; y < E.screenRows; y++){
        write(STDIN_FILENO, "~\r\n",3);
    }
}

void editorRefreshScreen(){
    write(STDIN_FILENO, "\x1b[2J",4); // x1b : 1st byte escape sequence , followed by 3 bytes [2J , 4: # of bytes 
    write(STDIN_FILENO, "\x1b[H", 3); // move cursor
    editorDrawRows();
    write(STDIN_FILENO, "\x1b[H", 3);
}

void editorPrompt(char *prompt, char *input, size_t size){
    int i = 0 ;
    char c;

    write(STDIN_FILENO, prompt , strlen(prompt));
    fflush(stdout);

    while(1){
        c = editorReadKey();

        if(c == '\r' || c == '\n'){ // enters key -> null terminates 
            input[i] = '\0';
            break;
        }
        else if (c == 127 || c == '\b'){
            if(i > 0){
                i--;
                write(STDIN_FILENO, "\b \b" , 3 );
            }
        }
        else{
            if(i < size-1){
                input[i] = c;
                i++;
                write(STDIN_FILENO, &c, 1);
            }
        }
        
    }
    write(STDIN_FILENO,"\n", 1);

}

int main (){
    
    FILE *file_pointer ; 
    char input_text[1000]; // TODO: OUR Input should be able to write the entire file/ Large or no limit 
    char outfile_name[50];
    char ch; 
    
    
    
    printf("Rich Editor!\n");
    editorPrompt("Please Enter The Name of You're New File and End it with a '.txt': ", outfile_name, sizeof(outfile_name) );
    
    int c; 
    
    file_pointer = fopen(outfile_name,"w"); // Pointer creates the output file 
    
    
    if(file_pointer == NULL){
        death("Error opening file");
    }
    
    write(STDIN_FILENO,"To stop writing to file press CTRL + q\n", 35);
   
    enableRawMode();
    initEditor();
   
    char read_c ;
    while(1){ 

        // read_c = '\0';
        // if(read(STDIN_FILENO, &read_c, 1) == -1 && errno != EAGAIN){death("read");}

        // if(iscntrl(read_c)){
        //     printf("%d\r\n",read_c);//tester
        // }
        // else{
        //  //   fputc(read_c,file_pointer); // use putc because we used char to consume a char to read for end condition which we need to add back/ have it included in string
        //     printf("%d, ('%c')\r\n",read_c,read_c);//tester
        // }
        // if(read_c == CTRL_KEY('`')){break;}
        editorRefreshScreen();
        editorProccessKey();
        
    }

    fclose(file_pointer);
    return 0;
}


