#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<unistd.h>
//#include<sys/types.h>
//#include<sys/stat.h>
//#include<mqueue.h>
#include<errno.h>  

void clearContent(char const fileNameToClear[])
{
	fopen(fileNameToClear, "w");
	printf("\nContent of file %s has been erased.\n", fileNameToClear);
}

int main(int argc, char const *argv[])
{	
	// no validation if file exist.	
	for(int i = 1; i < argc; i++)
	{	
		clearContent(argv[i]);
	}
}

// b) Access multiple file names with bash:
// find ./ -type f -name "clearFileContain?.txt" -exec tee {} \; </dev/null 
