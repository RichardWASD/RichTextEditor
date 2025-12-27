# The Rich Text Editor, Made Entirely in C-Lang

    ## Current Features
        - Have the ability to write a line out in a file
        
        - The ability to write to multiple lines.
            - [x] When enter key is pressed -> go to next line.
            - [x] Be able to scroll down (Have alot of lines).
            - [x] End writing into file with a key press
        
        - [x] Give the ability to name the file instead of pre-determined name

    
    ## Need to implement
        
        - [ ] live feed for typing onto the editor (transmit each key press!)
        - [ ]  "Raw mode"

    ## Roadblocks
        - Although I have implemented fputc and the writing every char in file, if I want to read live input from key board I must use something else
        -> Update: Used read() for reading keyboard presses
        