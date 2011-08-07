#define S_INIT 0 /* Attente de lancement des processus */
#define S_FILE_1 1 /* Le terminal 1 est autorisée à chargé dans le train */
#define S_FILE_2 2
#define S_FILE_3 3
#define S_FILE_4 4
#define S_FILE_5 5
#define S_FILE_6 6
#define S_FILE_7 7
#define S_FILE_8 8
#define S_FILE_9 9
#define S_FILE_10 10
#define S_FILE_11 11
#define S_FILE_12 12
#define S_CONTROLLEUR 13 /* Fin de chargement d'un terminal, le controleur continue */
#define S_TRAIN 14 /* Départ du train */
#define S_GEN_CAMION 15 /* Démarrage du générateur de camions et remplissage des files */
#define S_TERMINAUX 16 /* File chargée 1 fois */
#define S_NOUVEAU_TRAIN 17 /* Nouveau train vide arrivé en gare */

/* semaphore.h */
extern void erreurFin(const char* msg);
extern void initGet(int nbSem,char* argv0);
extern void initSem(int nbSem,char* nomFich,int* pvinit);
extern void libereSem();
extern void P(int numSem);
extern void V(int numSem);
