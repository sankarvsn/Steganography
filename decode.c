#include <stdio.h>
#include "common.h"
#include "decode.h"
#include "types.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/* Function Definitions */
void red () {
  printf("\033[1;31m");
}
void green()
{
    printf("\033[0;32m");
}
void yellow (){
  printf("\033[1;33m");
}

void blue ()
{
    printf("\033[0;34m ");
}

void cyan()
{
    printf("\033[0;36m ");
}
/* Function to reset text color to default */
void reset () {
  printf("\033[0m");
}

Status Open_files(DecodeInfo *decInfo)
{
    
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        /* it will tell why the file is not opening */
    	perror("fopen");
        red();
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        reset();
    	return e_failure;
    }
    if(strstr(decInfo -> output_fname,".txt") != NULL)
    // Secret file (secret.txt)
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    // Do Error handling
    if (decInfo->fptr_output == NULL)
    {
    	perror("fopen");
        red();
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);
        reset();
    	return e_failure;
    }
    // No failure return e_success
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo) {
    //call the open files function
    if (Open_files(decInfo) == e_failure) {
        return e_failure;
    }
    else{
    print();
    green();
    printf("\nINFO :: Files are opened Successfully\n");
    reset();
    printf("\n");
    }
    if (decode_magic_string(decInfo) != e_success) 
    {
        red();
        printf("ERROR :: Encoding of Magic_string is failed.....\n");
        reset();
        return e_failure;
    }
    else{
    print();
    green();
    printf("\nINFO :: Magic String decoded successfully \n");
    reset();
    printf("\n");
    }
    if(decode_file_extn_size(decInfo)!=e_success)
    {
        red();
        printf("ERROR :: FILE Extension is not decoded \n");
        reset();
    }
    else{
        print();
        green();
        printf("\nINFO :: Extension size decoded successfully\n");
        reset();
        printf("\n");
    }
    if(decode_secret_file_extn(decInfo)!=e_success)
    {
        red();
        printf("ERROR :: Secret_File Extension is not decoded\n");
        reset();
    }
    else{
    print();
    green();
    printf("\nINFO :: Secret File Extension decoded successfully \n");
    reset();
    printf("\n");
    }
    if(decode_secret_file_size(decInfo)!= e_success)
    {
        red();
        printf("ERROR :: Secret_file_size is not decoded\n");
        reset();
    }
    else{
    print();
    green();
    printf("\nINFO :: Secret File size decoded successfully \n");
    reset();
    printf("\n");
    }
    if(decode_secret_file_data(decInfo)!=e_success)
    {
        red();
        printf("ERROR :: secret_file_data is not decoded successfully\n");
        reset();
    }
    else
    {
    print();
    green();
    printf("\nINFO :: Secret File data decoded successfully \n");
    reset();
    printf("\n");
    }
    return e_success; // Add a return statement here
}

Status decode_magic_string(DecodeInfo *decInfo) {
    char magic_string[2];
    rewind(decInfo->fptr_stego_image);
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET); // Move to the pixel data

    if (decode_image_to_data(decInfo->fptr_stego_image, magic_string, 2) != e_success) {
        return e_failure; // Handle decode error
    }
    magic_string[2] = '\0'; // Null terminate the string
    /* comparing the decode string with the actual magic string*/
    if(strcmp(magic_string, MAGIC_STRING))									//comparing magicstring not then failure
	{
        red();
	    printf("ERROR: Magic String Not Matched\n");
        reset();
	    return e_failure;
	}
    // Store the magic string into DecodeInfo
    strcpy(decInfo->magic_str, magic_string);
    return e_success;
}

// Function to decode each character of magic string from the image
Status decode_image_to_data(FILE *fptr_stego_image, char *data, int size) {
    char image_buffer[8];
    for (int i = 0; i < size; i++) {
        // Read 8 bytes from the stego image
        fread(&image_buffer, 1, 8, fptr_stego_image) ;
        // Call the function to extract LSBs into data[i]
        data[i]=decode_lsb_to_byte(image_buffer); 
    }
    return e_success;
}

// Function to replace each bit of magic string to LSB bit of image
Status decode_lsb_to_byte(char *image_buffer)
{   
    //get the LSB bit from stego image
    //set the MSB and remaining  bits in size
    
    char character = {0};
    for(int i=0; i<8; i++)
    {
	image_buffer[i]= image_buffer[i] & 1;					
	character = character << 1;						//shifting the lsb bit to msb 			
	character = character | image_buffer[i];								
    }
    return character;
}
Status decode_file_extn_size(DecodeInfo *decInfo)
{
   char image_buffer[32];
   //read the next 32 bytes from source image
   fread(image_buffer,32,1,decInfo -> fptr_stego_image);
   //call size_to_lsb function
   decInfo->extn_size = decode_lsb_to_size(image_buffer);
   //printf("Extn_Size : %d\n",decInfo->extn_size);
   return e_success;
}
int decode_lsb_to_size(char* image_buffer)
{
    //get the LSB bit from stego image
    //set the MSB and remaining  bits in size
   int size = 0;
    for(int i=0; i<32; i++)
    {
	image_buffer[i]= image_buffer[i] & 1;					
	size = size<< 1;				//shifting the lsb bit to msb 							
	size = size | image_buffer[i];								
    }
    return size;
}
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[decInfo->extn_size+1];
    if(decode_image_to_data(decInfo->fptr_stego_image,decInfo->extn_secret_file,decInfo->extn_size)!= e_success)
    {
	return e_failure;
    }
    decInfo->extn_secret_file[decInfo->extn_size] = '\0';
    //merging the output name and extension to create the output file
    strcat(decInfo->output_fname,decInfo->extn_secret_file); 
    return e_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
   char image_buffer[32];
   //read the next 32 bytes from source image
   fread(image_buffer,32,1,decInfo -> fptr_stego_image);
   //call size_to_lsb function
   decInfo->size_secret_file = decode_lsb_to_size(image_buffer);
   return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
	char str[decInfo -> size_secret_file+1];
    // calling decode_image_to_data function
	decode_image_to_data(decInfo->fptr_stego_image,str, decInfo -> size_secret_file);
    if (Open_files(decInfo) == e_failure) {
        return e_failure;
    }
    else{
        str[decInfo -> size_secret_file] = '\0';
        //printing the secret data into the output file
        fprintf(decInfo ->fptr_output,"%s",str);
        //printf("%s",str);
        print();
        green();
        printf("\nINFO :: OUTPUT File  opened Successfully\n\n");
        reset();
    }
	return e_success;
}
