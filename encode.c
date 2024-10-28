#include <stdio.h>
#include "common.h"
#include "encode.h"
#include "types.h"
#include <string.h>
#include <unistd.h>
/* Function Definitions */

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
    
    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file (beautiful.bmp)
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        /* it will tell why the file is not opening */
    	perror("fopen");
        red();
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        reset();
    	return e_failure;
    }

    // Secret file (secret.txt)
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
        red();
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
        reset();
    	return e_failure;
    }

    // Stego Image file (output.bmp)
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
        red();
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
        reset();
    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_failure)
    {
        return e_failure;
    }
    else
    {
        print();
        printf("\n");
        green();
        printf("INFO :: Files are opened Successfully\n");
        reset();
        printf("\n");
    }
    if(check_capacity(encInfo)!= e_success)
    {
        red();
        printf("ERROR :: Encoding failed ,the image is having less capacity......\n");
        reset();
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("\nINFO :: Checking file capacity completed successfully\n");
        reset();
        printf("\n");
    }
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
        red();
        printf("ERROR :: Encoding of Src_Bmp_header is failed........\n");
        reset();
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("\nINFO :: Copy bmp header is done successfully\n");
        reset();
        printf("\n");
    }
    if(encode_magic_string(MAGIC_STRING,encInfo)==e_failure)
    {
        red();
        printf("ERROR :: Encoding of Magic_string is failed.....\n");
        reset();
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("\nINFO :: Magic String encoded successfully \n");
        reset();
        printf("\n");
    }
    if(encode_file_extn_size(strlen(encInfo -> extn_secret_file),encInfo)==e_failure)
    {
        red();
        printf("ERROR :: Encoding of secret_file_extn_size Failed......\n");
        reset();
    }
    else
    {
        print();
        green();
        printf("\nINFO :: Secret File Extension size is encoded successfully\n");
        reset();
        printf("\n");
    }
    if(encode_secret_file_extn(encInfo -> extn_secret_file,encInfo)==e_failure)
    {
        red();
        printf("ERROR :: Encode_secret_file_extn Failed..... \n");
        reset();
    }
    else
    {
        print();
        green();
        printf("\nINFO :: Secret file extension encoded successfully\n");
        reset();
        printf("\n");
    }
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_failure)
    {
        red();
        printf("ERROR :: Encode_secret_file_size Failed.....\n");
        reset();
    }
    else
    {
        print();
        green();
        printf("\nINFO :: Secret File size encoded successfully\n");
        reset();
        printf("\n");
    }
    if(encode_secret_file_data(encInfo) == e_failure)
    {
        red();
        printf("ERROR :: Encode_secret_file_data failed.....\n");
        reset();
    }
    else
    {
        print();
        green();
        printf("\nINFO :: Secret file data encoded Successfully\n");
        reset();
        printf("\n");
    }
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        red();
        printf("ERROR :: Encode_ remaining _ img _ data failed.......\n");
        reset();
    }
    else
    {
        print();
        green();
        printf("\nINFO :: Remaining source Image Data Encoded successfully\n ");
        reset();
        printf("\n");
    }
}
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}
Status check_capacity(EncodeInfo *encInfo) 
{
    //check capacity of bmp
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    //Fetching  size of the secret file
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    //Calculating the total no.of bytes in order to encode 
    int val = 54 +(strlen(MAGIC_STRING)*8) + 32 + (strlen(encInfo->extn_secret_file)*8)+32+ (encInfo->size_secret_file *8);

    //checking the image_caapacity is sufficient for emcoding or not
    if(encInfo->image_capacity >  val)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    //making offset to 0th position
    rewind(fptr_src_image);
    // copying the header file from src image to buffer 
    fread(buffer,54,1,fptr_src_image);
    //printing the header file inthe destination file
    fwrite(buffer,54,1,fptr_dest_image);
    return e_success;
}
//Function to encode the magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}
//Function to encode each character of magic string into the image
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    //declare buffer
    char image_buffer[8];
    //rewind(fptr_src_image);
    //fseek(fptr_src_image,54,SEEK_SET);
    for(int i=0;i<size;i++)
    {
        //read 8bytes from src_image
        fread(image_buffer,8,1,fptr_src_image);
        //call byte to lsb func (data[0],image_buffer)
        encode_byte_to_lsb(data[i],image_buffer);
        //printing in the stego image
        fwrite(image_buffer,8,1,fptr_stego_image);
    } 
    return e_success;
}
//Function to replace each bit of magic string to lsb bit of image
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    //clear the LSB bit image_buffer
    //get the MSB bit in size
    //set the bit in lsb
    for(int i=0;i<8;i++)
    {
        if(data & (1<<7-i))
        {
            image_buffer[i]= (image_buffer[i] & ~(1) | 1);
        }
        else
        {
            image_buffer[i]=(image_buffer[i] & ~(1) | 0);
        }
    }
}

Status encode_file_extn_size(int size,EncodeInfo *encInfo)
{
   char image_buffer[size*8];
   //read the next 32 bytes from source image
   fread(image_buffer,32,1,encInfo -> fptr_src_image);
   //call size_to_lsb function
   encode_size_to_lsb(size,image_buffer);
   /* write the 32 bytes to stego image */
   fwrite(image_buffer,32,1,encInfo -> fptr_stego_image); 
   return e_success;
}
Status encode_size_to_lsb(int size,char* image_buffer)
{
    //clear the LSB bit image_buffer
    //get the MSB bit in size
    //set the bit in lsb
    for(int i=0;i<32;i++)
    {
        if(size & (1<<31-i))
        {
            image_buffer[i]= (image_buffer[i] & ~(1) | 1);
        }
        else
        {
            image_buffer[i]=(image_buffer[i] & ~(1) | 0);
        }
    }
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    //call data to image
   encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
   return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
   char image_buffer[file_size*8];
   
   fread(image_buffer,32,1,encInfo -> fptr_src_image);
   encode_size_to_lsb(file_size,image_buffer);
   fwrite(image_buffer,32,1,encInfo -> fptr_stego_image); 
   return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo -> fptr_secret);
	char str[encInfo -> size_secret_file];
	fread(str, encInfo -> size_secret_file,sizeof(char), encInfo -> fptr_secret);
	encode_data_to_image(str, encInfo -> size_secret_file, encInfo-> fptr_src_image, encInfo->fptr_stego_image);
	return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	char ch[1];
	while(fread(ch,1,1,fptr_src) > 0)
	{
		fwrite(ch, 1, 1,fptr_dest);
	}
	return e_success;
}
void print()
{
    blue();
    printf("Loading");
    reset();
    sleep(1); 
    for(int i=0;i<20;i++)
    {
       printf(".");
        fflush(stdout);  // Force the output to appear immediately
        usleep(6000);        // 1-second delay
    }
}