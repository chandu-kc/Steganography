/*
NAME: CHANDAN K C
DATE: 08/10/2023
DESCRIPTION: Stegnography
SAMPLE INPUT:
SAMPLE OUTPUT:
 */

#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include <unistd.h>

int main(int argc,char *argv[])
{
    // int ret=check_operation_type(argv);
    if(argc == 1)
    {
	printf("Error : Please pass valid arguments \n"
		"For Encoding Pass : ./a.out -e beautiful.bmp secret.txt output.bmp\n"
		"For Decoding Pass : ./a.out -d output.bmp\n");
    }
    else if(check_operation_type(argv) == e_encode)
    {
	//	int ret=check_operation_type(argv);
	// if(ret == e_encode)
	if(argc ==2  || argc ==3)
	{
	    printf("Error : Please Pass valid arguments \n"
		    "For Encoding Pass : ./a.out -e beautiful.bmp secret.txt output.bmp\n");
	}

	else
	{
	    printf(" INFO :  Selected encoding\n");
	    printf("\n//------------------------------------------------------------//\n");
	    EncodeInfo encInfo;
	    if(read_and_validate_encode_args(argv, &encInfo) == e_success)
	    {
		printf("\n INFO : Read and validation completed successfully\n");
		sleep(1);

		if( do_encoding(&encInfo) == e_success)
		{
		    printf("\nEncoded successfully\n");
		}
		else
		{
		    printf("Encoding is not done\n");
		}
	    }
	    else
	    {
		printf("Failed to read and validate\n");
	    }
	}
	return 1;		

    }


    else if(check_operation_type(argv) == e_decode)
	//	else if(ret == e_decode)
    {
	if(argc == 2)
	{
	    printf("Error : Please pass valid arguments \n"
		    "For Decoding Pass : ./a.out -d output.bmp\n");
	}
	else
	{
		DecodeInfo decInfo;
		printf("selected decoding\n");
		printf("\n//----------------------------------------------------------//\n");
		if(read_and_validate_decode_args(argv, &decInfo) == e_success)
		{ 
		    printf("\nRead and Validation and completed successfully\n");
		   if( do_decoding(&decInfo) == e_success)
		   {

		    printf("\nDecoding done Succesfully\n");
		   }
		   else
		   {
		       printf("Decoding is not dne\n");
		   }
		

	    }
	else
	    {
		printf("Failed read and validate\n");
	    }
	}

    }
    else
    {
	printf("Unsupported format\n"
		"For Encoding Pass : ./a.out -e beautiful.bmp secret.txt output.bmp\n"
		"For Decoding Pass : ./a.out -d output.bmp\n");
	sleep(1);
    } 
    return 0;
}


OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1] ,"-e") == 0 )
	return e_encode;
    else if(strcmp(argv[1],"-d" ) == 0)
	return e_decode;
    else
	return e_unsupported;
}

