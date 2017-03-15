
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "filesys.h"
#include "mylib.h"



/* ____________________________________________________________________________

	START OF A BLOCK FOR SORTING
	
	Added becouse of sorting in a buffer.
   ____________________________________________________________________________
*/

#define MAX_OUTPUT_LINES 2048

char *sort_buff[MAX_OUTPUT_LINES];		//buffer for MAX_OUTPUT_LINES strings
int sort_pos = 0;						//number of strings added
char line_buff[MAX_LINE_LENGTH];		//buffer for one line
int line_pos = 0;						//position in the buffer for one line

/* ____________________________________________________________________________

	void free_buffer()
	
	Free the last allocated memory in sort_buff.
   ____________________________________________________________________________
*/
void free_buffer(){
	int i;
	for(i = 0; i<sort_pos; i++){
		free(sort_buff[i]);
	}	
	sort_pos = 0;
}

/* ____________________________________________________________________________

	void init_buffer()
	
	Calls free_buffer() and then initialize the buffer position.
   ____________________________________________________________________________
*/
void init_buffer(){	
	free_buffer();					
    sort_pos = 0;
}

/* ____________________________________________________________________________

	int compare_line(const void *a, const void *b)
	
	Comparing function as requested for qsort function. 
   ____________________________________________________________________________
*/
int compare_line(const void *a, const void *b) {
    const char* s1 = *(const char **) a;
    const char* s2 = *(const char **) b;
    
    if(strlen(s1) < strlen(s2)){
    	return -1;
	}
	else if(strlen(s1) > strlen(s2)){
		return 1;
	}
    
    return my_strcmp(s1, s2);
}

/* ____________________________________________________________________________

	void print_buffer()
	
	Prints buffer's actual content. 
   ____________________________________________________________________________
*/
void print_buffer(){
	int i;
	for(i = 0; i<sort_pos; i++){
		printf("%s\n", sort_buff[i]);
	}	
}

/* ____________________________________________________________________________

	void add_line(char *line)
	
	Allocates and adds a string into buffer. 
   ____________________________________________________________________________
*/
void add_line(char *line){
	char *nl;
	if(sort_pos < MAX_LINE_LENGTH){
		nl = malloc(sizeof(char) * strlen(line) + 1);
		strcpy(nl, line);
		sort_buff[sort_pos++] = nl;
	}
}

/* ____________________________________________________________________________

	void sort_buffer()
	
	Sorts a buffer using qsort function. 
   ____________________________________________________________________________
*/
void sort_buffer(){
	qsort(sort_buff, sort_pos, sizeof(char *), compare_line);
}

/* ____________________________________________________________________________

	void print_childs_to_buffer(node *start)
    
    Adds paths of childrens of the start unordered to the buffer.
   ____________________________________________________________________________
*/
void print_childs_to_buffer(node *start){
	node *tmp=start->leftest_child;
	
	while(tmp != NULL){
		if(CONSOLE){
			printf("\t%s", tmp->text);
			if(tmp->type == DIRECTORY_T){
				putchar(FILE_SEPARATOR);
			}
			printf("\n");
		}
		else{
			print_abs_path_to_buffer(tmp);
		}
		
		tmp=tmp->right_brother;
	}
}

/* ____________________________________________________________________________

	void print_abs_path_to_line_buffer(node *n)
    
    Adds a path of node n to the line_buffer.
   ____________________________________________________________________________
*/
void print_abs_path_to_line_buffer(node *n){

	if(n == NULL){
		return;
	}
	
	if(n->type == ROOT_T){
		strcpy(&line_buff[line_pos], "/");
		line_pos++;
	}

	print_abs_path_to_line_buffer(n->parent);
	
	strcpy(&line_buff[line_pos], n->text);
	line_pos += strlen(n->text);
	
	if(n->type == DIRECTORY_T){
		strcpy(&line_buff[line_pos], "/");
		line_pos++;
	}
}

/* ____________________________________________________________________________

	void print_abs_path_to_buffer(node *n)
    
    Adds a path of node n to the buffer using print_abs_path_to_line_buffer().
   ____________________________________________________________________________
*/
void print_abs_path_to_buffer(node *n){
	line_pos = 0;
	
	print_abs_path_to_line_buffer(n);
	
	add_line(line_buff);
}


/* ____________________________________________________________________________

	END OF A BLOCK FOR SORTING
   ____________________________________________________________________________
*/






/* ____________________________________________________________________________

    node * get_root()
    
    Function allocates and returns new root node.
   ____________________________________________________________________________
*/
node * get_root(){
	node *root=malloc(sizeof(node));

	if(root == NULL){
		return NULL;
	}

	root->parent=NULL;
	root->left_brother=NULL;
	root->right_brother=NULL;
	root->leftest_child=NULL;
	root->type=ROOT_T;
	root->text[0]=END_OF_TEXT;

	return root;
}

/* ____________________________________________________________________________

    void overwrite_free_n(node **n, node *put)
    
    Overwrites node n with put and free n.
   ____________________________________________________________________________
*/
void overwrite_free_n(node **n, node *put){
	node *tmp = *n;
	node *left;
	node *parent;
	node *right;
		
	put->parent = tmp->parent;
	put->left_brother = tmp->left_brother;
	put->right_brother = tmp->right_brother;
	put->leftest_child = tmp->leftest_child;
	put->type = tmp->type;
	strcpy(put->text, tmp->text);
	
	if(tmp->left_brother != NULL){
		left = tmp->left_brother;
		left->right_brother = put;
	}
	else {	//put is new leftest
		parent = tmp->parent;
		parent->leftest_child = put;
	}
	
	if(tmp->right_brother != NULL){
		right = tmp->right_brother;
		right->left_brother = put;
	}
	
	delete_all(n);
	//free(*n);
	*n = NULL;
}

/* ____________________________________________________________________________

	void put_after_n(node *n, node *put)
    
    Puts node put after n.
   ____________________________________________________________________________
*/
void put_after_n(node *n, node *put){
	node *right=n->right_brother;
	
	put->parent=n->parent;

	put->left_brother=n;	
	put->right_brother=right;
	
	n->right_brother=put;
	
	if(right != NULL){
		right->left_brother=put;
	}	
}

/* ____________________________________________________________________________

	void put_before_n(node *n, node *put)
    
    Puts node put before n.
   ____________________________________________________________________________
*/
void put_before_n(node *n, node *put){
	node *left=n->left_brother;
	node *parent;
	
	put->parent=n->parent;

	put->left_brother=left;	
	put->right_brother=n;
	
	n->left_brother=put;
	
	if(left != NULL){
		left->right_brother=put;
	}
	else {	//put is new leftest
		parent=n->parent;
		parent->leftest_child=put;
	}
}

/* ____________________________________________________________________________

	node * move_node(node *where, node *n)
    
    Moves node n into folder where.
    Returns node n if success or NULL if not.
   ____________________________________________________________________________
*/
node * move_node(node *where, node *n){
	int cmp;
	node *tmp;
	node *left;
	node *right;
	node *parent;
	
	if(where == NULL){
	//	printf("where NULL\n");
		return NULL;
	}
	if(n == NULL){
	//	printf("n NULL\n");
		return NULL;
	}
	if(where->type == FILE_T){
	//	printf("where->type FILE\n");
		return NULL;
	}
	
	//remove from old place
	left = n->left_brother;
	right = n->right_brother;
	parent = n->parent;
	
	if(parent != NULL && parent->leftest_child == n){	
		parent->leftest_child = right;
	}
	if(left != NULL){
		left->right_brother = right;
	}
	if(right != NULL){
		right->left_brother = left;
	}
		
	//move to new place
	n->parent = where;
	n->left_brother = NULL;
	n->right_brother = NULL;

	
	if(where->leftest_child == NULL){
		n->left_brother=NULL;
		n->right_brother=NULL;
		where->leftest_child=n;	
		
		return n;
	}	
	else { 
		if(n->type == DIRECTORY_T){			
			tmp = where->leftest_child;	//can not be NULL
			
			while(tmp->type == DIRECTORY_T && my_strcmp(tmp->text, n->text) < 0 && tmp->right_brother != NULL){	//browse directories
				tmp = tmp->right_brother;
			}
			
			if(tmp->type == DIRECTORY_T){
				cmp=my_strcmp(tmp->text, n->text);
			
				if(cmp == 0){	// directory with a same name -> free n and return directory
					free(n);	// directories could be merged, but it is not needed
					return tmp;
				}	
				else if(cmp < 0){	//put after tmp
					put_after_n(tmp, n);
					return n;
				}	
				else {	//put before tmp
					put_before_n(tmp, n);
					return n;
				}
			}
			else{
				put_before_n(tmp, n);
				return n;
			}
			
		}
		else if(n->type == FILE_T){
			tmp = where->leftest_child;	//can not be NULL
			
			while(tmp->type == DIRECTORY_T && tmp->right_brother != NULL){	//skip directories
				tmp = tmp->right_brother;
			}
			
			if(tmp->type == DIRECTORY_T){
				put_after_n(tmp, n);	//put after tmp, n is the first file in where directory
				return n;
			}
			
			while(my_strcmp(tmp->text, n->text) < 0  &&  tmp->right_brother != NULL){	//browse files
				tmp = tmp->right_brother;
			}
			
			cmp=my_strcmp(tmp->text, n->text);
			
			if(cmp == 0){	// same name
				if(n->type == tmp->type){	//should be the same but safety first 
					//overwrite file tmp
					overwrite_free_n(&tmp, n);
					return n;	
				}
				else{
					free(n);
					return NULL;
				}
			}  
			else if(cmp < 0){	//put after tmp
				put_after_n(tmp, n);
				return n;
			}	
			else {		//put before tmp
				put_before_n(tmp, n);
				return n;
			}		
		}
	}	
}

/* ____________________________________________________________________________

	node * put_node(node *where, char *file_name, node_type type)
    
    Create new node and place it in folder where.
   ____________________________________________________________________________
*/
node * put_node(node *where, char *file_name, node_type type){
	node *for_add;
	
	if(file_name == NULL){
		return NULL;
	}
	if(type == ROOT_T){
		return NULL;
	}
	
	for_add = malloc(sizeof(node));
	if(for_add == NULL){
		return NULL;
	}	
	
	for_add->parent = NULL;
	for_add->type = type;
	strcpy(for_add->text, file_name);
	for_add->leftest_child = NULL;
	for_add->left_brother = NULL;
	for_add->right_brother = NULL;	
	
	return move_node(where, for_add);
}

/* ____________________________________________________________________________

	return_state fill_nodes_abs_path(node *root, char *path)
    
    Adds all nodes in abs. path from root.
   ____________________________________________________________________________
*/
return_state fill_nodes_abs_path(node *root, char *path){
	node *tmp;
	char buff[MAX_TEXT_LENGTH];
	int i, j;
	char c;
	node_type type;
	return_state state;
	
	if(root == NULL){
		return PARAM_WRONG;
	}
	if(path == NULL){
		return PARAM_WRONG;
	}
				
	tmp=root;
	i=0, j=1;	//j = 1 becouse of abs. path starting /
	c='a';
	type=ROOT_T;
		
	while(c != END_OF_TEXT){
			
		for(i=0; i < MAX_TEXT_LENGTH; i++){
			c=path[j];
			state=validate_char(c);
			
			if(state == INVALID){
				printf("Unaccepted character '%c' was found in '%s'. Adding stopped there.\n", c, path);			
				return ERROR;
			}
			
			j++;
			
			if(state == VALID){
				buff[i]=c;
			}
			else{		
				if(state == SEPARATOR){
					type=DIRECTORY_T;
				}
				else /*if(state == TEXT_END)*/{
					type=FILE_T;
				}
				
				buff[i]=END_OF_TEXT;
				break;
			}
		}
		
		tmp=put_node(tmp, buff, type);
		
		if(tmp == NULL){
			printf("'%s' couldn't be added.\n", buff);
			
			return ERROR;
		}
			
		if(path[j]==END_OF_TEXT){ //the last is directory which was added
			break;
		}
	}	
	return OK;
}

/* ____________________________________________________________________________

	void delete_all(node **start)
    
    Recursively deletes all from start.
   ____________________________________________________________________________
*/
void delete_all(node **start){
	node *tmp;
	node *tmp2;
	
	if(*start==NULL){
		return;
	}
	
	tmp=(*start)->leftest_child;
	
	while(tmp != NULL){
		tmp2=tmp->right_brother;
		delete_all(&tmp);
		tmp=tmp2;
	}
	
	free(*start);
	*start=NULL;
	
	return;
}

/* ____________________________________________________________________________

	void print_abs_path(node *n)
    
    Prints absolute path of node n.
   ____________________________________________________________________________
*/
void print_abs_path(node *n){
	if(n == NULL || n->type == ROOT_T){
		return;
	}
	
	print_abs_path(n->parent);
	
	printf("/%s",n->text);
}

/* ____________________________________________________________________________

	void print_all(node *start)
    
    Recursively prints abs. path of start and its content.
   ____________________________________________________________________________
*/
void print_all(node *start){
	node *tmp;
	node *tmp2;
	
	if(start==NULL){
		return;
	}
	
	print_abs_path(start);
	if(start->type == DIRECTORY_T){
		printf("/");
	}
	printf("\n");
	
	tmp=start->leftest_child;

	while(tmp != NULL){
		tmp2=tmp->right_brother;
		print_all(tmp);
		tmp=tmp2;
	}
	
	return;
}

/* ____________________________________________________________________________

	void print_childs(node *start)
    
    Prints childrens of start.
   ____________________________________________________________________________
*/
void print_childs(node *start){
	node *tmp=start->leftest_child;
	
	while(tmp != NULL){
		if(CONSOLE){
			printf("\t%s", tmp->text);
		}
		else{
			print_abs_path(tmp);
		}
		
		if(tmp->type == DIRECTORY_T){
			putchar(FILE_SEPARATOR);
		}
		printf("\n");
		tmp=tmp->right_brother;
	}
}

/* ____________________________________________________________________________

	node * change_node(node *from, char *path)
    
    Returns node if path 'path' is used from node 'from'. If problem occurs returns NULL
   ____________________________________________________________________________
*/
node * change_node(node *from, char *path){
	node *tmp=from;
	char buff[MAX_TEXT_LENGTH];
	int i=0, j=0;
	char c='a';
	return_state state;
	
	while(c != END_OF_TEXT){
	
		for(i=0; i < MAX_TEXT_LENGTH; i++){	//getting name of directory
			c=path[j];
			state=validate_char(c);
			
			if(state == INVALID){
		//		printf("ERROR unaccepted character '%c' was found in '%s'.\n", c, path);			
				return NULL;
			}
			
			j++;
			
			if(state == VALID){
				buff[i]=c;
			}
			else{					
				buff[i]=END_OF_TEXT;
				break;
			}
		}
		
		if(strcmp(buff, BACK_SEQUENCE) == 0){		// found .. -> go back
			tmp = tmp->parent;
		
			if(tmp == NULL){
		//		printf("ERROR path does not exist. Path '%s' goes beyond the root.\n", path);
				return NULL;
			}
			
		}
		else{
			tmp = has_child(tmp, buff);
		
			if(tmp == NULL){
		//		printf("ERROR path does not exist. Directory '%s' does not exist.\n", buff);
				return NULL;
			}
		}
			
		if(path[j]==END_OF_TEXT){ //the last is directory which was added
			break;
		}
	}
	
	return tmp;	
}

/* ____________________________________________________________________________

	node * has_child(node *who, char *name)
    
    If node 'who' has a child with name 'name', has_child returns this child as a node. Otherwise returns NULL.
   ____________________________________________________________________________
*/
node * has_child(node *who, char *name){
	node *tmp = who->leftest_child;
				
	while(tmp != NULL){
		if(my_strcmp(tmp->text, name) == 0){	
			break;								
		}			
		tmp=tmp->right_brother;
	}
		
	return tmp;	
}

/* ____________________________________________________________________________

	node * get_copy(const node *n)
    
    Returns a copy of node n with the same text and type.
   ____________________________________________________________________________
*/
node * get_copy(const node *n){
	node *ret;
	
	if(n == NULL){
		return NULL;
	}
	if(n->type != FILE_T){
		return NULL;
	}
	
	ret = malloc(sizeof(node));
	if(ret == NULL){
		return NULL;
	}
				
	ret->type = n->type;
	strcpy(ret->text, n->text);	
				
	ret->parent = NULL;
	ret->leftest_child = NULL;
	ret->left_brother = NULL;
	ret->right_brother = NULL;
		
	return ret;	
}


