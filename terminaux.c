/*
Projet LO41, printemps 2008
Fichier : comm_shm.h
Terminaux de chargement 
Auteurs : Jean-Christophe HERR, Hyacinthe CARTIAUX
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#include "comm_shm.h"
#include "semaphore.h"

struct camion_buf {
	long m_type; /* 0 remorque, 1 tracteur, 2 tracteur + remorque */
	int id; 
};

/* Création de la i-ème file, et enregistrement de son id en shm */
int creation_file(int i);

/* Lit un message (struct camion_buf) de la file */
struct camion_buf * lecture_file(int num_file);

/* Code du terminal */
void terminal(int i);

/* Chargement du train */
void chargement_train(struct train_info * t, struct camion_buf * c);

/* Fonction appelée lors de l'arrêt du processus, tue tous les processus fils */
void fin_processus();

int main(int argc,char **argv)
{

	struct processus_info * p = get_processus_info_addr();
	struct terminaux_info * t = get_terminaux_info_addr();
	
	p->p_terminaux = getpid();
	
	int j;

	initGet(18, "lo41");

	/* création des 12 files*/
	for(j = 1; j < 13; j++)
	{
		creation_file(j);
	}

	/* On remplit les files de chaque terminal de 3 camions */
	for (j = 0; j < 36; j++)
	{
		t->term_to_load = j % 12 + 1;
		V(S_GEN_CAMION);
		P(S_TERMINAUX);
	}

	/* On lance les 12 terminaux, pour les mettre en attente d'autorisation de chargement du contrôleur*/
	for (j = 0; j < 12; j++)
	{
		V(S_TERMINAUX);
	}
	
	sleep(1);

	/* Lancement des terminaux dans des processus fils, créés par fork() */
	if ( ( p->pid_t1 = fork() ) == 0 ) { p->pid_t1 = getpid(); terminal(1); exit(0); }
	if ( ( p->pid_t2 = fork() ) == 0 ) { p->pid_t2 = getpid(); terminal(2); exit(0); }
	if ( ( p->pid_t3 = fork() ) == 0 ) { p->pid_t3 = getpid(); terminal(3); exit(0); }
	if ( ( p->pid_t4 = fork() ) == 0 ) { p->pid_t4 = getpid(); terminal(4); exit(0); }
	if ( ( p->pid_t5 = fork() ) == 0 ) { p->pid_t5 = getpid(); terminal(5); exit(0); }
	if ( ( p->pid_t6 = fork() ) == 0 ) { p->pid_t6 = getpid(); terminal(6); exit(0); }
	if ( ( p->pid_t7 = fork() ) == 0 ) { p->pid_t7 = getpid(); terminal(7); exit(0); }
	if ( ( p->pid_t8 = fork() ) == 0 ) { p->pid_t8 = getpid(); terminal(8); exit(0); }
	if ( ( p->pid_t9 = fork() ) == 0 ) { p->pid_t9 = getpid(); terminal(9); exit(0); }
	if ( ( p->pid_t10 = fork() ) == 0 ) { p->pid_t10 = getpid(); terminal(10); exit(0); }
	if ( ( p->pid_t11 = fork() ) == 0 ) { p->pid_t11 = getpid(); terminal(11); exit(0); }
	if ( ( p->pid_t12 = fork() ) == 0 ) { p->pid_t12 = getpid(); terminal(12); exit(0); }

	/* Sémaphore initialisation passée */
	V(S_INIT);

	signal(SIGINT, fin_processus);

	/* Attente des processus fils */
	waitpid(p->pid_t1, NULL, 0);
	waitpid(p->pid_t2, NULL, 0);
	waitpid(p->pid_t3, NULL, 0);
	waitpid(p->pid_t4, NULL, 0);
	waitpid(p->pid_t5, NULL, 0);
	waitpid(p->pid_t6, NULL, 0);
	waitpid(p->pid_t7, NULL, 0);
	waitpid(p->pid_t8, NULL, 0);
	waitpid(p->pid_t9, NULL, 0);
	waitpid(p->pid_t10, NULL, 0);
	waitpid(p->pid_t11, NULL, 0);
	waitpid(p->pid_t12, NULL, 0);

	return 0;
}

int creation_file(int i)
{
	struct terminaux_info * t = get_terminaux_info_addr();
	key_t cle = ftok ("/etc", (char) i);

	/* création de la i-ème file et enregistrement de son id dans la shm */
	switch (i) {
		case 1: return t->file_t1 = msgget(cle,IPC_CREAT | 0600); break;
		case 2: return t->file_t2 = msgget(cle,IPC_CREAT | 0600); break;
		case 3: return t->file_t3 = msgget(cle,IPC_CREAT | 0600); break;
		case 4: return t->file_t4 = msgget(cle,IPC_CREAT | 0600); break;
		case 5: return t->file_t5 = msgget(cle,IPC_CREAT | 0600); break;
		case 6: return t->file_t6 = msgget(cle,IPC_CREAT | 0600); break;
		case 7: return t->file_t7 = msgget(cle,IPC_CREAT | 0600); break;
		case 8: return t->file_t8 = msgget(cle,IPC_CREAT | 0600); break;
		case 9: return t->file_t9 = msgget(cle,IPC_CREAT | 0600); break;
		case 10: return t->file_t10 = msgget(cle,IPC_CREAT | 0600); break;
		case 11: return t->file_t11 = msgget(cle,IPC_CREAT | 0600); break;
		case 12: return t->file_t12 = msgget(cle,IPC_CREAT | 0600); break;
		default: return -1; break;
	}

}

struct camion_buf * lecture_file(int num_file)
{
	struct camion_buf * c = (struct camion_buf *) malloc(sizeof(struct camion_buf));

	/* Lecture d'un camion sur la file num_file */
	if (msgrcv(num_file, c, sizeof(struct camion_buf) - sizeof(long), 0, 0) == -1) { /* Erreur */
    		perror("msgrcv");
    		exit(1);
	}
	
	return c;
}

void terminal(int i)
{
	/* Récupération des structures en shm */
	struct terminaux_info * t = get_terminaux_info_addr();
	struct train_info * train_info = get_train_info_addr();
	
	struct camion_buf * c;
	
	while (1)
	{
		P(S_TERMINAUX); // les files sont pleines

		/* On lit le premier camion de la file du terminal, et on se place en attente d'autorisation de chargement par le contrôleur */
		switch (i) {
			case 1: c = lecture_file(t->file_t1);
				t->nb_units_t1 = c->m_type ;
				t->term_to_load = 1;
				P(S_FILE_1); break;
			case 2: c = lecture_file(t->file_t2);
				t->nb_units_t2 = c->m_type ;
				t->term_to_load = 2;
				P(S_FILE_2); break;
			case 3: c = lecture_file(t->file_t3);
				t->nb_units_t3 = c->m_type ;
				t->term_to_load = 3;
				P(S_FILE_3); break;
			case 4: c = lecture_file(t->file_t4);
				t->nb_units_t4 = c->m_type ;
				t->term_to_load = 4;
				P(S_FILE_4); break;
			case 5: c = lecture_file(t->file_t5);
				t->nb_units_t5 = c->m_type ;
				t->term_to_load = 5;
				P(S_FILE_5); break;
			case 6: c = lecture_file(t->file_t6);
				t->nb_units_t6 = c->m_type ;
				t->term_to_load = 6;
				P(S_FILE_6); break;
			case 7: c = lecture_file(t->file_t7);
				t->nb_units_t7 = c->m_type ;
				t->term_to_load = 7;
				P(S_FILE_7); break;
			case 8: c = lecture_file(t->file_t8);
				t->nb_units_t8 = c->m_type ;
				t->term_to_load = 8;
				P(S_FILE_8); break;
			case 9: c = lecture_file(t->file_t9);
				t->nb_units_t9 = c->m_type ;
				t->term_to_load = 9;
				P(S_FILE_9); break;
			case 10: c = lecture_file(t->file_t10);
				t->nb_units_t10 = c->m_type ;
				t->term_to_load = 10;
				P(S_FILE_10); break;
			case 11: c = lecture_file(t->file_t11);
				t->nb_units_t11 = c->m_type ;
				t->term_to_load = 11;
				P(S_FILE_11); break;
			case 12: c = lecture_file(t->file_t12);
				t->nb_units_t12 = c->m_type ;
				t->term_to_load = 12;
				P(S_FILE_12); break;

		}

		V(S_GEN_CAMION); /* On envoie un camion sur la file lue */
		usleep(1000000);
		
		/* Affichage */
		if(c->m_type == 1) printf("terminal %d : tracteur %d\n", i, c->id);
		if(c->m_type == 2) printf("terminal %d : remorque %d\n", i, c->id);
		if(c->m_type == 3) printf("terminal %d : tracteur+remorque %d\n", i, c->id);

		// chargement du train

		chargement_train(train_info, c);

		free(c); /* Libération de la mémoire */

		V(S_CONTROLLEUR); /* Chargement fini, on redonne la main au contrôleur */
	}
}

void chargement_train(struct train_info * t, struct camion_buf * c)
{
	int j;
	int wagon[2];
	int demiwagon = -1;

	wagon[0] = -1;
	wagon[1] = -1;

	for (j = 0; j < t->nbr_wagon; j++)
	{
		if (t->w[j].id1 == -1 && t->w[j].id2 == -1)
		{
			if (wagon[0] == -1)
				wagon[0] = j;
			else if (wagon[1] == -1)
				wagon[1] = j;
		}
		else if (t->w[j].id1 == -1 || t->w[j].id2 == -1)
		{
			demiwagon = j;
		}
	}

	if (c->m_type == 3)
	{
		if (demiwagon != -1 && wagon[0] != -1)
		{
			if (t->w[demiwagon].id1 == -1)
			{
				t->w[demiwagon].id1 = c->id;
			}
			else if (t->w[demiwagon].id2 == -1)
			{
				t->w[demiwagon].id2 = c->id;
			}
		}
		else if (demiwagon != -1 && wagon[0] != -1)
		{
			if (t->w[demiwagon].id1 == -1)
			{
				t->w[demiwagon].id1 = c->id;
			}
			else if (t->w[demiwagon].id2 == -1)
			{
				t->w[demiwagon].id2 = c->id;
			}

			t->w[wagon[0]].id1 = c->id;
			t->w[wagon[0]].id2 = c->id;
		}
		else if (wagon[0] != -1 && wagon[1] != -1)
		{
			t->w[wagon[0]].id1 = c->id;
			t->w[wagon[0]].id2 = c->id;
			t->w[wagon[1]].id1 = c->id;

		}
	}
	else if (wagon[0] != -1 && c->m_type == 2)
	{
  		t->w[wagon[0]].id1 = c->id;
  		t->w[wagon[0]].id2 = c->id;
	}
	else if (c->m_type == 1 && demiwagon != -1)
	{
		if (t->w[demiwagon].id1 == -1)
		{
			t->w[demiwagon].id1 = c->id;
		}
		else if (t->w[demiwagon].id2 == -1)
		{
			t->w[demiwagon].id2 = c->id;
		}
	}
	
}

void fin_processus()
{
	struct processus_info * pi = get_processus_info_addr();

	/* On termine les processus fils */
	kill(pi->pid_t1, SIGINT);
	kill(pi->pid_t2, SIGINT);
	kill(pi->pid_t3, SIGINT);
	kill(pi->pid_t4, SIGINT);
	kill(pi->pid_t5, SIGINT);
	kill(pi->pid_t6, SIGINT);
	kill(pi->pid_t7, SIGINT);
	kill(pi->pid_t8, SIGINT);
	kill(pi->pid_t9, SIGINT);
	kill(pi->pid_t10, SIGINT);
	kill(pi->pid_t11, SIGINT);
	kill(pi->pid_t12, SIGINT);

	printf("\nArrêt des terminaux\n");
	exit(0);
}
