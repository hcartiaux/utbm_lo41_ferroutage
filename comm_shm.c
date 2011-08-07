/*
Projet LO41, printemps 2008
Fichier : comm_shm.c
Fonctions pour la communication par shm
Auteurs : Jean-Christophe HERR, Hyacinthe CARTIAUX
*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#include "comm_shm.h"

/* Retourne la taille de la shm */
int shm_size()
{
	return sizeof(struct train_info) + sizeof(struct terminaux_info) + sizeof(struct processus_info);
}

/* Retourne l'id de la shm */ 
int get_shm_id()
{
	static int shm_id = -1;

	/* Création de la shm */
	if (shm_id == -1)
	{
		shm_id = shmget(KEY, shm_size(), IPC_CREAT | 0600);
		if (shm_id == -1) {  /* Echec */
			perror("shmget: ");
			exit(1);
		}
	}

	return shm_id;
	
}

char * get_shm_adr()
{
	static char *shm_addr;

	/* Récupération de l'adresse de la SHM */
	if (shm_addr == NULL)
	{
	
		shm_addr = shmat(get_shm_id(), NULL, 0);
		
		if (!shm_addr) { /* Echec */
			perror("shmat: ");
			exit(1);
		}

	}

	return shm_addr;
}

void shm_destroy()
{

	struct shmid_ds shm_desc;
	
	/* Destruction de la SHM */
	if (shmctl(get_shm_id(), IPC_RMID, &shm_desc) == -1) { /* Echec */
		perror("main: shmctl: ");
	}

}

struct terminaux_info * get_terminaux_info_addr()
{
	char * shm_adr = get_shm_adr();
	/* Cast de l'adresse de base de la shm */
	return (struct terminaux_info *) shm_adr; 
}

struct processus_info * get_processus_info_addr()
{
	char * shm_adr = get_shm_adr();
	 /* Cast et calcul de l'adresse de processus_info dans la shm */
	return (struct processus_info *) (shm_adr + sizeof(struct terminaux_info));
}

struct train_info * get_train_info_addr()
{
	char * shm_adr = get_shm_adr();
	 /* Cast et calcul de l'adresse de train_info dans la shm */
	return (struct train_info *) (shm_adr + sizeof(struct terminaux_info) + sizeof(struct processus_info));
}
