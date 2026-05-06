/*
 * ================================================================
 *  fonctions.h  -  Fichier d'en-tete (Header File)
 * ================================================================
 *  NOTE : Nous n'avons pas encore etudie les fichiers .h en cours,
 *  mais nous avons fait des recherches et choisi de l'utiliser
 *  car le projet demande de separer le code en plusieurs fichiers.
 *
 *  Un fichier .h sert a :
 *   - Partager la definition de la structure Etudiant entre fichiers
 *   - Declarer les prototypes des fonctions (leur signature)
 *   - Definir des constantes avec #define
 *
 *  Le bloc #ifndef evite les problemes de double inclusion.
 * ================================================================
 */

#ifndef FONCTIONS_H
#define FONCTIONS_H

#define MAX_ETUDIANTS 100
#define NOM_FICHIER "etudiants.dat"

typedef struct {
  char nom[50];
  char prenom[50];
  char matricule[20]; /* taille augmentee pour les longs matricules */
  float note1;
  float note2;
  float note3;
  float moyenne;   /* calculee automatiquement */
  char statut[15]; /* "Admis", "Redoublant", "Majore" */
} Etudiant;

extern Etudiant etudiants[MAX_ETUDIANTS];
extern int nbEtudiants;

/* Utilitaires */
void viderBuffer();
void afficherLigne();
void afficherEntete();
void afficherUnEtudiant(Etudiant e);
float saisirNote(int numero);

/* Fichier */
void chargerEtudiants();
void sauvegarderEtudiants();

/* Logique */
void calculerStatuts();

/* Fonctions du menu */
void ajouterEtudiant();
void afficherTous();
void modifierNotes();
void rechercherParStatut();
void afficherMajore();

#endif
