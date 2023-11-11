#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp3.h"


/* **************************************
 * Creation et initialisation Transaction
 ************************************** */
T_Transaction* creerTransaction(int id, float montant, char* descr) {

    T_Transaction* nouvelleTransaction = malloc(sizeof(T_Transaction));
    if (nouvelleTransaction != NULL) {
        // l'allocation memoire s'est bien passee
        // Important : malloc permet d'avoir l'espace memoire dedie pour notre champ de structure
        nouvelleTransaction->description = malloc(strlen(descr) + 1);
        strcpy(nouvelleTransaction->description, descr);
        nouvelleTransaction->idEtu = id;
        nouvelleTransaction->montant = montant;
        nouvelleTransaction->suivant = NULL;
    }

    return nouvelleTransaction;
}


/* ********************************
 * 1.	Ajout d'une transaction en tete d'une liste de transactions :
 ******************************** */
T_Transaction* ajouterTransaction(int idEtu, float montant, char* descr, T_Transaction* listeTransaction) {
    T_Transaction* tempT = creerTransaction(idEtu, montant, descr); //creer une nouvel transaction « tempT »
    tempT->suivant = listeTransaction; //ajouter la nouvel transaction
    //listeTransaction = tempT; //faire de « tempT » la tete de listeTransaction

    return tempT;
}


/* ********************************
 * Creation et initialisation Bloc
 ******************************** */
T_Block* creerBloc(int id, char* date) {
    T_Block* nouveauBloc = malloc(sizeof(T_Block));
    if (nouveauBloc != NULL) {
        // l'allocation memoire s'est bien passee
        nouveauBloc->dateBloc = malloc(strlen(date) + 1);
        // reserver la space pour '\0'
        strcpy(nouveauBloc->dateBloc, date);
        nouveauBloc->idBloc = id;
        nouveauBloc->listeTransactions = NULL; //la tete d'une liste de transactions
        nouveauBloc->suivant = NULL;//le nouveaubloc est le dernier
    }

    return nouveauBloc;
}





void changerdate(char* date_ancien, char* date_now, int update){

    int date = atoi(date_ancien);
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;

    int daysInMonth;

    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        // Months with 31 days
        daysInMonth = 31;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        // Months with 30 days
        daysInMonth = 30;
    } else if (month == 2) {
        // fevrier
        // Handle leap years
        int daysInFebruary = 28;
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            // Leap year
            daysInFebruary = 29;
        }
        daysInMonth = daysInFebruary;
    } else {
        printf("le date est une erreur ");
        return;
    }

    if (day + update > daysInMonth) {

        month += (day + update) / daysInMonth;
        if( month == 13){
            month = 1;
            year++;
        }

        day = (day + update) % daysInMonth;
        if( day == 0)
            day = daysInMonth ;
    }
    else {
        day += update;
    }

    sprintf(date_now, "%04d%02d%02d", year, month, day);

}




extern int blocid;
/* ********************************
 * 2.	Ajout d'un bloc en tete de la BlockChain :
 ******************************** */
BlockChain ajouterBlock(BlockChain bc) {
    char date_ancien[10];

    char date_now[10];

    int update;
    printf("Vous souhaitez etablir la date dans combien de jours ?(<=28)\n");
    scanf("%d", &update);
    while(update > 28){
        printf("erreur\nVous souhaitez etablir la date dans combien de jours ?(<=28)\n");
        scanf("%d", &update);
    }
    //verifier s'il existe une date identique
    T_Block* pB = bc;
    strcpy(date_ancien,pB->dateBloc);

    /*while (pB != NULL) { ////////////////////////////////////////////////// à modifier
        if (strcmp(date, pB->dateBloc) == 0) {
            printf("Cette date existe deja !\n");
            return bc;
        }
        id += 1;//id: le nombre de blocs;
        pB = pB->suivant;
    }*/
    blocid ++;

    changerdate(date_ancien,date_now, update);

    T_Block* tempB = creerBloc(blocid, date_now); //creer un nouveau bloc « tempB »
    tempB->suivant = bc; // attacher le nouveau bloc a la tete de la liste existante
    printf("Succes !\n");
    return tempB;
}






/* ********************************
 * 3.	Calcul de la somme des EATCoin credites et depenses par un etudiant sur une journee :
 ******************************** */
float totalTransactionEtudiantBlock(int idEtu, T_Block b) {
    float total = 0;
    T_Transaction* pT = b.listeTransactions; //« pT » pointe sur la tete de listeTransactions
    while (pT != NULL) {
        if (pT->idEtu == idEtu) {
            total += pT->montant;
        }
        pT = pT->suivant;
    }
    return total;
}


/* ********************************
 * 4.	Calcul du solde total d'un etudiant:
 ******************************** */
float soldeEtudiant(int idEtu, BlockChain bc) {
    float solde = 0;
    T_Block* pB = bc; //« pB » pointe sur la tete de BlockChain
    while (pB != NULL) {
        solde += totalTransactionEtudiantBlock(idEtu, *pB);
        pB = pB->suivant;
    }
    return solde;
}


/* ********************************
 * 5.	Rechargement du compte d'un etudiant :
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
 * 7.	Historique d'un etudiant :
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
                printf("\n* date : %s, blockID : %d\n",pB->dateBloc, pB->idBloc); //imprimer la date
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
 * 8.	Transfert de EATCoins entre deux etudiants :
 ******************************** */
int transfert(int idSource, int idDestination, float montant, char* descr, BlockChain bc) {
    T_Block* p = bc;//verifier que le pointeur pointe vers ce jour, sans modification
    float money = soldeEtudiant(idSource, bc);
    if (montant < money)
    {
        p->listeTransactions = ajouterTransaction(idSource, -montant, descr, bc->listeTransactions);
        p->listeTransactions = ajouterTransaction(idDestination, montant, descr, bc->listeTransactions);
    }
    else
    {
        printf("\n* L'etudiant (ID ==%d) n'a pas assez d'argent.\n", idSource);
    }
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
