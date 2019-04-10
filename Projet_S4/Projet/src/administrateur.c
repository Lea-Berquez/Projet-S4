#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../include/entrees.h"
#include "../include/structure.h"
#include "../include/clients_administrateur.h"
#include "../include/menu.h"
#include "../include/administrateur.h"

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"


//Banque
void afficherBanque(Banque B){ //Affiche le nombre de clients et de comptes de la banque
    int n;
    n = B->ListeComptes->CCourants->taille + B->ListeComptes->LivretsA->taille + B->ListeComptes->PEL->taille + B->ListeComptes->CJoints->taille;
    printf("Cette banque a %d clients, %d comptes.\n", B->ListeClients->taille, n);
}

void modifierMotDePasseAdmin(Banque B){ //Permet de modifier le mot de passe de l'admin
    int mdp, mdpa, mdpn1, mdpn2;
    mdpa = demanderAncienMDPAdmin(B); //Demande l'ancien mot de passe 
    if (mdpa == -1){
        return;
    }
    //Demande de rentrer le nouveau mot de passe 
    printf("Entrez le nouveau mot de passe:\n>");
    mdpn1 = lire_MDP(); //Lit le nouveau mot de passe 
    printf("Entrez le nouveau mot de passe une deuxième fois (Vérification):\n>");
    mdpn2 = lire_MDP(); //Lit le nouveau mot de passe de nouveau 
    if (mdpn1 != mdpn2){ //Vérifie que les deux nouveaux mot de passe rentrés sont égaux 
        printf(RED "Les mots de passe ne correspondent pas, retour au menu.\n" RESET);
        return;
    }
    mdp = crypter(mdpn1); //Crypte le mot de passe 
    B->mdpAdmin = mdp;
    printf("Le mot de passe Administrateur a été modifié\n");
    
}

int demanderAncienMDPAdmin(Banque B){ //Renvoie l'ancien mot de passe admin pour le comparer avec celui entré
    int mdp, mdpv;
    mdp = decrypter(B->mdpAdmin); //Récupère le mot de passe et le décrypte 
    printf("Entrez l'ancien mot de passe pour continuer:\n>");
    mdpv = lire_MDP(); //Lit l'ancien mot de passe 
    if (mdp != mdpv){
        printf(RED "Vérification de sécurité échouée, l'ancien mot de passe est incorrect.\n" RESET);
        return -1;
    }
    return mdp;
}


//Comptes
void afficherListeComptes(Banque B){ //Affiche la liste des comptes dans la banque
    printf("Liste des comptes dans la banque:\n");
    printf("Comptes Courants:\n");
    afficherListe(B, B->ListeComptes->CCourants); //Affiche la liste des comptes courants 
    printf("Comptes Joints:\n");
    afficherListe(B, B->ListeComptes->CJoints); //Affiche la liste des comptes joints 
    printf("Livrets A:\n");
    afficherListe(B, B->ListeComptes->LivretsA); //Affiche la liste des Livrets A 
    printf("PEL:\n");
    afficherListe(B, B->ListeComptes->PEL); //Affiche la liste des PEL 
}

void afficherListeNumerosCompte(Banque B){ //Affiche seulement les numéros de tous les comptes
    printf("Liste des numéros de comptes dans la banque:\n");
    printf("\nComptes Courants:\n");
    for (int i = 0; i<B->ListeComptes->CCourants->taille; i++){
        printf("%d\n", getNumC(retournerElement(B->ListeComptes->CCourants, i)->contenu.Cp)); //Affiche les numéros des comptes courants
    }
    printf("\nComptes Joints:\n");
    for (int i = 0; i<B->ListeComptes->CJoints->taille; i++){
        printf("%d\n", getNumC(retournerElement(B->ListeComptes->CJoints, i)->contenu.Cp)); //Affcihe les numéros des comptes joints 
    }
    printf("\nLivrets A:\n");
    for (int i = 0; i<B->ListeComptes->LivretsA->taille; i++){
        printf("%d\n", getNumC(retournerElement(B->ListeComptes->LivretsA, i)->contenu.Cp)); //Affiche les numéros des livrets A
    }
    printf("\nPEL:\n");
    for (int i = 0; i<B->ListeComptes->PEL->taille; i++){
        printf("%d\n", getNumC(retournerElement(B->ListeComptes->PEL, i)->contenu.Cp)); //Affiche les numéros des PEL 
    }
}


//Titulaires
Titulaire_Compte choisirTitulaire(Banque B){ //Fonction qui permet de choisir un titulaire parmis une liste de titulaires  
    printf("Liste des clients:\n");
    afficherListeIdTitulaires(B); //Affiche la liste des titulaires 
    printf("Entrez l'identifiant du titulaire choisi:\n>");
    int id;
    lire_entier(&id); //Récupère l'identifiant choisis
    while (rechercherTitulaire(B, id) == -1){ //Verifie qie cet identifiant est bien associé à un utilisateur 
        printf(RED "Cet identifiant n'est associé à aucun client, veuillez réessayer:\n>" RESET); 
        lire_entier(&id);
    }
    return retournerTitulaire(B, id); //Renvoie le titulaire choisis 
}

void afficherListeTitulaires(Banque B){ //Fonction qui affiche la liste des titulaires 
    printf("Liste des clients de la banque:\n");
    afficherListe(B, B->ListeClients);
}

void supprimerClient(Banque B, Titulaire_Compte Tc){ //Fonction qui permet de supprimer le client passé en paramètre 
    printf(RED "Etes vous sûr de vouloir supprimer ce titulaire? (Y/N)\n>" RESET); //Vérifie le choix de suppression 
    char* rep;
    lire_string(&rep);
    if (*rep != 'Y'){
        return;
    }

    int id;
    id = Tc->identifiant;
    int pos;
    pos = rechercherElement(B->ListeClients, id); //Recherche le client dans la banque 
    Element E;
    Compte Cp;
    for (int i=0; i<Tc->ListeNumCmpt->taille; i++){
        E = retournerElement(Tc->ListeNumCmpt, i);
        Cp = retournerCompte(B, E->contenu.NCp);
        if (Cp->nbTitulaires == 1){
            modifierEtatCompte(Cp);
            Cp->nbTitulaires = 0;
        }
        else if (Cp->nbTitulaires == 2){//Si c'est un compte joint, il faut traiter le cas où le clients supprimer est le titulaire principal
            Titulaire_Compte Tc1 = retournerTitulaire(B, Cp->NumeroTit1);
            //Titulaire_Compte Tc2 = *(Cp->Titulaire2);
            if (id == Tc1->identifiant){//Le titulaire 2 du compte passe premier 
                printf("Un compte duquel ce client était titulaire principal a comme nouveau titulaire principal le deuxième titulaire.\n");
                Cp->NumeroTit1 = Cp->NumeroTit2;
                Cp->nbTitulaires = 1;
            }
        }
    }
    supprimerElement(B->ListeClients, pos); //Suppression du client 
}

void afficherListeIdTitulaires(Banque B){  //Fonction qui permet d'afficher la liste des identifiants des titulaires 
    Element E;
    int id;
    char* nom_prenom;
    //char* prenom;
    for (int i = 0; i<B->ListeClients->taille; i++){ //Boucle pour parcourir toute la liste de clients 
        E = retournerElement(B->ListeClients, i);
        id = E->contenu.Tc->identifiant;
        nom_prenom = E->contenu.Tc->nom_prenom;
        //prenom = E->contenu.Tc->prenom;
        printf("%d  %s\n", id, nom_prenom); //Affiche suelement l'identifiant, le nom et le prénom 
    }
}


//Compte
Compte choisirCompte(Banque B){//Fonction qui permet de sélectionner un compte 
    printf("\nVoici l'ensemble des numéros de compte\n");
    afficherListeNumerosCompte(B); //Affiche tous les numéros de comptes 
    printf("Entrez l'identifiant d'un compte:\n>");
    int num; 
    lire_entier(&num); //Permet d'entrer l'identifiant du compte 
    while (rechercherCompte(B, num) == -1){//Vérifie que l'identifiant rentrer correspond bien à un compte 
        printf(RED "Ce numéro n'est associé à aucun compte, veuillez réessayer:\n>" RESET);
        lire_entier(&num);
    }
    return retournerCompte(B, num); //Renvoie le compte sélectionné 
}

void afficherTitulairesCompte(Banque B, Compte Cp){ //Fonction qui affiche le(s) titulaire(s) d'un compte 
    if (Cp->nbTitulaires > 0){ //Vérifie que le compte à bien un ou des titulaire(s) 
        printf("\nTitulaire 1 de ce compte\n"); 
        afficherClient(retournerTitulaire(B, Cp->NumeroTit1)); //Affiche les informations du premier titulaire
        if (Cp->nbTitulaires == 2){ //Regarde si le compte à un deuxième titulaire 
            printf("\nTitulaire 2 de ce compte\n");
            afficherClient(retournerTitulaire(B, Cp->NumeroTit2)); //Si oui, affiche les informations du deuxième titulaire
        }
    }
    else { //Si le compte n'a pas de titulaire(s) 
        printf("\nCe compte n'a pas de Titulaire (sans doute dû à une suppression)\n");
    }
}


void modifierTitulairesCompte(Banque B, Compte Cp){ //Fonction qui permet de modifier les titulaires d'un compte 
    int m = 0;
    while (m!=4){ //Demande à l'utilisateur ce qu'il souhaite modifier 
        printf("\nVoulez vous modifier le premier titulaire, le deuxième titulaire (Compte Joint)\nou enlever le deuxième titulaire?\n");
        printf("1 - Modifier le Premier\n");
        printf("2 - Modifier le Deuxième (Uniquement si compte joint)\n");
        printf("3 - Enlever le deuxième (Uniquement si compte joint)\n");
        printf("4 - Annuler et quitter\n>");
        
        lire_entier(&m);
        while (m<1 || m>4){ //Vérifie que le choix fait par l'utilisateur est bien possible 
            printf(RED "Erreur lors du choix, Veuillez réessayer\n>" RESET);
            lire_entier(&m); //Propose à 'utilisateur de refaire un choix 
            }
        switch(m){
            case 1 : modifierTitulaireCompte(B, Cp, 1); break; //Si 1, modifie le premier titulaire 
            case 2 : { //Si 2, modifie le deuxième titulaire seulement si il existe 
                if (Cp->typeCmpt == 2){
                    modifierTitulaireCompte(B, Cp, 2);
                }
                else {
                    printf(RED "Ceci n'est pas un Compte Joint, impossible d'y affecter un deuxième titulaire\n" RESET);
                }
                break;
            }
            case 3 : { //Si 2, enlève le deuxième titulaire seulement si il existe 
                if (Cp->typeCmpt == 2){
                    modifierTitulaireCompte(B, Cp, 3);
                }
                else {
                    printf(RED "Ceci n'est pas un Compte Joint, impossible de supprimer un deuxième titulaire\n" RESET);
                }
                break;
            }
            case 4 : return; //Permet de quitter le menu 
        }
        afficherCompte(B, Cp); 
    }
    
}

void modifierEtatCompte(Compte Cp){ //Modifie l'état du compte, si il est actif il passe inactif et inversement 
    //Regarde su le compte est actif ou inactif 
    if (Cp->actif == INACTIF){
        Cp->actif = ACTIF;
        return;
    } 
    Cp->actif = INACTIF;

    //Creer et ajoute une opération concerant ce changement dans la liste d'opérations du compte
    Operation Op = initialiserOperation();
    Op->typeOp = 3;
    Element E = initialiserElement();
    E->contenu.Op = Op;
    ajoutListe(Cp->ListeOperations, E);
} 

void modifierSolde(Compte Cp){ //Fonction qui permet à l'administarteur de modifier le compte (exemple: le clients donne de l'argent liquide au banquier)
    float ancienSolde = Cp->solde;
    printf("Entrez le nouveau solde:\n>");
    float flt;
    lire_decimal(&flt);
    printf("Le nouveau solde sera %f.\n", flt);
    Cp->solde = flt;

    //Creer et ajoute une opération concerant cet ajout dans la liste d'opérations du compte
    Operation Op = initialiserOperation();
    Op->typeOp = 2;
    Op->montant = flt - ancienSolde;
    Element E = initialiserElement();
    E->contenu.Op = Op;
    ajoutListe(Cp->ListeOperations, E);
    
}

void supprimerCompte(Banque B, Compte Cp){ //Fonction qui permet de supprimer un compte passé en paramètre 
    printf(RED "Etes vous sûr de vouloir supprimer le compte? (Y/N)\n>" RESET); //Vérifie que l'utilisateur souhaite vraiment supprimer ce compte
    char* rep;
    lire_string(&rep);
    if (*rep != 'Y'){
        return;
    }

    int num = Cp->NumeroCompte;
    
    if (Cp->nbTitulaires != 0){
        int pos2;
        Titulaire_Compte Tc1;
        Titulaire_Compte Tc2;
        Tc1 = retournerTitulaire(B, Cp->NumeroTit1); //On prend son titulaire
        pos2 = rechercherElement(Tc1->ListeNumCmpt, Cp->NumeroCompte); //on cherche le numéro du compte dans la liste du titulaire
        supprimerElement(Tc1->ListeNumCmpt, pos2); //On supprime l'occurence
        if (Cp->nbTitulaires == 2){ //De même si un 2ème titulaire
            Tc2 = retournerTitulaire(B, Cp->NumeroTit2);
            pos2 = rechercherElement(Tc2->ListeNumCmpt, Cp->NumeroCompte);
            supprimerElement(Tc2->ListeNumCmpt, pos2);
        }
    }

    int pos;
    int type = Cp->typeCmpt;
    switch(type){ //On va aller supprimer le numéro du compte chez les titulaires
        case 1 : {  
        pos = rechercherElement(B->ListeComptes->CCourants, num); //On cherche d'ou vient le compte
        supprimerElement(B->ListeComptes->CCourants, pos); //On supprime le compte
        break;
        }
        case 2 : {  
        pos = rechercherElement(B->ListeComptes->CJoints, num);
        supprimerElement(B->ListeComptes->CJoints, pos);
        break;
        }
        case 3 : {  
        pos = rechercherElement(B->ListeComptes->LivretsA, num);
        supprimerElement(B->ListeComptes->LivretsA, pos);
        break;
        }
        case 4 : {  
        pos = rechercherElement(B->ListeComptes->PEL, num);
        supprimerElement(B->ListeComptes->PEL, pos);
        break;
        }
    }
}

void ajouterArgentCompte(Compte Cp){ //Fonction qui permet d'ajouter de l'argent au compte passé en paramètre 
    float money;
    printf("Quel montant voulez vous ajouter?\n>");
    lire_decimal(&money); //Demande le montant 
    while (money<0){ //Vérifie que c'est bien un montant positif 
        printf(RED "Cela ne semble pas être un montant très positif..." RESET);
        lire_decimal(&money);
    }
    Cp->solde = Cp->solde + money; //Ajoute le montant au solde 

    //Creer et ajoute une opération concerant cet ajout dans la liste d'opérations du compte 
    Operation Op = initialiserOperation();
    Op->typeOp = 2;
    Op->montant = money;
    Element E = initialiserElement();
    E->contenu.Op = Op;
    ajoutListe(Cp->ListeOperations, E);
}

void creerCompte(Banque B){ //Fonction qui permet de creer un compte 
    if (B->ListeClients->taille == 0){ //Test si il y a des des clients dans la banque 
        printf(RED "Il vous faut des clients pour créer un compte!\n" RESET);
        return;
    }

    //Demande à l'utilisateur ce quel type de compte il souhaite creer 
    printf("Quel type de compte?\n1 - CCourant\n2 - CJoint\n3 - Livret A\n4 - PEL\n>"); 
    int t;
    lire_entier(&t);
    while (t<1 || t>4){//Vérifie que le choix existe 
        printf(RED "Cela ne correspond à aucun type, veuillez réessayer\n>" RESET);
        lire_entier(&t);
    }
    int num = creerNumero(B->ListeComptes, t);
    Element e = initialiserElement();
    switch(t){
        case 1 : ajoutListe(B->ListeComptes->CCourants, e); break; //Creer un compte courant
        case 2 : ajoutListe(B->ListeComptes->CJoints, e); break; //Creer un compte joint 
        case 3 : ajoutListe(B->ListeComptes->LivretsA, e); break; //Creer un livrets A
        case 4 : ajoutListe(B->ListeComptes->PEL, e); break; //Creer un PEL 
    }

    Compte Cp = initialiserCompte();
    Cp->typeCmpt = t;

    if (Cp->typeCmpt ==2 && B->ListeClients->taille <2){ //Vérifie qu'il y ai bien deux clients dans la banque pour creer un compte joint 
            printf(RED "Il vous faut au moins 2 clients pour créer un compte joint!\n" RESET);
            return;
    }
    Cp->NumeroCompte = num;
    modifierTitulaireCompte(B, Cp, 1);
    if (Cp->typeCmpt == 2){
        printf("\nDeuxième titulaire\n");
        modifierTitulaireCompte(B, Cp, 2);
    }
    Cp->ListeOperations->premier = NULL;
    Cp->ListeOperations->taille = 0;
    e->contenu.Cp = Cp;
    printf("\nRécapitulatif:\n");
    afficherCompte(B, e->contenu.Cp); //Affiche le compte créé 
}

void modifierCompte(Banque B, Compte Cp){ //Fonction qui permet de modifier les informations du compte passé en paramètre 
    int m;
    do {//Demande à l'utilisateur ce qu'il souhaite modifier 
        printf("Que voulez vous modifier?\n");
        printf("1 - Solde\n");
        printf("2 - Titulaires\n");
        printf("3 - Etat\n");
        printf("4 - Quitter ce menu\n>");
        lire_entier(&m);
        while (m<1 || m>4){
            printf(RED "Erreur lors du choix, Veuillez réessayer\n>" RESET);
            lire_entier(&m);
        }
        switch(m){
            case 1 : modifierSolde(Cp); break; //Modifie le solde 
            case 2 : modifierTitulairesCompte(B, Cp); break; //Modifie le(s) titulaire(s) du compte 
            case 3 : modifierEtatCompte(Cp); break; //Modifie l'état (Actif/Inactif) du compte 
            case 4 : return; //Quitte le menu 
        }
    } while (m != 4);
}
