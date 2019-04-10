#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <string.h> 
#include "../include/structure.h"
#include "../include/client.h"
#include "../include/entrees.h"
#include "../include/administrateur.h"
#include "../include/clients_administrateur.h" 
#include "../include/parson.h"

void chargementMDPBanque(Banque b){ //Fonction qui permet de charge le mot de passe de la banque présent dans un fichier .json 
    JSON_Value *root_value;
    JSON_Array *commits;
    JSON_Object *commit;

    FILE * fichier = NULL;
    fichier = fopen("mdpBanque.json","r");
    if (fichier == NULL){ //Vérifie que le fichier existe et qu'il a pu être ouvert 
        printf("le fichier n'a pas été ouvert\n");
        return; 
    }

    root_value = json_parse_file("mdpBanque.json");

    commits = json_value_get_array(root_value); //Récupère le tableau présent dans le fichier 

    if (json_value_get_type(root_value) != JSONArray) { //Vérifie que le fichier est un bien un tableau 
        return; 
    }

    commit = json_array_get_object(commits, 0); //Récupère le premier object du tableau (comme ce n'est que le mot de passe il n'y a qu'un objet)

    b->mdpAdmin = (int)json_object_dotget_number(commit, "mdp"); //Récupère le chiffre du champ "mdp" dans le fichier qui correspond au mot de passe et la sauvegarde dans la banque 
}


void  remplirTitulaireStructurePremierePhase (Banque b){ //Fonction qui permet de charger tout les clients de la banque à partir d'un fichier .json
    JSON_Value *root_value;
    JSON_Array *commits;
    JSON_Object *commit;
    size_t i;

    FILE * fichier = NULL;
    fichier = fopen("clients.json","r");
    if (fichier == NULL){ //Vérifie que le fichier existe et qu'il peut s'ouvrir 
        printf("le fichier n'a pas été ouvert\n");
        return; 
    }

    root_value = json_parse_file("clients.json");

    commits = json_value_get_array(root_value); //Il récupère le tableau dans le fichier 

    if (json_value_get_type(root_value) != JSONArray) { //Vérifie que l'élement récupérer est bien un tableau 
        return; 
    }
    
    for (i = 0; i < json_array_get_count(commits); i++) { //Avec cette boucle on récupère chaque élément du taleau 

        commit = json_array_get_object(commits, i); //Récupère l'objet du tableau à la position i 

        Titulaire_Compte tc = initialiserTitulaire(); 

        tc->nom_prenom = malloc(50 * sizeof(char));  

        //Récupération des informations dans le .json et enregiste dans la banque : 

        strcpy(tc->nom_prenom, json_object_dotget_string(commit, "Titulaire_Compte.nom_prenom")); //Récupère le nom_prenom 

        tc->identifiant = (int)json_object_dotget_number(commit, "Titulaire_Compte.id"); //Récupère l'identifiant 

        strcpy(tc->mail, json_object_dotget_string(commit, "Titulaire_Compte.mail")); //Récupère le mail 

        strcpy(tc->numTelephone, json_object_dotget_string(commit, "Titulaire_Compte.numTelephone")); //Récupère le numéro de téléphone 

        tc->mdp = (int)json_object_dotget_number(commit, "Titulaire_Compte.mdp"); //Récupère le mot de passe 

        strcpy(tc->msgT, json_object_dotget_string(commit, "Titulaire_Compte.msgT")); //récupère le message 

        JSON_Array *commits2;
        JSON_Object *commit2;
        size_t j; 

        //Création d'une deuxième boucle pour récupérer les numéros des comptes qui sont dans un tableau 
        commits2 = json_object_dotget_array(commit, "Titulaire_Compte.ListeNumCmpt"); 

        for(j = 0; j < json_array_get_count(commits2); j++){ 
            commit2 = json_array_get_object(commits2, j); 

            Element e = initialiserElement(); 

            ajoutListe(tc->ListeNumCmpt, e); 

            Element e2 = retournerElement(tc->ListeNumCmpt, tc->ListeNumCmpt->taille-1); 

            e2->contenu.NCp = (int)json_object_dotget_number(commit2, "NumCmpt"); 

        } 

        //Ajout du titulaire traiter dans la banque 
        Element e1 = initialiserElement(); 

        ajoutListe(b->ListeClients, e1); 

        Element e2 = retournerElement(b->ListeClients, b->ListeClients->taille-1); 
        e2->contenu.Tc = tc; 

    }

    json_value_free(root_value); 
}

void remplirComptesStructureDeuxiemePhase(Banque b){ //Fonction qui permet de charger tout les comptes de la banque à partir d'un fichier .json
    JSON_Value *root_value;
    JSON_Array *commits;
    JSON_Object *commit;
    size_t i;

    FILE * fichier = NULL;
    fichier = fopen("comptes.json","r");
    if (fichier == NULL){ //Vérifie que le fichier existe et qu'il s'ouvre 
        printf("le fichier n'a pas été ouvert\n");
        return; 
    }

    root_value = json_parse_file("comptes.json");
    
    commits = json_value_get_array(root_value); //Récupère le tableau avec tous les comptes contenu dans le .json 

    if (json_value_get_type(root_value) != JSONArray) { //Vérifie que l'élément récupérer est bien un tableau 
        return; 
    }

    for (i = 0; i < json_array_get_count(commits); i++) { //Boucle qui passe tous les éléments du tableau pour récupérer tous les comptes 
        commit = json_array_get_object(commits, i); //Récupère l'objet dans la case i 

        Compte c = initialiserCompte();  

        //Récupération des informations dans le .json et enregiste dans la banque :

        c->NumeroCompte = (int)json_object_dotget_number(commit, "Compte.NumeroCompte"); //Récupère le numéro du compte
        c->typeCmpt = (int)json_object_dotget_number(commit, "Compte.typeCmpt"); //Récupère le type de compte 
        c->nbTitulaires = (int)json_object_dotget_number(commit, "Compte.nbTitulaires"); //Récupère le nombre de titulaires 
        c->NumeroTit1 = (int)json_object_dotget_number(commit, "Compte.NumeroTit1"); //Récupère le numéro du premier titulaire du compte 
        c->NumeroTit2 = (int)json_object_dotget_number(commit, "Compte.NumeroTit2"); //Récupère le numéro du deuxième titulaire du compte 
        c->solde = (float)json_object_dotget_number(commit, "Compte.solde"); //Récupère le solde 

        c->actif = (int)json_object_dotget_number(commit, "Compte.actif"); //Récupère si le compte est actif ou inactif 

        size_t j; 
        JSON_Array *tab; 
        tab = json_object_dotget_array(commit, "Compte.ListeOperation"); 

        JSON_Object  *elem; 
        for(j = 0; j < json_array_get_count(tab); j++){ //Deuxième boucle pour récupérer la liste des opérations qui se trouve dans un nouveau tableau 
            elem = json_array_get_object(tab, j); 
            Operation op = initialiserOperation(); 
 
            op->date = (int)json_object_dotget_number(elem, "date"); //Récupère la date de l'opération 
            op->identifiantCompteE = (int)json_object_dotget_number(elem, "identifiantTitE"); //Récupère l'identifiant du titulaire émetteur 
            op->identifiantCompteR = (int)json_object_dotget_number(elem, "identifiantTitR"); //Récupère l'indentifiant du titulaire récepteur 
            op->montant = (int)json_object_dotget_number(elem, "montant"); //Récupère le montant de l'opération 

            //Ajoute cette nouvelle opération à la liste des opérations 
            Element e2 = initialiserElement(); 

            ajoutListe(c->ListeOperations, e2); 

            Element e3 = retournerElement(c->ListeOperations, c->ListeOperations->taille-1); 
            e3->contenu.Op = op; 
        }

        Element e1 = initialiserElement();  

        Element e2; 

        switch(c->typeCmpt){ //Ajoute le compte dans la bonne liste suivant son type 
            case 1: //Ajout dans la liste des comptes courants 
                ajoutListe(b->ListeComptes->CCourants, e1); 
                e2 = retournerElement(b->ListeComptes->CCourants, b->ListeComptes->CCourants->taille-1); 
                e2->contenu.Cp = c; 
                break; 
            case 2 : //Ajout dans la liste des comptes joints 
                ajoutListe(b->ListeComptes->CJoints, e1); 
                e2 = retournerElement(b->ListeComptes->CJoints, b->ListeComptes->CJoints->taille-1); 
                e2->contenu.Cp = c;  
                break; 
            case 3 : //Ajout dans la liste des livrets A 
                ajoutListe(b->ListeComptes->LivretsA, e1); 
                e2 = retournerElement(b->ListeComptes->LivretsA, b->ListeComptes->LivretsA->taille-1); 
                e2->contenu.Cp = c; 
                break; 
            case 4 : //Ajout dans la liste des PEL 
                ajoutListe(b->ListeComptes->PEL, e1); 
                e2 = retournerElement(b->ListeComptes->PEL, b->ListeComptes->PEL->taille-1); 
                e2->contenu.Cp = c; 
                break; 
        } 
    }
    json_value_free(root_value); 
}

int chargementJSON(Banque b){ //Fonction globale qui permet de charger les informations de la banque au début du programme 
    chargementMDPBanque(b); //Chargment du mot de passe de la banque 
    remplirTitulaireStructurePremierePhase(b); //Chargement des clients 
    remplirComptesStructureDeuxiemePhase(b); //Chargement de comptes 
    return 0; 
}


void fermetureTitulaires(Banque b){ //Fonction qui à la fin du programme enregistre les clients sur un fichier .json 
    int t; 
    t = b->ListeClients->taille; 
    Element e = b->ListeClients->premier; 

    FILE* fichier = NULL; 
    fichier = fopen("clients.json", "w+"); //On ouvre ou creer le fichier et on écrase ce qu'il y a dedans. On a les fonctions de lire et écrire 

    if(fichier == NULL){ //Vérifie que le fichier s'ouvre et existe 
        return; 
    }

    fprintf(fichier, "[\n"); //Ouverture du tableau 

    //On parcrours tous les éléments de la liste de clients de la banque 
    if(t>0){ //Boucle qui s'arrete un élément avant la fin pour éviter des problèmes de virgule sur le dernier élément 
        while (t>1){
            //Toutes les fonctions suivantes sont là pour écrire dans le fichier 
            fprintf(fichier, "  {\n");
            fprintf(fichier, "      \"Titulaire_Compte\": {\n"); 
            fprintf(fichier, "          \"nom_prenom\":\"%s\",\n", e->contenu.Tc->nom_prenom); 
            fprintf(fichier, "          \"mail\":\"%s\",\n", e->contenu.Tc->mail); 
            fprintf(fichier, "          \"numTelephone\":\"%s\",\n", e->contenu.Tc->numTelephone); 
            fprintf(fichier, "          \"id\": %d,\n", e->contenu.Tc->identifiant); 
            fprintf(fichier, "          \"mdp\": %d,\n", e->contenu.Tc->mdp); 
            fprintf(fichier, "          \"ListeNumCmpt\": [\n"); 

            if( e->contenu.Tc->ListeNumCmpt->taille > 0){ //Regarde si le titulaire possède des comptes 
                int i; 
                Element e1 =  e->contenu.Tc->ListeNumCmpt->premier; 
                for(i = 0; i < ( e->contenu.Tc->ListeNumCmpt->taille-1); i++){ //Boucles juqu'à l'avant-dernier élément pour écrire tous les comptes 
                    fprintf(fichier, "              {\n"); 
                    fprintf(fichier, "                  \"NumCmpt\": %d\n", e1->contenu.NCp);  
                    fprintf(fichier, "              },\n"); //Présence de virgule
                    e1 = e1->suivant;
                }
                //Traitement du dernier élément sans la virgule de fin 
                fprintf(fichier, "              {\n"); 
                fprintf(fichier, "                  \"NumCmpt\": %d\n", e1->contenu.NCp);  
                fprintf(fichier, "              }\n"); //Pas de virgule 
            }
            fprintf(fichier, "          ],\n"); 
            fprintf(fichier, "          \"msgT\":\"%s\"\n", e->contenu.Tc->msgT);  
            fprintf(fichier, "       }\n");
            fprintf(fichier, "  },\n"); //Présence de virgule 
            t--;
            e = e->suivant; //On passe à l'élément suivant 
        }
        //Traitement du dernier élément de la liste sur le même principe que précédemment : 
        fprintf(fichier, "  {\n");
        fprintf(fichier, "      \"Titulaire_Compte\": {\n"); 
        fprintf(fichier, "          \"nom_prenom\":\"%s\",\n", e->contenu.Tc->nom_prenom); 
        fprintf(fichier, "          \"mail\":\"%s\",\n", e->contenu.Tc->mail); 
        fprintf(fichier, "          \"numTelephone\":\"%s\",\n", e->contenu.Tc->numTelephone); 
        fprintf(fichier, "          \"id\": %d,\n", e->contenu.Tc->identifiant); 
        fprintf(fichier, "          \"mdp\": %d,\n", e->contenu.Tc->mdp); 
        fprintf(fichier, "          \"ListeNumCmpt\": [\n"); 

        if( e->contenu.Tc->ListeNumCmpt->taille > 0){
            int i; 
            Element e1 =  e->contenu.Tc->ListeNumCmpt->premier; 
            for(i = 0; i < ( e->contenu.Tc->ListeNumCmpt->taille-1); i++){
                fprintf(fichier, "              {\n"); 
                fprintf(fichier, "                  \"NumCmpt\": %d\n", e1->contenu.NCp);  
                fprintf(fichier, "              },\n"); 
                e1 = e1->suivant;
            }
            fprintf(fichier, "              {\n"); 
            fprintf(fichier, "                  \"NumCmpt\": %d\n", e1->contenu.NCp);  
            fprintf(fichier, "              }\n"); 
        }
        fprintf(fichier, "          ],\n"); 
        fprintf(fichier, "          \"msgT\":\"%s\"\n", e->contenu.Tc->msgT);  
        fprintf(fichier, "       }\n");
        fprintf(fichier, "  }\n"); //Disparition de la virgule 
    }
    fprintf(fichier, "]"); //Fermeture du tableau 

    fclose(fichier); //Fermeture du fichier 

}

void ecrireComptes(Banque b, Liste liste, FILE* fichier){ //Fonction qui écrit les comptes d'une liste entrée en paramètre dans un fichier entré en paramètre 

    int t; 
    t = liste->taille; 
    Element e = liste->premier;  

    //On parcrours tous les éléments de la liste 
    if(t>0){ //Boucle qui s'arrete un élément avant la fin pour éviter des problèmes de virgule sur le dernier élément 
        while (t>1){
            //Toutes les fonctions suivantes sont là pour écrire dans le fichier 
            fprintf(fichier, "  {\n");
            fprintf(fichier, "      \"Compte\": {\n"); 
            fprintf(fichier, "          \"NumeroCompte\": %d,\n", e->contenu.Cp->NumeroCompte);
            fprintf(fichier, "          \"typeCmpt\": %d,\n", e->contenu.Cp->typeCmpt);
            fprintf(fichier, "          \"nbTitulaires\": %d,\n", e->contenu.Cp->nbTitulaires);
            fprintf(fichier, "          \"NumeroTit1\": %d,\n", e->contenu.Cp->NumeroTit1);
            fprintf(fichier, "          \"NumeroTit2\": %d,\n", e->contenu.Cp->NumeroTit2);
            fprintf(fichier, "          \"solde\": %f,\n", e->contenu.Cp->solde);
            fprintf(fichier, "          \"actif\": %d,\n", e->contenu.Cp->actif);
            fprintf(fichier, "          \"ListeOperation\": [\n"); 

            if( e->contenu.Cp->ListeOperations->taille > 0){ //Vérifie qu'il y a des opérations associées au compte 
                int i; 
                Element e1 =  e->contenu.Cp->ListeOperations->premier; 
                for(i = 0; i < ( e->contenu.Cp->ListeOperations->taille-1); i++){ //Boucles juqu'à l'avant-dernier élément pour écrire toutes les opérations
                    fprintf(fichier, "              {\n"); 
                    fprintf(fichier, "                  \"date\": %d,\n", e1->contenu.Op->date);  
                    fprintf(fichier, "                  \"identifiantTitR\": %d,\n", e1->contenu.Op->identifiantCompteR);  
                    fprintf(fichier, "                  \"identifiantTitE\": %d,\n", e1->contenu.Op->identifiantCompteE);  
                    fprintf(fichier, "                  \"montant\": %f\n", e1->contenu.Op->montant);  
                    fprintf(fichier, "              },\n"); //Présence de virgule 
                    e1 = e1->suivant;
                }
                //Traitement du dernier élément pour éviter des problèmes de virgule 
                fprintf(fichier, "              {\n"); 
                fprintf(fichier, "                  \"date\": %d,\n", e1->contenu.Op->date);  
                fprintf(fichier, "                  \"identifiantTitR\": %d,\n", e1->contenu.Op->identifiantCompteR);  
                fprintf(fichier, "                  \"identifiantTitE\": %d,\n", e1->contenu.Op->identifiantCompteE);  
                fprintf(fichier, "                  \"montant\": %f\n", e1->contenu.Op->montant);  
                fprintf(fichier, "              }\n"); //Absence de virgule 
            }
            fprintf(fichier, "          ]\n");  
            fprintf(fichier, "       }\n");
            fprintf(fichier, "  },\n"); //Présence de virgule 
            t--;
            e = e->suivant; //Passage à l'élément suivant 
        }
        //Traitement du dernier élément pour éviter des problèmes de virgule sur le même principe que précédemment 
        fprintf(fichier, "  {\n");
        fprintf(fichier, "      \"Compte\": {\n"); 
        fprintf(fichier, "          \"NumeroCompte\": %d,\n", e->contenu.Cp->NumeroCompte);
        fprintf(fichier, "          \"typeCmpt\": %d,\n", e->contenu.Cp->typeCmpt);
        fprintf(fichier, "          \"nbTitulaires\": %d,\n", e->contenu.Cp->nbTitulaires);
        fprintf(fichier, "          \"NumeroTit1\": %d,\n", e->contenu.Cp->NumeroTit1);
        fprintf(fichier, "          \"NumeroTit2\": %d,\n", e->contenu.Cp->NumeroTit2);
        fprintf(fichier, "          \"solde\": %f,\n", e->contenu.Cp->solde);
        fprintf(fichier, "          \"actif\": %d,\n", e->contenu.Cp->actif);
        fprintf(fichier, "          \"ListeOperation\": [\n"); 

        if( e->contenu.Cp->ListeOperations->taille > 0){
            int i; 
            Element e1 =  e->contenu.Cp->ListeOperations->premier; 
            for(i = 0; i < ( e->contenu.Cp->ListeOperations->taille-1); i++){
                fprintf(fichier, "              {\n"); 
                fprintf(fichier, "                  \"date\": %d,\n", e1->contenu.Op->date);  
                fprintf(fichier, "                  \"identifiantTitR\": %d,\n", e1->contenu.Op->identifiantCompteR);  
                fprintf(fichier, "                  \"identifiantTitE\": %d,\n", e1->contenu.Op->identifiantCompteE);  
                fprintf(fichier, "                  \"montant\": %f\n", e1->contenu.Op->montant);  
                fprintf(fichier, "              },\n"); 
                e1 = e1->suivant;
            }
            fprintf(fichier, "              {\n"); 
            fprintf(fichier, "                  \"date\": %d,\n", e1->contenu.Op->date);  
            fprintf(fichier, "                  \"identifiantTitR\": %d,\n", e1->contenu.Op->identifiantCompteR);  
            fprintf(fichier, "                  \"identifiantTitE\": %d,\n", e1->contenu.Op->identifiantCompteE);  
            fprintf(fichier, "                  \"montant\": %f\n", e1->contenu.Op->montant);  
            fprintf(fichier, "              }\n"); 
        }
        fprintf(fichier, "          ]\n");  
        fprintf(fichier, "       }\n"); //Absence de virgule 
        //fprintf(fichier, "  }\n");
    }
}

void fermetureComptes(Banque b){ //Fonction qui à la fin du programme enregistre les comptes sur un fichier .json 
    
    FILE* fichier = NULL; 
    fichier = fopen("comptes.json", "w+"); //On ouvre ou creer le fichier et on écrase ce qu'il y a dedans. On a les fonctions de lire et écrire 

    if(fichier == NULL){ //Vérifie que le fichier s'ouvre et existe 
        return; 
    }

    fprintf(fichier, "[\n"); //Ouverture du tableau 

    //Initialistaion des tailles des différentes listes de comptes 
    int tCCourants;  
    int tCCJoints;  
    int tLivretsA; 
    int tPEL; 

    tCCourants = b->ListeComptes->CCourants->taille; 
    tCCJoints = b->ListeComptes->CJoints->taille; 
    tLivretsA = b->ListeComptes->LivretsA->taille;
    tPEL = b->ListeComptes->PEL->taille;  

    //Test qui permet de gérer les problèmes de virgules 
    //Vigule si il y a des éléments après 
    //Pas de virgules si il n'y a pas d'élements après 
    if(tCCourants != 0 || tCCJoints !=0 || tLivretsA !=0 || tPEL != 0){
        if(tCCourants != 0){
            ecrireComptes(b, b->ListeComptes->CCourants, fichier); //Ecris les comptes courants dans le fichier 
            if(tCCJoints ==0  && tLivretsA ==0 && tPEL ==0){
                fprintf(fichier, "  }\n"); //Absence de virgule 
            }
            else {
                fprintf(fichier, "  },\n"); //Présence de virgule 
            }
        }
        if(tCCJoints != 0){
            ecrireComptes(b, b->ListeComptes->CJoints, fichier); //Ecris les comptes joints dans le fichier 
            if(tLivretsA ==0 && tPEL ==0){
                fprintf(fichier, "  }\n"); //Absence de virgule 
            }
            else {
                fprintf(fichier, "  },\n"); //Présence de virgule 
            }
        }
        if(tLivretsA != 0){
            ecrireComptes(b, b->ListeComptes->LivretsA, fichier); //Ecris les livrets A dans le fichier 
            if(tPEL ==0){
                fprintf(fichier, "  }\n"); //Absence de virgule 
            }
            else {
                fprintf(fichier, "  },\n"); //Présence de virgule 
            }
        }
        if(tPEL != 0){
            ecrireComptes(b, b->ListeComptes->PEL, fichier); //Ecris les PEL dans le fichier 
            fprintf(fichier, "  }\n"); //Absence de virgule 
        }


    }
    
    fprintf(fichier, "]"); //Fermeture du tableau 

    fclose(fichier); //Fermeture du fichier 

}

void fermetureMDP(Banque b){ //Fonction qui à la fin du programme enregistre le mot de passe de la banque sur un fichier .json 
    FILE* fichier = NULL; 
    fichier = fopen("mdpBanque.json", "w+"); //On ouvre ou creer le fichier et on écrase ce qu'il y a dedans. On a les fonctions de lire et écrire 

    if(fichier == NULL){ //Vérifie que le fichier s'ouvre et existe 
        return; 
    }

    //Toutes les fonctions suivantes sont là pour écrire dans le fichier 
    fprintf(fichier, "[\n"); 
    fprintf(fichier, "  {\n"); 
    fprintf(fichier, "      \"mdp\":%d\n", b->mdpAdmin); 
    fprintf(fichier, "  }\n"); 
    fprintf(fichier, "]\n"); 

    fclose(fichier); //Fermeture du fichier 
}


void fermetureJSON(Banque b){ //Fonction globale qui permet de charger les informations de la banque sur un fichier .json en fin du programme 
    fermetureMDP(b); //Sauvegarde du mot de passe dans un fichier .json 
    fermetureTitulaires(b); //Sauvegarde des titulaires dans un fichier .json 
    fermetureComptes(b); //Sauvegarde des comptes dans un fichier .json 
} 

//Export Operations Periode
void exporterCSV(Compte cp, int datemin, int datemax){ //Fonctions qui permet de sauvegarde dans un fichier sous le format CSV la liste des opérations d'un compte sur une certaine pèriode 
    FILE* fichier = NULL; 
    fichier = fopen("OperationPeriode.txt", "w+"); //On ouvre ou creer le fichier et on écrase ce qu'il y a dedans. On a les fonctions de lire et écrire 

    if(fichier == NULL){ //Vérifie que le fichier s'ouvre et existe 
        return; 
    }

    fprintf(fichier, "Date de l'opération,Type de l'opération,Identifiant du compte émetteur, Identifiant du compte récepteur,Montant\n"); //On écrit dans le fichier les initialisations de champs 
    int t = 0;
    Element e = cp->ListeOperations->premier;
    int c = 0;
    while (t < cp->ListeOperations->taille){ //Boucle qui passe toutes les opérations en revues 
        if (e->contenu.Op->date >= datemin && e->contenu.Op->date <= datemax){ //Vérifie que l'opération est bien entre les dates choisies 
            //Ecris les informations dans le fichier au format CSV, chaque champ séparé par une virgule 
            fprintf(fichier, "%d,", e->contenu.Op->date);
            if (e->contenu.Op->typeOp == 1){
                fprintf(fichier, "VIREMENT,");
                fprintf(fichier, "%d,", e->contenu.Op->identifiantCompteE);
                fprintf(fichier, "%d,", e->contenu.Op->identifiantCompteR);
                fprintf(fichier, "%f\n", e->contenu.Op->montant);
            }
            else if (e->contenu.Op->typeOp == 2){
                fprintf(fichier, "CHANGEMENT DU SOLDE,");
                fprintf(fichier, " ,"); //Quand il n'y a pas de champs correspondant à l'initialisation on ne met rien on laisse un espace 
                fprintf(fichier, " ,"); 
                fprintf(fichier, "%f\n", e->contenu.Op->montant);
            }
            else {
                fprintf(fichier, "MODIFICATION D'INFORMATIONS DU COMPTE");
                fprintf(fichier, " ,"); 
                fprintf(fichier, " ,"); 
                return;
            }
                c++;
            }
            t++;
            e = e->suivant; //Passage à l'opération suivante 
        }
    if (c == 0){//Regarde si il y a bien des opérations sur cette pèriode 
            fprintf(fichier, "Rien sur cette période\n"); //Si non 
    }
}

