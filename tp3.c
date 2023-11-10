#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp3.h"


/* **************************************
 * Création et initialisation Transaction
 ************************************** */
T_Transaction* creerTransaction(int id, float montant, char* descr) {

    T_Transaction* nouvelleTransaction = malloc(sizeof(T_Transaction));
    if (nouvelleTransaction != NULL) {
        // l'allocation mémoire s'est bien passée
        // Important : malloc permet d'avoir l'espace mémoire dédié pour notre champ de structure
        nouvelleTransaction->description = malloc(strlen(descr) + 1);
        strcpy(nouvelleTransaction->description, descr);
        nouvelleTransaction->idEtu = id;
        nouvelleTransaction->montant = montant;
        nouvelleTransaction->suivant = NULL;
    }

    return nouvelleTransaction;
}


/* ********************************
 * 1.	Ajout d'une transaction en tête d'une liste de transactions :
 ******************************** */
T_Transaction* ajouterTransaction(int idEtu, float montant, char* descr, T_Transaction* listeTransaction) {
    T_Transaction* tempT = creerTransaction(idEtu, montant, descr); //créer une nouvel transaction « tempT »
    tempT->suivant = listeTransaction; //ajouter la nouvel transaction
    //listeTransaction = tempT; //faire de « tempT » la tête de listeTransaction

    return tempT;
}


/* ********************************
 * Création et initialisation Bloc
 ******************************** */
T_Block* creerBloc(int id, char* date) {
    T_Block* nouveauBloc = malloc(sizeof(T_Block));
    if (nouveauBloc != NULL) {
        // l'allocation mémoire s'est bien passée
        nouveauBloc->dateBloc = malloc(strlen(date) + 1);
        // reserver la space pour '\0'
        strcpy(nouveauBloc->dateBloc, date);
        nouveauBloc->idBloc = id;
        nouveauBloc->listeTransactions = NULL; //la tête d'une liste de transactions
        nouveauBloc->suivant = NULL;//le nouveaubloc est le dernier
    }

    return nouveauBloc;
}


/* ********************************
 * 2.	Ajout d'un bloc en tête de la BlockChain :
 ******************************** */
BlockChain ajouterBlock(BlockChain bc) {
    char date[12];
    int id = 0;
    printf("Entrer la date :\n");
    scanf("%s", date);
    //vérifier s'il existe une date identique
    T_Block* pB = bc;
    while (pB != NULL) {
        if (strcmp(date, pB->dateBloc) == 0) {
            printf("Cette date existe déjà !\n");
            return bc;
        }
        id += 1;//id: le nombre de blocs;
        pB = pB->suivant;
    }

    T_Block* tempB = creerBloc(id, date); //créer un nouveau bloc « tempB »
    tempB->suivant = bc; // attacher le nouveau bloc à la tête de la liste existante
    printf("Succes !\n");
    return tempB;
}


/* ********************************
 * 3.	Calcul de la somme des EATCoin crédités et dépensés par un étudiant sur une journée :
 ******************************** */
float totalTransactionEtudiantBlock(int idEtu, T_Block b) {
    float total = 0;
    T_Transaction* pT = b.listeTransactions; //« pT » pointe sur la tête de listeTransactions
    while (pT != NULL) {
        if (pT->idEtu == idEtu) {
            total += pT->montant;
        }
        pT = pT->suivant;
    }
    return total;
}


/* ********************************
 * 4.	Calcul du solde total d'un étudiant:
 ******************************** */
float soldeEtudiant(int idEtu, BlockChain bc) {
    float solde = 0;
    T_Block* pB = bc; //« pB » pointe sur la tête de BlockChain
    while (pB != NULL) {
        solde += totalTransactionEtudiantBlock(idEtu, *pB);
        pB = pB->suivant;
    }
    return solde;
}


/* ********************************
 * 5.	Rechargement du compte d'un étudiant :
 ******************************** */
void crediter(int idEtu, float montant, char* descr, BlockChain bc) {
    T_Block* p = bc;//verifier que le pointeur pointe vers ce jour, sans modification
    p->listeTransactions = ajouterTransaction(idEtu, montant, descr, bc->listeTransactions);
}


/* ********************************
 * 6.	Paiement d'un repas :
 ******************************** */
int payer(int idEtu, float montant, char* descr, BlockChain bc) {
    T_Block* p = bc;//verifier que le pointeur pointe vers ce jour, sans modification
    float money = soldeEtudiant(idEtu, bc);
    if (montant > money) {
        return 0;
    }
    else {
        montant = -montant;
        p->listeTransactions = ajouterTransaction(idEtu, montant, descr, bc->listeTransactions);
        return 1;
    }
}


/* ********************************
 * 7.	Historique d'un étudiant :
 ******************************** */
int consulter(int idEtu, BlockChain bc) {
    BlockChain pB = bc;
    T_Transaction* pT = NULL;
    int nb_Etu = 0;
    int nb_Trans = 0;
    int tmpVal = 1;

    while (pB != NULL)
    {
        pT = pB->listeTransactions;
        while (pT != NULL)
        {
            if (tmpVal == 1)
            {
                printf("\n* date : %s\n", pB->dateBloc); //imprimer la date
                tmpVal = 0;
            }
            if (pT->idEtu == idEtu)
            {
                printf("  %.2f        (%s)\n", pT->montant, pT->description); //imprimer le montant et la description
                nb_Etu += 1;
                nb_Trans += 1;
            }
            pT = pT->suivant;
        }
        pB = pB->suivant;
        tmpVal = 1;
        if(nb_Trans == 0 && nb_Etu != 0) {printf("  Pas de transactions.\n");}
    }
    return nb_Etu;
}


/* ********************************
 * 8.	Transfert de EATCoins entre deux étudiants :
 ******************************** */
int transfert(int idSource, int idDestination, float montant, char* descr, BlockChain bc) {
    T_Block* p = bc;//verifier que le pointeur pointe vers ce jour, sans modification
    p->listeTransactions = ajouterTransaction(idSource, -montant, descr, bc->listeTransactions);
    p->listeTransactions = ajouterTransaction(idDestination, montant, descr, bc->listeTransactions);
    return 1;
}


/* *********************
 * Fonctions utilitaires
 ********************* */
 // Vider le buffer (utile quand on utlise des getchar() )
void viderBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}
