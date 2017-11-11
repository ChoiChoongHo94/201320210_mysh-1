#include "signal_handlers.h"
#include <signal.h>
#include <stdio.h>

void catch_sigint(int signalNo)
{
  // TODO: File this!
	
}

void catch_sigtstp(int signalNo)
{
  // TODO: File this!
	printf("\n");	
	signal(SIGTSTP, &catch_sigtstp);			
}
