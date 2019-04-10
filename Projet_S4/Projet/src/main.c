#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include "../include/entrees.h"
#include "../include/structure.h"
#include "../include/administrateur.h"
#include "../include/clients_administrateur.h"
#include "../include/menu.h"
#include "../include/client.h"
#include "../include/fichier.h" 
#include "../include/entrees.h"

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

void message() { //Fonction qui affiche un message 
        printf(RED "\nN'essayez pas de fermer ce programme autrement que par le menu ça ne marchera pas\n" RESET);
}

//Main
int main(int argc, char* argv[]){
		//Fonction qui bloque les ctrl+C, ctrl+Z, ... : 
		//Cela oblige l'utilisateur a quitter le programme par le menu et a sauvegarder la banque 
        signal(SIGQUIT, message); 
        signal(SIGINT, message);
        signal(SIGTSTP, message);
        Banque B = initialiserBanque(); //Initialise la banque 
        chargementJSON(B); //Charge toutes les informations de la banque à partir de fichiers .json 
        menuPrincipal(B); //Menu principal qui contient l'interface 
       	fermetureJSON(B); //Enregistre toutes les informations de la banque dans des fichiers .json 
        return 0;
}