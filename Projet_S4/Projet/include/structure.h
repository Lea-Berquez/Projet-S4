typedef struct _Titulaire_Compte *Titulaire_Compte;
typedef struct _Operation *Operation;
typedef struct _Compte *Compte;
typedef struct _Comptes *Comptes;
typedef struct _Element *Element;
typedef struct _Liste *Liste;
typedef struct _Banque *Banque; 



//Liste chaînée
struct _Element {
    union {
        Operation Op;
        Compte Cp;
        int NCp;
        Titulaire_Compte Tc;
    } contenu;
    Element suivant;
};

struct _Liste {
    enum _type {OPERATIONS = 1, COMPTES = 2, NUMEROS_COMPTES = 3, CLIENTS = 4} type;
    int taille;
    Element premier;
};


//Structures du programme
struct _Titulaire_Compte {
    char* nom_prenom;
    char * mail;
    char* numTelephone;
    int identifiant;
    int mdp;
    Liste ListeNumCmpt;
    char* msgT;
};

struct _Operation {
    int date;  //format AAAAMMJJ
    enum _typeOp { VIREMENT = 1, CHANGEMENT_DU_SOLDE = 2, CHANGEMENT_INFOS = 3} typeOp;
    int identifiantCompteE;
    int identifiantCompteR;
    float montant;
};

typedef enum _typeCompte typeCompte;
enum _typeCompte {C_COURANT = 1, C_JOINT = 2, LIVRET_A = 3, PEL = 4};

struct _Compte {
    int NumeroCompte;
    typeCompte typeCmpt;
    enum _nbTitulaires { UN = 1, DEUX = 2} nbTitulaires;
    int NumeroTit1;
    int NumeroTit2;
    float solde;
    Liste ListeOperations;
    enum _actif { INACTIF = 0, ACTIF = 1} actif;
};

struct _Comptes {
    Liste LivretsA;
    Liste CCourants;
    Liste PEL;
    Liste CJoints;
};

struct _Banque {
    Liste ListeClients;
    Comptes ListeComptes;
    int mdpAdmin;
};


//Fonction d'initialisation
Element initialiserElement();
Liste initialiserListe(int type);
Titulaire_Compte initialiserTitulaire();
Operation initialiserOperation();
Compte initialiserCompte();
Comptes initialiserComptes();
Banque initialiserBanque();


//Fonctions Utilisation Liste
void afficherListe(Banque, Liste);
void ajoutListe(Liste liste, Element elem);
int rechercherElement(Liste liste, int identifiant);
Element retournerElement(Liste liste, int position);
int supprimerElement(Liste liste, int position);


