#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <wait.h>
#include <sys/un.h>
#include <pthread.h>

#include "commands.h"
#include "built_in.h"
#include "utils.h"
void* serverSocket(void*);

static struct built_in_command built_in_commands[] = {
  { "cd", do_cd, validate_cd_argv },
  { "pwd", do_pwd, validate_pwd_argv },
  { "fg", do_fg, validate_fg_argv }
};

static int is_built_in_command(const char* command_name)
{
  static const int n_built_in_commands = sizeof(built_in_commands) / sizeof(built_in_commands[0]);

  for (int i = 0; i < n_built_in_commands; ++i) {
    if (strcmp(command_name, built_in_commands[i].command_name) == 0) {
      return i;
    }
  }

  return -1; // Not found
}

 /*
 * Description:ou should modify this structure to launch process and offer pipeline functionality.
 */
int evaluate_command(int n_commands, struct single_command (*commands)[512])
{ 
   if (n_commands > 0) {

	    struct single_command* com = (*commands);
    
  	    assert(com->argc != 0);
  	    short int isPipe =0 ;
  	    short int isBg =0;

	  //pipe test
	  if(n_commands>1) isPipe =1;
	  printf("%d\n",isPipe);
	  //bg test
	  int last = n_commands -1;
  	  if (strcmp(com->argv[(com->argc)-1],"&")==0) {
     		  isBg = 1; com->argv[(com->argc)-1] = NULL;
	  }
 	  printf("%d\n",isBg);  

    int built_in_pos = is_built_in_command(com->argv[0]);
    if (built_in_pos != -1) {
        if (built_in_commands[built_in_pos].command_validate(com->argc, com->argv)) {
        if (built_in_commands[built_in_pos].command_do(com->argc, com->argv) != 0) {
          fprintf(stderr, "%s: Error occurs\n", com->argv[0]);
        }
      } else {
        fprintf(stderr, "%s: Invalid arguments\n", com->argv[0]);
        return -1;
      }
    } else if (strcmp(com->argv[0], "") == 0) {
      return 0;
    } else if (strcmp(com->argv[0], "exit") == 0) {
      return 1;
    } 

      else if(com->argv[0][0]=='/'){  // may be changed later

				if (isPipe == 1) { //pipeline implement
					int i = 0;
					int pid = fork();
					if (pid >  0) {  
						/*pthread_t tid;
                                                pthread_attr_t attr;
                                                pthread_attr_init(&attr);
						pthread_create(&tid,&attr,serverSocket, (void *)com) */
						if(fork() ==0){
					
							int server_socket;// = (int *)malloc(sizeof(int));
                                               		int client_socket;
                                               		int client_addr_size;

                                            		struct sockaddr_un server_addr;
                                               		struct sockaddr_un client_addr;

                                                	server_socket = socket(AF_UNIX, SOCK_STREAM,0);
                                              		memset(&server_addr, 0, sizeof(server_addr));
                                               		server_addr.sun_family = AF_UNIX;
                                               		unlink("server_socket");
                                             		strcpy(server_addr.sun_path, "server_socket");
                                             		int option =1;
                                                	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
                                       	        	if (-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
                                       	                	 perror("blind error");

						
                                               		if (-1 == listen(server_socket, 5)) perror("listen error");
                                              		client_addr_size = sizeof(client_addr);
                                               		client_socket = accept(server_socket, (struct sockaddr*)&client_addr,
                                          	              &client_addr_size);
                                               		if (-1 == client_socket) perror("accept error");

                                               		if(fork() ==0){
								 printf("s\n");
                                               	        	 dup2(client_socket,1);
                                               	        	 execv(com[i].argv[0], com[i].argv);
                                               		}

                                                	//parent
							else{
        	                                       		int status;
                	                              		wait(&status);
                        	                    		close(client_socket);
                                	               		close(server_socket);
								exit(1);
							}
						}
							
					}

					else if(pid ==0){ // 
                                                	int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);

                                            		struct sockaddr_un server_addr;
                                                	memset(&server_addr, 0, sizeof(server_addr));
                                                	server_addr.sun_family = AF_UNIX;
                                                	strcpy(server_addr.sun_path,"server_socket");
                                                	int tc=connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)); 
							if( tc == -1) perror("connect error");
							
                          		                if (fork() == 0) {
								printf("c\n");
								dup2(client_socket,0);
                                                                execv(com[i + 1].argv[0], com[i + 1].argv);
                                                	}

                                                	else{
								int status;
								wait(&status);
                                                		close(client_socket);
                                                		exit(1);
							}
                                        }
                                        	
					
					return 4;
			
				}// end pipe
 
       		   else if(isBg==1) {  //bg implementation
       			   int pid=fork();
		  	   if(pid == 0){
 				if(fork() ==0 ){
					 printf("%d\n",getppid());
					 execv(com->argv[0], com->argv);
				}
				else {
                              		 int status;
                               		 wait(&status);
                              		 printf("%d  done  %s\n",getpid(),com->argv[0]);
                          	}
       			   }
		   	   return 3;
		   }//end bg
	
        	 else{  //just process creacion
        	  	 int pid = fork();
     		     	 if(pid ==0) {
       		     		 execv((*commands)->argv[0], (*commands)->argv);  exit(1);
       		   	 }
        		 return 2;
      		 }// end pc
    
    }
    else {
      fprintf(stderr, "%s: command not found\n", com->argv[0]);
      return -1;
    }

  return 0;
}
}


void free_commands(int n_commands, struct single_command (*commands)[512])
{
  for (int i = 0; i < n_commands; ++i) {
    struct single_command *com = (*commands) + i;
    int argc = com->argc;
    char** argv = com->argv;
    for (int j = 0; j < argc; ++j) {
      free(argv[j]);
    }

    free(argv);
  }
   memset((*commands), 0, sizeof(struct single_command) * n_commands);
}


void* serverSocket(void *data){
							struct single_command* com = /*(struct single_command)*/data;
                                        		int server_socket;// = (int *)malloc(sizeof(int));
                                                        int client_socket;
                                                        int client_addr_size;

                                                        struct sockaddr_un server_addr;
                                                        struct sockaddr_un client_addr;

                                                        server_socket = socket(AF_UNIX, SOCK_STREAM,0);
                                                        memset(&server_addr, 0, sizeof(server_addr));
                                                        server_addr.sun_family = AF_UNIX;
                                                        unlink("server_socket");
                                                        strcpy(server_addr.sun_path, "server_socket");
                                                        int option =1;
                                                        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
                                                        if (-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
                                                                 perror("blind error");

                                                        if (-1 == listen(server_socket, 5)) perror("listen error");
                                                        client_addr_size = sizeof(client_addr);
                                                        client_socket = accept(server_socket, (struct sockaddr*)&client_addr,
                                                              &client_addr_size);
                                                        if (-1 == client_socket) perror("accept error");

                                                        if(fork() ==0){
                                                                 printf("s\n");
                                                                 dup2(client_socket,1);
                                                                 execv(com[0].argv[0], com[0].argv);
                                                        }

                                                        //parent
                                                        int status;
                                                        wait(&status);
                                                        close(client_socket);
                                                        close(server_socket);
                 					 pthread_exit(0);
}	


