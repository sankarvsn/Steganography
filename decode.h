#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    //Magic String
    char magic_str[20];
    /* Source Image info */
    // to store the source image file name [beautiful.bmp]
    char *stego_image_fname;
    // to store the address of the image
    FILE *fptr_stego_image;
    int extn_size; //size of the extension (.txt)

    char extn_secret_file[MAX_FILE_SUFFIX]; //to store extension size
    long size_secret_file; //to store the size of secret file
    char secret_data[MAX_SECRET_BUF_SIZE];
    char image_data[MAX_IMAGE_BUF_SIZE];
    

    /* Output File Info */
    /* To store the name of the secret file name*/
    char *output_fname;
    /* to store the base address of the secret file*/
    FILE *fptr_output;

} DecodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status Open_files(DecodeInfo *decInfo);

/* check capacity 
Status check_capacity(EncodeInfo *decInfo);

/* Get image size 
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size 
uint get_file_size(FILE *fptr);


/* Copy bmp image header 
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);
*/

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn( DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size( DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode function, which does the real encoding (GEneric function)*/ 
Status decode_image_to_data(FILE *fptr_stego_image,char *data, int size);

/* Encode a byte into LSB of image data array */
Status decode_lsb_to_byte(char *image_buffer);


/* Encode secret file extension */
Status decode_file_extn_size(DecodeInfo *decInfo);

/* */
int decode_lsb_to_size(char* image_buffer);

void red () ;

void green();

void yellow ();

void blue ();

void cyan();

/* Function to reset text color to default */
void reset ();

void print();

void Print();
#endif
