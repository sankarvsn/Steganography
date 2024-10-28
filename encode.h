#ifndef ENCODE_H
#define ENCODE_H

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

typedef struct _EncodeInfo
{
    /* Source Image info */
    // to store the source image file name [beautiful.bmp]
    char *src_image_fname;
    // to store the address of the image
    FILE *fptr_src_image;
    
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];
    

    /* Secret File Info */
    /* To store the name of the secret file name*/
    char *secret_fname;
    /* to store the base address of the secret file*/
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX]; //to store extension size
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file; //to store the size of secret file
    

    /* Stego Image Info */
    /* to store the name of the output.bmp*/
    char *stego_image_fname;
    /* to store the address of the output.bmp*/
    FILE *fptr_stego_image;

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding (GEneric function)*/ 
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/* Encode secret file extension */
Status encode_file_extn_size(int extn_size,EncodeInfo *encInfo);

/* */
Status encode_size_to_lsb(int extn_size,char* image_buffer);
void red () ;

void green();

void yellow ();

void blue ();

void cyan();

/* Function to reset text color to default */
void reset ();

void print();


void print();
#endif
