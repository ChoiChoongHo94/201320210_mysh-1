#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <wait.h>


#include "commands.h"
#include "built_in.h"
#include "utils.h"

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
<<<<<<< HEAD
 
  short int isPipe =0 ;
  short int isBg =0;
 
  //pipe test
  if(n_commands>1) isPipe =1;
  printf("%d\n",isPipe);
  //bg test
  int i=0;
  while((*commands + n_commands -1)->argv[i] != NULL){
	if((*commands + n_commands -1)->argv[i] == "&" && (*commands + n_commands -1)->argv[i+1]==NULL )
		 isBg =1;
	else i++;
  }
  printf("%d\n",isBg);
  //test end
  
  if (n_commands > 0) {
=======


  short int isPipe =0 ;
  short int isBg =0;
  
  if (n_commands > 1) isPipe = 1;
  printf("%d\n", isPipe);

  int i = 0;
  while ((*commands + n_commands - 1)->argv[i] != NULL) {
	  if ((*commands + n_commands - 1)->argv[i] == "&" && (*commands + n_commands - 1)->argv[i] == NULL)
		  isBg = 1;
	  else i++;
  }

  

  else if (n_commands > 0) {
>>>>>>> 275e4e6dce61337c1a5300aa020aa3de8e63678f
    struct single_command* com = (*commands);

    assert(com->argc != 0);

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
<<<<<<< HEAD
    } else if (isPipe == 1) { //pipeline implement
		i = 0;
	//	while (i<n_commands-1) {
			int pid = fork();
			if (pid > 0) {  //parent
			
				int server_socket;
				int client_socket;
				int client_addr_size;

				struct sockaddr server_addr;
				struct sockaddr client_addr;


				server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
				//if(server_socket == -1) printf("not server socket!\n");
				
				memset(&server_addr, 0, sizeof(server_addr));
				server_addr.sa_family = AF_UNIX;
				if(-1==bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
					printf("not binded!\n");
			
					
				if(-1==listen(server_socket,5)) printf("not listened!\n");
				client_addr_size = sizeof(client_addr);
	                        client_socket = accept(server_socket, (struct sockaddr*)&client_addr,
                                &client_addr_size);
				if(-1 == client_socket) printf("not accept!\n");

				if(fork()==0){
				dup2(client_socket,1);
				execv(com[i].argv[0],com[i].argv);
				}

				else{ //parent
					int status;
					wait(&status);
					close(client_socket);
				}	
				
			}

			else { // child
				int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
			//	if(client_socket == -1) printf("not client socket!\n");
				struct sockaddr server_addr;
				memset(&server_addr, 0, sizeof(server_addr));
				server_addr.sa_family = AF_UNIX;
			//	sleep(0.5);
				if(-1 == connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
							printf("not connect!\n");
			
				if(fork()==0){
				dup2(client_socket,0);
                         	execv(com[i+1].argv[0], com[i+1].argv);
				}
	
				else{
					int status;
					wait(&status);
					exit(1);
				}
			}
	//		i++;
	//	}
		return 4;

	}

	else if(com->argv[0][0]=='/'){  // may be changed later
=======
    } else if(com->argv[0][0]=='/'){  // may be changed later
		  if (isPipe == 1) { //pipeline implement
		  	i = 0;
		  	
		  	while (i < n_commands -1){
		  		int pid = fork();
		  		if (pid > 0) { //parent
		  			int server_socket;
		  			int client_socket;
		  			int client_addr_size;
		  
		  			struct sockaddr_un server_addr;
		  			struct sockaddr_un client_addr;
		  
		  
		  			server_socket = socket(PF_FILE, SOCK_STREAM, 0);
		  			//if error
		  			memset(&server_addr, 0, sizeof(server_addr));
		  			server_addr.sun_family = AF_UNIX;
		  			//strcpy(server_addr.sunpath, com[i+1]->argv[0]);
		  			bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))
		  				while (1) {
		  					// pthread should be made
		  					listen(server_socket, 5);
		  
		  					client_addr_size = sizeof(client_addr);
		  					client_socket = accept(server_socket, (struct sockaddr*)&client_addr,
		  						&client_Addr_size);
		  					// communicate
							if (fork() == 1) {
								execv(com[i + 1]->argv[0], com[i + 1]->argv);
								dup2(server_socket, 1);
								sleep(1); exit(1);
							}

		  					
		  					close(client_socket);
		  				}
		  		}
		  
		  		else { // child
		  			int client_socket = socket(PF_FILE, SOCK_STREAM, 0);
		  			int client_addr_size;
		  			struct sockaddr_un server_addr;
		  			memset(&server_addr, 0, sizeof(server_addr));
		  			server_addr.sun_family = AF_UNIX;
		  			//strcpy(server_addr.sun_path, (*commands)[j + 1]->argv[0]);
		  			connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
		  			//communicate
					if (fork() == 1) {
						execv(com[i + 1]->argv[0], com[i + 1]->argv);
						exit(1);
					}
		  		}
		  	}
		  	return 4;
		  
		  }
>>>>>>> 275e4e6dce61337c1a5300aa020aa3de8e63678f
         int  pid;
         if(isBg==1) {  //bg implementation
           pid=fork();
           if(pid == 0){
             printf("%d\n",getpid());
             execv((*commands)->argv[0],(*commands)->argv);
             // should implememt strcat
             printf("%d done  %s",getpid(), (*commands)->argv[0]);  exit(1);
           }
           return 3;
         }

<<<<<<< HEAD
         else{  //just process creacion
           pid = fork();
           if(pid ==0) {
             execv((*commands)->argv[0], (*commands)->argv);  exit(1);
           }
         return 2;
         }
    }

=======
     	 else{  //just process creacion
     	   pid = fork();
     	   if(pid ==0) {
                  execv((*commands)->argv[0], (*commands)->argv);  exit(1);
                }       
              return 2;
              }
    }  
	
>>>>>>> 275e4e6dce61337c1a5300aa020aa3de8e63678f
    else {
      fprintf(stderr, "%s: command not found\n", com->argv[0]);
      return -1;
    }
  }

  return 0;
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
