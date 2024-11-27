#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include <unistd.h>
 
int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;
    //CLA -> count
    if (argc>=3)
    {
        //check the operation type
        Status operation = check_operation_type(argv); 
        if(operation == e_encode)       //return value
        {
            printf("INFO: Encoding selected\n");
            usleep(500000);
            if(read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                if(do_encoding(&encInfo)==e_success) //open files
                {
                    usleep(500000);
                    printf("\033[0;32mINFO: ##Encoding Done Successfully##\033[0m\n");
                }
                else
                {
                    printf("ERROR(do encoding)\n");
                    return 0;
                }
            }
            else
            {
                printf("ERROR(read and validate encode arguments)...\n");
                return 0;
            }
        }
        else if(operation == e_decode)
        {
            printf("INFO: Decoding Selected\n");
            if(read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                if(do_decoding(&decInfo)==e_success) //open files
                {
                    printf("\033[0;32mINFO: ##Decoding Done Successfully##\n\033[0m\n");
                }
                else
                {
                    printf("ERROR(do decoding)\n");
                    return 0;
                }
            }
            else
            {
                printf("ERROR(read and validate decode arguments)...\n");
                return 0;
            }
        }
        else
        {
            printf("INFO: Unsupported operation\n");
        } 
    }
    else
    {
        printf("ERROR:\nEncoding: ./a.out -e <.bmp file> <(.txt/.c/.sh)file> [output file]\nDecoding: ./a.out -d <.bmp file> [output file]\n");
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    //check the argv[1]
    // if( -e )
    //     return e_encode;      //./a.out -e/-d  
    // else if( -d )             //   0      1  
    //     return e_decode;
    // else
    //     return e_unsupported
    if(!(strcmp(argv[1], "-e")))    
        return e_encode;
    else if(!(strcmp(argv[1], "-d")))
        return e_decode;
    else
        return e_unsupported;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    //1. argv[2] is .bmp file or not
    if(strstr(argv[2], ".bmp"))                         
    {
        //store the file name in structure members
        encInfo->src_image_fname = (char *)malloc(strlen(argv[2])*sizeof(char));
        strcpy(encInfo->src_image_fname, argv[2]);

        //2. argv[3] is.txt file or not
        if(strstr(argv[3], ".txt") )//|| strstr(argv[3], ".c") || strstr(argv[3], ".sh"))
        {
            // store the file name to structure members
            encInfo->secret_fname = (char*)malloc(strlen(argv[3])*sizeof(char));
            strcpy(encInfo->secret_fname, argv[3]);

            //storing file exten in char array
            // if(strstr(argv[3], ".txt"))  
            strcpy(encInfo->extn_secret_file, ".txt");
            // else if(strstr(argv[3], ".c"))  strcpy(encInfo->extn_secret_file, ".c");
            // else /*if(strstr(argv[3], ".sh"))*/  strcpy(encInfo->extn_secret_file, ".sh");
        }    
        else
        {
            printf("ERROR: secret file is not txt file\n");
            return e_failure;
        }                     

        //3. argv[4] is empty or not
        // store the file name to stucture members
        //not a NULL 
        //if (argv[4] is .bmp or not)
        // store the file name to stucture members
        if(argv[4] == NULL)
        {
            printf("INFO: Output file not mentioned. Creating output.bmp as default\n");
            // output.bmp
            encInfo->stego_image_fname = (char*)malloc(strlen("output.bmp")*sizeof(char));
            strcpy(encInfo->stego_image_fname, "output.bmp");
        }
        else if(strstr(argv[4], ".bmp"))
        {
            encInfo->stego_image_fname = (char*)malloc(strlen(argv[4])*sizeof(char));
            strcpy(encInfo->stego_image_fname, argv[4]);
        }
        else
        {
            printf("ERROR: output file is not bmp file...\n");
            return e_failure;
        }
        return e_success;   //if everything is correct or true
    }
    else
    {
        printf("ERROR: Input file is not bmp file...\n");
        return e_failure;
    }
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //1. check argv[2] is .bmp or not
    if(strstr(argv[2], ".bmp"))
    {
        decInfo->dec_bmp_fname = (char *)malloc(strlen(argv[2])*sizeof(char));
        strcpy(decInfo->dec_bmp_fname, argv[2]);
        
        if(argv[3]==NULL) decInfo->output_file = 'N';
        else if(strstr(argv[3], ".txt"))
        { 
            decInfo->output_file = 'Y';
            strcpy(decInfo->output_file_extn, ".txt");
            decInfo->output_fname = (char *)malloc(strlen(argv[3])*sizeof(char));
            strcpy(decInfo->output_fname, argv[3]);
        }
        // else if(strstr(argv[3], ".sh"))
        // {
        //     decInfo->output_file = 'Y';
        //     strcpy(decInfo->output_file_extn, ".sh");
        //     strcpy(decInfo->output_fname, argv[3]);
        // }
        // else if(strstr(argv[3], ".c"))
        // {
        //     decInfo->output_file = 'Y';
        //     strcpy(decInfo->output_file_extn, ".c");
        //     strcpy(decInfo->output_fname, argv[3]);
        // }
        else 
        {
            printf("ERROR: Output file should be .txt file\n");
            return e_failure;
        }
        

    }
    else
    {
        printf("ERROR: Input file is not .bmp file\n");
        return e_failure;
    }
    // printf("output = %c\n", decInfo->output_file);
    // printf("extn = %s\n", decInfo->output_file_extn);
    return e_success;
}