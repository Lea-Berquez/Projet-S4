#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h> 
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "../include/entrees.h"

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

int lire_fin_ligne(){ //Fonction qui permet de supprimer les espaces 
    char c ;
    c=getchar(); 
    int cp;
    cp=0; 
    while(c!='\n')
    {
        if (isspace(c))
        {
        	c=getchar(); 
        	continue; 
        }
        else 
        {
        	c=getchar(); 
            cp=cp+1;
        }
    }
    return cp; //Renvoie la taille de mot 
}

int lire_format(char*str, void*adr){ //Fonction qui permet de lire quelque chose rentrer au clavier suivant un format passé en parametre 
    int ret1;
    int ret2;
    do 
	{
		ret1=scanf(str, adr); //Demande à l'utilisateur ce qu'il souhaite écrire 
		ret2=lire_fin_ligne(); //Lit tout les caractères entrer par l'utilisateur 
		if (ret1!=1 || ret2>0) //Regarde si ce qui est entrer par l'utilisateur est bien au bon format 
		{
		    printf(RED "Saisie non conforme à l'attente, veuillez réessayer\n" RESET);
		}
	}while(ret2>0 || ret1!=1); //Regarde si ce qui est entrer par l'utilisateur est bien au bon format  
	return 0; 
}

int lire_decimal(float*dec){ //Fonction qui permet de lire un décimal 
    lire_format("%f", dec); 
    return 0;
}

int lire_entier(int*ent){ //Fonction qui permet de lire un entier 
    lire_format("%d", ent); 
    return 0; 
}

int lire_string(char** str){ //Fonction qui permet de lire une chaine de caractères 
    lire_format("%ms", str);
    return 0;
}


int lire_MDP(){ //Fonction qui permet de lire un mot de passe 
    //Fonction qui lit un mot de passe au clavier et qui n'affiche pas ce dernier sur l'écran, il affiche des "*" à la place de chaque caraCtère 
    char passwd[16];
    char *in = passwd;
    struct termios  tty_orig;
    char c;
    tcgetattr( STDIN_FILENO, &tty_orig );
    struct termios  tty_work = tty_orig;

    puts("Entrez le mot de passe (6 chiffres minimum, 8 chiffres maximum):");
    tty_work.c_lflag &= ~( ECHO | ICANON );
    tty_work.c_cc[ VMIN ]  = 1;
    tty_work.c_cc[ VTIME ] = 0;
    tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_work );

    int cp; 
    cp = 0; 

    while (1) {
            if (read(STDIN_FILENO, &c, sizeof c) > 0) {
                    if (('\n' == c && cp >= 6)|| cp==8) { //Fin si l'utilisateur appuie sur la touche entrée et que le mot de passe fait plus de 6 chiffres 
                            break;
                    }
                    if (c == '0' ||c == '1' ||c == '2' ||c == '3' ||c == '4' ||c == '5' ||c == '6' ||c == '7' ||c == '8' ||c == '9'){ //Regarde si l'utilisateur rentre bien des chiffres 
                        *in++ = c;
                        write(STDOUT_FILENO, "*", 1); //Affiche une "*" à la place du chiffre entrer par l'utilisateur 
                        cp = cp + 1; 
                    }
            }
    }

    tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_orig );

    *in = '\0';
    fputc('\n', stdout);

    int n; 
    n = convertCharInInt(passwd); //Convertir le mot de passe entrer sous forme de chaine de caractère en entier 

    return n; //Renvoie le mot de passe 
}

int convertCharInInt(char * c){ //Fonction qui permet de convertir une string en int 
    int cp;
    cp=0; 

    int i; 
    i = 0; 
    while(c[i]!='\0'){ //Boucle qui compte le nombre de caractères dans la chaine 
        cp = cp + 1;
        i++;  
    }

    cp = cp - 1; 

    int n; 
    n = 0; 
    i = 0;

    int test;
    test = 0; 

    while(c[i]!='\0' && test == 0 ) //Boucle qui test chaque caractères 
    {
        int n1; 

        //Avec la table ASCII, on regarde la valeur de caractère pour retrouver le chiffre en type int 
        if(c[i] == 48){
            n1 = 0; 
        }
        else if(c[i] == 49){
            n1 = 1; 
        }
        else if(c[i] == 50){
            n1 = 2; 
        }
        else if(c[i] == 51){
            n1 = 3; 
        }
        else if(c[i] == 52){
            n1 = 4; 
        }
        else if(c[i] == 53){
            n1 = 5; 
        }
        else if(c[i] == 54){
            n1 = 6; 
        }
        else if(c[i] == 55){
            n1 = 7; 
        }
        else if(c[i] == 56){
            n1 = 8; 
        }
        else if(c[i] == 57){
            n1 = 9; 
        } 
        else{
            test = 1; 
        }
        n = n + n1*pow(10, cp); //On ajoute chaque int trouvé en le mutipliant par sa position dans la chaine 
        cp = cp - 1;
        i++; 
    }
    return n; //On renvoie la chaine de caratère sous la forme d'un entier 
}

