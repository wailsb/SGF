#include <stdio.h>
#include "CreateCustomType.c"

void afficherMenu() {
    printf("=== Menu Principal ===\n");
    printf("1. Initialiser la mémoire centrale\n");
    printf("2. Créer un fichier\n");
    printf("3. Ajouter un enregistrement à un fichier\n");
    printf("4. Afficher les fichiers et leurs métadonnées\n");
    printf("5. Quitter\n");
}


int main() {
    int choix;
    int choixCreation;
    int FCount;
    char TName[15];
    char FieldName[11];
    char FieldType;

    while (1) {
        afficherMenu();
        printf("Choisissez une option : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
            /*
                printf("Nombre de blocs pour la mémoire centrale : ");
                int nombreBlocs;
                scanf("%d", &nombreBlocs);
                mc = initialiserMemoireCentrale(nombreBlocs);
                printf("Mémoire initialisée avec %d blocs.\n", nombreBlocs);
                */
               
                break;
            case 2:
                printf("\n--- Menu Principal ---\n");
                printf("1. Créer un type\n");
                printf("2. Afficher les types\n");
                printf("3. Ajouter un enregistrement\n");
                printf("4. Afficher les enregistrements\n");
                printf("5. Quitter\n");
                printf("Choix : ");
                scanf("%d", &choixCreation);
                switch (choixCreation) {
                    case 1:
                        printf("Nom du type des valeurs :");
                        scanf("%s",TName);
                        printf("Nombre des champs du type :");
                        scanf("%d",&FCount);
                        TableDesc Custtype=CreateCustomType(TName,FCount);
                        for (int i = 0; i < FCount; i++)
                        {
                            printf("type du champ %d :",i+1);
                            scanf(" %c",&FieldType);
                            printf("\nNom du champ %d :",i+1);
                            scanf("%10s",FieldName);
                            *(Custtype.TBTypesStart+i*sizeof(char))=FieldType;
                            strcpy(Custtype.TBNamesStart+i*11*sizeof(char),FieldName);
                            
                        }
                        for (int i = 0; i < FCount; i++)
                        {
                            printf("%s %c\n",(Custtype.TBNamesStart+i*11*sizeof(char)),*(Custtype.TBTypesStart+i*sizeof(char)));
                        }
                        
                        
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        printf("Au revoir !\n");
                        break;
                    default:
                        printf("Choix invalide.\n");
                    }


                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                printf("Au revoir !\n");
                return 0;
            default:
                printf("Option invalide. Réessayez.\n");
        }
    }
}
