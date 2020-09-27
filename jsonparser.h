#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct json_object
{
    void **id;
    void **value;
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
    out[n] = malloc(sizeof(char));
    out[n] = NULL;

    return (out);
}

char *json_format_oneline(char *buffer,char *output)
{
    if (buffer == NULL)
        return (NULL);
    
    memset(output,0,sizeof(output));

    char localbuffer[1024] = {0};
    strcpy(localbuffer,buffer);
    char **out = Split(localbuffer," \n");
    for (int i = 0;out[i] != NULL;i++)
    {
        strcat(output,out[i]);
    }

    free(out);
    return (output);
}

char *json_read_file(char *file,char *output)
{
    FILE *fp;
    
    fp = fopen(file,"r");

    if (fp == NULL)
        return (NULL);
    
    fread(output,4096,1,fp);
    fclose(fp);

    return (output);
}

char *reformat_double_cote(char *buffer,char *output)
{
    int n = 0;
    for (int i = 1;buffer[i] != '"';i++)
    {
        output[n] = buffer[i];
        n++;
    }

    return (output);
}

void *json_get_value_by_object(json_object_t *j,char *id)
{
    int nvalue = 0;
    int found_object = 0;

    for (int i = 0;j->id[i] != NULL;i++)
    {
        if (strstr(j->id[i],id)){
            nvalue = i;
            found_object = 1;
            break;
        }

        nvalue++;
    }

    if (found_object != 1)
        return (NULL);

    return ((void *)j->value[nvalue]);
}

void json_parse(json_object_t *j,char *jsondata)
{
    memset(j,0,sizeof(j));
    char localbuffer[4096] = {0};

    strcpy(localbuffer,jsondata);
    // call check json_format_oneline
    
    int id_malloc_true = 0;
    int id_alloc = 1;
    int value_malloc_true = 0;
    int value_alloc = 1;

    char **out = Split(localbuffer,"{ : ,}");
    for (int i = 0;out[i] != NULL;i++)
    {
        if (i % 2){

            if (value_malloc_true == 0){
                j->value = malloc(sizeof(char *) * value_alloc);
                value_alloc++;
            } else {
                j->value = realloc(j->value,sizeof(char *) * value_alloc);
                value_alloc++;
            }

            j->value[value_malloc_true] = malloc(sizeof(char) * strlen(out[i]));
            j->value[value_malloc_true] = out[i];
            value_malloc_true++;
        } else {
            
            if (value_malloc_true == 0){
                j->id = malloc(sizeof(char *) * id_alloc);
                id_alloc++;
            } else {
                j->id = realloc(j->id,sizeof(char *) * id_alloc);
                id_alloc++;
            }

            j->id[id_malloc_true] = malloc(sizeof(char) * strlen(out[i]));
            j->id[id_malloc_true] = out[i];
            id_malloc_true++;
        }
    }

    j->value = realloc(j->value,sizeof(char *) * value_alloc);
    j->value[value_malloc_true] = malloc(sizeof(void *));
    j->value[value_malloc_true] = NULL;

    j->id = realloc(j->id,sizeof(char *) * id_alloc);
    j->id[id_malloc_true] = malloc(sizeof(void *));
    j->id[id_malloc_true] = NULL;
}