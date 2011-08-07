/*
Projet LO41, printemps 2008
Fichier : controleur.c
Contrôleur de la simulation de ferroutage
Auteurs : Jean-Christophe HERR, Hyacinthe CARTIAUX
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "comm_shm.h"
#include "semaphore.h"

int choix_terminal(); /* Détermine quel terminal embarquer */
int chargement_possible(int i);
int get_nbunits_by_file(int file);
int recherche_file(int m_type);
void suppression_file(); /* Permet de supprimer les files de messages en mémoire */
void fin_processus(); /* Nettoie la mémoire à la fin du processus controleur */
void affichage_train(struct train_info * t);

int main(int argc,char **argv)
{
	int term;
	struct processus_info * pi = get_processus_info_addr();
	struct train_info * t = get_train_info_addr();
	pi->p_controleur = getpid();

	signal(SIGINT, fin_processus); /* appel de la fonction fin_processus sur réception d'un signal SIGINT (terminaison du processus avec ctrl+c) */

	srand (time (NULL)); /* initialisation du générateur de nombres pseudo aléatoires */

	initSem(18, "lo41", NULL); /* création de toutes les sémaphores */


	P(S_INIT); /* les 12 files des terminaux sont créées et remplies par le processus file_camion */


	sleep(1); /* attente de lecture des files */

	while (1)
	{

		P(S_NOUVEAU_TRAIN);  /* Le train est arrivé en gare */

		/* optimisation du chargement des wagons  */
		while ((term = choix_terminal()) != -1) /* détermination d'un terminal à embarquer, retourne -1 quand le train est rempli au mieux */
		{

			/* S autorisation chargement des 12 files */
			switch (term) {
				case 1: V(S_FILE_1); break;
				case 2: V(S_FILE_2); break;
				case 3: V(S_FILE_3); break;
				case 4: V(S_FILE_4); break;
				case 5: V(S_FILE_5); break;
				case 6: V(S_FILE_6); break;
				case 7: V(S_FILE_7); break;
				case 8: V(S_FILE_8); break;
				case 9: V(S_FILE_9); break;
				case 10: V(S_FILE_10); break;
				case 11: V(S_FILE_11); break;
				case 12: V(S_FILE_12); break;
			}
			P(S_CONTROLLEUR); /* attente de la fin du chargement */

			affichage_train(t);

		}


		V(S_TRAIN); /* Autorisation de départ du train */
	}
	
	return 0;
}

int choix_terminal()
{
	static int file = 0;

	while(chargement_possible(0) != -1)
	{
		if (file == 12)
			file = 0;

		file++;
	
		if (chargement_possible(get_nbunits_by_file(file)) == 0)
			return file;

	}
	
	return -1;
}



int chargement_possible(int i)
{
	int j;

	int wagon = 0, demiwagon = 0;
	
	struct train_info * t = get_train_info_addr(); /* récupération de la structure train_info en shm */

	for (j = 0; j < t->nbr_wagon; j++)
	{
		if (t->w[j].id1 == -1 && t->w[j].id2 == -1)
		{
			wagon++;
		}
		else if (t->w[j].id1 == -1 || t->w[j].id2 == -1)
		{
			demiwagon++;
		}
	}

	if (i == 0)
	{
		if (	(demiwagon > 0 && recherche_file(1) != -1)
			|| (wagon > 0 && (recherche_file(2) != -1 || recherche_file(1) != -1))
			|| (wagon > 0 && demiwagon > 0 && recherche_file(3) != -1)
			|| (wagon > 1 && recherche_file(3) != -1)
		)
		return 0;
	}
	else
	{
		if (	(demiwagon > 0 && i == 1)
			|| (wagon > 0 && (i == 2 || i == 1))
			|| (wagon > 0 && demiwagon > 0 && i == 3)
			|| (wagon > 1 && i == 3)
		)
		return 0;
	}

	return -1;

}

int get_nbunits_by_file(int file)
{
	struct terminaux_info * te = get_terminaux_info_addr();

	switch (file) {
		case 1: return te->nb_units_t1; break;
		case 2: return te->nb_units_t2; break;
		case 3: return te->nb_units_t3; break;
		case 4: return te->nb_units_t4; break;
		case 5: return te->nb_units_t5; break;
		case 6: return te->nb_units_t6; break;
		case 7: return te->nb_units_t7; break;
		case 8: return te->nb_units_t8; break;
		case 9: return te->nb_units_t9; break;
		case 10: return te->nb_units_t10; break;
		case 11: return te->nb_units_t11; break;
		case 12: return te->nb_units_t12; break;
	}

	return -1;
}

int recherche_file(int m_type)
{
	struct terminaux_info * te = get_terminaux_info_addr(); /* récupération de la structure terminaux_info en shm */

	int j;

	/* On recherche un terminal avec un camion de type m_type 1 par 1 sur les 12 terminaux */
	for (j = 1; j < 13; j++)
	{

		switch (j) {
			case 1: if (te->nb_units_t1 == m_type) return 1; break;
			case 2: if (te->nb_units_t2 == m_type) return 2; break;
			case 3: if (te->nb_units_t3 == m_type) return 3; break;
			case 4: if (te->nb_units_t4 == m_type) return 4; break;
			case 5: if (te->nb_units_t5 == m_type) return 5; break;
			case 6: if (te->nb_units_t6 == m_type) return 6; break;
			case 7: if (te->nb_units_t7 == m_type) return 7; break;
			case 8: if (te->nb_units_t8 == m_type) return 8; break;
			case 9: if (te->nb_units_t9 == m_type) return 9; break;
			case 10: if (te->nb_units_t10 == m_type) return 10; break;
			case 11: if (te->nb_units_t11 == m_type) return 11; break;
			case 12: if (te->nb_units_t12 == m_type) return 12; break;
		}

	}

	return -1;
}

void suppression_file()
{
	struct terminaux_info * t = get_terminaux_info_addr();

	msgctl (t->file_t1, IPC_RMID, NULL); /* suppression de la file du terminal 1 */
	msgctl (t->file_t2, IPC_RMID, NULL);
	msgctl (t->file_t3, IPC_RMID, NULL);
	msgctl (t->file_t4, IPC_RMID, NULL);
	msgctl (t->file_t5, IPC_RMID, NULL);
	msgctl (t->file_t6, IPC_RMID, NULL);
	msgctl (t->file_t7, IPC_RMID, NULL);
	msgctl (t->file_t8, IPC_RMID, NULL);
	msgctl (t->file_t9, IPC_RMID, NULL);
	msgctl (t->file_t10, IPC_RMID, NULL);
	msgctl (t->file_t11, IPC_RMID, NULL);
	msgctl (t->file_t12, IPC_RMID, NULL);
}

void kill_process()
{
	struct processus_info * pi = get_processus_info_addr();
	kill(pi->p_file_camion, SIGINT);
	kill(pi->p_train, SIGINT);
	kill(pi->p_terminaux, SIGINT);
}

void fin_processus()
{
	kill_process();
	suppression_file(); /* suppression des files */
	shm_destroy(); /* suppression des shm */
	libereSem(); /* suppression des sémaphores */

	printf("\nArrêt\n");
	exit(0);

}

void affichage_train(struct train_info * t)
{
	int i;

	printf("Train : %d, Nombre de wagon : %d\n\n", t->id_train, t->nbr_wagon);

	for (i = 0; i < t->nbr_wagon; i++)
	{
		printf("Wagon n° %d, Place 1 :%d, Place 2 : %d\n\n", i, t->w[i].id1, t->w[i].id2);
	}
}
