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
#include "common.h"
#include "types.h"
#include<unistd.h>
/* Function Definitions */
/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    // printf("size of secret file =%ld\n",encInfo -> size_secret_file);

    if(encInfo -> image_capacity > (2 + 4 + 4 + encInfo -> size_secret_file +encInfo -> size_secret_file) * 8)
	return e_success;
}
//------------------------------------------------------------------------------//
/* Get file size */
uint get_file_size(FILE *fptr)
{
    fseek (fptr, 0 ,SEEK_END);
    return ftell(fptr);
}
//---------------//-----------------------------------------------------------//
/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{    
    rewind(fptr_src_image);
    char arr[54];
    fread(arr,54,1,fptr_src_image);
    fwrite(arr,54,1,fptr_dest_image);
    return e_success;
}
//-----------------------------------------------------------------------------//
/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(MAGIC_STRING, strlen(MAGIC_STRING), encInfo ->fptr_src_image, encInfo ->fptr_stego_image);
}
//----------------------------------------------------------------------------//
/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];
    for(int i=0;i<size;i++)
    {
	fread(image_buffer,sizeof(char),8,fptr_src_image);
	encode_byte_to_lsb (image_buffer,data[i]);       //function call of encode_byte_to_lsb 
	fwrite(image_buffer,sizeof(char),8,fptr_stego_image);
    }
    return e_success;
}
//-------------------------------------------------------------------------------//
/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char *buffer,char data)
{
    for(int i=0;i<8;i++)
    {
	buffer[i]=((data & (1<<(7 -i))) >> (7-i) ) | (buffer[i] & ~1);
    }
}
//--------------------------------------------------------------------------------//
/* Encode a size into LSB of image data array */
Status encode_size_to_lsb(char *buffer,int data)
{
    for(int i=0;i<32;i++)
    {
	buffer[i]=((data & (1<<(31 -i))) >> (31-i) )| (buffer[i] & ~1);
    }
    return e_success;
}
// ------------------------------------------------------------------------------//
/* Encode secret file size */
Status encode_secret_file_extn_size(int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
    char buffer[32];
    fread(buffer,32,1,fptr_src_image);
    encode_size_to_lsb(buffer,size);      //function call of encode_size_to_lsb
    fwrite(buffer,32,1,fptr_stego_image);
    return e_success;
}
//-------------------------------------------------------------------------------//
/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn , EncodeInfo *encInfo)
{
    if(encode_data_to_image( file_extn, strlen(file_extn), encInfo-> fptr_src_image, encInfo -> fptr_stego_image) == e_success)        //function call of encode_data_to_image
    {
	return e_success;
    }
}
//-------------------------------------------------------------------------------//
/* Encode secret file size */
Status encode_secret_file_size( long file_size , EncodeInfo *encInfo)
{
    encode_secret_file_extn_size(file_size , encInfo -> fptr_src_image , encInfo -> fptr_stego_image);  //function call of encode_secret_file_size
    return e_success;
}
//-------------------------------------------------------------------------------//
/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char arr[encInfo -> size_secret_file];
    rewind(encInfo -> fptr_secret);
    fread(arr, 1, encInfo -> size_secret_file , encInfo -> fptr_secret);
    encode_data_to_image(arr, strlen(arr), encInfo -> fptr_src_image, encInfo -> fptr_stego_image);  //function call of encode_data_to_image

    return e_success;
}
//------------------------------------------------------------------------------//
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data( FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,sizeof(char),1,fptr_src) > 0)
	fwrite(&ch,1,1,fptr_dest);
    return e_success;
}

//------------------------------------------------------------------------------//
/* Read and validate Encode args from argv */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strcmp(strstr(argv[2],"."),".bmp") == 0)
	encInfo->src_image_fname = argv[2];
    else
    {
	printf("pass correct extension\n");
	return e_failure;
    }
    if((strcmp(strstr(argv[3],"."),".txt") == 0) || (strcmp(strstr(argv[3],"."),".c") == 0))
	encInfo->secret_fname = argv[3];
    else
    {
	printf("pass correct extension\n");
	return e_failure;
    }
    if (argv[4] != NULL)
    {
	if(strcmp(strstr(argv[4],"."),".bmp") == 0)
	    encInfo->stego_image_fname = argv[4];
	else
	{
	    printf("pass correct extension\n");
	    return e_failure;
	}
    }
    else
	encInfo->stego_image_fname = "output.bmp";
    return e_success;
}
//-----------------------------------------------------------------------//
/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
	printf("\n INFO : Successfully opened files\n");
	sleep(1);
	if( check_capacity(encInfo) == e_success)
	{
	    printf("\n INFO : Encoding is possible\n");
	    sleep(1);
	    if (copy_bmp_header(encInfo-> fptr_src_image, encInfo-> fptr_stego_image) == e_success)
	    {
		printf("\n INFO :Bmp header copied successfully\n");
		sleep(1);

		if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
		{
		    printf("\n INFO : Magic string copied successfully\n");
		    sleep(1);
		    if((encode_secret_file_extn_size((strlen(strstr(encInfo-> secret_fname,"."))),encInfo-> fptr_src_image, encInfo-> fptr_stego_image) == e_success))
		    {
			printf("\n INFO : Encoding secret file extension size successfully\n");
			sleep(1);
			if(encode_secret_file_extn(strstr (encInfo->secret_fname ,"."),encInfo) == e_success)
			{ 
			    printf("\n INFO : Encoding secret file extension successfully\n");
			    sleep(1);
			    if(encode_secret_file_size( encInfo -> size_secret_file , encInfo) == e_success)
			    {
				printf("\n INFO : Encoding of secret file size successfully\n");
				sleep(1);
				if(encode_secret_file_data(encInfo) == e_success)
				{
				    printf("\n INFO : Encoding of secret file data successfully\n");
				    sleep(1);
				    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
				    {
					printf("\n INFO : Encoding of remaining data from secrect file successfully\n");
					sleep(1);


				    }

				    else
				    {
					printf("\nEncoding of remaining data from secret file is failed\n");
				    }
				}

				else
				{
				    printf("Encoding of secret file data failed\n");
				}
			    }

			    else
			    {
				printf("Failed in encoding of secret file size\n");
			    }
			}

			else
			{
			    printf("Encoding secret file extension failed");
			}
		    }
		    else
		    {
			printf("Encoding secret file size failed\n");
		    }
		}
		else
		{
		    printf("encoding magic string is failed\n");
		}
	    }
	    else
	    {
		printf("Bmp is header is not copied\n");
	    } 
	}
	else
	{
	    printf("Encoding is not possible\n");
	    return e_failure;
	}
    }
    else
    {
	printf("Failed in  opening  files\n");
    }
    return e_success;
}
//---------------------------------------------------------------------------//
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf(" width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf(" height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}
//-----------------------------------------------------------------------------//
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
