#include "cjson.h"
#include <stdio.h>

int main(int argc,char **argv)
{
    char json[1024] = "{ \"id\": 1457, \"name\":\"Hello World\", \"age\": 17, \"lol\":\"Samuel\" }";
    json_object_t id = {0};
    json_object_t age = {0};
    json_object_t stri = {0};
    json_object_t ip = {0};
    json_text j;
    char jsondata[1024] = {0};
    j.json_content = malloc(sizeof(char) * 1024);

    if (j.json_content == NULL)
        fprintf(stderr,"[-] Error Allocate memory !");
    
    strcpy(j.json_content,json);

    json_copy_buffer(&j,json);

    json_parse(&id,json,"int","id");
    json_copy_buffer(&j,json);
    json_parse(&age,json,"int","age");
    printf("%s: %d\n",id.key,id.value.number);
    printf("%s: %d\n",age.key,age.value.number);
    json_copy_buffer(&j,json);

    json_parse(&stri,json,"string","loc");

    printf("%s: %s\n",stri.key,stri.value.str);

    json_free(&stri);

    free(j.json_content);

    return (0);
}