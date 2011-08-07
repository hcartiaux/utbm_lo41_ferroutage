# UTBM, Projet de LO41, programmation système et simulation de ferroutage

Le principe de la simulation est simple : 12 terminaux d'accueils symbolisés par des files de messages, reçoivent chacun des camions et leurs remorques destinés à être chargés sur un train. Quand le train est entièrement chargé, un nouveau train arrive en gare !

Ce projet met en oeuvre quelques concepts importants de programmation système : la création de processus (fork) et les IPC (signaux, mémoire partagée shm, sémaphores et files de messages).

Le programme est composé de 4 processus parallèles : 

* un contrôleur, affiche le chargement du train pas à pas ;
* le train, affiche son arrivée, son remplissage et son départ ;
* le processus file\_camion, un générateur de camions, qui permet de remplir les terminaux.
* le processus terminaux, gère les terminaux de chargement, chacun étant un processus associé à une file.

Le programme a été testé avec succès sous Linux et Solaris. Le code du fichier semaphore.c nous a été donné par le professeur.

Pour compiler le programme, compilez avec make : 

    $ git clone git://github.com/hcartiaux/utbm_lo41_ferroutage.git 
    $ cd utbm_lo41_ferroutage
    $ make

Pour le lancer, sur 4 terminaux, lancez les commandes suivantes : 

    $ ./controleur
    $ ./train
    $ ./terminaux
    $ ./file_camion

Arrêter le processus controleur en le tuant (via ctrl+c ou kill) arrête tous les autres processus.

