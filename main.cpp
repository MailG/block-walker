#include "readmtf.h"
#include <iostream>

//TODO add ntbackup file here 

int main(int argc, char *argv[])
{
	if ( argc == 2 ){
		readmtf(argv[1]);
	} else {
		printf("usage : %s <mtf-file>\n", argv[0]);
	}
	return 0;
}
