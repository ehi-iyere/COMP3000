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

void sparsecheck(const char *path){

	int filesize, blocksize, numblock;
	struct stat buf;
	

	if(path==NULL){return;}

	if(stat(path, &buf)){return;}

	if(!((S_ISLNK(buf.st_mode))||(S_ISREG(buf.st_mode)))){return ;}

	filesize= buf.st_size;
	blocksize= buf.st_blksize;
	numblock=buf.st_blocks;

	if(filesize>(blocksize*numblock)){
	printf("\tThis file has holes\n");
	}
	else{
	  printf("\tThis file does not have holes\n");
	}


}
int main(int argc, char *argv[])
{
        struct stat statbuf;
        char *fn;
        int fd;
        size_t len, i, count;
        
        char *data;

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
        printf("File %s: \n", fn);
        printf("\tinode %ld\n", statbuf.st_ino);
        printf("\tlength %ld\n", len);        

        if (S_ISREG(statbuf.st_mode)) {
                fd = open(fn, O_RDONLY);
                if (fd == -1) {
                        report_error(strerror(errno));
                }
                data = (char *) mmap(NULL, len,
                                     PROT_READ, MAP_SHARED, fd, 0);
                if (data == MAP_FAILED) {
                        report_error(strerror(errno));
                }

                count = 0;
                for (i=0; i<len; i++) {
                        if (data[i] == 'a') {
                                count++;
                        }
                }

                printf("\ta count %ld\n", count);

                if (munmap(data, len) == -1) {
                        report_error(strerror(errno));                        
                }
                close(fd);
        }
	printf("\t----using sparse check-----\n");
	sparsecheck(fn);

        return 0;
}
