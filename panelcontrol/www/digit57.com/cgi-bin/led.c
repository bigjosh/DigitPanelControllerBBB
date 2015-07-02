
#include <stdio.h>

int main( int argc, char **argv) {
    
    if (argc!=4) {
        fprintf( stderr , "Sytnax: strrep count string\r\n");
        return(-1);
    }
    
    int x = atoi( argv[1] );
    int y = atoi( argv[2] );
    char command = argv[3][0];

    switch (command) {

	case 'C': {

		int len = x*y*3;	// 3 bytes per pixel

		char *pos = argv[3]+1;

		int r,g,b;

		sscanf( pos , "%2x", &r);
		pos +=2;
		sscanf( pos , "%2x", &g);
		pos +=2;
		sscanf( pos , "%2x", &b);
		pos +=2;

		printf( "%c%c" , 0x00, 0x00 );		// Channel=0 , Command=0
		printf("%c%c" , (len>>8) , len & 0xff );  // High byte/low byte of daat length

		int loopy=y;

 		while (loopy--) {

		 int loopx = x;
		 while (loopx--) {
                   
                   printf("%c%c%c", r ,  g, b);

		 }
                }

           }
           break;


    }
    return(0);
    
}
