#include <kernel.h>
#include <stdio.h>
#include <string.h>

command xsh_reverse(ushort stdout, ushort stdin, ushort stderr, ushort nargs,
char *args[]){
	int size = strlen(args[1]);
	int i = 0;			
	char str[size+1]; 		
	strcpy(str, args[1]);
	char ret[size+1];

	fprintf(stdout, "\n");

	//Case for too little arguments.
	if(nargs == 1){
		fprintf(stdout, "A string is required as a command line argument.\n\n");
		fprintf(stdout, "Type 'reverse --help' for more information.\n\n");
		return SYSERR;
		exit(-1);
	}

	//Case for too many arguments.
	if(nargs > 2){
		fprintf(stdout, "Only one command line argument at a time should be used with this shell command.\n\n");
		fprintf(stdout, "Type 'reverse --help' for more information.\n\n");
		return SYSERR;
		exit(-1);
	}

	//Case for the help command.
	if(nargs == 2 && strncmp(args[1], "--help", 6) == 0){
		fprintf(stdout, "In order to properly use this shell command, type 'reverse' followed by a space and then the string you wish to reverse.\nExample: reverse abc\nThe output should be 'cba'\n\n");
		return OK;
		exit(-1);
	}
	//Reverses the user-inputted string
	for(i=size-1;i>=0;i--){
		ret[size-1-i] = str[i]; 
	}

	//Null-terminated character is added to end of reversed string.
	ret[size] = '\0';

	//The reverse string is printed.
	fprintf(stdout, "%s\n\n", ret);

	return OK;
}
