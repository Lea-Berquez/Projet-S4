#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../include/structure.h"
#include "../include/administrateur.h"
#include "../include/clients_administrateur.h"

//Fonction d'initialisation

Element initialiserElement(){ //Fonction qui alloue la mémoire pour un élément 
    Element E = malloc(10*sizeof(Element));
    E->suivant = NULL;
    return E; //Renvoie l'élément 
}

Liste initialiserListe(int type){ //Fonction qui alloue la mémoire pour une liste suivant le type 
    Liste liste = malloc(10*sizeof(Liste));
    liste->taille = 0;
    liste->premier = NULL;
    if (liste == NULL){
        exit(EXIT_FAILURE);
    }

    if (type == 1){
        liste->type = OPERATIONS;
    }
    else if (type == 2){
        liste->type = COMPTES;
    }
    else if (type == 3){
        liste->type = NUMEROS_COMPTES;
    }
    else{
        liste->type = CLIENTS;
    }
    return liste; //Renvoie la liste 
}

Titulaire_Compte initialiserTitulaire() { //Fonction qui alloue la mémoire pour un titulaire 
    Titulaire_Compte Tc = malloc(10*sizeof(Titulaire_Compte));  
    Tc->nom_prenom = malloc(50 * sizeof(char));
    Tc->identifiant = 0;
    Tc->numTelephone = malloc(100 * sizeof(char)); 
    Tc->mail = malloc(100*sizeof(char));
    Tc->mdp = 0;
    Tc->ListeNumCmpt = initialiserListe(3);
    Tc->msgT = malloc(140 * sizeof(char));
    return Tc; //Retourne le titulaire 
}

Operation initialiserOperation() { //Fonction qui alloue de la mémoire pour une opération 
    Operation Op = malloc(10*sizeof(Operation));
    Op->date = 0;
    Op->typeOp = 1;
    Op->identifiantCompteE = 0;
    Op->identifiantCompteR = 0;
    Op->montant = 0;
    time_t intps;
    struct tm * date;
    intps = time(NULL);
    date = localtime(&intps);
    Op->date = ((date->tm_year+1900)*10000 + (date->tm_mon+1)*100 + date->tm_mday);
    return Op; //Retourne l'opération 
}

Compte initialiserCompte() { //Fonction qui alloue la mémoire pour un compte 
    Liste liste = initialiserListe(1);
    Compte Cp = malloc(10*sizeof(Compte));
    malloc(100*sizeof(char)); 
    Cp->NumeroCompte = 0;
    Cp->typeCmpt = 1;
    Cp->nbTitulaires = 0;
    Cp->NumeroTit1 = 0;
    Cp->NumeroTit2 = 0;
    Cp->solde = 0;
    Cp->ListeOperations = liste;
    Cp->actif = 1;
    return Cp; //Renvoie un compte 
}

Comptes initialiserComptes() { //Fonction qui alloue de la mémoire pour les comptes 
    Comptes Cps = malloc(10*sizeof(Comptes));
    Cps->CCourants = initialiserListe(2);
    Cps->LivretsA = initialiserListe(2);
    Cps->PEL = initialiserListe(2);
    Cps->CJoints = initialiserListe(2);
    return Cps; //Renvoie les comptes 
}

Banque initialiserBanque() { //Fonction qui alloue de la mémoire pour la banque 
    Banque B = malloc(10*sizeof(Banque));
    B->ListeClients = initialiserListe(4);
    B->ListeComptes = initialiserComptes();
    B->mdpAdmin = 0;
    return B; //Renvoie la banque 
}


//Fonctions pour Liste
void afficherListe(Banque B, Liste liste){ //Fonction qui permet d'afficher une liste 
    int t = 0;
    Element e = liste->premier; //Récupère le premier élément de la liste 
    while (t<liste->taille){
        //Regarde quel type de liste c'est : 
        if (liste->type == 1){
            afficherOperation(e->contenu.Op); //Affiche la liste des opérations 
        }
        if (liste->type == 2){
            afficherCompte(B, e->contenu.Cp); //Affiche la liste des comptes 
        }
        if (liste->type == 4){
            afficherClient(e->contenu.Tc); //Affiche la liste des clients 
        }
        t++;
        e = e->suivant; //Passe à l'élément suivant 
    }
}

void ajoutListe(Liste liste, Element elem) { //Fonction pour ajouter un élément dans la liste 
    if (liste == NULL || elem == NULL) { //Regarde si la liste et l'élément existent 
        exit(EXIT_FAILURE);
    }
    
    //Insertion de l'élément a la fin de la liste 
    int t = liste->taille;
    if (t == 0){ //Vérifie si la liste n'a pas d'éléments
        liste->premier = elem; //Si oui, on ajoute au premier élément 
    }
    else { //Si non, on ajoute l'élément à la fin 
        Element E = retournerElement(liste, liste->taille-1);
        E->suivant = elem;
    }
    liste->taille++; //Augmentation de la taille de la liste 
}

int rechercherElement(Liste liste, int identifiant){ //Fonction qui permet de rechercher un élément 
    if (liste == NULL) return -1; //Regarde si la liste est nulle 
    if (liste->taille==0) return -1; //Regarde si la liste est vide 

    Element e = liste->premier; //Récupération du premier élément 
    for (int i = 0; i<liste->taille; i++){ //Parcours de la liste 
        e = retournerElement(liste, i); //Récupération de l'élément 
        if (liste->type == 2){
            if (e->contenu.Cp->NumeroCompte == identifiant){
                return i;
            }
        }
        if (liste->type == 3){
            if (e->contenu.NCp == identifiant){
                return i;
            }
        }
        if (liste->type == 4){
            if (e->contenu.Tc->identifiant == identifiant){
                return i;
            }
        } 
    }
    return -1;
    //suivant le type de la liste 3 cas:
    //-->comptes: recherche dans la liste le numero de compte et renvoie sa position dans la liste 
    //-->numeros de comptes: recherche dans la liste la position du compte avec ce numéro et renvoie
    //-->clients: recherche le client avec cet identifiant et renvoie sa position dans la liste
}

Element retournerElement(Liste liste, int position){  //Fonction qui renvoie un élément 
    if (position+1>liste->taille || position<0) return NULL;
    if (liste == NULL) return NULL;
    if (liste->taille==0) return NULL;
    if (position == 0) return liste->premier;
    int t = 0;
    Element e = liste->premier;
    while (t<position){
        e = e->suivant;
        t++;
    }
    return e;
    
}

int supprimerElement(Liste liste, int position){ //Fonction qui permet de supprimer des éléments 
    if (position+1>liste->taille || position<0) return -1;
    if (liste == NULL) return -1;
    if (liste->taille == 0) return -1;
    if (liste->taille == 1){
        free(liste->premier);
        liste->taille = 0;
        return 0;
    }
    if (position == 0){
        Element E = liste->premier;
        liste->premier = liste->premier->suivant;
        free(E);
        liste->taille--;
        return 0;
    }

    Element e = retournerElement(liste, position-1);

    if (e->suivant == NULL){
        free(e);
        liste->taille--;
        return 0;
    }
    
    Element elem = e->suivant;
    e->suivant = e->suivant->suivant;
    free(elem);
    liste->taille--;
    return 0;
}
