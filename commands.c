#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "commands.h"
#include "filesys.h"
#include "mylib.h"


node *root;			//root node of application
node *actual;		//actual node of application
int find_count;		//number of files found in find command

/* ____________________________________________________________________________

    void start()
    
    Initialization of system.
   ____________________________________________________________________________
*/
void start(){
	root = get_root();
	actual = root;
}

/* ____________________________________________________________________________

    void stop()
    
    Tidy up.
   ____________________________________________________________________________
*/
void stop(){
	delete_all(&root);
}

/* ____________________________________________________________________________

    void pwd_print()
    
    Prints actual absolute path.
   ____________________________________________________________________________
*/
void pwd_print(){
	if(actual == root){
		printf("/");
	}
	else{
		print_abs_path(actual);
		printf("/");
	}
}

/* ____________________________________________________________________________

    void pwd_println()
    
    Prints actual absolute path and newline.
   ____________________________________________________________________________
*/
void pwd_println(){
	pwd_print();
	printf("\n");
}

/* ____________________________________________________________________________

    return_state cd_set(node **for_set, char *path)
    
    Sets for_set on path and decides if use actual or root. 
    Returns info if it's directory or not or other info.
   ____________________________________________________________________________
*/
return_state cd_set(node **for_set, char *path){	//ret OK (is dir), ERROR, NOT_A_DIRECTORY, PATH_NOT_EXISTS
	node *tmp;
	
	if(strcmp(path, "/") == 0){
		*for_set = root;	
		return OK;
	}
	
	*for_set = NULL;
	
	if(path[0] == FILE_SEPARATOR){	//absolute
		tmp = change_node(root, &path[1]); //becouse of '/' on path[0]
	}
	else{	//relative
		tmp = change_node(actual, path);
	}
	
	*for_set = tmp;
	
	if(tmp == NULL){	
		return PATH_NOT_EXISTS;
	}
	else{
		if(tmp->type != FILE_T){
			return OK;
		}
		else{
			return NOT_A_DIRECTORY;
		}
	}
	
	return ERROR;
}

/* ____________________________________________________________________________

    void pwd()
    
    Execute pwd command.
   ____________________________________________________________________________
*/
void pwd(){
	printf("$pwd\n");
	pwd_println();
}

/* ____________________________________________________________________________

    void cd(char *path)
    
    Execute cd command.
   ____________________________________________________________________________
*/
void cd(char *path){	
	node *set;
	return_state state;
	
	if(strcmp(path, "") == 0 || path == NULL){
		printf("$cd\n");
		printf("ERROR not enough arguments\n");
		return;
	}
	
	printf("$cd %s\n", path);	
	
	set = NULL;
	state = cd_set(&set, path);	//set node
	
	if(state == OK){	//decide how was set
		actual = set;
	/*	if(CONSOLE){
			
		}
		else{
			pwd_println();	//cd command has no output
		}*/
	}
	else if(state == NOT_A_DIRECTORY){
		printf("ERROR target is not a directory\n");
	}
	else if(state == PATH_NOT_EXISTS){
		printf("ERROR path does not exist\n");
	}
	else{
		printf("ERROR occured\n");
	}
}

/* ____________________________________________________________________________

    void ls(char *path)
    
    Execute ls command.
   ____________________________________________________________________________
*/
void ls(char *path){
	node *set;
	return_state state;
	
	if(path == NULL || strcmp(path, "") == 0){
		printf("$ls\n");
	//	print_childs(actual);		//prints childs 
		
		init_buffer();						//repaired due to sorting
		print_childs_to_buffer(actual);
		sort_buffer();
		print_buffer();
		free_buffer();
		
		return;
	}
		
	printf("$ls %s\n", path);	
	
	set = NULL;
	state = cd_set(&set, path); //set node
	
	if(state == OK){					//decide how was set
		//print_childs(set);
		init_buffer();						//repaired due to sorting
		print_childs_to_buffer(set);
		sort_buffer();
		print_buffer();
		free_buffer();
	}
	else if(state == NOT_A_DIRECTORY){
		if(CONSOLE){
			printf("\t%s\n", set->text);
		}
		else{
			//print_abs_path(set);	repaired
			printf("%s\n", set->text);
		}
		
	}
	else if(state == PATH_NOT_EXISTS){
		printf("ERROR path does not exist\n");
	}
	else{
		printf("ERROR occured\n");
	}	
}

/* ____________________________________________________________________________

    return_state mv_cp_check(char *source, char *target, node **s, node **t)
    
    Checks source and target paths of commands mv and cp. Prints and returns info.
   ____________________________________________________________________________
*/
return_state mv_cp_check(char *source, char *target, node **s, node **t){
	node *src;
	node *trg;
	return_state src_state, trg_state;
	
	if(strcmp(source, "") == 0 || source == NULL){
		printf("\n");
		printf("ERROR not enough arguments\n");
		return ERROR;
	}
	
	if(target == NULL || strcmp(target, "") == 0){
		trg = actual;
		trg_state = OK;
		printf(" %s\n", source);
	}
	else{
		trg_state = cd_set(&trg, target);
		printf(" %s %s\n", source, target);
	}
	src_state = cd_set(&src, source);
	
	switch(src_state){	//should be NOT_A_DIRECTORY
		case PATH_NOT_EXISTS:
		case ERROR:
			printf("ERROR source file not found\n");
			return ERROR;
		case OK:
			printf("ERROR source is directory\n");
			return ERROR;	
		default: ;	//becouse of warnings
	}
	switch(trg_state){	//should be OK means is directory or root
		case PATH_NOT_EXISTS:
		case ERROR:
			printf("ERROR target not found\n");
			return ERROR;
		case NOT_A_DIRECTORY:
			printf("ERROR target is not directory\n");
			return ERROR;	
		default: ;	//becouse of warnings
	}
	
	*s = src;
	*t = trg;
	
	return OK;
}

/* ____________________________________________________________________________

    void mv(char *source, char *target)
    
    Execute mv command.
   ____________________________________________________________________________
*/
void mv(char *source, char *target){	
	node *src;
	node *trg;
	node *ret;
	return_state check;
	
	printf("$mv");
	check = mv_cp_check(source, target, &src, &trg);
	
	if(check == OK){
	// move src to trg
		ret = move_node(trg, src);
	
	/*	if(ret == NULL){					//becouse of validation
			printf("ERROR '%s' was not moved.\n", source);
		}
		else{
			printf("'%s' was moved succesfully.\n", source);
		}*/
	}
}

/* ____________________________________________________________________________

    void cp(char *source, char *target)
    
    Execute cp command.
   ____________________________________________________________________________
*/
void cp(char *source, char *target){	
	node *src;
	node *trg;
	node *copy;
	node *ret;
	return_state check;
	
	printf("$cp");
	check = mv_cp_check(source, target, &src, &trg);
	
	if(check == OK){
	// copy src to trg
		copy = get_copy(src);
		ret = move_node(trg, copy);
	
	/*	if(ret == NULL){				//becouse of validation
			printf("ERROR '%s' was not copied.\n", source);
		}
		else{
			printf("'%s' was copied succesfully.\n", source);
		}*/
	}
	
}

/* ____________________________________________________________________________

    void finding(char *what, node *where)
    
    Printing only files with FILE_T node_type.
   ____________________________________________________________________________
*/
void finding(char *what, node *where){
	node *tmp = where->leftest_child;
	while(tmp != NULL){
		if(tmp->type == FILE_T){	
			if(accept_name(what, tmp->text) == YES){	//call of function in mylib to decide if tmp->text is acceptable
				//print_abs_path(tmp);
				//printf("\n");	
				print_abs_path_to_buffer(tmp);		//repaired due to sorting	
				
				find_count++;
			}	
		}
		else if(tmp->type == DIRECTORY_T){	//tmp is directory -> recursively finding in this dir.
			finding(what, tmp);
		}
		
		tmp = tmp->right_brother;
	}
}

/* ____________________________________________________________________________

    void find(char *what, char *where)
    
    Execute find command.
   ____________________________________________________________________________
*/
void find(char *what, char *where){
	node *whr;
	return_state whr_state;
	
	if(strcmp(what, "") == 0 || what == NULL){
		printf("$find\n");
		printf("ERROR not enough arguments\n");
		return;
	}
	
	if(where == NULL || strcmp(where, "") == 0){	//where not set -> use actual
		whr = actual;
		whr_state = OK;
		printf("$find %s\n", what);
	}
	else{
		whr_state = cd_set(&whr, where);		//set whr on path
		printf("$find %s %s\n", what, where);
	}
	//check how whr was set
	switch(whr_state){	//should be OK means is directory or root
		case ERROR:
			printf("ERROR occurred\n");
			return;
		case PATH_NOT_EXISTS:
			printf("ERROR path does not exist\n");
			return;
		case NOT_A_DIRECTORY:
			printf("ERROR where is not directory\n");
			return;	
		default: ;	//becouse of warnings
	}	
	//whr_state == OK
	//whr is directory node, what is name to find
	find_count = 0;
	
	//finding(what, whr);	
	
	init_buffer();						//repaired due to sorting
	finding(what, whr);
	sort_buffer();
	print_buffer();
	free_buffer();
	
	
	if(find_count == 0){
		printf("\n"); //printf empty line if nothing found
	}
}



/* ____________________________________________________________________________

    int choose_comm(prikaz, arg1, arg2)
    
    Function to choose and execute a command. If command is not recognized returns 0, otherwise 1.
   ____________________________________________________________________________
*/
int choose_comm(char *prikaz, char *arg1, char *arg2){
	if(strcmp(prikaz, "pwd") == 0){	//pwd
		pwd();
		return 1;
	}
	else if(strcmp(prikaz, "cd") == 0){	//cd
		cd(arg1);
		return 1;			
	}
	else if(strcmp(prikaz, "ls") == 0){
		ls(arg1);
		return 1;
	}
	else if(strcmp(prikaz, "mv") == 0){
		mv(arg1, arg2);
		return 1;
	}
	else if(strcmp(prikaz, "cp") == 0){
		cp(arg1, arg2);	
		return 1;
	}
	else if(strcmp(prikaz, "find") == 0){
		find(arg1, arg2);
		return 1;
	}
	
	return 0;
}

/* ____________________________________________________________________________

    int operate()
    
    Function to operate on CONSOLE mode. Reads commands and decides which function will be called.
    Used for testing.
   ____________________________________________________________________________
*/
int operate(){
	char line[MAX_LINE_LENGTH] = "";
	char prikaz[MAX_LINE_LENGTH] = "";
	char arg1[MAX_LINE_LENGTH] = "";
	char arg2[MAX_LINE_LENGTH] = "";
	
	pwd_print();
	printf(">");
	
	scanf("%1023[^\n]", line);
	fseek(stdin,0,SEEK_END);
		
	sscanf(line, "%15s %1023s %1023s\n",prikaz, arg1, arg2);
		
	if(strcmp(prikaz, "exit") == 0){	//exit
		printf("Exiting...\n");
		return 0;
	}	
	else if(choose_comm(prikaz, arg1, arg2) == 0){
		printf("Unrecognized operation.\nType 'exit' to exit.\n");
	}
	
	return 1;
}

/* ____________________________________________________________________________

    int load_files(char *fpath)
    
    Fills the root with lines in fpath file.
   ____________________________________________________________________________
*/
int load_files(char *fpath){
	FILE *file;
	char str[MAX_LINE_LENGTH];
	
    if((file = fopen(fpath, "r")) != NULL){
    	while(fgets(str, MAX_LINE_LENGTH, file) != NULL){
    		sscanf(str, "%s", str);			//scan string from line
    		if(strlen(str)>1){	//becouse of empty lines in document
    			fill_nodes_abs_path(root, str);	//fill root with input
    		}
		}
        fclose(file);
        return 1;			//file was read and root filled successfully
    }
    
	return 0;
}

/* ____________________________________________________________________________

    int exec_commands(char *fpath)
    
    Reads commands from fpath file and executes them.
   ____________________________________________________________________________
*/
int exec_commands(char *fpath){
	FILE *file;
	char str[MAX_LINE_LENGTH];
	char prikaz[MAX_LINE_LENGTH] = "";
	char arg1[MAX_LINE_LENGTH] = "";
	char arg2[MAX_LINE_LENGTH] = "";
	
    if((file = fopen(fpath, "r")) != NULL){
    	while(fgets(str, MAX_LINE_LENGTH, file) != NULL){	//read a line, maximum MAX_LINE_LENGTH including terminating character
			
			prikaz[0] = END_OF_TEXT;	//clearing buffers
			arg1[0] = END_OF_TEXT;
			arg2[0] = END_OF_TEXT;
				
			sscanf(str, "%s %s %s\n",prikaz, arg1, arg2);	//parse a line, no need of %2047s becouse of fgets is smaller then MAX_LINE_LENGTH
    		if(choose_comm(prikaz, arg1, arg2) == 0){		//choose and execute a command
    			printf("Unrecognized operation.\n");		//if not recognized
			}
		}
        fclose(file);
        return 1;
    }
	return 0;
}

