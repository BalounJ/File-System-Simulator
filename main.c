#include <stdio.h>

#include "commands.h"

/* ____________________________________________________________________________
 
    MAIN PROGRAM
   ____________________________________________________________________________
*/
int main(int argc, char *argv[]) {
	if(argc == 3){		// 2 parametres are necessary
		start();
		
		if(load_files(argv[1])){	//if filesystem was loaded
		
			if(CONSOLE)	{			//work in CONSOLE mode or not
				while(operate()){	
				}
			}else{
				if(exec_commands(argv[2]) == 0){	//if commands were not loaded
					printf("File with commands could not be loaded.\n");
				}
			}
		}
		else{
			printf("File with an image of filesystem could not be loaded.\n");
		}
	
		stop();	
	}
	else{
		printf("2 parametres are necessary\nfssim.exe <files> <commands>\n<files> file with image of filesystem\n<commands> file with commands\n");
	}
	return 0;
}

