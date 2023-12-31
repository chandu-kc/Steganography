/*
NAME: CHANDAN K C
DATE: 08/10/2023
DESCRIPTION: Stegnography
SAMPLE INPUT:
SAMPLE OUTPUT:
 */
#include<stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include "encode.h"
#include<unistd.h>
/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo )
{
    if((strcmp (strstr(argv[2] ,"."),".bmp")) == 0)
    {
	decInfo -> src_image_fname = argv[2];
    }
    else
    {
	printf("Please enter .bmp extension file\n");
	return e_failure;
    }
    if(argv[3] != NULL)
    {
	strcpy( decInfo -> secret_fname ,argv[3]);
    }
    else
    {
	strcpy( decInfo -> secret_fname, "decode");
    }
    return e_success;
}
//-----------------------------------------------------------------------//
/* Get File pointers for i/p and o/p files */
Status dec_open_files(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
//--------------------------------------------------------------------------//
/* Store Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo ->fptr_src_image,54,SEEK_SET);
    char data[strlen(magic_string)];
    //fseek(decInfo ->fptr_src_image,54,SEEK_SET);
    decode_data_to_image(data,strlen(magic_string),decInfo ->fptr_src_image,decInfo);  //function call of decode_data_to_image
    if(strcmp(magic_string,data) ==0)
	return e_success;
    else
	return e_failure;
}
//-------------------------------------------------------------------------------//
/* Decode function, which does the real decoding */
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image, DecodeInfo *decInfo)
{
    int i;
    char image_buffer[8];
    for(i=0;i<size;i++)
    {
	fread(image_buffer,8,1,fptr_src_image);
	data[i] = (decode_lsb_to_byte(data[i],image_buffer));
    }
    data[i]= '\0';
    return e_success;
}
//------------------------------------------------------------------------------//
/* Decode a LSB into byte image data array */
Status decode_lsb_to_byte(char data, char *image_buffer)
{
    char ch=0x00;
    for(int i=0;i<8;i++)
    {
	ch = (((image_buffer[i] & 1) << (7-i)) | ch);
    }
    return ch;
}
//---------------------------------------------------------------------------//
/* Decode secret file extension size */
Status decode_size_to_lsb(char *buffer, int *data)
{
    *data = 0;
    for(int i=0; i<32;i++)
    {
	*data = (((buffer[i] & 1) << (31-i)) | *data);
    }
    return e_success;
}
//-----------------------------------------------------------------------//
/* Decode secret file extn  size */
Status decode_secret_file_extn_size(int *size, FILE *fptr_src_image)
{
    char buffer[32];
    fread(buffer,32,1,fptr_src_image);
    decode_size_to_lsb(buffer, size);          //function call of decode_size_to_lsb
    return e_success;
}

//---------------------------------------------------------------------//
/* Encode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extn[decInfo -> size_secret_file_extn];
    if(decode_data_to_image(extn,decInfo -> size_secret_file_extn, decInfo -> fptr_src_image, decInfo) == e_success)  //function call of decode_data_to_image
    {
	strcat(decInfo -> secret_fname, extn);
	decInfo -> fptr_secret = fopen(decInfo -> secret_fname, "w");
	return e_success;
    }
}
//------------------------------------------------------------------//
/* Decode secret file size */

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_secret_file_extn_size(&(decInfo -> decode_size_secret_file), decInfo -> fptr_src_image);
    return e_success;
}
//----------------------------------------------------------------//
/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char data[decInfo -> decode_size_secret_file];
    decode_data_to_image(data, decInfo -> decode_size_secret_file, decInfo -> fptr_src_image, decInfo);
    fprintf(decInfo -> fptr_secret, "%s", data);
    return e_success;
}
//-------------------------------------------------------------------//
/* Perform the decoding */

Status do_decoding(DecodeInfo * decInfo)
{
    if(dec_open_files(decInfo) == e_success)
    {
	printf("\nINFO : Successfully opened decoding files\n");
	sleep(1);
	if(decode_magic_string( MAGIC_STRING , decInfo ) == e_success)
	{
	    printf("\nINFO : Successfully decoded magic string\n");
	    sleep(1);
	    if(decode_secret_file_extn_size( &(decInfo -> size_secret_file_extn), decInfo -> fptr_src_image) == e_success)
	    {
		printf("\nINFO : Successfully decoded secret file extension size \n");
		sleep(1);
		if(decode_secret_file_extn(decInfo) == e_success )
		{
		    printf("\nINFO : Successfully decoded secret file extension\n");
		    sleep(1);
		    if(decode_secret_file_size(decInfo) == e_success)
		    {
			printf("\nINFO : Successfully decoded secret file size\n");
			sleep(1);
			if(decode_secret_file_data(decInfo) == e_success)
			{
			    printf("\nINFO : Successfully Decoded secret File data\n");
			}
		    }
		}
	    }
	}
    }
    return e_success;
}

