#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "commands.h"
#include "built_in.h"
#include "utils.h"

int main()
{
  char buf[8096];

  while (1) {
    fgets(buf, 8096, stdin);

    struct single_command commands[512];
    int n_commands = 0;
    mysh_parse_command(buf, &n_commands, &commands);

    int ret = evaluate_command(n_commands, &commands);

    //me
        if(ret == 2){
          int pid;
          pid = fork();
          if(pid < 0) fprintf(stderr,"Fork Failed");
          else if(pid ==0) {  //child
	    int argc; char** argv; 	
	    parse_single_command(buf, &argc, &argv);
	    execv(argv[0],argv);
	  }
          
        }
    //me end



    free_commands(n_commands, &commands);
    n_commands = 0;


    if (ret == 1) {
      break;
    }
  }

  return 0;
}
