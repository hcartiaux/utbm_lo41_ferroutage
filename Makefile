CC=gcc
CFLAGS=-Wall -lrt
EXEC=controleur file_camion terminaux train

all: $(EXEC)

controleur: controleur.c comm_shm.o semaphore.o
	$(CC) -o controleur controleur.c comm_shm.o semaphore.o $(CFLAGS)

train: train.c comm_shm.o semaphore.o
	$(CC) -o train train.c comm_shm.o semaphore.o $(CFLAGS)

file_camion: file_camion.c comm_shm.o semaphore.o
	$(CC) -o file_camion file_camion.c comm_shm.o semaphore.o $(CFLAGS)

terminaux: terminaux.c comm_shm.o semaphore.o
	$(CC) -o terminaux terminaux.c comm_shm.o semaphore.o $(CFLAGS)

comm_shm.o: comm_shm.c comm_shm.h
	$(CC) -c comm_shm.c comm_shm.h $(CFLAGS)

semaphore.o: semaphore.c semaphore.h
	$(CC) -c semaphore.c semaphore.h $(CFLAGS)

test: test.c comm_shm.o semaphore.o
	$(CC) -o test test.c comm_shm.o $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

	