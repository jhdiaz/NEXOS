#include <kernel.h>
#include <stdlib.h>
#include <shell.h>
#include <stdio.h>
#include <string.h>
#include <tty.h>
#define LINE_LENGTH 33

//An array of characters is declared for each TTY device to read with.
char line0[LINE_LENGTH];
char line1[LINE_LENGTH];

//Prototype.
void clearChars(char [], int);

command xsh_chat(ushort stdout, ushort stdin, ushort stderr, ushort nargs, char* args[]){

	fprintf(stdout, "\nStarted chat application.\n");
	
	//Variables for successful login of each user.
	int success1 = 0;
	int success2 = 0;
	//This variable is used to hold the argument passed in to this function and will dictate number of users.
	int numberOfUsers = 0;

	//Variables which will be used as booleans for controlling when an exit occures.
//	int exit1 = 0;
//	int exit2 = 0;

	//If there are no arguments then it will default to a 1 user chat.
	if(nargs == 2){
		numberOfUsers = atoi(args[1]);
	}
	else if(nargs == 1){
		numberOfUsers = 1;
	}
	else{
		fprintf(stderr, "\nToo many arguments.\n");
		return SYSERR;
	}


	//Username prompt.
	fprintf(TTY0, "\nPlease enter the username and password corresponding to this TTY device (TTY0).\n");
	fprintf(TTY0, "Username: ");
	read(TTY0, line0, LINE_LENGTH);
	
	//Will also prompt user on other TTY device if argument for 2 users was inputted.
	if(numberOfUsers == 2){
		//After user1 inputs their correct username they must wait for user2 to do the same.
		fprintf(TTY0, "\nWaiting for user2 to enter their username...\n");
		fprintf(TTY1, "\nPlease enter the username and password corresponding to this TTY device (TTY1).\n");
		fprintf(TTY1, "Username: ");
		read(TTY1, line1, LINE_LENGTH);
	}

	//Only successful with correct username and password associated with device (TTY0).
	if(strncmp(line0, "user1", 5) == 0){
		fprintf(TTY1, "\nWaiting for user1 to login...\n");
		fprintf(TTY0, "Password: ");
		read(TTY0, line0, LINE_LENGTH);
		if(strncmp(line0, "pass1", 5) == 0){
			success1 = 1;
			fprintf(TTY0, "\nYou have successfully logged in with user1.\nType '--exit' if you wish to quit.\n");
			fprintf(TTY1, "\nUser1 has successfully logged in.\n");
		}
		else{
			fprintf(TTY0, "\nWrong password was entered.\n");
			fprintf(TTY1, "\nUser1 has failed their login attempt.\n");
			return SYSERR;
		}
		//Buffer is cleared after everytime a read is no longer needed to prevent unwanted characters from printing.
		clearChars(line0, LINE_LENGTH);
	}
	else{
		fprintf(TTY0, "\nWrong username was entered.\n");
		fprintf(TTY1, "\nUser1 has failed their login attempt.\n");
		return SYSERR;
	}

	//Only successful with correct username and password associated with device (TTY1).
	if(strncmp(line1, "user2", 5) == 0 && success1 == 1 && numberOfUsers == 2){
		fprintf(TTY0, "\nWaiting for user2 to enter their password.\n");
		fprintf(TTY1, "\nPassword: ");
		read(TTY1, line1, LINE_LENGTH);
		if(strncmp(line1, "pass2", 5) == 0){
			success2 = 1;
			fprintf(TTY1, "\nYou have successfully logged in with user2.\nType '--exit' if you wish to quit.\n");
			fprintf(TTY0, "\nUser 2 has successfully logged in.\n");
		}
		else{
			fprintf(TTY1, "\nWrong password was entered.\n");
			fprintf(TTY0, "User2 has failed their login attempt.\n");
			return SYSERR;
		}
		clearChars(line1, LINE_LENGTH);
	}
	else if(success1 == 1 && numberOfUsers == 2){
		fprintf(TTY1, "\nWrong username was entered.\n");
		fprintf(TTY0, "\nUser2 has failed their login attempt.\n");
		clearChars(line1, LINE_LENGTH);
		return SYSERR;
	}
	else{
		clearChars(line1, LINE_LENGTH);
	}

	//Loop will run, permitting user to enter and see text as long as 'exit' was not entered by user 1 or there are
	//enoug users active.
	while(success1 == 1){

		//Only one user (must be on TTY0). User writes to and reads from himself. Mostly used for testing purposes.
		if(numberOfUsers == 1){
			read(TTY0, line0, LINE_LENGTH);
			fprintf(TTY0, "\nUser1: ");
			if(strncmp(line0, "--exit", 6) == 0){
				fprintf(TTY0, "\nLogging off.\n");
				success1 = 0;
				//Program terminates if user1 logs off regardless of whether or not there are others.
			}
			fprintf(TTY0, "%s\n", line0);
//			write(TTY0, line0, LINE_LENGTH);
			clearChars(line0, LINE_LENGTH);
		}
		//There are two users. Text will be printed to other user. The chat is turn based.
		else if(numberOfUsers == 2){
			//First user1 sends a message.
			fprintf(TTY0, "\nIt is your turn to chat.\n");
			fprintf(TTY1, "\nWaiting for input from user1.\n");
			read(TTY0, line0, LINE_LENGTH);

			//Exit command for first user.
			if(strncmp(line0, "--exit", 4) == 0){
				fprintf(TTY0, "\nLogging off.\n");
				fprintf(TTY1, "\nUser1 has logged off. Chat is terminating.\n");
				//Program is terminated because only user1 can do a one-user chat.
				return OK;
			}

			//Kick command for first user.
			else if(strncmp(line0, "--kick", 6) == 0){
                                fprintf(TTY0, "\nYou have kicked user2.\n");
                                fprintf(TTY1, "\nUser1 has kicked you. Maybe you deserved it.\n");
				clearChars(line0, LINE_LENGTH);
                                numberOfUsers = 1;
                        }

			//Standard text print from first user to second user.
			else{
				fprintf(TTY1, "\nUser1: ");
				fprintf(TTY1, "%s\n", line0);
//				write(TTY1, line0, LINE_LENGTH);
				clearChars(line0, LINE_LENGTH);

				//After user1 sends a message it is user2's turn.
				fprintf(TTY1, "\nIt is your turn to chat.\n");
				fprintf(TTY0, "\nWaiting for input from user2.\n");
				read(TTY1, line1, LINE_LENGTH);

				//Exit command for second user.
	                	if(strncmp(line1, "--exit", 6) == 0){
	                        	fprintf(TTY1, "\nLogging off.\n");
					fprintf(TTY0, "\nUser2 has logged off.\n");
	         			numberOfUsers = 1;
					clearChars(line1, LINE_LENGTH);
		         	}
				//Standard text print from second user to first user.
				else{
					fprintf(TTY0, "\nUser2: ");
	                		fprintf(TTY0, "%s\n", line1);
					write(TTY0, line1, LINE_LENGTH);
	                		clearChars(line1, LINE_LENGTH);
				}
			
			}
			clearChars(line0, LINE_LENGTH);
			clearChars(line1, LINE_LENGTH);
		}
		else{
			fprintf(TTY1, "\nNot enough users or an error occurred.\n");
			fprintf(TTY0, "\nNot enough users or an error occurred.\n");
			return SYSERR;
		}
	}

	return OK;
}

void clearChars(char x[], int size){
	int i;
	for(i = 0;i < size;i++){
		x[i] = 0;
	}
}
