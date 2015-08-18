
#include <stdio.h>
#include <malloc.h>

int main( int argc, char **argv) {
    
    if (argc!=5) {
        fprintf( stderr , "Sytnax: xsize ysize stagger string\r\n");
        return(-1);
    }
    
    int xsize = atoi( argv[1] );
    int ysize = atoi( argv[2] );
	
    int stagger = atoi( argv[3] );
	
	int len = xsize*ysize*3;	// 3 bytes per pixel
	
	printf( "%c%c" , 0x00, 0x00 );		// Channel=0 , Command=0
	printf("%c%c" , (len>>8) , len & 0xff );  // High byte/low byte of data length

    char command = argv[4][0];

	char *pos = argv[4]+1;

	int r,g,b;

	sscanf( pos , "%2x", &r);
	pos +=2;
	sscanf( pos , "%2x", &g);
	pos +=2;
	sscanf( pos , "%2x", &b);
	pos +=2;
	
	
    switch (command) {

		case 'C': {			// Straight color display

	
			int loopy=ysize;

			while (loopy--) {

				int loopx = xsize;
				while (loopx--) {
					   
					   printf("%c%c%c", r ,  g, b);

				}
			}
		}
		break;


        case 'W': {
        
		
            unsigned int marginx = (xsize /4)+5 ;
        
        
            int pin;
            for(pin=0;pin<5;pin++) {
               int string;
               for(string=0;string<6;string++) {
                int x;
                for( x=0;x<xsize;x++) {
                    if ( x>=marginx && x<(xsize-marginx) ){
                           printf("%c%c%c", r ,  g, b);
                        } else {
                            printf("%c%c%c",0x00,0x00,0x00);
                        }
                    }
                }
            }
		}
		break;


		
		case 'R': {		// Reduced 1/2 area
		
			unsigned int marginx = (xsize /4)+2 ;
			unsigned int marginy = ysize /4 ;
            
            unsigned long *buffer=malloc( 97 * 7 * 3 );

            unsigned long rgb= ((unsigned long) r << 16) | (g << 8) | b ;
            
            
            int y;
            for( y=0;y<ysize;y++) {
                
                int x;
                for( x=0;x<xsize;x++) {

					if ( ( y >= marginy && y < (y-marginy) ) && ( x>=marginx && x<(x-marginx) ) ){
					
                       buffer[ x + (y*ysize)] = rgb; 
					   printf("%c%c%c", r ,  g, b);
					
                    } else {
					
                        buffer[ x + (y*ysize)] = 0x00;
						//Otherwise send black in the margin
					}


                }
            }
            
            // now send it.
            // row #4 first, then row 9 (4+5 spacing), then row 14... then row #3, row #8...
		
            int pin;
            for( pin = 0; pin<5; pin++ ) {
            
                int string;
                for ( string=0; string<5 ; string++ ) {

                    y = (4-pin) * (5*string);;
                    
                    int x;
                    for( x=0; x<97; x++ ) {
                    
                        printf( "%.6lx" , buffer[x+(y*ysize)] );
                    
                    }
                
                }
            }
        }
      
	}
	
    return(0);
    
}
