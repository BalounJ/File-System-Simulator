
#ifndef FILESYS_H_
#define FILESYS_H_

#include "mylib.h"


/* ____________________________________________________________________________

    Function Prototypes
   ____________________________________________________________________________
*/
node * get_root();												//allocates and returns new root node
return_state fill_nodes_abs_path(node *root, char *path);		//adds all nodes in abs. path from root
node * move_node(node *where, node *n);							//returns node if moved or directory exists, NULL if wasn't moved
node * put_node(node *where, char *file_name, node_type type);  //returns node if added or directory exists, NULL if wasn't added

void delete_all(node **start);									//recursively deletes all from start
void print_childs(node *start);									//prints abs. path childs of start
void print_all(node *start);									//prints abs. path of start and its content

void overwrite_free_n(node **n, node *put);						//overwrites node n with put and free n
void put_after_n(node *n, node *put);							//puts node put after n
void put_before_n(node *n, node *put);							//puts node put before n

void print_abs_path(node *n);									//prints absolute path of node n

node * change_node(node *from, char *path);						//returns node if exists, NULL if doesn't exists path

node * has_child(node *who, char *name);						//returns node if exists, NULL if doesn't exists

node * get_copy(const node *n);									//returns a copy of node n with the same text and type


/* ____________________________________________________________________________

    Added becouse of sorting
   ____________________________________________________________________________
*/
void free_buffer();

void init_buffer();

void print_buffer();

void sort_buffer();

void print_childs_to_buffer(node *start);

void print_abs_path_to_buffer(node *n);



#endif /* FILESYS_H_ */

