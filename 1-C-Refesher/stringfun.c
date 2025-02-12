#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    int curr_len = 0;     //keep track of current index
    int space_in_last_index = 1;    //set to true first, if there is space in the begining will not include it
    int user_str_len;
    char *ptr = buff; 

    //while we did not reach end of the user string and exceed the 50 memory slot
    while (*user_str != '\0' && curr_len < len) {
        
        if (*user_str == ' ' || *user_str == '\t' || *user_str == '\n') {
            //replace white space with 1 space
            if (!space_in_last_index) {
                *ptr = ' ';
                ptr++;
                curr_len++;
                space_in_last_index = 1; //telling system that the last charater is a space
            }
        }
        else {
            //copy character that is not space
            *ptr = *user_str;
            ptr++;
            curr_len++;
            space_in_last_index = 0; //telling system that the last index is not white space.
        }
        user_str++;
    }
        user_str_len = curr_len;

    //return error when string is too large
    if (*user_str != '\0') {
        exit(-1);
    }

    if (curr_len > 0 && *(ptr - 1) == ' ') {
        ptr--;
        curr_len--;
    }

    //fill the rest with '.'
    while (curr_len < len) {
        *ptr = '.';
        ptr++;
        curr_len++;
    }
    return user_str_len;
   
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int count = 0;
    int in_word = 0;
    char *ptr = buff;
    int buff_len = len;
    buff_len ++;
    buff_len --;

    for (int i = 0; i < str_len; i++) {
        if (*ptr != ' ' && !in_word) {
            count++;
            in_word = 1;
        } 
        else if (*ptr == ' ') {
            in_word = 0;
        }
        ptr++;
    }
    return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //It check if there is at least one argument, and
    //ensures that the first character of the second argument is a -
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //This if statement checks if there is less than 2 arguments
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = malloc(BUFFER_SZ);
    if (buff == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(99);
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            char temp;
            char *start = buff;
            char *end = buff + user_str_len - 1;

            //Swap characters from start and end
            while (start < end) {
                temp = *start;
                *start = *end;
                *end = temp;
                start++;
                end--;
            }
            break;

        case'w':
            int word_len = 0;
            char *ptr = buff;
            int word_count = count_words(buff, BUFFER_SZ, user_str_len);
            printf("Word Print\n");
            printf("----------\n");

            //print result
            for (int i = 1; i < word_count + 1; i++){
                printf("%d. ", i);
                while (*ptr != ' ' && *ptr != '.') {
                    putchar(*ptr);
                    ptr++;
                    word_len++;
                }
                printf("(%d)\n", word_len);
                word_len = 0;
                ptr++;
            }
            printf("\n");
            printf("Number of words returned: %d\n", word_count);
            break;

        case 'x':
            if (argc == 5) {
                char *old = argv[3];
                char *replace = argv[4];

                //Calculate lengths of old word
                int old_len = 0;
                char *old_temp = old;
                while (*old_temp != '\0') {
                    old_len++;
                    old_temp++;
                }

                //Calculate lengths of replace word
                int replace_len = 0;
                char *replace_temp = replace;
                while (*replace_temp != '\0') {
                    replace_len++;
                    replace_temp++;
                }

                //Search for first occurence of the word wanted to be replace
                char *buff_ptr = buff;
                char *match_ptr;
                int match_found = 0;

                while (*buff_ptr != '.') {
                    match_ptr = buff_ptr;
                    char *old_ptr = old;

                    while (*match_ptr == *old_ptr && *old_ptr != '\0') {
                        match_ptr++;
                        old_ptr++;
                    }

                    //Check it matches
                    if (*old_ptr == '\0' && (*match_ptr == ' ' || *match_ptr == '.')) {
                        match_found = 1;
                        break;
                    }
                    buff_ptr++;
                }


                if (match_found) {
                    
                    if (old_len == replace_len) {
                        //Replace old word
                        for (int i = 0; i < old_len; i++) {
                            *(buff_ptr + i) = *(replace + i);
                        }
                    } 
                    
                    else if (replace_len < old_len) {
                        char *read = buff_ptr + old_len;
                        char *write = buff_ptr + replace_len;

                        //Move remaining characters to the left
                        while (*read != '.' && *read != '\0') {
                            *write = *read;
                            write++;
                            read++;
                        }

                        //Fill remaining buffer with dots
                        while (write < buff + BUFFER_SZ) {
                            *write = '.';
                            write++;
                        }

                        //Replace the old word with the new word
                        for (int i = 0; i < replace_len; i++) {
                            *(buff_ptr + i) = *(replace + i);
                        }
                    }
                    
                    else {
                        char *end = buff + user_str_len;
                        
                        //move characters to the right
                        char *read = end;
                        char *write = end + (replace_len - old_len);

                        while (read >= buff_ptr + old_len) {
                            *write = *read;
                            write--;
                            read--;
                        }
                        
                        for (int i = 0; i < replace_len; i++) {
                            *(buff_ptr + i) = *(replace + i);
                        }
                    }
                }
                
                else {
                    printf("Word not found.\n");
                    exit(-1);
                }
            } 
            
            else {
                printf(" Incorrect number of arguments.\n");
                exit(-2);
            }
            break;
            
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
// 
//          PLACE YOUR ANSWER HERE
// By knowing the length, it helps us avoid accessing memory outside the buffer. It is also a good tool to help us loop over the buffer too.
