#include <stdio.h>
#include <fcntl.h>
#define MAX_NUM_FILES 3
//checks if there was a problem reading the file
int check_read(int read_stat, int fd_file1, int fd_file2){
	if (read_stat == -1) {
		printf("Error! Can't open the file");
		close(fd_file1);
		close(fd_file2);
		return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
	int buf1 = 0;
	int buf2 = 0;

	if (argc != MAX_NUM_FILES){//if fewer than 2 files were sent
		printf("1\n");
		return 1;
	}
	int fd_file1 = open(argv[1], O_RDONLY);//getting the first file's fd
	if (fd_file1 == -1) {
		printf("Error! can't open the first file\n");
		return 1;
	}
	int fd_file2 = open(argv[2], O_RDONLY);//getting the second file's fd
	if (fd_file1 == -1) {
		printf("Error! can't open the second file\n");
		close(fd_file1);//file1 is open, so...
		return 1;
	}
	int read_stat1 = read(fd_file1, &buf1, 1);//getting a bit from txt1
	if (check_read(read_stat1, fd_file1, fd_file2) == 1){
		printf("1\n");
		return 1;
	}
	int read_stat2 = read(fd_file2, &buf2, 1);//getting a bit from txt2
	if (check_read(read_stat2, fd_file1, fd_file2) == 1){
		printf("1\n");
		return 1;
	}
	while (read_stat1 != 0) {
		if (buf1 != buf2) {
			printf("1\n");
			close(fd_file1);
			close(fd_file2);
			return 1;
		}
		read_stat1 = read(fd_file1, &buf1, 1);
		if (check_read(read_stat1, fd_file1, fd_file2) == 1){
			printf("1\n");
			return 1;
		}
		read_stat2 = read(fd_file2, &buf2, 1);
		if (check_read(read_stat2, fd_file1, fd_file2) == 1){
			printf("1\n");
			return 1;
		}
	}
	//checking that file 2 isn't longer than file 1
	read_stat2 = read(fd_file1,&buf2, 1);
	if (read_stat2 != 0) {//file 2 is longer then file 1
		printf("1\n");
		close(fd_file1);
		close(fd_file2);
		return 1;
	}
	//the files are the same
	printf("2\n");
	close(fd_file1);
	close(fd_file2);
	return 2;
}
