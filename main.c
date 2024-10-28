#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "decode.h"
int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    OperationType op_type = check_operation_type(argv);
    if(op_type == e_encode)
    {
    if(argc <4 )
    {
        red();
        printf("ERROR :: Passed only %d arguments \n",argc);
        printf("Please pass atleast three arguments\n");
        reset();
        exit(0);
    }
    else if(argc > 5)
    {
        red();
        printf("ERROR :: Passed  %d arguments \n",argc);
        printf("Please pass 4 or 5 arguments \n");
        reset();
        exit(0);
    }
    if(argc == 4 || argc == 5)
    {
    /* Check argv[1] is -e or -d*/
    //OperationType op_type = check_operation_type(argv);
    if(op_type == e_encode)
    {
        
        print();
        green();
        printf("\nINFO :: Selected Encoding, Encoding process started\n");
        reset();
        printf("\n");
        
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
        print();
        green();
        printf("\nINFO :: Read and validations is Successfully executed......\n");
        reset();
        printf("\n"); 
        do_encoding(&encInfo);
        }
        else
        {
        red();
        printf("ERROR :: Read and validations are Failed \n");
        reset();
        }
    }
    }
    }
    if(op_type == e_decode)
    {
    if(argc < 3)
    {
        red();
        printf("ERROR :: Passed only %d arguments\n",argc);
        printf("Please enter atleast 2 arguments\n");
        reset();
    }
    if(argc>4)
    {
        red();
        printf("ERROR :: Passed %d arguments\n ",argc);
        printf("Please pass 2 or 3 arguments only\n");
        reset();
    }
    if(argc == 3 || argc == 4)
    {
    if(op_type == e_decode)
    {
        print();
        green();
        printf("\nINFO :: Selected Decoding, Decoding process started\n");
        reset();
        printf("\n");
        
        if(read_and_validate_decode_args(argv,&decInfo)==e_success)
        
        {
        print();
        green();
        printf("\nINFO :: Read and validations is Successfully executed\n");
        reset();
        printf("\n"); 
        
        do_decoding(&decInfo);
        }
        else
        {
        red();
        printf("ERROR :: Read and validations are Failed \n");
        reset();
        }
    }
    }
    }   
    if(op_type == e_unsupported)
    {
    red();
    printf("ERROR :: Invalid type,pass (-e or -d)\n");
    reset();
    }
    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else 
    return e_unsupported;
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /* Storing the image name into the structure element*/
    encInfo->src_image_fname = argv[2];
    /* Check the image consists of .bmp or not*/
    if(strstr(encInfo->src_image_fname,".bmp")==NULL)
    {
        red();
        printf("ERROR :: %s is not a .bmp file\n",encInfo->src_image_fname);
        reset();
        return e_failure;
    }
    /* Storing the file name into the structure element*/
    encInfo->secret_fname = argv[3];
    /* Check the file consists of .txt or not*/
    if((strstr(encInfo->secret_fname,".txt") == NULL) )
    {
        red();
        printf("ERROR :: %s is not a .txt or .c or .sh file\n",encInfo->secret_fname);
        reset();
        return e_failure;
    }
    strcpy(encInfo -> extn_secret_file,".txt");
    /* checking the user created output  .bmp file or not */
    if(argv[4] != NULL)
    {
    /* Storing the output image name into the structure element*/
    encInfo->stego_image_fname = argv[4];
    /* Check the output image consists of .bmp or not*/
    if((strstr(encInfo->stego_image_fname,".bmp") == NULL))
    {
        red();
        printf("ERROR :: %s is not a \".bmp\" file\n",encInfo->stego_image_fname);
        reset();
        return e_failure;
    }
    }
    else
    {
        green();
        printf("INFO :: Output file name not mentioed\nDefault file name is \"stego.bmp\"\n");
        reset();
        /* stego.bmp is initialised as the output  .bmp */
        encInfo->stego_image_fname = "stego.bmp";
    }
    return e_success;
}
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    /* Storing the image name into the structure element*/
    decInfo->stego_image_fname = argv[2];
    /* Check the image consists of .bmp or not*/
    if(strstr(decInfo->stego_image_fname,".bmp")==NULL)
    {
        red();
        printf("ERROR :: %s is not a .bmp file\n",decInfo->stego_image_fname);
        reset();
        return e_failure;
    }
    /* Storing the file name into the structure element*/
    decInfo->output_fname = argv[3];
    /* Check the file consists of .txt or not*/
    if((strlen(decInfo->output_fname) ==0) )
    {
        green();
        printf("INFO :: Default name is Decode\n");
        reset();
        decInfo->output_fname = "Decode";
        return e_success;
    }
    return e_success;
}
void Print() {
    for (int i = 0; i < 20; i++) {
        printf(".");
        fflush(stdout);  // Force the output to appear immediately
        usleep(60000); // 60 milliseconds
    }
    printf("\n");
}