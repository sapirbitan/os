#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
//defining the size of elements we want to read from a file
#define SIZE 1
#define MAXSIZE 100

void grade(int ret,char* current_student_file) {
	if (ret == 1)//our output and the student's output are the same
		printf("%s0\n", current_student_file);
	if (ret == 2)//the outputs are diffrent
		printf("%s100\n", current_student_file);
}

int main(int argc, char* argv[]){
	//variables:
	int fd_users, fd_input, output;//file descriptors
	int pid, status, i, ret;
	int read_users = 1;
	char current_name[SIZE + 1];
	char current_student_file[MAXSIZE + 1];

	fd_users = open(argv[1], O_RDONLY);//open the users file
	if (fd_users == -1) { printf("ERROR"); exit(-1); }//couldn't open file
	read_users = read(fd_users, current_name, SIZE);
	if (read_users == -1) { printf("ERROR"); close(fd_users); exit(-1); }//if couldn't read file, close the users file and exit the program
	while (read_users>0){//we haven't finished reading all of the user's name
		i = 0; //defining counter
		while (current_name[0] != '\n'){ //we haven't finished reading the user's name('\n' indicates we've finished reading the current name
			current_student_file[i] = current_name[0];
			read_users = read(fd_users, current_name, SIZE);
			if (read_users == -1) { printf("ERROR"); close(fd_users); exit(-1); }//if couldn't read file, close the users file and exit the program
			i++;
		}
		//in order to compile each student's file add a '.c' to the student's name
		current_student_file[i] = '.';
		if (current_student_file[0] == '.')//means there was an error - and that no name was taken out of the array
			break;
		current_student_file[i + 1] = 'c';
		current_student_file[i + 2] = '\0';
		//"getting rid" of the '\n' 'current_name' now contains '\n' and we need the letter after that - the next name's first letter
		read_users = read(fd_users, current_name, SIZE);
		if (read_users == -1) { printf("ERROR"); close(fd_users); exit(-1); }//if couldn't read file, close the users file and exit the program
		fd_input = open(argv[2], O_RDONLY);//oppening the inputs list
		if (fd_input == -1) { printf("ERROR"); exit(-1); close(fd_users); }//couldn't open file
		output = open("output2.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (output == -1) { printf("ERROR"); exit(-1); close(fd_users);close(fd_input); }//couldn't open file
		if (pid = fork() == 0)
			execlp("gcc", "gcc", current_student_file, "-o", "buff.out", NULL);//compile student's code
		waitpid(pid, &status, NULL);
		ret = WEXITSTATUS(status);
		if (ret != 0){//if there was an error - print: student's name: Compilation error
			current_student_file[i] = ':';
			current_student_file[i + 1] = ' ';
			printf("%sCompilation error\n", current_student_file);
		}
		else{
			if (pid = fork() == 0){
				dup2(fd_input, 0);
				dup2(output, 1);
				close(output);
				execlp("./buff.out", "./buff.out", NULL);
			}
			waitpid(pid, &status, NULL);
			ret = WEXITSTATUS(status);
			if (pid = fork() == 0)
				execlp("gcc", "gcc", "comp.c", "-o", "comp.out", NULL);//compile comp
			waitpid(pid, &status, NULL);
			if (pid = fork() == 0)
				execl("./comp.out", "./comp.out", "output.txt", "output2.txt", NULL);//run
			waitpid(pid, &status, NULL);
			ret = WEXITSTATUS(status);
			close(output);
			current_student_file[i] = ':';
			current_student_file[i + 1] = ' ';
			grade(ret, current_student_file);
		}
	}
	//close all files
	close(fd_users);
	close(fd_input);
	return 0;
}



