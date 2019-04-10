#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "../include/structure.h"
#include "../include/client.h"
#include "../include/entrees.h"
#include "../include/clients_administrateur.h"
#include "../include/administrateur.h"
#include "../include/menu.h"

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

void changerMotDePasse(Titulaire_Compte tc){ //Fonction qui permet à l'utilisateur de modifier son mot de passe 
    int mdp, mdpa; 
    mdpa = demanderAncienMDP(tc); //Demande l'ancien mot de passe à l'utilisateur 
    if (mdpa == -1){ //Vérifie si le mot de passe correspond bien 
        printf(RED "Error, password change aborted.\n" RESET);
        return;
    }
    int n; 
    n = demanderNouveauMDP(); //Demande le nouveau mot de passe 
    if(n!=-1){
        mdp = crypter(n); //Si le mot de passe est au bon format, crypte ce dernier 
        tc->mdp = mdp;
    }
    else {
        printf(RED "Error, password change aborted.\n" RESET); //Erreur si mot de passe pas au bon format 
        return;
    }
    
}

int demanderAncienMDP(Titulaire_Compte tc){ //Fontion qui permet de demander à l'utilisateur son ancien mot de passe 
    int mdp, mdpv;
    mdp = decrypter(tc->mdp); //Décrypte le mot de passe sauvegarder dans la structure 
    printf("Entrez l'ancien mot de passe pour continuer:\n>");
    mdpv = lire_MDP(); //Demande à l'utilisateur d'entrer son ancien mot de passe 
    if (mdp != mdpv){//Verifie si le deux mots de passe sont identiques 
        printf(RED "L'ancien mot de passe est incorrect.\n" RESET); //Si non, erreur 
        return -1;
    }
    return mdp; //Renvoie l'ancien mot de passe 
}

void virement(Compte c1, Compte c2){ //Fonction qui permet de faire un virement entre deux comptes passés en paramètre 
    if (c1->actif == 0){ //Vérifie que le compte à débiter est actif 
        printf(RED "Impossible de faire un virement depuis un compte inactif\n" RESET); //Si non, erreur 
        return;
    }
    float montant; 
    printf("Montant du virement : "); 
    lire_decimal(&montant); //Demande le montant du virement et vérifie que ce qui est rentré par l'utilisateur est au bon format 
    if (montant < 0){ //Vérifie que le montant est positif 
        printf(RED "Vous n'avez pas le droit d'effectuer un virement négatif\n" RESET); //Si non, erreur 
        return;
    }
    if((c1->solde - montant) >=0 ){ //Vérifie qu'il y a assez d'argent sur le compte pour le virement 
        //Creer et ajoute une opération concerant ce virement dans la liste d'opérations du compte 
        Element e = initialiserElement(); 
        Operation op = initialiserOperation();
        op = creerOperation(c1, c2, montant); 
        e->contenu.Op = op; 
        ajoutListe(c1->ListeOperations , e); 
        ajoutListe(c2->ListeOperations, e); 
        //Modifie les soldes des comptes 
        c1->solde = c1->solde - montant; 
        c2->solde = c2->solde + montant; 
        printf("Virement effectué\n"); 
        printf("Récapitulatif du virement\n"); 
        afficherOperation(op); //Affiche le récapitulatif du virement 
    }
    else { //Si il n'y a pas assez d'argent, erreur 
        printf("Virement non effectué\n"); 
    }
}     

void demandeCreationCompte(Banque B, Titulaire_Compte tc){ //Fonction qui permet à l'utilisateur de demander à la banque la création d'un compte 
    printf("\nQuel type de compte voulez vous?\n");
    printf("1 - Compte Courant\n2 - Compte Joint\n3 - Livret A\n4 - PEL\n>");
    int c;
    lire_entier(&c); //Demande quel type de compte l'utilisateur souhaite creer 
    while (c<1 || c>4){ //Vérifie que le type de compte existe 
        printf(RED "Choix non disponible, réessayez\n" RESET);
        lire_entier(&c);
    }
    if (c == 2){ //Teste si c'est un compte joint 
        //Si oui : 
        printf("Entrez l'identifiant du titulaire avec lequel ce compte sera partagé:\n>");
        int tit2;
        lire_entier(&tit2); //Demande à l'utilisateur avec quel titulaire il souhaite joindre le compte 
        while (rechercherTitulaire(B, tit2) == -1){ //Vérifie si le titulaire choisis existe 
            printf("Il n'y a pas de titulaire avec ce numéro, réessayez ou entrez 0 pour ne pas avoir de deuxième titulaire pour l'instant\n>");
            lire_entier(&tit2);
            if (tit2 == 0){
                break;
            }
        }
        //Met un message dans la structure du deuxime titulaire pour que l'administrateur puisse le voir 
        char Output[100];
        sprintf(Output, "%s%d%d", "DCC", c, tit2);
        tc->msgT = malloc(140*sizeof(char));
        strcpy(tc->msgT, Output);
        return;
    }
    //Met un message dans la structure du titulaire principal pour que l'administrateur puisse le voir 
    char Output[100];
    sprintf(Output, "%s%d", "DCC", c);
    tc->msgT = malloc(140*sizeof(char));
    strcpy(tc->msgT, Output);
}

void demandeSuppressionCompte(Banque B, Titulaire_Compte tc){ //Fonction qui permet à l'utilisateur de demander la suppression d'un compte 
    printf("Voici la liste de vos comptes:\n");
    //Affiche la liste des comptes de l'utilisateur 
    Element E;
    for (int i=0; i<tc->ListeNumCmpt->taille; i++){
        E = retournerElement(tc->ListeNumCmpt, i);
        Compte Cp = retournerCompte(B, E->contenu.NCp);
        printf("Compte numéro %d\n", Cp->NumeroCompte);
    }
    int n;
    printf("Entrez le numéro du compte dont vous souhaitez demander la suppression\n>");
    lire_entier(&n); //Demande à l'utilisateur quel compte il souhaite supprimer 
    if (rechercherElement(tc->ListeNumCmpt, n)==-1){ //Vérifie que ce numéro de compte existe et qu'il lui appartienne 
        printf(RED "Compte non trouvé ou ne vous appartenant pas\n" RESET); //Si non, erreur 
        return;
    }
    
    //Met un message dans la structure du titulaire principal pour que l'administrateur puisse le voir 
    char Output[100];
    sprintf(Output, "%s%d", "DSC", n);
    tc->msgT = malloc(140*sizeof(char));
    strcpy(tc->msgT, Output);
} 
