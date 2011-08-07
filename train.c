/*
Projet LO41, printemps 2008
Fichier : comm_shm.h
Simulation des trains
Auteurs : Jean-Christophe HERR, Hyacinthe CARTIAUX
*/

#include <stdio.h>
#include "comm_shm.h"
#include "semaphore.h"

/* Fonction d'affichage du train et du contenu de ses wagons */
void affichage_train(struct train_info * t);

int main(int argc,char **argv)
{
	int id_train = 0;
	int i;
	
	struct train_info * t = get_train_info_addr(); /* On récupère la structure train_info en shm */

	struct processus_info * pi = get_processus_info_addr();
	pi->p_train = getpid();
	
	initGet(18, "lo41");

	while (1)
	{

		/* Réinitialisation du train en shm*/
		t->id_train = id_train++;
		t->nbr_wagon = NBR_WAGON_MAX;

		for (i = 0; i < t->nbr_wagon; i++)
		{
			t->w[i].id1 = -1;
			t->w[i].id2 = -1;
		}

		printf("Nouveau train en gare\n");

		V(S_NOUVEAU_TRAIN); /* On prévient le contrôleur qu'un nouveau train vient d'arriver*/

		P(S_TRAIN); /* Attente de la fin de chargement du train */

		affichage_train(t);

		printf("Départ du train\n\n\n");

		sleep(1);

	}

	return 0;
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
