#include "bibliotheque_images.h"
#include "stdio.h"
#include "stdlib.h"


//PGM & PPM FUNCTIONS THAT READ AN IMAGE FILE
void nextChar(FILE *fp, char *c)										//Skip to the next character when it encounters a blank character
{
	while ((*c==' '||*c=='\n'||*c=='\t') && !feof(fp) )
	{
		*c=fgetc(fp);
	}
}

int getMeta(FILE *fp, char *c, struct MetaData *p_metadonnees)			//Reads MetaData of image file
{
	int loops=0 , index = 0;
		if(*c == '#'){
			*c=fgetc(fp);	
			(*p_metadonnees).lieuCreation[0] = '\0';
			(*p_metadonnees).dateCreation[0] ='\0'	;
			(*p_metadonnees).auteur[0] = '\0';					
			while(*c != '\n')
			{
				while(*c!='\n' && *c!= ';')
				{
					if(loops ==0)
					{
						(*p_metadonnees).auteur[index] = *c;
					}
					if(loops ==1)
					{

						(*p_metadonnees).dateCreation[index] = *c;
					}
					if(loops ==2)
					{
						(*p_metadonnees).lieuCreation[index] = *c;
					}
					
					*c = fgetc(fp);
					index++;
				}
				
				if(loops ==0)
				{
					(*p_metadonnees).auteur[index] = '\0';
				}
				if(loops ==1)
				{

					(*p_metadonnees).dateCreation[index] = '\0';
				}
				if(loops ==2)
				{
					(*p_metadonnees).lieuCreation[index] = '\0';
				}
				if(*c == ';')
				{
					*c = fgetc(fp);
				}
				index =0;
				loops++;
			}
			if((*p_metadonnees).lieuCreation[0] == '\0' || 
			   (*p_metadonnees).dateCreation[0] =='\0' || 
			   (*p_metadonnees).auteur[0] == '\0')
			{
				return -1;
			}
		}
	
	return 0;
}

int typeOfFilePGM(FILE *fp, char *c)										//Detects type of image file (P2,pgm)
{
	char type[10]; 				
	int k=0;
	while (*c!=' ' && *c!= '\n' && *c!= '\t')							
		{
			type[k] = *c;
			*c = fgetc(fp);
			k++;
		}
	type[k] = '\0';		
	if (type[0] =='P' && type[1]=='2')
	{
		return 0;
	}

	else
	{
		return ERREUR_FORMAT;
	}	
}

int typeOfFilePPM(FILE *fp, char *c)										//Detects type of image file (P3,ppm)
{
	char type[3]; 				
	int k=0;
	while (*c!=' ' && *c!= '\n' && *c!= '\t')							
		{
			type[k] = *c;
			*c = fgetc(fp);
			k++;
		}
	type[k] = '\0';		
		
	if (type[0] =='P')
	{
		if (type[1]=='3')
		{
			return OK;
		}

	}

	else
	{
		return ERREUR_FORMAT;
	}	
	return OK;
}

int nextInt(FILE *fp, char *c)											//Calculate integer from ASCII
{
	int k = 0;					
	while((int)*c>=48 && (int)*c<=57)
	{
		k=k*10 +((int)*c-48);
		*c = fgetc(fp);
	}
	
	return k;
}

void fillMatrix(FILE *fp, char *c,int matrice[MAX_HAUTEUR][MAX_LARGEUR], int *hauteur, int *largeur)//Fills an empty matrix with data stored within a B&W image file
{
	int value = 0;
		for (int i=0; i<*hauteur;i++)
		{
			for (int j=0; j<*largeur;j++)
			{
				while ((int)*c>=48 && (int)*c<=57)
					{
						value=value*10+((int)*c-48);
						*c = fgetc(fp);
					}
				matrice[i][j]=value;
				value=0;
				nextChar(fp,c);	
			}
		}
}

void fillColorMatrix(FILE *fp, char *c,									//Fills an empty matrix with data stored within a RGB image file and prints it on screen
					struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], int *hauteur, int *largeur) 
{
	int value = 0;
	
		for (int i=0; i<*hauteur;i++)
		{
			for (int j=0; j<*largeur;j++)
			{
				for(int loopCtr = 0;loopCtr<3;loopCtr++)
				{
					while ((int)*c>=48 && (int)*c<=57)
					{
						value=value*10+((int)*c-48);
						*c = fgetc(fp);
					}
					if (loopCtr == 0)
					{
						matrice[i][j].valeurR = value;
					}
					if (loopCtr == 1)
					{
						matrice[i][j].valeurG = value;
					}
					if (loopCtr == 2)
					{
						matrice[i][j].valeurB = value;
					}
					nextChar(fp, c);
					value = 0;
				}
				nextChar(fp,c);	
			}
		}
}

void showMatrix (int matrice[MAX_HAUTEUR][MAX_LARGEUR])					//Prints out matrix of BW image  TO DELETE LATER
{
	for (int i=0; i<MAX_LARGEUR;i++)
	{
		for (int j=0; j<MAX_HAUTEUR;j++)
		{
			printf("%d ", matrice[i][j]);
		}
		printf("\n");
	}
}

int pgm_lire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
             int *p_lignes, int *p_colonnes, 
             int *p_maxval, struct MetaData *p_metadonnees)
{
	FILE *fp;
	fp=fopen(nom_fichier, "r");
	char c = 'a'; //Initialize to store characters of image file

	if (fp != NULL)														//Actions to take if file successfully opens
	{
		c = fgetc(fp);
		int checkMeta = getMeta(fp,&c, p_metadonnees);					//Store MetaData 
		if(checkMeta == -1){
			return ERREUR;
		}
		nextChar(fp, &c);
		
		if (typeOfFilePGM(fp, &c)!=OK)									//Stores type of file (P2 or P3)										
			return ERREUR_FORMAT;
		nextChar(fp, &c);
		
		*p_colonnes = nextInt(fp,&c);									//Stores Width
		nextChar(fp, &c);
		
		*p_lignes = nextInt(fp,&c);										//Stores Height
		nextChar(fp, &c);
		
		*p_maxval = nextInt(fp,&c);										//Stores Max Pixel Value
		nextChar(fp, &c);
		
		
		
		if (*p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR)
		{
			return ERREUR_TAILLE; 
		}
		else
			fillMatrix(fp, &c, matrice, p_lignes, p_colonnes);
	}	
	
	else
	{
		return ERREUR_FICHIER;
	} 
	
	fclose(fp);
    return OK;
}

int ppm_lire(char nom_fichier[], struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], 
			int *p_lignes, int *p_colonnes, int *p_maxval, 
			struct MetaData *p_metadonnees)
{
	FILE *fp;
	fp=fopen(nom_fichier, "r");
	char c = 'a'; 
												//Initialize character to sort through characters of image file

	if (fp != NULL)														//Actions to take if file successfully opens
	{
		c = fgetc(fp);
		int checkMeta = getMeta(fp,&c, p_metadonnees);					//Store MetaData 
		if(checkMeta == -1)
		{
			return ERREUR;
		}								
		nextChar(fp, &c);
		
		int check = typeOfFilePPM(fp, &c);								//Stores type of file (P3)
		if (check < 0)
		{																								
			return ERREUR_FORMAT;									
		}
		nextChar(fp, &c);
		
		*p_colonnes = nextInt(fp,&c);									//Stores Width
		nextChar(fp, &c);
		
		*p_lignes = nextInt(fp,&c);										//Stores Height
		nextChar(fp, &c);
		
		*p_maxval = nextInt(fp,&c);										//Stores Max Pixel Value
		nextChar(fp, &c);
		
		
		if (*p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR)
		{
			return ERREUR_TAILLE; 
		}
		else
			fillColorMatrix(fp, &c, matrice, p_lignes, p_colonnes);
	}	
	
	else
	{
		return ERREUR_FICHIER;
	} 
	
	return OK;
}


//PGM & PPM FUNCTIONS THAT WRITE A NEW FILE

void writeMeta(struct MetaData *metadonnees, FILE *fp)
{
	fputs("#", fp);
	fputs((*metadonnees).auteur, fp);
	fputs(";", fp);
	fputs((*metadonnees).dateCreation, fp);
	fputs(";", fp);
	fputs((*metadonnees).lieuCreation, fp);
	fputs("\n", fp);
}

void writeInfo(char stringEquiv[], int infoValue, FILE *fp)
{
	sprintf(stringEquiv,"%d", infoValue);
	fputs(stringEquiv, fp);
} 

void writeBWMatrix(int matrice[MAX_HAUTEUR][MAX_LARGEUR], FILE *fp, int *p_lignes, int *p_colonnes)
{
	char svalue[7];
	for (int i=0; i<*p_lignes;i++)
	{
		for (int j=0; j<*p_colonnes;j++)
		{
			sprintf(svalue,"%d",matrice[i][j]);
			fputs (svalue, fp);
			fputs(" ", fp);
		}
		fputs("\n", fp);
	}
}

int pgm_ecrire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
               int lignes, int colonnes, 
               int maxval, struct MetaData metadonnees){
	FILE *fp;
	fp=fopen(nom_fichier, "w");	
	
	if (fp != NULL)														//Actions to take if file successfully opens
	{
		writeMeta(&metadonnees, fp);
		fputs("P2", fp);
		fputs("\n", fp);
		
		char scolonnes[4], slignes[4],smaxval[7];						// variables that store string equivalent of integer values
		
		writeInfo(scolonnes, colonnes, fp);
		fputs("\n", fp);
		
		writeInfo(slignes, lignes, fp);
		fputs(" ", fp);
		
		writeInfo(smaxval, maxval, fp);
		fputs("\n", fp);
		
		writeBWMatrix(matrice, fp, &lignes, &colonnes);
	}
	
	else
	{
		return ERREUR_FICHIER;
	} 
	
	fclose(fp);
    return OK;
}

void writeRGBMatrix(struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], FILE *fp, int *p_lignes, int *p_colonnes)
{
	char svalue[7];
	for (int i=0; i<*p_lignes;i++)
	{
		for (int j=0; j<*p_colonnes;j++)
		{
			sprintf(svalue,"%d",matrice[i][j].valeurR);
			fputs (svalue, fp);
			fputs(" ", fp);
				
			sprintf(svalue,"%d",matrice[i][j].valeurG);
			fputs (svalue, fp);
			fputs(" ", fp);
				
			sprintf(svalue,"%d",matrice[i][j].valeurB);
			fputs (svalue, fp);
			fputs(" ", fp);
		}
		fputs("\n", fp);
	}
}

int ppm_ecrire(char nom_fichier[], struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], 
				int lignes, int colonnes, int maxval, struct MetaData metadonnees)
{
	FILE *fp;
	fp=fopen(nom_fichier, "w");	
	
	if (fp != NULL)														//Actions to take if file successfully opens
	{
		writeMeta(&metadonnees, fp);
		fputs("P3", fp);
		fputs("\n", fp);
		
		char scolonnes[4], slignes[4], smaxval[7];
		
		writeInfo(scolonnes, colonnes, fp);
		fputs(" ", fp);
		
		writeInfo(slignes, lignes, fp);
		fputs("\n", fp);
		
		writeInfo(smaxval, maxval, fp);
		fputs("\n", fp);
		
		writeRGBMatrix(matrice, fp, &lignes, &colonnes);
	}
	
	else
	{
		return ERREUR_FICHIER;
	} 
	
	fclose(fp);
    return OK;
}

//PGM & PPM FUNCTIONS THAT COPY AN IMAGE FILE
int pgm_copier(int matrice1[MAX_HAUTEUR][MAX_LARGEUR], 
				int lignes1, int colonnes1, 
				int matrice2[MAX_HAUTEUR][MAX_LARGEUR], 
				int *p_lignes2, int *p_colonnes2)
{
	*p_lignes2=lignes1;
	*p_colonnes2=colonnes1;

	int value = 0;
	for (int i=0; i<lignes1;i++)
	{
		for (int j=0;j<colonnes1;j++)
		{
			value=matrice1[i][j];
			matrice2[i][j]=value;
		}
	}
    return OK;         
}

int ppm_copier(struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR], 
				int lignes1, int colonnes1, 
				struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR], 
				int *p_lignes2, int *p_colonnes2)
{
	*p_lignes2=lignes1;
	*p_colonnes2=colonnes1;

	int valueR = 0, valueG = 0, valueB = 0;
	
	for (int i=0; i<lignes1;i++)
	{
		for (int j=0;j<colonnes1;j++)
		{
			valueR=matrice1[i][j].valeurR;
			matrice2[i][j].valeurR=valueR;
			
			valueG=matrice1[i][j].valeurG;
			matrice2[i][j].valeurG=valueG;
			
			valueB=matrice1[i][j].valeurB;
			matrice2[i][j].valeurB=valueB;
		}
	}
    return OK;         
}

//PGM FUNCTION THAT CREATES HISTOGRAM OF IMAGE FILE
void resetHistogram(int histogramme[MAX_VALEUR+1])
{
	for (int counter = 0; counter<(MAX_VALEUR+1);counter++)
	{
		histogramme[counter]=0;
	}
}

int pgm_creer_histogramme(int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
						  int lignes, int colonnes, 
						  int histogramme[MAX_VALEUR+1])
{
	int colorIdx=0;
	
	resetHistogram(histogramme);
	
	for (int i=0;i<lignes;i++)
	{
		for (int j=0;j<colonnes;j++)
		{
			colorIdx = matrice[i][j];
			histogramme[colorIdx]+=1;
		}
	}
	return OK;
}

//PGM FUNCTION THAT FINDS THE COLOR WITH THE MOST OCCURENCE
int pgm_couleur_preponderante(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int lignes, int colonnes)
{
	int histogram[MAX_VALEUR+1];
	pgm_creer_histogramme(matrice, lignes, colonnes, histogram);
	
	int predominantColor = histogram[0];
	for (int k=0; k<(MAX_VALEUR+1);k++)
	{
		if (histogram[k]>histogram[predominantColor])
		{
			predominantColor=k;
		}
	}
	return predominantColor;
}

//PGM FUNCTION THAT BRIGHTENS OR DARKENS IMAGE
int pgm_eclaircir_noircir(int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
						  int lignes, int colonnes, int maxval, int valeur)
{
for (int i=0;i<lignes;i++)
	{
		for (int j=0;j<colonnes;j++)
		{
			matrice[i][j]+=valeur;
			if (matrice[i][j]<=0)
			{
				matrice [i][j]=0;
			}
			if (matrice[i][j]>maxval)
			{
				matrice[i][j]=maxval;
			}	
		}
	}
	return OK;
}

//PGM FUNCTION THAT TURNS IMAGE INTO A NEGATIVE
int pgm_creer_negatif(int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
					  int lignes, int colonnes, int maxval)
{
	for (int i=0;i<lignes;i++)
	{
		for (int j=0;j<colonnes;j++)
		{
			int pixelValue = matrice[i][j];
			pixelValue = maxval - pixelValue;
			matrice[i][j] = pixelValue;
		}
	}	
	return OK;
}

//PGM FUNCTION THAT EXTRACTS A SECTION OF THE IMAGE AND PUTS IN THE TOP LEFT CORNER

int pgm_extraire(int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
				 int lignes1, int colonnes1, int lignes2, int colonnes2, 
				 int *p_lignes, int *p_colonnes)
{

	if (*p_lignes <= 0 || *p_colonnes<=0)
		return -1;
	if (lignes1<0 || colonnes1<0)
		return -1;
	if (lignes2<0 || colonnes2<0)
		return -1;
	if (lignes1>*p_lignes || colonnes1>*p_colonnes)
		return -1;
	if (lignes2>*p_lignes || colonnes2>*p_colonnes)
		return -1;
	if (*p_lignes>MAX_HAUTEUR || *p_colonnes>MAX_LARGEUR)
		return -1;
		
	int linesOrigin = *p_lignes;
	int rowsOrigin = *p_colonnes;
	
	*p_lignes = (lignes2 - lignes1) + 1;
	*p_colonnes = (colonnes2 - colonnes1) + 1;	
	
	if (linesOrigin<*p_lignes || rowsOrigin<*p_colonnes)
		return -1;
		
	for (int i=0;i<*p_lignes;i++)
	{
		for (int j=0;j<*p_colonnes;j++)
		{
			matrice[i][j] = matrice[lignes1+i][colonnes1+j];	
		}
	}
	return OK;
}

//PGM & PGM FUNCTIONs THAT IDENTIFIES IF TWO IMAGES ARE IDENTICAL
int pgm_sont_identiques(int matrice1[MAX_HAUTEUR][MAX_LARGEUR], 
						int lignes1, int colonnes1, 
						int matrice2[MAX_HAUTEUR][MAX_LARGEUR], 
						int lignes2, int colonnes2)
{
	if (lignes1!=lignes2 || colonnes1!=colonnes2)
	{
		return DIFFERENTES;
	}
	else
	{	
		for (int i=0;i<lignes1;i++)
		{
			for (int j=0;j<colonnes1;j++)
			{
				if (matrice1[i][j]!=matrice2[i][j])
				{
					return DIFFERENTES;
				}	
			}
		}
	}		
	return IDENTIQUES;
}

int ppm_sont_identiques(struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR], 
						int lignes1, int colonnes1, 
						struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR], 
						int lignes2, int colonnes2)
{
	if (lignes1!=lignes2 || colonnes1!=colonnes2)
	{
		return DIFFERENTES;
	}
	else
	{	
		for (int i=0;i<lignes1;i++)
		{
			for (int j=0;j<colonnes1;j++)
			{
				if (matrice1[i][j].valeurR!=matrice2[i][j].valeurR)
				{
					return DIFFERENTES;
				}
				
				if (matrice1[i][j].valeurG!=matrice2[i][j].valeurG)
				{
					return DIFFERENTES;
				}
				
				if (matrice1[i][j].valeurB!=matrice2[i][j].valeurB)
				{
					return DIFFERENTES;
				}	
			}
		}
	}		
	return IDENTIQUES;
}


//PGM & PPM FUNCTIONS THAT ROTATE IMAGES 90 DEGREES
int pgm_pivoter90(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int *p_lignes, int *p_colonnes, int sens){
	
	int matriceTemp[MAX_HAUTEUR][MAX_LARGEUR];
	
	if(sens ==1){
		for(int x=0;x<*p_lignes;x++){
			for(int y=0;y<*p_colonnes;y++){
				matriceTemp[y][*p_lignes -1 -x] = matrice[x][y];
				
			}
		}
	}
	
	else if(sens==0){
		for(int x=0;x<*p_lignes;x++){
			for(int y=0;y<*p_colonnes;y++){
				matriceTemp[*p_colonnes-1 - y][x] = matrice[x][y];
			}
		}
	}
	else{
		return -1;
	}
	
	for(int x=0;x<MAX_HAUTEUR;x++){
		for(int y=0;y<MAX_LARGEUR;y++){
			matrice[x][y] = matriceTemp[x][y];
		}
	}
	

	int temp =0;
	temp = *p_lignes;
	*p_lignes = *p_colonnes;
	*p_colonnes = temp;
	return 0;
	
}

int ppm_pivoter90(struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], int *p_lignes, int *p_colonnes, int sens){
	
	struct RGB matriceTemp[MAX_HAUTEUR][MAX_LARGEUR];
	int valeur =0;
	if(sens ==1){
		for(int x=0;x<*p_lignes;x++){
			for(int y=0;y<*p_colonnes;y++){
				
				matriceTemp[y][*p_lignes -1 -x].valeurR = matrice[x][y].valeurR;
				matriceTemp[y][*p_lignes -1 -x].valeurG = matrice[x][y].valeurG;
				matriceTemp[y][*p_lignes -1 -x].valeurB = matrice[x][y].valeurB;
				
			}
		}
	}
	
	else if(sens==0){
		for(int x=0;x<*p_lignes;x++){
			for(int y=0;y<*p_colonnes;y++){
				valeur = matrice[x][y].valeurR;
				matriceTemp[*p_colonnes-1 - y][x].valeurR = valeur;
				valeur = matrice[x][y].valeurG;
				matriceTemp[*p_colonnes-1 - y][x].valeurG = valeur;
				valeur = matrice[x][y].valeurB;
				matriceTemp[*p_colonnes-1 - y][x].valeurB = valeur;
			}
		}
	}
	else{
		return -1;
	}
	
	for(int x=0;x<MAX_HAUTEUR;x++){
		for(int y=0;y<MAX_LARGEUR;y++){
			matrice[x][y].valeurR = matriceTemp[x][y].valeurR;
			matrice[x][y].valeurG = matriceTemp[x][y].valeurG;
			matrice[x][y].valeurB = matriceTemp[x][y].valeurB;
		}
	}
	int temp =0;
	temp = *p_lignes;
	*p_lignes = *p_colonnes;
	*p_colonnes = temp;
	return 0;
	
}
