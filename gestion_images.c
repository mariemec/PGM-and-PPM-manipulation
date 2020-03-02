/********
Fichier: gestion_images.c
Auteurs: Domingo Palao Munoz
         Charles-Antoine Brunet
Date: 25 octobre 2018
Description: Fichier de distribution pour GEN145.
********/

#include <stdio.h>
#include "bibliotheque_images.h"
#include "AutoValidation.h"


int image1[MAX_HAUTEUR][MAX_LARGEUR];
int image2[MAX_HAUTEUR][MAX_LARGEUR];
struct RGB imageRGB1[MAX_HAUTEUR][MAX_LARGEUR];
struct RGB imageRGB2[MAX_HAUTEUR][MAX_LARGEUR];

int main()
{
	AutoValidation();
	/*
    int lignes1, colonnes1;
    int lignes2, colonnes2;
    int p_lignes = 0 , p_colonnes = 0;
	int coord1x = 50, coord1y = 50, coord2x = 100, coord2y = 100;
    int maxval;
    int histogramme[MAX_VALEUR+1];
    char nomPGM1[MAX_CHAINE] = "Sherbrooke_Frontenac_nuit.pgm";
    char nomPGM2[MAX_CHAINE] = "test5-extraire.pgm";
    struct MetaData metadonnees;

	int retour;

    printf("-> Debut!\n");
    

	//Test copy PGM
    retour = pgm_lire(nomPGM1, image1, &lignes1, &colonnes1, &maxval, &metadonnees);
    pgm_copier(image1, lignes1, colonnes1, image2, &lignes2, &colonnes2);
	pgm_ecrire(nomPGM2, image2, lignes2, colonnes2, maxval, metadonnees);
	
	//Test Histogram
	retour = pgm_lire(nomPGM1, image1, &lignes1, &colonnes1, &maxval, &metadonnees);
	pgm_creer_histogramme(image1, lignes1, colonnes1, histogramme);
	
	//Test predominant color
	retour = pgm_lire(nomPGM1, image1, &lignes1, &colonnes1, &maxval, &metadonnees);
	pgm_couleur_preponderante(image1, lignes1, colonnes1);

	//Test brighten
	retour = pgm_lire(nomPGM1, image1, &lignes1, &colonnes1, &maxval, &metadonnees);
	pgm_eclaircir_noircir(image1, lignes1, colonnes1, maxval, 100);
	pgm_ecrire(nomPGM2, image1, lignes1, colonnes1, maxval, metadonnees);
	
	//Test darken
	retour = pgm_lire(nomPGM1, image1, &lignes1, &colonnes1, &maxval, &metadonnees);
	pgm_eclaircir_noircir(image1, lignes1, colonnes1, maxval, -100);
	pgm_ecrire(nomPGM2, image1, lignes1, colonnes1, maxval, metadonnees);
	
	//Test negative
	retour = pgm_lire(nomPGM1, image1, &lignes1, &colonnes1, &maxval, &metadonnees);
	pgm_creer_negatif(image1, lignes1, colonnes1, maxval);
	pgm_ecrire(nomPGM2, image1, lignes1, colonnes1, maxval, metadonnees);
	
	//Test extract
	retour = pgm_lire(nomPGM1, image1, &lignes1, &colonnes1, &maxval, &metadonnees);
	pgm_extraire(image1, coord1x, coord1y, coord2x, coord2y, &p_lignes, &p_colonnes);
	pgm_ecrire(nomPGM2, image1, lignes1, colonnes1, maxval, metadonnees);

	//Test identical
	retour = pgm_lire(nomPGM1, image1, &lignes1, &colonnes1, &maxval, &metadonnees);
	pgm_lire(nomPGM2, image2, &lignes2, &colonnes2, &maxval, &metadonnees);
	pgm_sont_identiques(image1, lignes1, colonnes1, image2, lignes2, colonnes2);


	// exemple detraitement d'un code de retour (erreur ou reussite)
	printf("-> Retour: ");
	if (retour == OK)
		printf("-> OK");
	else
		printf("-> ERREUR");
	printf("\n");


   

    printf("-> Fin!\n");
*/
    return 0;
}
