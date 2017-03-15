#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "mylib.h"

/* ____________________________________________________________________________

    Function Prototypes
   ____________________________________________________________________________
*/
void pwd_print();			//Prints actual absolute path
void pwd_println();			//Prints actual absolute path and newline
return_state cd_set(node **for_set, char *path);		//Sets for_set on path
void pwd();				//Execute pwd command
void cd(char *path);	//Execute cd command
void ls(char *path);	//Execute ls command

return_state mv_cp_check(char *source, char *target, node **s, node **t);	//Checks source and target paths of commands mv and cp

void mv(char *source, char *target);	//Execute mv command
void cp(char *source, char *target);	//Execute cp command.
void find(char *what, char *where);		//Execute find command.
void finding(char *what, node *where);	//prints only files with FILE_T node_type
int choose_comm(char *prikaz, char *arg1, char *arg2);	//Function to choose and execute a command
int operate();			//to operate on CONSOLE mode

int load_files(char *fpath);	//Fills the root with lines in fpath file
int exec_commands(char *fpath);		//Reads commands from fpath file and executes them

void start();			//Initialization of system.
void stop();			//Tidy up.

#endif
