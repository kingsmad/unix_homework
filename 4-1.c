/*************************************************************************
    > File Name: 4-1.c
    > Author: Archer Liu
    > Mail: maple.km2041@me.com 
    > Created Time: Fri Apr  3 14:56:55 2015
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/file.h>

#define BUF_SIZ 1024

struct tee_args_t {
    const char* outfilename; 
    int append_flag;
} global_args;

int main(int argc, char** argv) {
    int opt = 0;
    memset(&global_args, 0, sizeof(global_args));
    
    while((opt = getopt(argc, argv, "ao:")) != -1 ){
	switch (opt) {
	    case 'a':
		global_args.append_flag = 1;
		break;
	    case 'o':
		global_args.outfilename = optarg;
		break;
	    default:
		break;
	}
    }
    
    printf("\n%s", global_args.outfilename);
    printf("\nappend = %d\n", global_args.append_flag);
    
    /* Start to work as tee */
    char* buf = (char*) malloc (BUF_SIZ);
    int out_fd = open(global_args.outfilename, \
		 O_RDWR | O_CREAT, DEFFILEMODE); 
    if(out_fd == -1) {
	printf("errno is: %d", errno);
	printf("\nCould not open file %s", global_args.outfilename);
    }
    if(global_args.append_flag) {
	int flag = fcntl(out_fd, F_GETFL);
	flag = flag | O_APPEND;
	if(fcntl(out_fd, F_SETFL, flag) == -1)
	    printf("\nCannot set\n");
    }
    ssize_t cnt = 0; 
    while((cnt = read(0, buf, BUF_SIZ)) > 0) {
	cnt = write(out_fd, buf, cnt); 		
	printf("\nSuccessfully write %ld bytes\n", cnt);
    }

    int offset = lseek(out_fd, 0, SEEK_CUR); 
    ftruncate(out_fd, offset);
    close(out_fd);

    return 0;
}

