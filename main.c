/*
 * ================================================================
 *  main.c  -  Point d'entree du programme
 *  Projet 2 : Gestionnaire de notes avec statut
 *  Groupe 14 -- 2025/2026
 * ================================================================
 * To execute the program run in the terminal:
 * cd Group_14_Project_2
 * gcc main.c fonctions.c -o gestionnaire
 * ./gestionnaire
 * ================================================================
 */
#include "fonctions.h"
#include <stdio.h>

int main() {
  int choix;

  chargerEtudiants();

  do {
    printf("\n");
    printf("  +---------------------------------+\n");
    printf("  |   GESTIONNAIRE DE NOTES         |\n");
    printf("  |   Groupe 14  --  2025/2026      |\n");
    printf("  +---------------------------------+\n");
    printf("  |  1. Ajouter un etudiant         |\n");
    printf("  |  2. Afficher tous               |\n");
    printf("  |  3. Modifier les notes          |\n");
    printf("  |  4. Rechercher par statut       |\n");
    printf("  |  5. Afficher le majore          |\n");
    printf("  |  6. Quitter                     |\n");
    printf("  +---------------------------------+\n");
    printf("  Choix : ");

    if (scanf("%d", &choix) != 1) {
      viderBuffer();
      choix = 0;
    } else {
      viderBuffer();
    }

    switch (choix) {
    case 1:
      ajouterEtudiant();
      break;
    case 2:
      afficherTous();
      break;
    case 3:
      modifierNotes();
      break;
    case 4:
      rechercherParStatut();
      break;
    case 5:
      afficherMajore();
      break;
    case 6:
      printf("\n  Au revoir !\n\n");
      break;
    default:
      printf("  Choix invalide, entrez un nombre entre 1 et 6.\n");
    }

  } while (choix != 6);

  return 0;
}