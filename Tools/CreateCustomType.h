#ifndef MYFILE_H
#define MYFILE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char FieldName[11];
typedef struct{
    char Name[15];
    char *TBNamesStart;
    char *TBTypesStart;
}TableDesc;
TableDesc CreateCustomType(char *NameFile,int FieldCount);
#endif
