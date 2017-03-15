#include "mylib.h"
#include <string.h>


/* ____________________________________________________________________________

    return_state group_of_char(char c)
    
    Returns which group of char is c.
   ____________________________________________________________________________
*/
return_state group_of_char(char c){
	if(c>='a' && c<='z'){
		return LOWER_CHAR;
	}
	if(c>='A' && c<='Z'){
		return UPPER_CHAR;
	}
	if(c>='0' && c<='9'){
		return NUMBER_CHAR;
	}
	if(c == END_OF_TEXT){
		return TEXT_END;
	}
	
	return OTHER_CHAR;
}

/* ____________________________________________________________________________

    return_state validate_char(char c)
    
    Validates char c. There are only a-z, A-Z, 0-9, / , dot and \0 usable chars. If its not one of them function returns INVALID.
   ____________________________________________________________________________
*/
return_state validate_char(char c){
	if(group_of_char(c) == OTHER_CHAR){			
		switch(c){		
			case FILE_SEPARATOR:	
				return SEPARATOR;
				break;
			case DOT_CHARACTER:	//dot is a valid character
				return VALID;
				break;
		}	
		
		return INVALID;
	}
	
	if(c == END_OF_TEXT){
		return TEXT_END;
	}
	
	return VALID;
}

/* ____________________________________________________________________________

    int my_strcmp(const char * str1, const char * str2)
    
    My function for comparing becouse of groups. Small letters first, numbers last.
   ____________________________________________________________________________
*/
int my_strcmp(const char * str1, const char * str2){
	char c1, c2;
	int i = 0;
	int ret = 0;
	return_state gr1, gr2;
	
	while(ret == 0){
		c1 = str1[i];
		c2 = str2[i];		
		ret = c1 - c2;
		
		if(c1 == END_OF_TEXT || c1 == END_OF_TEXT){
			return ret;
		}
		i++;
	}
	
	gr1 = group_of_char(c1);
	gr2 = group_of_char(c2);
	
	if(gr1 == gr2){	//same groups
		return ret;
	}
	
	if(gr1 < gr2){	//group of 1. char is before gr. of 2. char
		if(ret > 0){
			ret = -ret;
		}		
		return ret;
	}
	else {	//otherwise
		if(ret < 0){
			ret = -ret;
		}		
		return ret;
	}
	
	return ret;	//not necessary
}

/* ____________________________________________________________________________

    return_state accept_name(const char * mask, const char * name)
    
    Accepting two strings for find command. This function is using backtracking and probably is not optimal but fast enough.
    Offers more options than requested. For example 'fdsf*fsdf*fds.ex*'.
   ____________________________________________________________________________
*/
return_state accept_name(const char * mask, const char * name){
	int end = 0;
	int num = 0;
	int n = 0;
	int m = 0;
	
	while(mask[end] != END_OF_TEXT){
		if(mask[end] == ITER_SYMBOL){
			num++;
		}
		end++;
	}
	
	if(end == num){	//mask is full of ***
		return YES;
	}
	
	if(num == 0){	//no * in mask
		if(strcmp(mask, name) == 0){
			return YES;
		}
		else{
			return NO;
		}
	}
	
	// has to be some * somewhere
	n = 0;
	m = 0;
	
	if(mask[0] != ITER_SYMBOL){
		while(name[n] == mask[m]){
			m++;
			n++;
		}
	}
	
	if(mask[m] != ITER_SYMBOL){
		return NO;
	}
	else{
		m++;
		if(mask[m] == END_OF_TEXT){	//sucess* rest of name is not important
			return YES;
		}
	}
	
	// mask  *asdasd*sfafsaf*af*afs.....
	while(name[n] != END_OF_TEXT){
		if(accept_name(&mask[m], &name[n]) == YES){	// sending all possible names behind last *
			return YES;
		}
		n++;
	}
	
	return NO; //name was too short mask has still characters
}











