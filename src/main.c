#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

#include "commands.h"
#include "built_in.h"
#include "utils.h"
#include "signal_handlers.h"
int cpid=-1;

int main()
{  
  int bgstatus;
  char buf[8096];
  signal(SIGTSTP, &catch_sigtstp);
  signal(SIGINT, &catch_sigint); 	
  while (1) {
    if(cpid>0) 
	if(waitpid(cpid, &bgstatus, WNOHANG) != 0) cpid = -1; 
    fgets(buf, 8096, stdin);
    if(cpid>0)
        if(waitpid(cpid, &bgstatus, WNOHANG) != 0) cpid = -1;

  //  signal(SIGTSTP, &catch_sigtstp);

    struct single_command commands[512];
    int n_commands = 0;
    mysh_parse_command(buf, &n_commands, &commands);
    int ret = evaluate_command(n_commands, &commands);

    free_commands(n_commands, &commands);
    n_commands = 0;


    if (ret == 1) {
      break;
    }
  }

  return 0;
}
