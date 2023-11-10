#ifndef TP3_H_INCLUDED
#define TP3_H_INCLUDED

// Structures et types
struct Transaction {
    int idEtu;
    float montant;
    char* description;
    struct Transaction* suivant;
};
typedef struct Transaction T_Transaction;



struct Block {
    int idBloc;
    char* dateBloc; // date du bloc sous la forme AAAAMMJJ (ex : 20231009 pour le 09/10/2023)
    T_Transaction* listeTransactions;
    struct Block* suivant;
};
typedef struct Block T_Block;//les transaction de toutes les etudiants



typedef T_Block* BlockChain; //la tête de chaîne de T_Block



// Création et initialisation des structures
T_Transaction* creerTransaction(int id, float montant, char* descr);
T_Block* creerBloc(int id, char* date);


// 1.	Ajout d'une transaction en tête d'une liste de transactions :
T_Transaction* ajouterTransaction(int idEtu, float montant, char* descr, T_Transaction* listeTransaction);

// 2.	Ajout d'un bloc en tête de la BlockChain :
BlockChain ajouterBlock(BlockChain bc);

// 3.	Calcul de la somme des EATCoin crédités et dépensés par un étudiant sur une journée :
float totalTransactionEtudiantBlock(int idEtu, T_Block b);

// 4.	Calcul du solde total d'un étudiant:
float soldeEtudiant(int idEtu, BlockChain bc);

// 5.	Rechargement du compte d'un étudiant :
void crediter(int idEtu, float montant, char* descr, BlockChain bc);

// 6.	Paiement d'un repas :
int payer(int idEtu, float montant, char* descr, BlockChain bc);

// 7.	Historique d'un étudiant :
int consulter(int idEtu, BlockChain bc);

// 8.	Transfert de EATCoins entre deux étudiants :
int transfert(int idSource, int idDestination, float montant, char* descr, BlockChain bc);



// UTILS
void viderBuffer();

#endif // TP3_H_INCLUDED
