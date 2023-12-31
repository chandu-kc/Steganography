/*
NAME: CHANDAN K C
DATE: 08/10/2023
DESCRIPTION: Stegnography
SAMPLE INPUT:
SAMPLE OUTPUT:
 */

#include<stdio.h>
#include"types.h"
//#include "encode.h"
typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;

    /* Secret file Info*/
    char secret_fname[20];
    FILE *fptr_secret;
    char *secret_file_extn;
    int size_secret_file_extn;
    int decode_size_secret_file;
}DecodeInfo;

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status dec_open_files(DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *dencInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
 Status decode_secret_file_data(DecodeInfo *decInfo);
 
/* Decode function, which does the real decoding */
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image, DecodeInfo *decInfo);

/* Decode a LSB into byte image data array */
Status decode_lsb_to_byte(char data, char *image_buffer);

/* Decode secret file extension size */
Status decode_size_to_lsb(char *buffer, int *data);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(int *size, FILE *fptr_src_image);



