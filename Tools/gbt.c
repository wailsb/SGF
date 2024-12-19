
//hadou c'est la declaration tae les structures

#define MAX_FIELDS 10
#define MAX_NAME 20
#define MAX_RECORDS 100

// Structure représentant un type
typedef struct {
    char typeName[MAX_NAME];           // Nom du type
    int fieldCount;                    // Nombre de champs
    char fieldNames[MAX_FIELDS][MAX_NAME]; // Noms des champs
    char fieldTypes[MAX_FIELDS][MAX_NAME]; // Types des champs
} Type;

// Structure représentant un enregistrement
typedef struct {
    char data[MAX_FIELDS][MAX_NAME]; // Données sous forme de chaînes de caractères
} Record;

// Tableau global pour stocker les types
Type types[MAX_RECORDS];
int typeCount = 0;
//création d'un type :
void createType() {
    if (typeCount >= MAX_RECORDS) {
        printf("Limite maximale de types atteinte.\n");
        return;
    }

    Type newType;
    printf("Nom du type : ");
    scanf(" %[^\n]", newType.typeName);

    printf("Nombre de champs : ");
    scanf("%d", &newType.fieldCount);

    for (int i = 0; i < newType.fieldCount; i++) {
        printf("Nom du champ %d : ", i + 1);
        scanf(" %[^\n]", newType.fieldNames[i]);

        printf("Type du champ %d (int, float, char) : ", i + 1);
        scanf(" %[^\n]", newType.fieldTypes[i]);
    }

    types[typeCount++] = newType;
    printf("Type '%s' créé avec succès.\n", newType.typeName);
}
//affichage de types :
void displayTypes() {
    if (typeCount == 0) {
        printf("Aucun type défini.\n");
        return;
    }

    for (int i = 0; i < typeCount; i++) {
        printf("Type : %s\n", types[i].typeName);
        printf("Champs :\n");
        for (int j = 0; j < types[i].fieldCount; j++) {
            printf("  %s : %s\n", types[i].fieldNames[j], types[i].fieldTypes[j]);
        }
    }
}
//ajouter un enregistrement:
void addRecord(Type *type, Record *record) {
    printf("Ajout d’un enregistrement pour le type '%s'.\n", type->typeName);
    for (int i = 0; i < type->fieldCount; i++) {
        printf("Valeur pour le champ '%s' (%s) : ", type->fieldNames[i], type->fieldTypes[i]);
        scanf(" %[^\n]", record->data[i]);
    }
    printf("Enregistrement ajouté avec succès.\n");
}
//affichage des enregistrements :
void displayRecords(Type *type, Record records[], int recordCount) {
    printf("Enregistrements pour le type '%s' :\n", type->typeName);
    for (int i = 0; i < recordCount; i++) {
        printf("Enregistrement %d :\n", i + 1);
        for (int j = 0; j < type->fieldCount; j++) {
            printf("  %s : %s\n", type->fieldNames[j], records[i].data[j]);
        }
    }
}
//exemple kifach yji kima 9al wail :
int main() {
    Type *selectedType = NULL;
    Record records[MAX_RECORDS];
    int recordCount = 0;
    int choice;

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Créer un type\n");
        printf("2. Afficher les types\n");
        printf("3. Ajouter un enregistrement\n");
        printf("4. Afficher les enregistrements\n");
        printf("5. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createType();
                break;
            case 2:
                displayTypes();
                break;
            case 3:
                if (typeCount == 0) {
                    printf("Aucun type disponible. Créez un type d'abord.\n");
                } else {
                    printf("Choisissez un type (1-%d) : ", typeCount);
                    int typeIndex;
                    scanf("%d", &typeIndex);
                    if (typeIndex < 1 || typeIndex > typeCount) {
                        printf("Type invalide.\n");
                    } else {
                        selectedType = &types[typeIndex - 1];
                        addRecord(selectedType, &records[recordCount++]);
                    }
                }
                break;
            case 4:
                if (selectedType == NULL) {
                    printf("Aucun enregistrement à afficher. Ajoutez d'abord un enregistrement.\n");
                } else {
                    displayRecords(selectedType, records, recordCount);
                }
                break;
            case 5:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choice != 5);

    return 0;
}
//cibon 
//recherche d'un enregistrement :
void searchRecord(Type *type, Record records[], int recordCount, char *fieldName, char *value) {
    int fieldIndex = -1;

    // Trouver l'index du champ
    for (int i = 0; i < type->fieldCount; i++) {
        if (strcmp(type->fieldNames[i], fieldName) == 0) {
            fieldIndex = i;
            break;
        }
    }

    if (fieldIndex == -1) {
        printf("Champ '%s' introuvable dans le type '%s'.\n", fieldName, type->typeName);
        return;
    }

    // Rechercher dans les enregistrements
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].data[fieldIndex], value) == 0) {
            printf("Enregistrement trouvé :\n");
            for (int j = 0; j < type->fieldCount; j++) {
                printf("  %s : %s\n", type->fieldNames[j], records[i].data[j]);
            }
            return;
        }
    }

    printf("Aucun enregistrement avec %s = %s.\n", fieldName, value);
}
//supression logique 
void logicalDelete(Type *type, Record records[], int *recordCount, char *fieldName, char *value) {
    int fieldIndex = -1;

    // Trouver l'index du champ
    for (int i = 0; i < type->fieldCount; i++) {
        if (strcmp(type->fieldNames[i], fieldName) == 0) {
            fieldIndex = i;
            break;
        }
    }

    if (fieldIndex == -1) {
        printf("Champ '%s' introuvable dans le type '%s'.\n", fieldName, type->typeName);
        return;
    }

    // Trouver et marquer l'enregistrement
    for (int i = 0; i < *recordCount; i++) {
        if (strcmp(records[i].data[fieldIndex], value) == 0) {
            strcpy(records[i].data[0], "__LOGICALLY_DELETED__");
            printf("Enregistrement marqué comme supprimé.\n");
            return;
        }
    }

    printf("Aucun enregistrement avec %s = %s à supprimer.\n", fieldName, value);
}
//supression physique :
void physicalDelete(Type *type, Record records[], int *recordCount, char *fieldName, char *value) {
    int fieldIndex = -1;

    // Trouver l'index du champ
    for (int i = 0; i < type->fieldCount; i++) {
        if (strcmp(type->fieldNames[i], fieldName) == 0) {
            fieldIndex = i;
            break;
        }
    }

    if (fieldIndex == -1) {
        printf("Champ '%s' introuvable dans le type '%s'.\n", fieldName, type->typeName);
        return;
    }

    // Trouver et supprimer l'enregistrement
    for (int i = 0; i < *recordCount; i++) {
        if (strcmp(records[i].data[fieldIndex], value) == 0) {
            for (int j = i; j < *recordCount - 1; j++) {
                records[j] = records[j + 1];
            }
            (*recordCount)--;
            printf("Enregistrement supprimé physiquement.\n");
            return;
        }
    }

    printf("Aucun enregistrement avec %s = %s à supprimer.\n", fieldName, value);
}
//void defragmentRecords(Type *type, Record records[], int *recordCount) {
    int newCount = 0;

    for (int i = 0; i < *recordCount; i++) {
        if (strcmp(records[i].data[0], "__LOGICALLY_DELETED__") != 0) {
            records[newCount++] = records[i];
        }
    }

    *recordCount = newCount;
    printf("Défragmentation terminée. Nombre d'enregistrements restants : %d.\n", *recordCount);
}
//defragmentation:
void defragmentRecords(Type *type, Record records[], int *recordCount) {
    int newCount = 0;

    for (int i = 0; i < *recordCount; i++) {
        if (strcmp(records[i].data[0], "__LOGICALLY_DELETED__") != 0) {
            records[newCount++] = records[i];
        }
    }

    *recordCount = newCount;
    printf("Défragmentation terminée. Nombre d'enregistrements restants : %d.\n", *recordCount);
}
//menu principale
int main() {
    Type *selectedType = NULL;
    Record records[MAX_RECORDS];
    int recordCount = 0;
    int choice;

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Créer un type\n");
        printf("2. Afficher les types\n");
        printf("3. Ajouter un enregistrement\n");
        printf("4. Afficher les enregistrements\n");
        printf("5. Rechercher un enregistrement\n");
        printf("6. Suppression logique\n");
        printf("7. Suppression physique\n");
        printf("8. Défragmentation\n");
        printf("9. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createType();
                break;
            case 2:
                displayTypes();
                break;
            case 3:
                if (typeCount == 0) {
                    printf("Aucun type disponible. Créez un type d'abord.\n");
                } else {
                    printf("Choisissez un type (1-%d) : ", typeCount);
                    int typeIndex;
                    scanf("%d", &typeIndex);
                    if (typeIndex < 1 || typeIndex > typeCount) {
                        printf("Type invalide.\n");
                    } else {
                        selectedType = &types[typeIndex - 1];
                        addRecord(selectedType, &records[recordCount++]);
                    }
                }
                break;
            case 4:
                if (selectedType == NULL) {
                    printf("Aucun enregistrement à afficher. Ajoutez d'abord un enregistrement.\n");
                } else {
                    displayRecords(selectedType, records, recordCount);
                }
                break;
            case 5: {
                if (selectedType == NULL) {
                    printf("Aucun enregistrement à rechercher. Ajoutez d'abord un enregistrement.\n");
                } else {
                    char fieldName[MAX_NAME], value[MAX_NAME];
                    printf("Nom du champ : ");
                    scanf(" %[^\n]", fieldName);
                    printf("Valeur à rechercher : ");
                    scanf(" %[^\n]", value);
                    searchRecord(selectedType, records, recordCount, fieldName, value);
                }
                break;
            }
            case 6: {
                char fieldName[MAX_NAME], value[MAX_NAME];
                printf("Nom du champ : ");
                scanf(" %[^\n]", fieldName);
                printf("Valeur à supprimer (logique) : ");
                scanf(" %[^\n]", value);
                logicalDelete(selectedType, records, &recordCount, fieldName, value);
                break;
            }
            case 7: {
                char fieldName[MAX_NAME], value[MAX_NAME];
                printf("Nom du champ : ");
                scanf(" %[^\n]", fieldName);
                printf("Valeur à supprimer (physique) : ");
                scanf(" %[^\n]", value);
                physicalDelete(selectedType, records, &recordCount, fieldName, value);
                break;
            }
            case 8:
                defragmentRecords(selectedType, records, &recordCount);
                break;
            case 9:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choice != 9);

    return 0;
}

