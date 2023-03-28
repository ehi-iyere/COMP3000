/* 3000test.c */
/* v1 Oct. 1, 2017 */
/* Licenced under the GPLv3, copyright Anil Somayaji */
/* You really shouldn't be incorporating parts of this in any other code,
   it is meant for teaching, not production */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void report_error(char *error)
{
        fprintf(stderr, "Error: %s\n", error);

        exit(-1);
}

int main(int argc, char *argv[])
{
        struct stat statbuf;
        char *fn;	
        int fd;
        size_t len, i, count;
        
        
	

        if (argc < 2) {
                if (argc < 1) {
                        report_error("no command line");
                        fprintf(stderr, "Usage: %s <file>\n", argv[0]); 
                } else {
                        report_error("Not enough arguments");
                        fprintf(stderr, "Usage: %s <file>\n", argv[0]); 
                }
        }

        fn = argv[1];

        if (stat(fn, &statbuf)) {
                report_error(strerror(errno));
        }
	
	
	
        len = statbuf.st_size;

	char *buf =  malloc(len * sizeof(char));

	
        printf("File %s: \n", fn);
        printf("\tinode %ld\n", statbuf.st_ino);
        printf("\tlength %ld\n", len);        
        if (S_ISREG(statbuf.st_mode)) {
                fd = open(fn, O_RDONLY);
                if (fd == -1) {
                        report_error(strerror(errno));
                }
	}
		
	
	read(fd, buf, len);
	buf[len] ='\0';
	count = 0;

        close(fd);
	//printf("%s\n",buf);
	for(i=0;i<len;i++){
		if(buf[i]=='a'){
			count++;
		}
	}
	printf("\ta count %ld\n", count);
	free(buf);

        return 0;
}
