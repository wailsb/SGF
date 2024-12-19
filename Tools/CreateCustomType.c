#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//FieldName Stores the field name
char FieldName[11];
typedef struct{
    char Name[15];
    char *TBNamesStart;
    char *TBTypesStart;
}TableDesc;
TableDesc CreateCustomType(char *NameFile,int FieldCount) {
    //Creating the custom type
    TableDesc CustomType;
    strcpy(CustomType.Name,NameFile);
    // number of fields are stored in FieldCount
    CustomType.TBNamesStart=malloc(FieldCount*11*sizeof(char));
    CustomType.TBTypesStart=malloc(FieldCount*sizeof(char));

    return CustomType;
}
