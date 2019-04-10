#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "../include/structure.h"
#include "../include/client.h"
#include "../include/entrees.h"
#include "../include/clients_administrateur.h"
#include "../include/administrateur.h"

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"


//Compte
int getNumC(Compte Cp){ //Fonction qui renvoie le numéro du compte passé en parametre 
    return Cp->NumeroCompte;
}

void afficherCompte(Banque B, Compte Cp){ //Fonction qui affiche les informations du compte passé en paramètre 
    printf("--------------------------------\n");
    printf("Numéro de compte: %d\n", Cp->NumeroCompte);
    printf("Type du compte: %d\n", Cp->typeCmpt);
    printf("Etat: ");
    if (Cp->actif == INACTIF){
        printf("INACTIF\n");
    }
    else {
        printf("ACTIF\n");
    }
    printf("Type de compte: %d\n", Cp->typeCmpt);
    if (Cp->nbTitulaires > 0){ //Regarde le nombre de titulaires 
        printf("Numéro du titulaire 1: %d\n", Cp->NumeroTit1);
        if (Cp->nbTitulaires == 2){
            printf("Numéro du titulaire 2: %d\n", Cp->NumeroTit2);
        }
    }
    else{
        printf("Ce compte n'a pas de titulaire\n");
    }
    printf("Solde: %f\n", Cp->solde);
    printf("Nombre d'opérations effectuées: %d\n", Cp->ListeOperations->taille);
    printf("--------------------------------\n");
}

int rechercherCompte(Banque B, int num){ //Fonction qui permet de rechercher un compte dans la banque à partir de son numéro 
    int b = 1;
    //Suivant le premier chiffre (1 = compte courant, 2 = compte joint, 3 = livret A, 4 = PEL) on peut retrouve le compte 
    if (num>=1000 && num <2000){
        b = rechercherElement(B->ListeComptes->CCourants, num);
        if (b!= -1) return 1;
    }
    else if (num>=2000 && num <3000){
        b = rechercherElement(B->ListeComptes->CJoints, num);
        if (b!= -1) return 2;
    }
    else if (num>=3000 && num <4000){
        b = rechercherElement(B->ListeComptes->LivretsA, num);
        if (b!= -1) return 3;
    }
    else{
        b = rechercherElement(B->ListeComptes->PEL, num);
        if (b!= -1) return 4;
    }
    return b; //Renvoie l'indice de sa position dans la liste 
}

Compte retournerCompte(Banque B, int num){ //Fonction qui permet de retourner un compte un partir de son numéro 
    int t = rechercherCompte(B, num);
    Element E;
    int pos;
    if (t == 1){
        pos = rechercherElement(B->ListeComptes->CCourants, num);
        E = retournerElement(B->ListeComptes->CCourants, pos);
    }
    else if (t == 2){
        pos = rechercherElement(B->ListeComptes->CJoints, num);
        E = retournerElement(B->ListeComptes->CJoints, pos);

    }
    else if (t == 3){
        pos = rechercherElement(B->ListeComptes->LivretsA, num);
        E = retournerElement(B->ListeComptes->LivretsA, pos);
        
    }
    else {
        pos = rechercherElement(B->ListeComptes->PEL, num);
        E = retournerElement(B->ListeComptes->PEL, pos);
    }
    return E->contenu.Cp;
}

int creerNumero(Comptes C, int type){ //Fonction qui permet de creer un numéro pour un compte 
    int n;
    //On crée le numéro à partir de son type (premier chiffre =  (1 = compte courant, 2 = compte joint, 3 = livret A, 4 = PEL)) et les chiffres suivant dépendent du nombre déja present de comptes dans la banque 
    if (type == 1){
        n = C->CCourants->taille;
    }
    if (type == 3){
        n = C->LivretsA->taille;
    }
    if (type == 4){
        n = C->PEL->taille;
    }
    if (type == 2){
        n = C->CJoints->taille;
    }
    return (type*1000)+n;
}

void modifierTitulaireCompte(Banque B, Compte Cp, int Tit){ //Fonction qui permet de modifier le titulaire d'un compte 
    //Création d'une opération associé au compte pour notifier le fait que l'on a modifier le titulaire 
    Operation Op = initialiserOperation();
    Op->typeOp = 3;
    Element E = initialiserElement();
    E->contenu.Op = Op;
    ajoutListe(Cp->ListeOperations, E);
    
    if (Cp->nbTitulaires == 0){ //Si le compte n'a pas de titulaire 1 (dû à une suppression ou initialisation)
        Cp->nbTitulaires = 1;
        printf("Ce compte n'a pas de Titulaire principal pour l'instant\nDéclaration du Titulaire principal ou Titulaire1:\n");

        Titulaire_Compte Tc = choisirTitulaire(B); //On retourne le client concerné
        Cp->NumeroTit1 = Tc->identifiant; //On met le numéro du client dans le compte
        Element E1  = initialiserElement();
        E1->contenu.NCp = Cp->NumeroCompte; //On crée un élément contenant le numéro du compte
        ajoutListe(Tc->ListeNumCmpt, E1); //On ajoute cet élément à la liste des numeros du compte du client
        return;
    }

    Titulaire_Compte AncienTc;
    if (Tit == 3){
        if (Cp->nbTitulaires == 1){
            printf("Ce compte n'a pas de deuxième titulaire!\n");
            return;
        }
        AncienTc = retournerTitulaire(B, Cp->NumeroTit2);
        int pos;
        pos = rechercherElement(AncienTc->ListeNumCmpt, Cp->NumeroCompte); //On cherche la position de l'ACp dans la liste de l'ancien tit
        supprimerElement(AncienTc->ListeNumCmpt, pos); //On supprime l'occurence
        Cp->nbTitulaires = 1;
        return;
    }

    Titulaire_Compte Tc = choisirTitulaire(B);

    if (Tit == 1){ //Suivant si Titulaire 1 ou 2 à modifier
        printf("Changement du premier Titulaire\n");
        AncienTc = retournerTitulaire(B, Cp->NumeroTit1); //On sauve l'ancien titulaire
        Cp->NumeroTit1 = Tc->identifiant; //On met le numéro du nouveau titulaire dans le compte
    }
    else {
        if (Cp->nbTitulaires == 2){ //s'il y a déja un deuxième tit
            Titulaire_Compte ATc1 = retournerTitulaire(B, Cp->NumeroTit1);
            if (ATc1->identifiant == Tc->identifiant){
                printf("Ce Client est déja le Titulaire 1, impossible de l'ajouter à nouveau\n");
                return;
            }
            printf("Changement du deuxième Titulaire\n");
            AncienTc = retournerTitulaire(B, Cp->NumeroTit2);
            Cp->NumeroTit2 = Tc->identifiant;
        }
        else { //s'il n'y a pas de deuxième tit
            Titulaire_Compte ATc1 = retournerTitulaire(B, Cp->NumeroTit1);;
            if (ATc1->identifiant == Tc->identifiant){
                printf("Ce Client est déja le Titulaire 1, impossible de l'ajouter à nouveau\n");
                return;
            }
            printf("Ajout d'un deuxième titulaire\n");
            Cp->nbTitulaires = 2;
            Cp->NumeroTit2 = Tc->identifiant;
            Element E1  = initialiserElement();
            E1->contenu.NCp = Cp->NumeroCompte; //On crée un élément contenant le numéro du compte
            ajoutListe(Tc->ListeNumCmpt, E1); //On ajoute cet element à la liste des ACp du nouveau Tit
            return;
        }
    }

    int pos;
    pos = rechercherElement(AncienTc->ListeNumCmpt, Cp->NumeroCompte); //On cherche la position de l'ACp dans la liste de l'ancien tit
    supprimerElement(AncienTc->ListeNumCmpt, pos); //On supprime l'occurence
    Element E1  = initialiserElement();
    E1->contenu.NCp = Cp->NumeroCompte; //On crée un élément contenant le numéro du compte
    ajoutListe(Tc->ListeNumCmpt, E1); //On ajoute cet element à la liste des numeros de compte du nouveau Tit
}

void modifierType(Banque B, Compte Cp){ //Fonction qui permet de modifier le type d'un compte passé en parametre 
    printf("Quel type de compte?\n1 - CCourant\n2 - CJoint\n3 - Livret A\n4 - PEL\n>");
    int t;
    lire_entier(&t); //Demande le type choisis 
    while (t<1 || t>4){ //Vérifie que ce type existe bien 
        printf(RED "Cela ne correspond à aucun type, veuillez réessayer\n>" RESET);
        lire_entier(&t);
    }

    Cp->typeCmpt = t;
} 

//Operations

Operation creerOperation(Compte c1, Compte c2, int montant){ //Fonction qui permet de creer une opération à partir de deux comptes et d'un entier correspondant au montant 
    Operation op = initialiserOperation();  
    op->identifiantCompteE = c1->NumeroCompte; 
    op->identifiantCompteR = c2->NumeroCompte; 
    op->montant = montant;
    return op; 
}

void afficherOperation(Operation Op){ //Fonction qui permet d'afficher les informations d'une opération 
    printf("--------------------------------\n");
    printf("Date de l'opération: %d\n", Op->date);
    printf("Type de l'opération: ");
    //Test si c'est un virement, un changement de solde ou une modification d'informations du compte 
    if (Op->typeOp == 1){
        printf("VIREMENT\n");
        printf("Identifiant du compte émetteur: %d\n", Op->identifiantCompteE);
        printf("Identifiant du compte récepteur: %d\n", Op->identifiantCompteR);
    }
    else if (Op->typeOp == 2){
        printf("CHANGEMENT DU SOLDE\n");
    }
    else {
        printf("MODIFICATION D'INFORMATIONS DU COMPTE\n");
        printf("--------------------------------\n");
        return;

    }
    printf("Montant: %f\n", Op->montant);
    printf("--------------------------------\n");
}

void afficherListeOpPeriode(Compte cp, int datemin, int datemax){ //Fonction qui permet d'afficher la liste des opérations sur une période choisie par l'utilisateur 
    int t = 0;
    Element e = cp->ListeOperations->premier;
    int c = 0;
    while (t < cp->ListeOperations->taille){ //Boucle permettant d'afficher toutes les opérations sur la période 
        if (e->contenu.Op->date >= datemin && e->contenu.Op->date <= datemax){
            afficherOperation(e->contenu.Op); //Affiche chaque opérations de la période 
            c++;
        }
        t++;
        e = e->suivant;
    }
    if (c == 0){ //Test si il existe bien des opérations 
        printf("\nRien à afficher sur cette période\n"); //Si non 
    }
}

//Titulaire_Compte
void afficherListeComptesClient(Banque B, Titulaire_Compte Tc){ //Affiche la liste des comptes d'un client passé en parametre 
    printf("Liste des comptes de ce client:\n");
    Compte Cp;
    Element elem;
    for (int i = 0; i<Tc->ListeNumCmpt->taille; i++){ //Regarde dans la liste des comptes d'un client 
        elem = retournerElement(Tc->ListeNumCmpt, i);
        Cp = retournerCompte(B, elem->contenu.NCp);
        afficherCompte(B, Cp);
    }
}

int rechercherTitulaire(Banque B, int identifiant){ //Fontion qui permet de rechercher un titulaire dans la banque à partir de son identifiant 
    return rechercherElement(B->ListeClients, identifiant);
}

Titulaire_Compte retournerTitulaire(Banque B, int identifiant){ //Fonction qui permet de retourner un titulaire à partir de son identifiant 
    Element E;
    int pos = rechercherElement(B->ListeClients, identifiant);
    E = retournerElement(B->ListeClients, pos);
    return E->contenu.Tc;
}

void modifierTitulaireNom(Titulaire_Compte Tc){ //Fonction qui permet de modifier le nom et prénom d'un client passé en paramètre 
    printf("Entrez le nouveau nom complet:\n>");
    char* nom_prenom;
    lire_string(&nom_prenom); //Demande à l'utilisateur son nouveau nom et prénom 
    printf("Le nouveau nom sera %s.\n", nom_prenom); //Affiche ce que l'utilisateur a choisis 
    Tc->nom_prenom = nom_prenom; //Sauvegarde de cette nouvelle information
}

void modifierTitulaireMail(Titulaire_Compte Tc){ //Fonction qui permet de modifier le mail d'un client passé en parametre 
    printf("Entrez le nouveau mail:\n>");
    char* mail;
    lire_string(&mail); //Demande à l'utilisateur son nouveau mail 
    printf("Le nouveau mail sera %s.\n", mail); //Affiche ce que l'utilisateur a choisis 
    Tc->mail = mail; //Sauvegarde de cette nouvelle information 
}

void modifierTitulaireTel(Titulaire_Compte Tc){ //Fonction qui permet de modifier le numéro de téléphone d'un client passé en parametre 
    printf("Entrez le nouveau numéro de téléphone:\n>");
    char* num;
    lire_string(&num); //Demande à l'utilisateur son nouveau numéro de téléphone 
    printf("Le nouveau numéro de téléphone sera %s.\n", num); //Affiche ce que l'utilisateur a choisis
    Tc->numTelephone = num; //Sauvegarde de cette nouvelle information 
}

void modifierClientMessage(Banque B, Titulaire_Compte tc){ //Fonction qui permet de modifier le message qu'un client passé en parametre souhaite faire passer à l'administrateur
    printf("Si vous souhaitez supprimer un compte rentrez SC, demander la création d'un compte rentrez CC :\n>"); 
    char* n; 
    lire_string(&n); //Demande à l'utilisateur ce qu'il souhaite entrer comme message (SC = suppimer compte, CC = créer compte) 
    if(strcmp("CC",n)==0){ //Si CC, 
        demandeCreationCompte(B, tc); //Lance la demande de création de compte 
    }
    else if(strcmp("SC",n)==0){ //Si SC, 
        if (tc->ListeNumCmpt->taille == 0){ //Vérifie que le client possède bien des comptes 
            printf(RED "Vous n'êtes titulaire d'aucun compte, retour au menu\n" RESET);
            return; //Si non arrêt de la fonction 
        }
        demandeSuppressionCompte(B, tc); //Si oui, lance la demande de suppression de compte 
    }
}

void modifierTitulaire(Banque B, Titulaire_Compte Tc){ //Fonction qui permet de modifier les informations d'un titulaires 
    int m;
    do {
        printf("Que voulez vous modifier?\n");
        printf("1 - Nom-Prénom\n");
        printf("2 - Numéro de Téléphone\n");
        printf("3 - Mail\n");
        printf("4 - Quitter ce menu\n>");

        lire_entier(&m); //Demande à l'utilisateur ce qu'il souhaite modifier 
        while (m<1 || m>4){ //Vérifie que le choix fait par l'utilisateur existe 
            printf(RED "Erreur lors du choix, Veuillez réessayer\n>" RESET);
            lire_entier(&m);
        }
        switch(m){ 
            case 1 : modifierTitulaireNom(Tc); break; //Si 1, modifie le nom 
            case 2 : modifierTitulaireTel(Tc); break; //Si 2, modifie le numéro de téléphone 
            case 3 : modifierTitulaireMail(Tc); break; //Si 3, modifie le mail 
            case 4 : return; //Si 4, quitte le menu 
        }
    } while (m != 4);
}

void afficherClient(Titulaire_Compte Tc){ //Fontion qui permet d'afficher les informations d'un client passé en parametre 
    printf("--------------------------------\n");
    printf("Client numéro %d\n", Tc->identifiant); 
    printf("Nom-Prénom: %s\n", Tc->nom_prenom);
    printf("Numéro Tel: %s\n", Tc->numTelephone);
    printf("Mail: %s\n", Tc->mail);
    printf("Nombre de comptes: %d\n", Tc->ListeNumCmpt->taille);
    if(Tc->ListeNumCmpt->taille > 0){
        printf("Numero de comptes:\n"); 
        int i; 
        Element e = Tc->ListeNumCmpt->premier; 
        for(i = 0; i < Tc->ListeNumCmpt->taille; i++){ //Affiche tous les comptes du client 
            printf("     Compte : %d\n", e->contenu.NCp); 
             e = e->suivant;
        }
    }
    printf("--------------------------------\n");
}

void creerTitulaire(Banque B){ //Fonction qui permet de creer un nouveau titulaire 
    int id;
    if (B->ListeClients->taille == 0){ //Regarde si la banque est vide 
        id = 600000; //Si oui, on creer un tout premier client 
    }
    else{ //Si non, on rajoute un client à la suite et son numéro est égal au précédent plus un 
        Element E = retournerElement(B->ListeClients, B->ListeClients->taille-1);
        id = E->contenu.Tc->identifiant + 1;
    }

    //On ajoute se titulaire à la liste des clients 
    Element e = initialiserElement();
    ajoutListe(B->ListeClients, e);
    Element e2 = retournerElement(B->ListeClients, B->ListeClients->taille -1);

    //On rentre les informations de ce nouveau client 
    Titulaire_Compte Tc = initialiserTitulaire();
    Tc->identifiant = id; //Son identifiant 
    Tc->msgT = malloc(100 * sizeof(char)); 
    strcpy(Tc->msgT, "Ok"); //Son message 
    printf("Entrez le nom_prenom (pas d'espaces):\n>");
    lire_string(&(Tc->nom_prenom)); //Son nom et son prénom 
    printf("Entrez le numéro de Téléphone:\n>");
    lire_string(&(Tc->numTelephone)); //Son numéro de téléphone 
    printf("Entrez le mail:\n>");
    lire_string(&(Tc->mail)); //Son mail 
    Tc->mdp = crypter(demanderNouveauMDP()); //Son mot de passe que l'on pense à crypter 
    e2->contenu.Tc = Tc;
    printf("Récapitulatif:\n"); //Affiche le récapitulatif des informations rentrées 
    afficherClient(e2->contenu.Tc);
}


//Mot de Passe
int demanderNouveauMDP(){ //Fonction qui permet de rentrer son nouveau mot de passe 
    int mdpn1, mdpn2; 
    mdpn1 = -1;
    mdpn2 = 0;
    while (mdpn1 != mdpn2){ //Boucle qui test si les deux mot de passe rentrés sont égaux 
        printf("Entrez le nouveau mot de passe:\n>");
        mdpn1 = lire_MDP(); //Demande à l'utilisateur de rentrer une première fois son nouveau mot de passe 
        printf("Entrez le nouveau mot de passe une deuxième fois (Vérification):\n>");
        mdpn2 = lire_MDP(); //Demande à l'utilisateur de rentrer une seconde fois son nouveau mot de passe pour vérification 
        if (mdpn1 != mdpn2){ //Regarde si les deux mot de passe sont égaux 
            printf("Les mots de passe ne correspondent pas, réessayez.\n");
            mdpn1 = -1; 
        }
    }
    return mdpn1; //Renvoie le nouveau mot de passe 
}

int crypter(int i){ //Fonction qui permet de crypter le mot de passe 
    int ncrypt; 
    ncrypt = i * 3 + 12; //Choix de crypter le mot de passe en faisant fois trois plus douze 
    return ncrypt; //Renvoie du mot de passe crypter 
}

int decrypter(int i){ //Fonction qui permet de décrypter le mot de passe 
    int ndcrypt; 
    ndcrypt = (i - 12) / 3; //Décryptage à partir du calcul de cryptage 
    return ndcrypt; //Renvoie le mot de passe choisis par l'utilisateur, celui décrypter 
}
