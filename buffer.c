/*************************************************************************
* Name:        bcltest.c
* Author:      Marcus Geelnard
* Description: Basic Compression Library tester.
* $Id: bcltest.c,v 1.1 2004/12/07 21:24:27 marcus256 Exp $
*
* This program can be used to test any of the compression algorithms in
* the Basic Compression Library. The program reads a file, which is
* compressed and decompressed and compared in order to detect any errors
* in the compression and/or decompression routines.
*
*-------------------------------------------------------------------------
* Copyright (c) 2003-2004 Marcus Geelnard
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
* Marcus Geelnard
* marcus.geelnard at home.se
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rle.h"
#include "huffman.h"
#include "compression.h"



/*************************************************************************
* GetFileSize()
*************************************************************************/

long GetFileSize( FILE *f )
{
    long pos, size;

    pos = ftell( f );
    fseek( f, 0, SEEK_END );
    size = ftell( f );
    fseek( f, pos, SEEK_SET );

    return size;
}


/*************************************************************************
* TestFile()
*************************************************************************/
//#pragma warning(disable : 4996).

int TestFile( char *name, int algo )
{
    unsigned int  insize, outsize, bufsize, *work, k, err_count;
    unsigned char *in, *out, *buf;
    FILE          *f;

    printf( "Testing %s...", name );

    /* Open input file */
   
    if(fopen_s(&f, name, "rb")!=0)
    {
	    printf( "unable to open!\n" );
	    return 0;
    }//else opened successfully

    /* Get input size */
    insize = GetFileSize( f );
    if( insize < 1 )
    {
	    printf( "empty file!\n" );
	    fclose( f );
	    return 0;
    }

    /* Worst case output buffer size */
    bufsize = (insize*104+50)/100 + 384;

    /* Allocate memory */
    in = (unsigned char *) malloc( insize + 2*bufsize );
    if( !in )
    {
	    printf( "out of memory!\n" );
	    fclose( f );
	    return 0;
    }

    /* Pointers to compression buffer and output memory */
    buf = &in[ insize ];
    out = &buf[ bufsize ];

    /* Read and close input file */
    fread( in, 1, insize, f );
    fclose( f );

    /* Compress and decompress */
    switch( algo )
    {
        case 1:
            outsize = RLE_Compress( in, buf, insize );
            RLE_Uncompress( buf, out, outsize );
            break;
        case 2:
            outsize = Huffman_Compress( in, buf, insize );
            Huffman_Uncompress(buf, out, outsize, insize);
            break;
        default:
            /* Should never happen... */
            outsize = 0;
            break;
    }

    /* Show compression result */
    printf( "\n  Compression: %d/%d bytes (%.1f%%)", outsize, insize,
            100*(float)outsize/(float)insize );

    /* Compare input / output data */
    err_count = 0;
    for( k = 0; k < insize; ++ k )
    {
	    if( in[ k ] != out[ k ] )
	    {
		    if( err_count == 0 ) printf( "\n" );
		    if( err_count == 30 ) printf( "    ...\n" );
		    else if( err_count < 30 )
		    {
		        printf( "    %d: %d != %d\n", k, out[ k ], in[ k ] );
	    	}
		    ++ err_count;
	    }
    }
    if( err_count == 0 )
      	printf( " - OK!\n" );
    else
    {
      	printf( "    *******************************\n" );
      	printf( "    ERROR: %d faulty bytes\n", err_count );
      	printf( "    *******************************\n" );
    }

    /* Free all memory */
    free( in );

    return (err_count == 0);
}


