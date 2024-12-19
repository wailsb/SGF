#include <stdlib.h>
#include <stdio.h>
#include "initVdisk.h" 
//le mode d'organisation dans ce fichier est chainee
#define MAXFileName 15
typedef struct {
    long id;           
    char full_name[40];   
    char mat[20];         
    char section;      
}Student;
typedef struct{
    Student nbrec;
    struct Block* next; 
}Block;

// Fonction pour allouer et initialiser les blocs d'un fichier
void createfiledata(char name[MAXFileName], int nbrecord) {
    FILE *file = fopen(name, "wb");  // Ouvre un fichier binaire en mode écriture
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", name);
        return;
    }

    Block *head = NULL;  // Pointeur vers le premier bloc
    Block *current = NULL;  // Pointeur vers le bloc actuel
    Block *new_block = NULL;  // Pointeur pour le nouveau bloc
    Student student;  // Déclaration d'un étudiant

    for (int i = 0; i < nbrecord; i++) {
        // Allocation d'un nouveau bloc
        new_block = (Block *)malloc(sizeof(Block));
        if (new_block == NULL) {
            printf("Erreur d'allocation de mémoire.\n");
            fclose(file);
            return;
        }

        student.id = i + 1;  // ID de l'étudiant
        snprintf(student.full_name, sizeof(student.full_name), "Etudiant %d", i + 1);
        snprintf(student.mat, sizeof(student.mat), "MATRICULE%d", i + 1);
        student.section = 'A';  // Section par défaut

        new_block->nbrec = student;  // Remplir le bloc avec l'étudiant
        new_block->next = NULL;  // Le prochain bloc est initialement NULL

        // Si c'est le premier bloc, il devient le head
        if (head == NULL) {
            head = new_block;
        } else {
            // Sinon, on lie le bloc précédent au nouveau bloc
            current->next = new_block;
        }
        current = new_block;  // Avancer au nouveau bloc
    }

    // Maintenant, on écrit les blocs chaînés dans le fichier
    Block *temp = head;
    while (temp != NULL) {
        fwrite(temp, sizeof(Block), 1, file);
        Block *next = temp->next;
        free(temp);  // Libérer la mémoire du bloc
        temp = next;
    }

    printf("Fichier %s créé avec %d enregistrements.\n", name, nbrecord);
    fclose(file);  // Fermer le fichier
}


// Fonction principale
int main() {
    char file_name[MAXFileName];  // Nom du fichier à créer
    int nbrecord;  // Nombre d'enregistrements (étudiants) à écrire dans le fichier

    // Vérifier si le vdisk existe, sinon le créer
    if (!check_vdisk("vdisk.bin")) {
        printf("Le vdisk n'existe pas. Création du vdisk...\n");
        create_vdisk();  // Cette fonction est définie dans la partie de votre collègue
    }

    // Demander à l'utilisateur de saisir le nom du fichier et le nombre d'enregistrements
    printf("Entrez le nom du fichier (max %d caractères) : ", MAXFileName - 1);
    scanf("%14s", file_name);  // Limite la saisie à 14 caractères pour éviter les dépassements

    printf("Entrez le nombre d'enregistrements : ");
    scanf("%d", &nbrecord);  // Saisie du nombre d'enregistrements

    // Créer le fichier et y insérer les enregistrements sous forme de blocs chaînés
    createfiledata(file_name, nbrecord);  // Créer et remplir le fichier avec les enregistrements

    return 0;
}

