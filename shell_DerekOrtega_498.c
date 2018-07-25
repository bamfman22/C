/*
 ============================================================================
 Name        : shell.c
 Author      : Derek Ortega
 Student ID  : 010378498
 Copyright   : Your copyright notice
 Description : Hw1 shell program
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 80

int main(void) {
	char *args[MAXLINE/2 +1];
	char input[1000];


	int should_run = 1;

	printf("CS149 Shell from Derek Ortega\n");
	while(should_run){

		printf("Derek-498>");
		fflush(stdout);
		gets(input);
		int i = 0;
		if(strstr(input,"&") != NULL)
		{
			int run = 1;
			int x;

			for(x = 0; input[x] != '\0'; ++x){
				if(input[x] == "&")
				{
					input[x] = '\0';
				}
			}

			i = 1;
			parse(input,args);
		}

		else{
			parse(input, args);
		}


		if(strcmp(input, "exit") == 0){
			should_run = 0;
			exit(1);
		}
		else{



			if(i){


				foo(args,1);
			}
			else
			{
				foo(args,0);
			}
		}

	}


}


void parse(char* input,char **args)
{
	int i = 0;
	char* tok;

	tok = strtok(input, " ");

	args[i] = tok;


	while(tok != NULL)
	{
		tok = strtok(0, " ");


		args[++i] = tok;



	}



}

void foo(char **args, int bWait)
{
	pid_t pid = fork();
	long status;


	if(pid  < 0)
	{
		printf("Error: forking\n");
		exit(1);
	}
	else if(pid == 0)
	{
		execvp(args[0], args);
		printf("Error in exec\n");
		exit(1);

	}
	else if (!bWait){
			wait(&status);
		}

}



