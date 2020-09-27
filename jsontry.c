#include "jsonparser.h"

int main(int argc,char **argv)
{
    char buffer[1024] = {0};
    char newformat[1024] = {0};
    char nfm[1024] = {0};
    json_object_t j;

    json_read_file("test.json",buffer);
    json_format_oneline(buffer,newformat); // format json one line
    printf("%s\n",newformat);
    json_parse(&j,newformat); // parse json
    
    void * value = json_get_value_by_object(&j,"name");

    if (value == NULL){
        fprintf(stderr,"[-] Error for get value\n");
        exit(-1);
    }

    reformat_double_cote(value,nfm); // Split " in buffer

    printf("Value of name : %s\n",value); // get value of name
    printf("Value of name : %s\n",nfm); // show value without "
    free(j.value); // free memory !

    return (0);
}