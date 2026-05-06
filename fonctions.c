/*
 * fonctions.c
 * Projet 2 : Gestionnaire de notes avec statut
 * Groupe 14 -- 2025/2026
 * Veuillez consulter le fichier main.c pour les instructions d'execution.
 * Veuillez noter que nous avons utilise certains qu'on a pas vu en cours, comme les fichiers .h et la gestion de fichiers binaires, pour respecter les consignes du projet.
 * Nous avons ajoute des commentaires detailles pour expliquer notre code et faciliter la lecture.
 * Merci de votre attention et n'hesitez pas a nous contacter pour toute question ou suggestion d'amelioration !
 */

#include "fonctions.h"
#include <stdio.h>
#include <string.h>

/* on declare le tableau et le compteur ici, ils sont utilises dans tout le
 * fichier */
Etudiant etudiants[MAX_ETUDIANTS];
int nbEtudiants = 0;

/* ==========================================================
   FONCTIONS UTILITAIRES
   ========================================================== */

/* apres un scanf il reste toujours un '\n' dans le buffer
   cette fonction le vide pour eviter des bugs de saisie */
void viderBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

/* on met la premiere lettre en majuscule et le reste en minuscule
   comme ca l'utilisateur peut ecrire "majore" ou "MAJORE" ca marche pareil */
void normaliserStatut(char s[]) {
  int i;
  if (s[0] >= 'a' && s[0] <= 'z')
    s[0] = s[0] - 32;
  for (i = 1; s[i] != '\0'; i++)
    if (s[i] >= 'A' && s[i] <= 'Z')
      s[i] = s[i] + 32;
}

/* retourne 1 si la chaine ne contient que des chiffres, 0 sinon
   utilise pour rejeter les noms/prenoms qui sont purement numeriques
   ex : "1234" est rejete, mais "Ben2Ali" est accepte */
int estPurementNumerique(const char s[]) {
  int i;
  if (s[0] == '\0')
    return 0;
  for (i = 0; s[i] != '\0'; i++)
    if (s[i] < '0' || s[i] > '9')
      return 0;
  return 1;
}

/* les 3 fonctions d'affichage du tableau */
void afficherLigne() {
  printf("+--------------------+-----------------+-----------------+------+----"
         "--+------+-------+------------+\n");
}

void afficherEntete() {
  afficherLigne();
  printf("| %-18s | %-15s | %-15s | %-4s | %-4s | %-4s | %-5s | %-10s |\n",
         "Matricule", "Nom", "Prenom", "N1", "N2", "N3", "Moy", "Statut");
  afficherLigne();
}

void afficherUnEtudiant(Etudiant e) {
  printf(
      "| %-18s | %-15s | %-15s | %-4.1f | %-4.1f | %-4.1f | %-5.2f | %-10s |\n",
      e.matricule, e.nom, e.prenom, e.note1, e.note2, e.note3, e.moyenne,
      e.statut);
}

/* saisie d'une note avec verification : on redemande si c'est invalide */
float saisirNote(int numero) {
  float note;
  int lu;
  do {
    printf("  Note %d (0-20) : ", numero);
    lu = scanf("%f", &note);
    viderBuffer();
    if (lu != 1)
      printf("  Entree invalide, entrez un nombre.\n");
    else if (note < 0 || note > 20) {
      printf("  La note doit etre entre 0 et 20.\n");
      lu = 0;
    }
  } while (!lu);
  return note;
}

/* ==========================================================
   LECTURE ET ECRITURE DU FICHIER BINAIRE
   ========================================================== */

/* on sauvegarde d'abord sizeof(Etudiant) comme "version"
   si la structure change un jour, on peut le detecter au chargement */
void chargerEtudiants() {
  int taille;
  FILE *f = fopen(NOM_FICHIER, "rb");
  nbEtudiants = 0;
  if (f == NULL)
    return;

  fread(&taille, sizeof(int), 1, f);

  /* si la taille ne correspond pas, le fichier vient d'une ancienne version
     on le supprime pour eviter de lire des donnees corrompues */
  if (taille != (int)sizeof(Etudiant)) {
    fclose(f);
    remove(NOM_FICHIER);
    printf("  [INFO] Fichier reinitialise (structure modifiee).\n\n");
    return;
  }

  fread(&nbEtudiants, sizeof(int), 1, f);
  if (nbEtudiants < 0 || nbEtudiants > MAX_ETUDIANTS)
    nbEtudiants = 0;
  else
    fread(etudiants, sizeof(Etudiant), nbEtudiants, f);

  fclose(f);
}

void sauvegarderEtudiants() {
  int taille = (int)sizeof(Etudiant);
  FILE *f = fopen(NOM_FICHIER, "wb");
  if (f == NULL) {
    printf("  Erreur : impossible d'ecrire dans le fichier.\n");
    return;
  }
  fwrite(&taille, sizeof(int), 1, f);
  fwrite(&nbEtudiants, sizeof(int), 1, f);
  fwrite(etudiants, sizeof(Etudiant), nbEtudiants, f);
  fclose(f);
}

/* ==========================================================
   CALCUL DES MOYENNES ET STATUTS
   ========================================================== */

/*
 * cette fonction recalcule TOUT a chaque fois qu'on l'appelle.
 * on est obliges de le faire pour tous les etudiants et pas juste un,
 * parce que le statut "Majore" depend de toute la liste.
 *
 * etape 1 : calculer les moyennes de tout le monde
 * etape 2 : trouver qui a la meilleure moyenne (le futur majore)
 * etape 3 : attribuer les statuts en fonction des regles
 */
void calculerStatuts() {
  int i;
  int indexMajore = 0;

  if (nbEtudiants == 0)
    return;

  /* etape 1 */
  for (i = 0; i < nbEtudiants; i++)
    etudiants[i].moyenne =
        (etudiants[i].note1 + etudiants[i].note2 + etudiants[i].note3) / 3.0;

  /* etape 2 : on cherche l'indice de la meilleure moyenne */
  for (i = 1; i < nbEtudiants; i++)
    if (etudiants[i].moyenne > etudiants[indexMajore].moyenne)
      indexMajore = i;

  /* etape 3 : on attribue les statuts */
  for (i = 0; i < nbEtudiants; i++) {
    if (i == indexMajore)
      strcpy(etudiants[i].statut, "Majore");
    else if (etudiants[i].moyenne >= 10)
      strcpy(etudiants[i].statut, "Admis");
    else
      strcpy(etudiants[i].statut, "Redoublant");
  }
}

/* ==========================================================
   1. AJOUTER UN ETUDIANT
   ========================================================== */

void ajouterEtudiant() {
  int i, existe;
  Etudiant nouvel;

  printf("\n--- Ajouter un etudiant ---\n\n");

  if (nbEtudiants >= MAX_ETUDIANTS) {
    printf("  Liste pleine (%d etudiants maximum).\n", MAX_ETUDIANTS);
    return;
  }

  /* on met tout a zero pour eviter des valeurs bizarres au debut */
  memset(&nouvel, 0, sizeof(Etudiant));

  /* on verifie que le matricule est valide (8-12 chiffres) et qu'il n'existe
   * pas deja */
  do {
    int longueur;
    printf("  Matricule (8 chiffres - 12 chiffres) : ");
    scanf("%19s", nouvel.matricule);
    viderBuffer();
    longueur = (int)strlen(nouvel.matricule);
    if (longueur < 8) {
      printf("  Matricule trop court (minimum 8 chiffres), reessayez.\n");
      existe = 1;
      continue;
    }
    if (longueur > 12) {
      printf("  Matricule trop long (maximum 12 chiffres), reessayez.\n");
      existe = 1;
      continue;
    }
    existe = 0;
    for (i = 0; i < nbEtudiants; i++) {
      if (strcmp(etudiants[i].matricule, nouvel.matricule) == 0) {
        printf("  Ce matricule existe deja, reessayez.\n");
        existe = 1;
        break;
      }
    }
  } while (existe);

  /* le " " avant % permet d'ignorer les espaces restants dans le buffer
     et [^\n] permet de lire des noms avec espaces comme "Ben Ali"
     on boucle tant que la saisie est purement numerique */
  do {
    printf("  Nom        : ");
    scanf(" %49[^\n]", nouvel.nom);
    viderBuffer();
    if (estPurementNumerique(nouvel.nom))
      printf("  Erreur : le nom ne peut pas etre compose uniquement de chiffres."
             " Veuillez entrer un nom valide.\n");
  } while (estPurementNumerique(nouvel.nom));

  do {
    printf("  Prenom     : ");
    scanf(" %49[^\n]", nouvel.prenom);
    viderBuffer();
    if (estPurementNumerique(nouvel.prenom))
      printf("  Erreur : le prenom ne peut pas etre compose uniquement de chiffres."
             " Veuillez entrer un prenom valide.\n");
  } while (estPurementNumerique(nouvel.prenom));

  nouvel.note1 = saisirNote(1);
  nouvel.note2 = saisirNote(2);
  nouvel.note3 = saisirNote(3);

  /* on ajoute l'etudiant au tableau puis on recalcule tout */
  etudiants[nbEtudiants] = nouvel;
  nbEtudiants++;

  calculerStatuts();
  sauvegarderEtudiants();

  printf("\n  Ajoute !  Moyenne : %.2f  |  Statut : %s\n",
         etudiants[nbEtudiants - 1].moyenne, etudiants[nbEtudiants - 1].statut);
}

/* ==========================================================
   2. AFFICHER TOUS LES ETUDIANTS
   ========================================================== */

void afficherTous() {
  int i;

  printf("\n--- Liste des etudiants ---\n\n");

  if (nbEtudiants == 0) {
    printf("  Aucun etudiant enregistre.\n");
    return;
  }

  afficherEntete();
  for (i = 0; i < nbEtudiants; i++)
    afficherUnEtudiant(etudiants[i]);
  afficherLigne();
  printf("  Total : %d etudiant(s).\n", nbEtudiants);
}

/* ==========================================================
   3. MODIFIER LES NOTES D'UN ETUDIANT
   ========================================================== */

void modifierNotes() {
  int i, idx;
  char matricule[20];

  printf("\n--- Modifier les notes ---\n\n");

  printf("  Matricule : ");
  scanf("%19s", matricule);
  viderBuffer();

  /* on cherche l'etudiant dans le tableau */
  idx = -1;
  for (i = 0; i < nbEtudiants; i++) {
    if (strcmp(etudiants[i].matricule, matricule) == 0) {
      idx = i;
      break;
    }
  }

  if (idx == -1) {
    printf("  Matricule '%s' introuvable.\n", matricule);
    return;
  }

  printf("\n  Etudiant trouve :\n");
  afficherEntete();
  afficherUnEtudiant(etudiants[idx]);
  afficherLigne();

  printf("\n  Nouvelles notes :\n");
  etudiants[idx].note1 = saisirNote(1);
  etudiants[idx].note2 = saisirNote(2);
  etudiants[idx].note3 = saisirNote(3);

  /* recalcul obligatoire pour tout le monde car le majore a peut-etre change */
  calculerStatuts();
  sauvegarderEtudiants();

  printf("\n  Modifie !  %s %s -> Moyenne : %.2f  |  Statut : %s\n",
         etudiants[idx].prenom, etudiants[idx].nom, etudiants[idx].moyenne,
         etudiants[idx].statut);
}

/* ==========================================================
   4. RECHERCHER PAR STATUT
   ========================================================== */

void rechercherParStatut() {
  int i, nbTrouves;
  char statut[15];

  printf("\n--- Recherche par statut ---\n\n");
  printf("  Valeurs possibles : Admis | Redoublant | Majore\n");
  printf("  Statut : ");
  scanf("%14s", statut);
  viderBuffer();

  /* on normalise la casse pour accepter "admis", "ADMIS", "Admis" etc. */
  normaliserStatut(statut);

  printf("\n  Resultats pour \"%s\" :\n", statut);
  afficherEntete();

  nbTrouves = 0;
  for (i = 0; i < nbEtudiants; i++) {
    if (strcmp(etudiants[i].statut, statut) == 0) {
      afficherUnEtudiant(etudiants[i]);
      nbTrouves++;
    }
  }

  afficherLigne();
  if (nbTrouves == 0)
    printf("  Aucun etudiant avec le statut \"%s\".\n", statut);
  else
    printf("  %d etudiant(s) trouve(s).\n", nbTrouves);
}

/* ==========================================================
   5. AFFICHER LE MAJORE
   ========================================================== */

void afficherMajore() {
  int i;

  printf("\n--- Major de promotion ---\n\n");

  if (nbEtudiants == 0) {
    printf("  Aucun etudiant enregistre.\n");
    return;
  }

  for (i = 0; i < nbEtudiants; i++) {
    if (strcmp(etudiants[i].statut, "Majore") == 0) {
      afficherEntete();
      afficherUnEtudiant(etudiants[i]);
      afficherLigne();
      printf("  Felicitations au major de promotion !\n");
      return;
    }
  }

  /* cas de securite : si on arrive ici c'est que calculerStatuts()
     n'a pas ete appele correctement, on le refait */
  calculerStatuts();
  sauvegarderEtudiants();
  for (i = 0; i < nbEtudiants; i++) {
    if (strcmp(etudiants[i].statut, "Majore") == 0) {
      afficherEntete();
      afficherUnEtudiant(etudiants[i]);
      afficherLigne();
      return;
    }
  }
}
