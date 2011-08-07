/*
Projet LO41, printemps 2008
Fichier : comm_shm.h
File de camions, dispatchée sur les terminaux
Auteurs : Jean-Christophe HERR, Hyacinthe CARTIAUX
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h> /* standard system data types.       */
#include <sys/ipc.h>   /* common system V IPC structures.   */
#include <sys/msg.h>   /* message-queue specific functions. */
#include "comm_shm.h"
#include "semaphore.h"

/* Structure décrivant un camion et envoyée comme message sur les files */
struct camion_buf {
	long mtype; /* 1 tracteur, 2 remorque, 3 tracteur + remorque */
	int id; 
};

/* Génération aléatoire d'un camion */
struct camion_buf * generation_camion();

struct camion_buf * c;

/* Envoi d'un camion sur la file passée en paramètre */
void envoi_sur_file_camion(struct camion_buf * c, int id_file);
void fin_processus();

int main(int argc,char **argv)
{

	struct terminaux_info * ti = get_terminaux_info_addr();

	struct processus_info * pi = get_processus_info_addr();
	pi->p_file_camion = getpid();

	signal(SIGINT, fin_processus); /* appel de la fonction fin_processus sur réception d'un signal SIGINT */

	int qid;

	initGet(18, "lo41"); /* Récupération des sémaphores */


	srand (time (NULL));

	while (1)
	{
		P(S_GEN_CAMION); /* Attente de l'autorisation de remplissage de file envoyée par un terminal */
		c = generation_camion(); /* Génération aléatoire d'un camion */

		printf("Remplissage File Terminal %d : id %d, type %d\n", ti->term_to_load, c->id, (int) c->mtype);

		/* Sélection de la file en fonction du terminal à charger */
		if (ti->term_to_load == 1)
		{
			qid = ti->file_t1;
		}
		else if (ti->term_to_load == 2)
		{
			qid = ti->file_t2;
		}
		else if (ti->term_to_load == 3)
		{
			qid = ti->file_t3;
		}
		else if (ti->term_to_load == 4)
		{
			qid = ti->file_t4;
		}
		else if (ti->term_to_load == 5)
		{
			qid = ti->file_t5;
		}
		else if (ti->term_to_load == 6)
		{
			qid = ti->file_t6;
		}
		else if (ti->term_to_load == 7)
		{
			qid = ti->file_t7;
		}
		else if (ti->term_to_load == 8)
		{
			qid = ti->file_t8;
		}
		else if (ti->term_to_load == 9)
		{
			qid = ti->file_t9;
		}
		else if (ti->term_to_load == 10)
		{
			qid = ti->file_t10;
		}
		else if (ti->term_to_load == 11)
		{
			qid = ti->file_t11;
		}
		else if (ti->term_to_load == 12)
		{
			qid = ti->file_t12;
		}

		/* Envoie du camion sur la file sélectionnée */
		envoi_sur_file_camion(c, qid);

		/* On redonne la main au terminal qui a appelé file */
		V(S_TERMINAUX);

		/* libération de la mémoire */
		free(c);
		c = NULL;

		
	}

	return 0;
}

struct camion_buf * generation_camion()
{
	static int id = 0;

	/* On alloue une zone de mémoire pour une structure camion_buf */
	struct camion_buf * c1 = (struct camion_buf *) malloc(sizeof(struct camion_buf)); 

	c1->id = ++id;
	c1->mtype =  rand () % 3 + 1;	/* le nombre est soit
					 * 1 => tracteur
					 * 2 => remorque
					 * 3 => remorque + tracteur
					 */

	return c1;
}

void envoi_sur_file_camion(struct camion_buf * c, int id_file)
{

	/* On envoie la structure c sur la file id_file */
	int rc = msgsnd(id_file, c, sizeof(*c) - sizeof(long), 0);

	if (rc == -1) {
		perror("msgsnd");
		exit(1);
	}
	
}

void fin_processus()
{
	if (c != NULL)
		free(c);
	printf("\nArrêt\n");
	exit(0);
}
