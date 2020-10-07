#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

typedef union
{
    char *str;
    long number;
} json_value_t;

typedef union
{
    char *json_content;
} json_text;


typedef struct json_object
{
    char *key;
    json_value_t value;
} json_object_t;


char **Split(char *buffer,char *delim)
{
    char *token = strtok(buffer,delim);
    int n = 0;
    int space = 1;
    
    char **out = malloc(sizeof(char *) * space);
    
    if (out == NULL)
        return (NULL);

    while (token != NULL)
    {
        if (space == 1){
            out[n] = malloc(sizeof(char) * strlen(token));
            out[n] = token;
            token = strtok(NULL,delim);
            n++;
            space++;
        } else {
            out = realloc(out,sizeof(char *) * (space+1));
            out[n] = malloc(sizeof(char) * strlen(token));
            out[n] = token;
            token = strtok(NULL,delim);
            n++;
            space++;
        }
    }

    out = realloc(out,sizeof(char *) * (space+1));
    out[n] = malloc(sizeof(char *));
    out[n] = NULL;

    return (out);
}

bool json_copy_buffer(json_text *t,char *buffer)
{
    if (t->json_content == NULL)
        return (false);
    

    memset(buffer,0,sizeof(buffer));
    strcpy(buffer,t->json_content);
    return (true);
}

bool json_parse_int(json_object_t *o,char *buffer)
{
    long value = 0;
    
    for (int i = 0;buffer[i] != '\0';i++)
    {
        if (buffer[i] >= '0' && buffer[i] <= '9')
        {
            value *= 10;
            value += buffer[i]++ & 0xF;
        }
    }

    char **splited = Split(buffer,"\": ");

    o->key = malloc(sizeof(char) * strlen(splited[0])+1);

    if (o->key == NULL)
        return (false);
    
    strcpy(o->key,splited[0]);
    o->value.str = NULL;
    o->value.number = value;
    free(splited);

    return (true);
}


void json_split_char(char *buffer,char code)
{
    char buf[sizeof(buffer)+1] = {0};
    strcpy(buf,buffer);
    int n = 0;

    for (int i = 0;buf[i] != '\0';i++)
    {
        if (buf[i] != code){
            buffer[n] = buf[i];
            n++;
        }
    }

    buffer[n] = '\0';
}

static void json_split_quotation_mark(char *buffer)
{
    char buf[sizeof(buffer)+1] = {0};
    strcpy(buf,buffer);
    int n = 0;

    for (int i = 0;buf[i] != '\0';i++)
    {
        if (buf[i] != '"'){
            buffer[n] = buf[i];
            n++;
        }
    }

    buffer[n] = '\0';
}

void json_split_space(char *buffer)
{
    char buf[sizeof(buffer)+1] = {0};
    strcpy(buf,buffer);
    memset(buffer,0,sizeof(buffer));
    int n = 0;

    for (int i = 0;buf[i] != '\0';i++)
    {
        if (buf[i] != ' '){
            buffer[n] = buf[i];
            n++;
        }
    }
}

bool json_parse_string(json_object_t *o,char *buffer)
{
    char **split = Split(buffer,":");
    char key[255] = {0};
    strcpy(key,split[0]);
    json_split_space(key);
    json_split_quotation_mark(key);

    /*for (int i = 1;split[i] != NULL;i++)
    {
        printf("%s\n",split[i]);
    }*/

    o->key = malloc(sizeof(char) * strlen(split[0])); // allocate memory for key

    if (o->key == NULL)
        return (false);

    strcpy(o->key,key); // copy key name in char * key
    o->value.number = 0;
    
    o->value.str = malloc(sizeof(char) * strlen(split[1]));

    if (o->value.str == NULL)
        return (false);

    json_split_quotation_mark(split[1]);
    strcpy(o->value.str,split[1]);


    free(split);

    return (true);
}

void json_parse(json_object_t *o,char *buffer,char *type,char *key)
{
    char **split = Split(buffer,"{,}");

    for (int x = 0;split[x] != NULL;x++)
    {
        if (strcmp(type,"int") == 0)
        {
            if (strstr(split[x],key)){
                json_parse_int(o,split[x]);
                break;
            }
        } else if (strcmp(type,"str") == 0 || strcmp(type,"string") == 0){
            if (strstr(split[x],key)){
                json_parse_string(o,split[x]);
            }
        }
    }

    free(split);
    return;
}

void json_free(json_object_t *o)
{
    if (o->key != NULL)
        free(o->key);
    
    if (o->value.str != NULL)
        free(o->value.str);
    
    o->value.number = 0;
}