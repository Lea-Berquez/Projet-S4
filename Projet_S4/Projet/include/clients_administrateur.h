//Compte
int getNumC(Compte);
void afficherCompte(Banque, Compte);
int rechercherCompte(Banque, int);
Compte retournerCompte(Banque, int);
int creerNumero(Comptes, int);
void modifierTitulaireCompte(Banque, Compte, int);
void modifierType(Banque, Compte); 


//Operations
Operation creerOperation(Compte , Compte , int); 
void afficherOperation(Operation); 
void afficherListeOpPeriode(Compte, int, int);


//Titulaire_Compte
void afficherListeComptesClient(Banque, Titulaire_Compte);
int rechercherTitulaire(Banque, int);
Titulaire_Compte retournerTitulaire(Banque, int);
void modifierTitulaireNom(Titulaire_Compte ); 
void modifierTitulaireMail(Titulaire_Compte);
void modifierTitulaireTel(Titulaire_Compte);
void afficherClient(Titulaire_Compte); 
void modifierClientMessage(Banque, Titulaire_Compte);
void modifierTitulaire(Banque, Titulaire_Compte );
void creerTitulaire(Banque);


//Mot de Passe
int demanderNouveauMDP();
int crypter(int);
int decrypter(int);
