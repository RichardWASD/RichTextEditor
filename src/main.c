#include "../include/text.h"

int main (){

    FILE *file_pointer ; 
    char input_text[1000]; // TODO: OUR Input should be able to write the entire file/ Large or no limit 
    char outfile_name[50];
    char ch; 
    // char final_output_name;


    printf("Rich Editor!\n");
    printf("Please Enter The Name of You're New File and End it with a '.txt': ");
    scanf("%s",outfile_name);

    int c; 
    while((c = getchar()) != '\n' && c != EOF);// gets rid of trailing \n that would stay in the buffer

    file_pointer = fopen(outfile_name,"w"); // Pointer creates the output file 

    printf("To stop writing to file press '~' key");

    while(file_pointer == NULL){
        exit(1);
    }

    // fgets(input_text, sizeof(input_text),stdin); // for entire line of text (including whitespace!)
    // fprintf(file_pointer, "%s", input_text); // writes to file. Ability to format data/ use multiple data types 
    // fputs(input_text,file_pointer); // Writes text
    
    while(1){ // Todo: make condition for EOF reached.
        ch = getchar(); 

        if(ch == '~'){ // end key
            break;
        }
        if(ch == EOF){
            break;
        }

        // fgets(input_text, sizeof(input_text),stdin);
        fputc(ch,file_pointer); // use putc because we used char to consume a char to read for end condition which we need to add back/ have it included in string
        
        
    }


    fclose(file_pointer);
    return 0;
}



/*
Implement further
*/