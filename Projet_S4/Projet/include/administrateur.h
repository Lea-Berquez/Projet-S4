//Banque
void afficherBanque(Banque);
void modifierMotDePasseAdmin(Banque);
int demanderAncienMDPAdmin(Banque);


//Comptes
void afficherListeComptes(Banque);
void afficherListeNumerosCompte(Banque);


//Titulaires_Compte
Titulaire_Compte choisirTitulaire(Banque);
void afficherListeTitulaires(Banque); 
void supprimerClient(Banque, Titulaire_Compte);
void afficherListeIdTitulaires(Banque);


//Compte
Compte choisirCompte(Banque B);
void afficherTitulairesCompte(Banque, Compte);
void modifierCompte(Banque, Compte);
void modifierTitulairesCompte(Banque, Compte); 
void modifierEtatCompte(Compte); 
void modifierSolde(Compte);
void supprimerCompte(Banque, Compte);
void ajouterArgentCompte(Compte);
void creerCompte(Banque);
