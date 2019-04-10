#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/entrees.h"
#include "../include/structure.h"
#include "../include/clients_administrateur.h"
#include "../include/administrateur.h"
#include "../include/client.h"
#include "../include/menu.h"
#include "../include/fichier.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

//Menu
void menuPrincipal(Banque B){ //Fonction qui gère le menu principal 
    printf(GRN "\n\n\n######    ##    ######     ######                       \n" RESET); //Affiche le beau logo
    printf(GRN "#     #  #  #   #     #    #     #   ##   #    # #    # \n" RESET);
    printf(GRN "#     #   ##    #     #    #     #  #  #  ##   # #   # \n" RESET);
    printf(GRN "######   ###    ######     ######  #    # # #  # #### \n" RESET);
    printf(GRN "#     # #   # # #   #      #     # ###### #  # # #  # \n" RESET);
    printf(GRN "#     # #    #  #    #     #     # #    # #   ## #   # \n" RESET);
    printf(GRN "######   ###  # #     #    ######  #    # #    # #    # \n" RESET);
    printf(GRN "\n  Welcome to B&R Bank, the best place for your money\n\n" RESET); //Affiche le message de bienvenue 
    
    int i = 0;
    while (i != 4){ //Boucle qui demande à l'utilisateur de s'identifier 
        printf(CYN "\nEtes vous un client existant, un nouveau client ou un administrateur?\n" RESET);
        printf("1 - Client\n2 - Nouveau Client\n3 - Administrateur\n4 - Me déconnecter\n>");
        lire_entier(&i); //Demande à l'utilisateur son choix 
        while (i<1 || i>4){ //Vérifie que le choix fait par l'utilisateur existe 
            printf(RED "Ce choix n'est pas disponible, veuillez réessayer\n>" RESET);
            lire_entier(&i);
        }

        switch(i){ //Suivant le nombre entré par l'utilisateur : 
            case 1 : ConnectionClients(B); break; //Si 1, c'est un client 
            case 2 : menuNouveauClient(B); break; //Si 2, c'est un nouveau client 
            case 3 : ConnectionAdministrateur(B); break; //Si 3, c'est l'administrateur 
            case 4 : return; //Si 4, on quitte le menu, ferme la banque 
        }
    }
}

void ConnectionAdministrateur(Banque B){ //Fonction qui gère la connexion de l'administrateur 
    int mdp, mdpv;
    mdp = decrypter(B->mdpAdmin); //Récupère le mot de passe 
    printf(MAG "\nCOMPTE ADMINISTRATEUR\n" RESET);
    printf("Entrez le mot de passe administrateur pour continuer:\n>");
    mdpv = lire_MDP(); //Demande à l'administrateur d'entrer son mot de passe 
    if (mdp != mdpv){ //Vérifie que les deux mot de passe sont égaux (celui taper et celui récupérer)
        printf(RED "Le mot de passe est incorrect.\n" RESET); //Si non, retour au menu précédent 
        return;
    }
    printf(YEL "\nBienvenue Administrateur\n" RESET);
    afficherBanque(B); //Affiche l'état de la banque 
    menuAdministrateur(B); //Passe au menu administrateur 
    return;
}

void menuAdministrateur(Banque B){ //Fonction qui gère le menu administrateur 
    //On va commencer par regarder s'il y a des demandes
    Titulaire_Compte Tc;
    Element E;
    char * message = malloc(140*sizeof(char));
    //Regarde si il y'a des demandes parmi les utilisateurs, l'administrateur à la possibilités de les gérés 
    for (int i=0; i<B->ListeClients->taille; i++){ //Boucle qui passe en revu tout les comptes pour voir si il y a des messages 
        E = retournerElement(B->ListeClients, i);
        Tc = E->contenu.Tc;
        strcpy(message, Tc->msgT);
        if (message[0] == 'D'){ //Regarde si il y a une demande 
            if (message[1] == 'C'){ //Regarde si c'est une demande de création de compte 
                afficherClient(Tc); //Affiche le client qui fait cette demande 
                printf("\nCe titulaire demande la création d'un compte de type ");
                int t = (int)message[3] - (int)'0';
                int tit2 = 0;
                //Affiche le type de compte que le client souhaite creer : 
                if (t == 1){
                    printf("Compte Courant\n");
                }
                else if (t == 2) {
                    printf("Compte Joint");
                    if (message[4]=='0'){
                        printf("sans autre titulaire pour l'instant.\n");
                        
                    }
                    else {
                        tit2 = ((int)message[4]-(int)'0')*100000;
                        tit2 = tit2 + ((int)message[5]-(int)'0')*10000;
                        tit2 = tit2 + ((int)message[6]-(int)'0')*1000;
                        tit2 = tit2 + ((int)message[7]-(int)'0')*100;
                        tit2 = tit2 + ((int)message[8]-(int)'0')*10;
                        tit2 = tit2 + ((int)message[9]-(int)'0')*1;
                        printf("avec le titulaire %d", tit2);
                    }
                }
                else if (t == 3) {
                    printf("Livret A\n");
                }
                else {
                    printf("PEL\n");
                }
                //Demande à l'administrateur si il souhiate réponde à cette demande : 
                printf("\nVoulez vous accéder à son humble requête?\n");
                char * acc;
                do{
                    printf("Entrez votre réponse (Y/N)\n");
                    lire_string(&acc); //Entre la réponde (Y = Yes ou N = No)
                } while (*acc != 'Y' && *acc != 'N');
                if (*acc == 'Y'){ //Si oui,  
                    //Création du compte : 
                    Compte Cp = initialiserCompte();
                    Cp->actif = 1;
                    Cp->ListeOperations = initialiserListe(1);
                    Cp->nbTitulaires = 1;
                    if (tit2 != 0){
                        Cp->nbTitulaires = 2;
                    }
                    Cp->NumeroTit1 = Tc->identifiant;
                    Cp->NumeroTit2 = tit2;
                    Cp->NumeroCompte = creerNumero(B->ListeComptes, t);
                    Cp->solde = 0;
                    Cp->typeCmpt = t;
                    Element E = initialiserElement();
                    //Ajout du compte à la banque : 
                    if (t == 1){
                        ajoutListe(B->ListeComptes->CCourants, E);
                    }
                    else if (t == 2){
                        ajoutListe(B->ListeComptes->CJoints, E);
                    }
                    else if (t == 3){
                        ajoutListe(B->ListeComptes->LivretsA, E);
                    }
                    else{
                        ajoutListe(B->ListeComptes->PEL, E);
                    }
                    E->contenu.Cp = Cp;
                    printf("Récapitulatif:\n");
                    afficherCompte(B, E->contenu.Cp); //Affiche les informations du compte créé 
                    printf("\nCompte créé avec succès\n");
                }
                else {//Si non, 
                    printf("\nQuel dommage, il sera peut-être très déçu!\n"); //Affichage du message de refus 
                }
            }
            else if (message[1] == 'S'){//Regarde si c'est une demande de suppression de compte 
                afficherClient(Tc);
                printf("\nCe titulaire demande la suppression du compte suivant:\n");
                int ncp;
                //Affiche le compte que le client souhaite supprimer 
                ncp = ((int)message[3]-(int)'0')*1000;
                ncp = ncp + ((int)message[4]-(int)'0')*100;
                ncp = ncp + ((int)message[5]-(int)'0')*10;
                ncp = ncp + ((int)message[6]-(int)'0')*1;
                Compte Cps = retournerCompte(B, ncp);
                afficherCompte(B, Cps);
                printf("\nVoulez vous accéder à son humble requête?\n"); //Demande à l'administrateur si il souhaite répondre à cette demande 
                char * acc;
                do{
                    printf("Entrez votre réponse (Y/N)\n");
                    lire_string(&acc); //Réponse de l'administrateur (Y = Yes, N = No) 
                } while (*acc != 'Y' && *acc != 'N');
                if (*acc == 'Y'){ //Si oui, 
                    supprimerCompte(B, Cps); //Suppression du compte 
                }
                else { //Si non, 
                    printf("\nQuel dommage, il sera peut-être très déçu!\n"); //Affichage du message de refus 
                }
            }
            Tc->msgT = ""; //Message effacé dans tous les cas 
        }
    }
    printf("\nAucune nouvelle demande\n"); //Affichage du message de fin des demandes 
    
    //Ouverture du menu administrateur : 
    int i = 0;
    while (i != 4){
        printf(CYN "\nMenu Administrateur:\n" RESET);
        printf("1 - Gestion des comptes\n2 - Gestion des clients\n3 - Administration (Changer le mot de passe)\n4 - Quitter la session\n>");
        lire_entier(&i); //Demande à l'administrateur ce qu'il souhaite faire 
        while (i<1 || i>4){ //Vérifie que le choix fait est possible 
            printf(RED "Ce choix n'est pas disponible, veuillez réessayer\n>" RESET);
            lire_entier(&i);
        }

        switch(i){//Suivant le nombre entré par l'administrateur 
            case 1 : menuGestionDesComptes(B); break; //Si 1, menu qui correspond à la gestion des comptes 
            case 2 : menuGestionDesClients(B); break; //Si, 2, menu qui correspond à la gestion des clients 
            case 3 : menuAdministrationAdmin(B); break; //Si, 3, possibilité de changer le mot de passe 
            case 4 : return; //Si 4, retour au menu précédent 
        }
    }
}

void menuGestionDesComptes(Banque B){ //Fontion qui permet de gérer les comptes de la banque 
    int i = 0;
    int n = 0;
    Compte Cp;
    while (i != 7){ //Tant que l'on souhaite rester dans ce menu : 
        n = B->ListeComptes->CCourants->taille + B->ListeComptes->LivretsA->taille + B->ListeComptes->PEL->taille + B->ListeComptes->CJoints->taille;
        printf(CYN "\nMenu de gestion des comptes:\n" RESET);
        printf("1 - Créer un compte\n2 - Modifier un compte\n3 - Désactiver ou Supprimer un compte\n4 - Afficher la liste des comptes\n5 - Afficher les titulaires d'un compte\n6 - Consulter les opérations d'un compte sur une période donnée\n7 - Retour au menu précédent\n>");
        lire_entier(&i); //Demande à l'administrateur ce qu'il souhaite faire 
        while (i<1 || i>7){//Vérifie si le choix fait existe 
            printf(RED "Ce choix n'est pas disponible, veuillez réessayer\n>" RESET);
            lire_entier(&i);
        }

        if ((i==2 || i==3 || i==4 || i==5 || i==6) &&n==0){ //Vérifie qu'il y a des comptes dans la banque 
                    printf(RED "\nIl n'y a aucun compte dans la banque, impossible de faire ce choix\n" RESET); //Si non, retour au menu 
                    continue;
        }
        switch(i){//Suivant le choix de l'administrateur 
            case 1 : creerCompte(B); break; //Si 1, création d'un compte 
            case 2 : Cp = choisirCompte(B); modifierCompte(B, Cp); break; //Si 2, modification d'un compte 
            case 3 : menuDesactiver_SupprimerCompte(B); break; //Si 3, désactiver ou supprimer un compte 
            case 4 : afficherListeComptes(B); break; //Si 4, affichage de la liste des comptes 
            case 5 : Cp = choisirCompte(B); afficherTitulairesCompte(B, Cp); break; //Si 5, affiche les titulaires d'un compte choisis 
            case 6 : { //Si 6, consultation des opérations sur une période donnée 
                Compte Cpm = choisirCompte(B);
                int t1, t2;
                printf("Entrez la date de début:\n>");
                lire_entier(&t1);
                printf("Entrez la date de fin:\n>");
                lire_entier(&t2);
                if (Cpm->ListeOperations->taille == 0){
                    printf(RED "\nRien à afficher sur cette période\n" RESET);
                    break;
                }
                afficherListeOpPeriode(Cpm, t1, t2); //Affichage des données sur la pèrionde 
                printf("Tapez 1 si vous souhaitez récupérer vos opération sur une période dans un fichier (deconnexion pour récupérer le fichier):\n>");
                int test; 
                scanf("%d", &test); 
                if(test == 1){
                    exporterCSV(Cpm, t1, t2); //Possibilité de récupéré les opérations dans un fichier au format CSV 
                }
                break;
            }
            case 7 : return; //Si 7, retour au menu précédent 
        }
    }
}

void menuGestionDesClients(Banque B){ //Fonction qui permet de gérer les clients de la banque 
    int i = 0;
    Titulaire_Compte Tc;
    while (i != 7){ //Tant que l'on souhaite rester dans ce menu 
        printf(CYN "\nMenu de gestion des clients:\n" RESET);
        printf("1 - Créer un Compte Client/Titulaire\n2 - Modifier un Compte Client/Titulaire\n3 - Supprimer un Client/Titulaire\n4 - Afficher la liste des comptes d'un titulaire\n5 - Afficher la liste des titulaires (numéro nom)\n6 - Afficher la liste des titulaires (toutes infos)\n7 - Retour au menu précédent\n>");
        lire_entier(&i); //Demande le choix de l'administrateur 
        while (i<1 || i>7){ //Vérifie que ce choix existe 
            printf(RED "Ce choix n'est pas disponible, veuillez réessayer\n>" RESET);
            lire_entier(&i);
        }

        if ((i==2 || i==3 || i==4 || i==5 || i==6) && B->ListeClients->taille==0){ //Vérifie qu'il y a des clients dans la banque 
                    printf(RED "\nIl n'y a aucun titulaire dans la banque, impossible de faire ce choix\n" RED);
                    continue; //Si non, retour au menu en cours 
        }

        switch(i){//Suivant le choix de l'administrateur 
            case 1 : creerTitulaire(B); break; //Si 1, création d'un nouveau titulaire 
            case 2 : Tc = choisirTitulaire(B); modifierTitulaire(B, Tc); break; //Si 2, modification d'un titulaire 
            case 3 : Tc = choisirTitulaire(B); supprimerClient(B, Tc); break; //Si 3, suppression d'un titulaire 
            case 4 : Tc = choisirTitulaire(B); afficherListeComptesClient(B, Tc); break; //Si 4, affichage de la liste des comptes d'un titulaire 
            case 5 : printf(CYN "\nListe des clients de la banque (identifiant nom prenom)\n" RESET); afficherListeIdTitulaires(B); break; //Si 5, affichage des titulaires de la banque seulement leur identifiant, leur nom et leur prenom 
            case 6 : afficherListeTitulaires(B); break; //Si 6, affiche la liste des titulaires au complet 
            case 7 : return; //Si 7, retour au menu précédent 
        }
    }
}

void menuAdministrationAdmin(Banque B){ //Fonction qui permet de modifier le mot de passe de l'administrateur 
    modifierMotDePasseAdmin(B); //Modifie le mot de passe de l'administrateur 
    return;
}

void menuDesactiver_SupprimerCompte(Banque B){ //Fonction qui gère le menu pour désactiver ou supprimer un compte 
    int i = 0;
    Compte Cp;
    while (i != 3){ //Tant que l'on souhaite rester dans ce menu : 
        printf(CYN "\nMenu de Désactivation/Suppression de Compte:\n" RESET);
        printf("1 - Désactiver un compte\n2 - Supprimer un compte\n3 - Retour au menu précédent\n>");
        lire_entier(&i); //Choix de l'administrateur 
        while (i<1 || i>3){ //Vérifie si ce choix est possible 
            printf(RED "Ce choix n'est pas disponible, veuillez réessayer\n>" RESET);
            lire_entier(&i);
        }

        switch(i){ //Suivant le choix fait par l'administrateur 
            case 1 : { //Si 1, désactivation d'un compte 
                Cp = choisirCompte(B);
                if (Cp->actif == 0){ //Vérifie si le compte est actif 
                    printf(RED "Ce compte est déja désactivé\n" RESET); //Si non, affichage du message comme quoi il est déjà inactif 
                    break;
                }
                modifierEtatCompte(Cp); //Si oui, modification de l'état, passage de actif à inactif 
                break;
            }
            case 2 : Cp = choisirCompte(B); supprimerCompte(B, Cp); break; //Si 2, suppression d'un compte après sa sélection 
            case 3 : return; //Si 3, retour au menu précédent 
        }
    }
    return;
}

void ConnectionClients(Banque B){ //Fonction qui gère la connexion des clients 
    printf(CYN "\nConnectez vous avec vos codes personnels\n" RESET);
    printf("Entrez un numéro de compte:\n>");
    int id;
    lire_entier(&id); //Demande le numéro de compte 
    if (rechercherTitulaire(B, id) == -1){ //Vérifie qi le compte existe 
        printf(RED "Aucun client avec ce numéro de compte n'a été trouvé, Retour au menu principal\n" RESET);
        return; //Si non, retour au menu précédent 
    }
    Titulaire_Compte Tc = retournerTitulaire(B, id);
    int mdp = decrypter(Tc->mdp); //Récupération du mot de passe de cette espace client 
    printf("Entrez le mot de passe associé\n>");
    int mdp2;
    mdp2 = lire_MDP(); //Demande au client de rentrer son mot de passe 
    if (mdp != mdp2){ //Vérifie que le mot de passe taper est correct 
        printf(RED "Le mot de passe est incorrect\n" RESET);
        return; //Si non, retour au menu précédent 
    }
    printf(YEL "\nBienvenue cher client\n" RESET); 
    menuClients(B, Tc); //Si oui, ouverture du menu client 
}

void menuClients(Banque B, Titulaire_Compte Tc){ //Fonction qui gère le menu des clients 
    printf("Voici le récapitulatif de votre compte:\n");
    afficherClient(Tc); //Affichage des informations du client 
    int i = 0;
    while (i != 4){ //Tant que l'on souhaite pas sortir de ce menu : 
        printf(CYN "\nMenu Client:\n" RESET);
        printf("1 - Gestion des comptes\n2 - Administration\n3 - Afficher les informations de votre compte\n4 - Quitter la session\n>");
        lire_entier(&i); //Choix de l'action du client 
        while (i<1 || i>4){ //Vérifie que ce choix est possible 
            printf(RED "Ce choix n'est pas disponible, veuillez réessayer\n>" RESET);
            lire_entier(&i); //Si non, recommencer 
        }
        if (i==1 && Tc->ListeNumCmpt->taille == 0){ //Vérifie que le client possède des comptes 
            printf(RED "\nVous n'avez aucun compte, veuillez demander une création de compte dans la rubrique Administration\n" RESET); //Si non, propose de demander la création d'un compte 
            continue;
        }
        switch(i){//Suivant le choix du client : 
            case 1 : menuGestionDesComptesClient(B, Tc); break; //Si 1, va dans le menu de gestion des comptes 
            case 2 : menuAdministrationClient(B, Tc); break; //Si 2, va dans le menu qui gère les informations du client 
            case 3 : afficherClient(Tc); break; //Si 3, affiche les informations du client 
            case 4 : return; //Si 4, retour au menu précédent 
        }
    }
}

void menuGestionDesComptesClient(Banque B, Titulaire_Compte Tc){ //Fonction qui gère le menu de gestions des comptes d'un client 
    int i = 0;
    while (i != 5){ //Tant que l'on souhaite rester dans ce menu : 
        printf(CYN "\nMenu de Gestion des comptes:\n" RESET);
        printf("1 - Consulter le solde de vos comptes\n2 - Consulter la liste des opérations d'un compte\n3 - Accéder au menu virements\n4 - Afficher la liste de vos comptes\n5 - Revenir au menu précédent\n>");
        lire_entier(&i); //L'utilisateur rentre son choix 
        while (i<1 || i>51){ //Vérifie que le choix existe 
            printf(RED "Ce choix n'est pas disponible, veuillez réessayer\n>" RESET);
            lire_entier(&i); //Si non, recommencer 
        }

        switch(i){ //suivant le choix du client 
            case 1 : { //Si 1, consultation des soldes de chaque comptes 
                printf("\nVoici la liste de vos comptes avec leurs soldes respectifs:\n");
                Element E;
                for (int i=0; i<Tc->ListeNumCmpt->taille; i++){ //Boucle qui permet d'afficher le solde pour chaque compte 
                    E = retournerElement(Tc->ListeNumCmpt, i);
                    printf("Compte numéro %d Solde: %f\n", retournerCompte(B, E->contenu.NCp)->NumeroCompte, retournerCompte(B, E->contenu.NCp)->solde);
                }
                break;
            }
            case 2 :{ //Si 2, consultation des opérations d'un compte 
                printf("Voici la liste de vos comptes:\n");
                Element E;
                //Affiche tous les numéros de comptes que possède le client 
                for (int i=0; i<Tc->ListeNumCmpt->taille; i++){
                    E = retournerElement(Tc->ListeNumCmpt, i);
                    printf("Compte numéro %d\n", retournerCompte(B, E->contenu.NCp)->NumeroCompte);
                }
                int m;
                printf("Entrez le numéro du compte dont vous souhaitez voir les opérations\n>");
                lire_entier(&m); //Choix du compte 
                if (rechercherElement(Tc->ListeNumCmpt, m)==-1){ //Vérifie que se compte existe ou qu'il appartient bien au client 
                    printf(RED "Compte non trouvé ou ne vous appartenant pas\n" RESET);
                    break;
                }
                int t1, t2;
                printf("Entrez la date de début:\n>");
                lire_entier(&t1); //Demande à l'utilisateur la date à partir de laquelle il souhaite consulter les opérations 
                printf("Entrez la date de fin:\n>");
                lire_entier(&t2); //Demande à l'utilisateur la date avant laquelle il souhaite consulter les opérations 
                Compte Cpm = retournerCompte(B, m);
                if (Cpm->ListeOperations->taille == 0){//Regarde si il existe des opéarions sur cette pèriode 
                    printf(RED "\nRien à afficher sur cette période\n" RESET);
                    break; //Si non, retour au menu 
                }
                afficherListeOpPeriode(Cpm, t1, t2); //Si oui, affiche la liste des opérations sur cette pèriode 
                printf("Tapez 1 si vous souhaitez récupérer vos opération sur une période dans un fichier (deconnexion pour récupérer le fichier):\n");
                int test; 
                scanf("%d", &test); 
                if(test == 1){
                    exporterCSV(Cpm, t1, t2); //Possibilité d'exporter dans un fichier au format CSV la liste des opérations sur cette période
                }
                break;
            }
            case 3 : menuVirement(B, Tc); break; //Si 3, accède au menu permettant de faire un virement 
            case 4 : afficherListeComptesClient(B, Tc); break; //Si 4, affiche la liste des comptes du client 
            case 5 : return; //Si 5, retour au menu précédent 
        }
    }
}

void menuVirement(Banque B, Titulaire_Compte Tc){ //Fonction qui gère le menu des virements 
    int i = 0;
    while (i != 4){
        printf(CYN "\nMenu des virements\n" RESET);
        printf("1 - Effectuer un virement\n2 - Consulter la liste de tous les virements d'un compte\n3 - Consulter la liste des virements entre vos comptes\n4 - Annuler et revenir au menu précédent\n>");
        lire_entier(&i); //Demande au client son choix 
        while (i<1 || i>4){ //Vérifie si ce choix existe 
            printf(RED "Ce choix n'est pas disponible, veuillez réessayer\n>" RESET);
            lire_entier(&i); //Si non, recommencer 
        }

        switch(i){ //Suivant le choix du client : 
            case 1 : { //Si 1, effectuer un virement 
                printf("Voici la liste de vos comptes:\n");
                Element E;
                for (int i=0; i<Tc->ListeNumCmpt->taille; i++){ //Boucle pour afficher tous les numéros de comptes 
                    E = retournerElement(Tc->ListeNumCmpt, i);
                    printf("Compte numéro %d\n", retournerCompte(B, E->contenu.NCp)->NumeroCompte);
                }
                int m;
                printf("Entrez le numéro du compte depuis lequel vous souhaitez effectuer un virement\n>");
                lire_entier(&m); //Choix du compte 
                if (rechercherElement(Tc->ListeNumCmpt, m)==-1){ //Vérifie si ce compte existe et appartient au client 
                    printf(RED "Compte non trouvé ou ne vous appartenant pas\n" RESET);
                    break; //Si non, retour au menu 
                }
                Compte C1 = retournerCompte(B, m);
                printf("Entrez le numéro du compte sur lequel vous souhaitez effectuer un virement\n>");
                int NC2;
                lire_entier(&NC2); //Demande à l'utilisateur un compte sur lequel faire le virement 
                if (rechercherElement(B->ListeComptes->LivretsA, m)==-1 && rechercherElement(B->ListeComptes->CCourants, m)==-1 && rechercherElement(B->ListeComptes->CJoints, m)==-1 && rechercherElement(B->ListeComptes->PEL, m)==-1){ //Vérifie si ce compte existe
                    printf(RED "Compte inexistant\n" RESET);
                    break; //Si non, retour au menu 
                }
                virement(C1, retournerCompte(B, NC2)); //Virement entre les deux comptes 
                break;
            }
            case 2 : {//Si 2, consulter la liste des virements d'un compte
                printf("Voici la liste de vos comptes:\n");
                Element E;
                for (int i=0; i<Tc->ListeNumCmpt->taille; i++){ //Boucle qui permet d'afficher toute la liste des comptes du client 
                    E = retournerElement(Tc->ListeNumCmpt, i);
                    printf("Compte numéro %d\n", retournerCompte(B, E->contenu.NCp)->NumeroCompte);
                }
                int m;
                printf("Entrez le numéro du compte dont vous souhaitez voir les virements\n>");
                lire_entier(&m); //Choix du compte 
                if (rechercherElement(Tc->ListeNumCmpt, m)==-1){ //Vérifie si le compte existe et qu'il appartien bien au client 
                    printf(RED "Compte non trouvé ou ne vous appartenant pas\n" RESET);
                    break; //Si non, retour au menu 
                }
                Compte Cp = retournerCompte(B, m);
                for (int j=0; j<Cp->ListeOperations->taille; j++){ //Boucle qui permet d'afficher toutes les opérations de type virement 
                    E = retournerElement(Cp->ListeOperations, j);
                    if (E->contenu.Op->typeOp == 1){ //Vérifie que l'opération est bien de type virement 
                        afficherOperation(E->contenu.Op); //Si oui, affichage de l'opération 
                    }
                }
                break;
            }
            case 3 : { //Si 3, afficher la liste des virements entre ses propres comptes 
                printf("Voici la liste de vos comptes:\n");
                Element E;
                for (int i=0; i<Tc->ListeNumCmpt->taille; i++){ //Affiche la liste des comptes du clients 
                    E = retournerElement(Tc->ListeNumCmpt, i);
                    printf("Compte numéro %d\n", retournerCompte(B, E->contenu.NCp)->NumeroCompte);
                }
                int m;
                printf("Entrez le numéro du compte dont vous souhaitez voir les virements avec vos autres comptes\n>");
                lire_entier(&m); //Choix du compte 
                if (rechercherElement(Tc->ListeNumCmpt, m)==-1){ //Vérifie si le compte existe et qu'il lui appartient 
                    printf(RED "Compte non trouvé ou ne vous appartenant pas\n" RESET);
                    break; //Si non, retour au menu 
                }
                Compte Cp = retournerCompte(B, m);
                Operation Op;
                int r1, r2;
                for (int j=0; j<Cp->ListeOperations->taille; j++){ //Affichage de la liste des opérations entre comptes 
                    E = retournerElement(Cp->ListeOperations, j);
                    Op = E->contenu.Op;
                    r1 = rechercherElement(Tc->ListeNumCmpt, Op->identifiantCompteE);
                    r2 = rechercherElement(Tc->ListeNumCmpt, Op->identifiantCompteR);
                    if (E->contenu.Op->typeOp == 1 && r1 != -1 && r2 != -1){ //Vérifie que les comptes de l'opération appartienne bien au client 
                        afficherOperation(E->contenu.Op); //Si oui, affiche l'opération 
                    }
                }
                break;
            }
            case 4 : return; //Si 4, retour au menu précédent 
        }
    }
}

void menuAdministrationClient(Banque B, Titulaire_Compte Tc){ //Fonction qui gère le menu pour modifier les informations du clients 
    int i = 0;
    while (i != 4){ //Tant que l'on souhaite pas sortir du menu 
        printf(CYN "\nMenu Administration:\n" RESET);
        printf("1 - Changer le mot de passe\n2 - Demander la création ou la suppression d'un compte\n3 - Modifier des informations personelles\n4 - Revenir au menu précédent\n>");
        lire_entier(&i); //Choix du client 
        while (i<1 || i>4){ //Vérifie si le choix est possible 
            printf(RED "Ce choix n'est pas disponible, veuillez réessayer\n>" RESET);
            lire_entier(&i); //Si non, recommencer 
        }

        switch(i){ //Suivant le choix : 
            case 1 : { //Si 1, changer le mot de passe 
                int mdpa = decrypter(Tc->mdp); //Récupère le mot de passe 
                int mdpav;
                printf("Veuillez entrer l'ancien mot de passe:\n>");
                mdpav = lire_MDP(); //Demande au client d'entrer son nouveau mot de passe 
                if (mdpa != mdpav){ //Vérifie que les mot de passe sont égaux 
                    printf(RED "L'ancien mot de passe est incorrect, retour\n" RESET);
                    break; //Si non, arrêt
                }
                int mdpn = demanderNouveauMDP(); //Demande le nouveau de passe 
                Tc->mdp = crypter(mdpn); //Crypte le nouveau mot de passe 
                printf("Mot de passe changé\n");
                break;
            }
            case 2 : printf("\nToute demande effectuée effacera la demande précédente si elle n'a pas encore été traitée.\n"); Tc->msgT = ""; modifierClientMessage(B, Tc); break; //Si 2, demande de suppression ou de création de compte 
            case 3 : modifierTitulaire(B, Tc); break; //Si 3, modifier les informations personnelles du client 
            case 4 : return; //Si 4, retour au menu précédent 
        }
    }
}

void menuNouveauClient(Banque B){ //Fonction qui gère le menu pour les nouveaux clients 
    printf(YEL "\nBienvenue honorable étranger(ère), ici vous pouvez créer un compte dans la banque la plus fabuleuse qui soit.\n" RESET);
    printf("Commencez donc par entrer vos coordonnées\n");
    creerTitulaire(B); //Permet d'entrer les coordonnées du nouveau client 
    Element E = retournerElement(B->ListeClients, B->ListeClients->taille-1);
    Titulaire_Compte Tc = E->contenu.Tc;
    //Création d'un compte 
    printf("\nVous allez aussi pouvoir créer votre premier compte bancaire!\n");
    Compte Cp = initialiserCompte();
    modifierType(B, Cp);
    int tit2 = 0;
    int t = Cp->typeCmpt;
    Cp->NumeroCompte = creerNumero(B->ListeComptes, t);
    if (Cp->typeCmpt == 2){ //Test si c'est un compte joint 
        //Si oui, rentre le deuxième titulaire ou non 
        printf("Entrez l'identifiant du titulaire avec lequel ce compte sera partagé:\n>");
        lire_entier(&tit2); //Lecture du deuxième titulaire 
        while (rechercherTitulaire(B, tit2) == -1){ //Vérifie si le titulaire existe 
            printf("Il n'y a pas de titulaire avec ce numéro, réessayez ou entrez 0 pour ne pas avoir de deuxième titulaire pour l'instant\n>");
            lire_entier(&tit2); //Si non, recommencer 
            if (tit2 == 0){ //Regarde si l'on souhaite ne pas en mettre 
                break; //Si oui, arrêt 
            }
        }
    }
    Cp->actif = 1;
    Cp->ListeOperations = initialiserListe(1);
    Cp->nbTitulaires = 1;
    if (tit2 != 0){
        Cp->nbTitulaires = 2;
    }
    Cp->NumeroTit1 = Tc->identifiant;
    Cp->NumeroTit2 = tit2;
    Cp->solde = 0;
    Element E2 = initialiserElement();
    //Ajout du compte dans la banque 
    if (t == 1){
        ajoutListe(B->ListeComptes->CCourants, E2);
    }
    else if (t == 2){
        ajoutListe(B->ListeComptes->CJoints, E2);
    }
    else if (t == 3){
        ajoutListe(B->ListeComptes->LivretsA, E2);
        }
    else{
        ajoutListe(B->ListeComptes->PEL, E2);
    }
    E2->contenu.Cp = Cp;
    Element E3 = initialiserElement();
    //Ajout le compte à la liste des comptes du client 
    ajoutListe(Tc->ListeNumCmpt, E3);
    E3->contenu.NCp = Cp->NumeroCompte;

    printf("Récapitulatif:\n");
    afficherCompte(B, E2->contenu.Cp); //Affiche le compte créer 
    printf("\nCompte créé avec succès\n");

    //Donne les informations pour la nouvelle connexion : 
    printf("\nExcellent! Vous pouvez dès à présent vous connecter sans aucune vérification de votre identité.\n");
    printf(MAG "Votre numéro de compte est \"%d\" (sans les guillemets), à tout de suite!\n>" RESET, retournerElement(B->ListeClients, B->ListeClients->taille -1)->contenu.Tc->identifiant);
}
