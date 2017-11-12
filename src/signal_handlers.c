#include "signal_handlers.h"
#include <signal.h>
#include <stdio.h>

void catch_sigint(int signalNo)
{
  // TODO: File this!
	printf("\n");
	signal(SIGINT, &catch_sigint);	
}

void catch_sigtstp(int signalNo)
{
  // TODO: File this!
	printf("\n");	
	signal(SIGTSTP, &catch_sigtstp);			
}
