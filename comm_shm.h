/*
Projet LO41, printemps 2008
Fichier : comm_shm.h
Fonctions pour la communication par shm
Auteurs : Jean-Christophe HERR, Hyacinthe CARTIAUX
*/

#include <unistd.h>

#define NBR_WAGON_MAX 12
#define KEY 100

/* Information des terminaux */
struct terminaux_info
{
	/* Id des files de messages */
	int file_t1, file_t2, file_t3, file_t4, file_t5, file_t6, file_t7, file_t8, file_t9, file_t10, file_t11, file_t12;

	/* Type de camion sur le terminal, en tête de file */
	int nb_units_t1, nb_units_t2, nb_units_t3, nb_units_t4, nb_units_t5, nb_units_t6, nb_units_t7, nb_units_t8, nb_units_t9, nb_units_t10, nb_units_t11, nb_units_t12;

	/* Terminal à charger */
	int term_to_load;
};

/* Information des processus */
struct processus_info
{
	pid_t p_terminaux, p_train, p_file_camion, p_controleur;
	pid_t pid_t1, pid_t2, pid_t3, pid_t4, pid_t5, pid_t6, pid_t7, pid_t8, pid_t9, pid_t10, pid_t11, pid_t12;
};

/* Structure décrivant un wagon (deux places) */
struct wagon
{
	/* id1 et id2, initialisé à -1 si vide, puis remplacé par l'id du camion occupant cette place */
	int id1, id2;
};

/* Structure décrivant le train */
struct train_info
{
	/* id du train */
	int id_train;

	/* Nombre de wagons */
	int nbr_wagon;

	/* Tables de wagons */
	struct wagon w[NBR_WAGON_MAX];
};

/* Récupération de l'adresse de la shm */
char * get_shm_adr();

/* Destruction de la shm */
void shm_destroy(); 

/* Retourne l'adresse de la structure terminaux_info en shm */
extern struct terminaux_info * get_terminaux_info_addr(); 

/* Retourne l'adresse de la structure processus_info en shm */
extern struct processus_info * get_processus_info_addr();

/* Retourne l'adresse de la structure terminaux_info en shm */
extern struct train_info * get_train_info_addr();
