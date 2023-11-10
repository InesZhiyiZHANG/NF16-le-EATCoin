#include <stdio.h>
#include <stdlib.h>
#include "tp3.h"

int main() {
    BlockChain ma_chaine_de_blocs = NULL;
    ma_chaine_de_blocs = ajouterBlock(ma_chaine_de_blocs);
    ma_chaine_de_blocs->suivant = NULL;
    viderBuffer();



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
                printf("\n* date : %s\n",pB->dateBloc);
                pT = pB->listeTransactions;
                while (pT != NULL)
                {
                    printf("  ¨¦tudiant %d, montant %.2f        (%s)\n", pT->idEtu, pT->montant, pT->description);
                    nb_Trans += 1;
                    pT = pT->suivant;
                }
                if(nb_Trans == 0) {printf("  Pas de transaction.\n");}
                nb_Trans = 0;
                pB = pB->suivant;
            }
            printf("\n**********  Fin  affichag¨¦  **********\n");
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
                    printf("\n* date : %s\n", pB->dateBloc);
                    T_Transaction* pT = pB->listeTransactions;
                    while (pT != NULL)
                    {
                        printf("  ¨¦tudiant %d, montant %.2f        (%s)\n", pT->idEtu, pT->montant, pT->description);
                        nb_Trans += 1;
                        pT = pT->suivant;
                    }
                    count_block++;
                }
                pB = pB->suivant;
            }

            if (count_block == 0) {printf("Le jour %s n'a pas ¨¦t¨¦ trouv¨¦ ", date);}
            else if (nb_Trans == 0) {printf("  Pas de transactions.");}
            break;
        }

        case '3':
        {
            T_Block* pB = ma_chaine_de_blocs;
            T_Transaction* pT = NULL;

            char date[12];
            int count_block = 0;//verifier si le block existe;
            printf("\nVous voudrez voir bloc de quel jour? (20231009 pour le 09/10/2020) ");
            scanf("%s", date);

            int idEtu;
            int count_etu = 0;
            printf("Quel est le ID d'¨¦tudiant que vous voudrez voir ? ");
            scanf("%d", &idEtu);

            while (pB != NULL)
            {
                if (strcmp(date, pB->dateBloc) == 0)
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
                    count_block++;
                }
                pB = pB->suivant;
            }

            if (count_block == 0) {printf("\nLe jour %s n'a pas ¨¦t¨¦ trouv¨¦. ", date);}
            else if (count_etu == 0) {printf("L'¨¦tudiant (ID == %d) n'a pas ¨¦t¨¦ trouv¨¦. ", idEtu);}
            break;
        }

        case '4':
        {
            viderBuffer();
            int idEtu;
            printf("\nQuel est le ID d'¨¦tudiant que vous voudrez voir ? ");
            scanf("%d", &idEtu);
            if (consulter(idEtu, ma_chaine_de_blocs) == 0)
            {
                printf("\nL'¨¦tudiant (ID == %d) n'a pas ¨¦t¨¦ trouv¨¦\n", idEtu);
            }
            break;
        }

        case '5':
        {
            int id;
            float montant;
            char descr[20];

            printf("\nEntrez ID de l'¨¦tudiant : ");
            scanf("%d", &id);
            printf("Entrez le montant a cr¨¦diter : ");
            scanf("%f", &montant);
            // Appeler la fonction crediter pour augmenter le solde de l'¨¦tudiant
            crediter(id, montant, "cr¨¦dite", ma_chaine_de_blocs);

            printf("Cr¨¦dit ajout¨¦ avec succ¨¨s.\n");
            break;
        }

        case '6'://n6. Payer un repas
        {
            int id;
            float montant;
            char descr[20];

            printf("\nEntrez ID de l'¨¦tudiant : ");
            scanf("%d", &id);
            printf("Entrez le montant ¨¤ payer (positif) :");
            scanf("%f", &montant);

            // Appeler la fonction crediter pour augmenter le solde de l'¨¦tudiant
            if (payer(id, montant, "payer", ma_chaine_de_blocs)) {
                printf("\nLe paiement a ¨¦t¨¦ effetu¨¦ avec succ¨¨s.\n");
            }
            else {
                printf("\nLe solde de l'¨¦tudiant est insuffisant !\n ");
            }
            break;
        }

        case '7':
        {
            int idSource, idDestination;
            float montant;
            char descr[20];

            printf("\nEntrez ID des ¨¦tudiants (ex:1 2): ");
            scanf("%d %d", &idSource, &idDestination);

            if (idSource == idDestination)
            {
                printf("\nVous ne pouvez pas vous transf¨¦rer de l'argent ¨¤ vous-m¨ºme\n");
            }
            else
            {
                printf("Entrez le montant ¨¤ exchanger (positif): ");
                scanf("%f", &montant);
                transfert(idSource, idDestination, montant, "exchange", ma_chaine_de_blocs);
            }

            break;
        }

        case '8':
        {
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
        viderBuffer(); // supprimer de l'entr¨¦e standard le retour charriot et les ¨¦ventuels caract¨¨res suppl¨¦mentaires tap¨¦s par l'utilisateur
    }

    return 0;
}
