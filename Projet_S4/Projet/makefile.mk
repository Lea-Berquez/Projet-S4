#construction du main

bin/all : lib/entrees.o lib/structure.o lib/administrateur.o lib/client.o lib/menu.o lib/clients_administrateur.o lib/parson.o lib/fichier.o lib/main.o
	gcc -o bin/banque lib/entrees.o lib/structure.o lib/administrateur.o lib/client.o lib/menu.o lib/clients_administrateur.o lib/parson.o lib/fichier.o lib/main.o -lm 

lib/entrees.o : src/entrees.c 
	gcc -c src/entrees.c -Wall -o lib/entrees.o -lm

lib/structure.o : src/structure.c
	gcc -c src/structure.c -Wall -o lib/structure.o

lib/administrateur.o : src/administrateur.c
	gcc -c src/administrateur.c -Wall -o lib/administrateur.o

lib/client.o : src/client.c 
	gcc -c src/client.c -Wall -o lib/client.o 

lib/clients_administrateur.o : src/clients_administrateur.c 
	gcc -c src/clients_administrateur.c -Wall -o lib/clients_administrateur.o

lib/parson.o : src/parson.c 
	gcc -c src/parson.c -Wall -o lib/parson.o 

lib/menu.o : src/menu.c
	gcc -c src/menu.c -Wall -o lib/menu.o

lib/fichier.o : src/fichier.c 
	gcc -c src/fichier.c -Wall -o lib/fichier.o 

lib/main.o : src/main.c
	gcc -c src/main.c -Wall -o lib/main.o 