#ifndef MYLIB_H_
#define MYLIB_H_

/* ____________________________________________________________________________

    Preprocessor-defined Symbols
   ____________________________________________________________________________
*/
//defines symbols
#define END_OF_TEXT '\0'
#define FILE_SEPARATOR '/'
#define DOT_CHARACTER '.'
#define ITER_SYMBOL '*'

//defines constants
#define MAX_TEXT_LENGTH 256
#define BACK_SEQUENCE ".."
#define MAX_LINE_LENGTH 2048	//max input line length


//if run in console mode or not 0-off 1-on
#define CONSOLE 0

/* ____________________________________________________________________________

    Structures and Datatypes
   ____________________________________________________________________________
*/
// type of node
typedef enum {
    FILE_T, DIRECTORY_T, ROOT_T
} node_type;

// used for returning information in functions
typedef enum {	//TEXT_END < LOWER_CHAR < UPPER_CHAR < NUMBER_CHAR < OTHER_CHAR because of sorting
    ERROR, OK, YES, NO, PARAM_WRONG, NOT_A_DIRECTORY, PATH_NOT_EXISTS, VALID, INVALID, SEPARATOR, DOT_CHAR, 
	TEXT_END, LOWER_CHAR, UPPER_CHAR, NUMBER_CHAR, OTHER_CHAR
} return_state;

// struct for node
struct NODE{
	   char text[MAX_TEXT_LENGTH]; //length of text is in range 1-255
	   node_type type;
	   struct NODE *parent;
	   struct NODE *left_brother;
	   struct NODE *right_brother;
	   struct NODE *leftest_child;
	};
typedef struct NODE node;

/* ____________________________________________________________________________

    Function Prototypes
   ____________________________________________________________________________
*/
return_state validate_char(char c);	//if its valid character
return_state group_of_char(char c);	//Returns which group of char is c.
int my_strcmp( const char * str1, const char * str2 );	//my string comaparing
return_state accept_name(const char * mask, const char * name);	//accept or not for find command

#endif /* MYLIB_H_ */

