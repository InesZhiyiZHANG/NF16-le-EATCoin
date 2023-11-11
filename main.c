#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS
#include "tp3.h"


int blocid;

int main() {

    blocid = 1;

    struct tm p;
    time_t nowtime;
    time(&nowtime);
    localtime_s(&p,&nowtime);
    printf("Aujourd'hui\nannee:%04d\ndate:%2d.%02d\n",p.tm_year+1900,p.tm_mon + 1 , p.tm_mday);
    BlockChain ma_chaine_de_blocs = NULL;
    long ddate = (p.tm_year+1900)*10000 + (p.tm_mon + 1)*100 + p.tm_mday;
    char cdate[10];
    itoa(ddate,cdate,10);
    printf("%s",cdate);
    T_Block* pB = ma_chaine_de_blocs;
    T_Block* tempB = creerBloc(blocid, cdate);
    tempB->suivant = ma_chaine_de_blocs;

    ma_chaine_de_blocs = tempB;
    ma_chaine_de_blocs->suivant = NULL;




    // ============= MENU UTILISATEUR ============= */
    char choix = '0';
    while (choix != '8') {
        printf("\n\n\n\n============================================================");
        printf("\n1. Afficher la liste des blocs de la BlockChain");
        printf("\n2. Afficher toutes les transactions d'un bloc");
        printf("\n3. Afficher toutes les transactions du jour pour un etudiant");
        printf("\n4. Afficher l'historique pour un etudiant");
        printf("\n5. Crediter un compte");
        printf("\n6. Payer un repas");
        printf("\n7. Transferer des EATCoins entre deux etudiants");
        printf("\n8. Quitter");
        printf("\n9. Ajouter un nouveau data");
        printf("\n============================================================");
        printf("\n>>La date du jour : %s, blockID : %d\n", ma_chaine_de_blocs->dateBloc, ma_chaine_de_blocs->idBloc);
        printf("\nVotre choix ? ");

        choix = getchar();

        switch (choix) {
        case '1':
        {
            printf("\n***** Affichage de la blockchain *****\n");
            T_Block* pB = ma_chaine_de_blocs;
            T_Transaction* pT = NULL;
            int nb_Trans = 0;
            while (pB != NULL)
            {
                printf("\n* date : %s, blockID : %d\n",pB->dateBloc, pB->idBloc);
                pT = pB->listeTransactions;
                while (pT != NULL)
                {
                    printf("  etudiant %d, montant %.2f        (%s)\n", pT->idEtu, pT->montant, pT->description);
                    nb_Trans += 1;
                    pT = pT->suivant;
                }
                if(nb_Trans == 0) {printf("  Pas de transaction.\n");}
                nb_Trans = 0;
                pB = pB->suivant;
            }
            printf("\n**********  Fin  affichage  **********\n");
            break;
        }

        case '2':
        {
            char date[12];
            printf("\nVous voudrez voir bloc de quel jour? (20231009 pour le 09/10/2020) ");
            scanf("%s", date);
            T_Block* pB = ma_chaine_de_blocs;
            int count_block = 0; //verifier si le block existe;
            int nb_Trans = 0;
            while (pB != NULL)
            {
                if (strcmp(date, pB->dateBloc) == 0)
                {
                    printf("\n* date : %s, blockID : %d\n",pB->dateBloc, pB->idBloc);
                    T_Transaction* pT = pB->listeTransactions;
                    while (pT != NULL)
                    {
                        printf("  etudiant %d, montant %.2f        (%s)\n", pT->idEtu, pT->montant, pT->description);
                        nb_Trans += 1;
                        pT = pT->suivant;
                    }
                    count_block++;
                }
                pB = pB->suivant;
            }

            if (count_block == 0) {printf("Le jour %s n'a pas ete trouve ", date);}
            else if (nb_Trans == 0) {printf("  Pas de transactions.");}
            break;
        }

        case '3':
        {
            T_Block* pB = ma_chaine_de_blocs;
            T_Transaction* pT = NULL;

            int idEtu;
            int count_etu = 0;
            printf("Quel est le ID d'etudiant que vous voudrez voir ? ");
            scanf("%d", &idEtu);
            printf("\n* Le solde d'etudiant (ID == %d) : %.2f\n", idEtu, soldeEtudiant(idEtu, ma_chaine_de_blocs));

            if (pB != NULL)
            {
                pT = pB->listeTransactions;
                printf("\n");
                while (pT != NULL)
                {
                    if (pT->idEtu == idEtu)
                    {
                        printf("  montant %.2f        (%s)\n", pT->montant, pT->description);
                        count_etu++;
                    }
                    pT = pT->suivant;
                }
            }

            if (count_etu == 0) {printf("L'etudiant (ID == %d) n'a pas ete trouve. ", idEtu);}
            break;
        }

        case '4':
        {
            viderBuffer();
            int idEtu;
            printf("\nQuel est le ID d'etudiant que vous voudrez voir ? ");
            scanf("%d", &idEtu);
            printf("\n* Le solde d'etudiant (ID == %d) : %.2f\n", idEtu, soldeEtudiant(idEtu, ma_chaine_de_blocs));
            if (consulter(idEtu, ma_chaine_de_blocs) == 0)
            {
                printf("\nL'etudiant (ID == %d) n'a pas ete trouve\n", idEtu);
            }
            break;
        }

        case '5':
        {
            int id;
            float montant;
            char descr[20];

            printf("\nEntrez ID de l'etudiant : ");
            scanf("%d", &id);
            printf("Entrez le montant a crediter : ");
            scanf("%f", &montant);
            // Appeler la fonction crediter pour augmenter le solde de l'etudiant
            crediter(id, montant, "credite", ma_chaine_de_blocs);

            printf("Credit ajoute avec succes.\n");
            break;
        }

        case '6'://n6. Payer un repas
        {
            int id;
            float montant;
            char descr[20];

            printf("\nEntrez ID de l'etudiant : ");
            scanf("%d", &id);
            printf("Entrez le montant a payer (positif) :");
            scanf("%f", &montant);

            // Appeler la fonction crediter pour augmenter le solde de l'etudiant
            if (payer(id, montant, "payer", ma_chaine_de_blocs)) {
                printf("\nLe paiement a ete effetue avec succes.\n");
            }
            else {
                printf("\nLe solde de l'etudiant est insuffisant !\n ");
            }
            break;
        }

        case '7':
        {
            int idSource, idDestination;
            float montant;
            char descr[20];

            printf("\nEntrez ID des etudiants (ex:1 2): ");
            scanf("%d %d", &idSource, &idDestination);

            if (idSource == idDestination)
            {
                printf("\nVous ne pouvez pas vous transferer de l'argent a vous-meme\n");
            }
            else
            {
                printf("Entrez le montant a exchanger (positif): ");
                scanf("%f", &montant);
                transfert(idSource, idDestination, montant, "exchange", ma_chaine_de_blocs);
            }

            break;
        }

        case '8':
        {
            T_Block* pB = ma_chaine_de_blocs;
            T_Block* pBtmp = NULL;
            T_Transaction* pT = NULL;
            T_Transaction* pTtmp = NULL;
            while (pB != NULL)
            {
                pT = pB->listeTransactions;
                while (pT != NULL)
                {
                    pTtmp = pT;
                    pT = pTtmp->suivant;
                    //printf("delet transaction idEtu = %d\n", pTtmp->idEtu); ////////////////////
                    free(pTtmp);
                }
                pBtmp = pB;
                pB = pBtmp->suivant;
                //printf("***delet block idBlock = %d\n", pBtmp->idBloc); ////////////////////
                free(pBtmp);
            }
            printf("\n======== PROGRAMME TERMINE ========\n");
            break;
        }

        case '9':
        {
            printf("\n");
            ma_chaine_de_blocs = ajouterBlock(ma_chaine_de_blocs);
            break;
        }

        default: printf("\n\nERREUR : votre choix n'est valide ! ");
        }
        viderBuffer(); // supprimer de l'entree standard le retour charriot et les eventuels caract¨¨res supplementaires tapes par l'utilisateur
    }

    return 0;
}
