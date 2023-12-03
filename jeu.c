#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "ANSI-color-codes.h" // Dictionnaire de couleurs - credit : https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
#include <sys/ioctl.h>

typedef struct
{
    int x; // numéro de ligne
    int y; // numéro de colonne
} T_Case;

typedef struct
{
    T_Case z; // case voisine stockée
} T_Tab_Case;

#define VOISMAX 4
#define VMIN 5
#define VMAX 30
    struct winsize w;
    
// Entêtes fonctions
int Lire_Entier(char p[], int min, int max);
int VerifInt(char p[], char e[]);
int Hasard(int min, int max);
void Parametres(int *nlig, int *ncol, int *niveau, int *next, int *rules);
void init_grille(int cm[][VMAX], int nlig, int ncol);
void Voisine(T_Tab_Case vm[VOISMAX], int cm[][VMAX], T_Case c, int nlig, int ncol, int *n);
void Nimber(int cm[][VMAX], T_Case c, int nlig, int ncol, int *nim);
void print_grille(int cm[][VMAX], int nlig, int ncol, T_Case pion);
T_Case Coup_Joueur(int cm[][VMAX], int nlig, int ncol, T_Case pion);
T_Case Coup_Ordi_Hasard(int cm[][VMAX], int nlig, int ncol, T_Case pion);
T_Case Coup_Ordi_Gagnant(int cm[][VMAX], int nlig, int ncol, T_Case pion);
void print_rules(int nb_space, int nb_space2, char space[]);
//
int main()
{
    //  - Definitions des variables de base -       //
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    int nlig, ncol, niveau = 1, next = 1, loop = 1, rules = 1, start;
    int C[VMAX][VMAX];
    srand(time(0));

    //     Saisie des valeurs de bases             //
    Parametres(&nlig, &ncol, &niveau, &next, &rules);
    // Initilisation de la grille avec ses Nimbers
    init_grille(C, nlig, ncol);
    T_Case pion = {0, 0};

    if (rules == 0)
    {
    char space[]= " ";
    int nb_space=((w.ws_col+1)-strlen("Règlement :"))/2;
    int nb_space2=((w.ws_col+1)-strlen("Bonne chance !"))/2;
    print_rules(nb_space, nb_space2, space);
    }
    while (loop == 1)
    {
        // Affichage de celle-ci
        print_grille(C, nlig, ncol, pion);
        start = next;
        next = start;
        if (next == 1)
        {
            pion = Coup_Joueur(C, nlig, ncol, pion);
            next++;
        }
        else if (next == 2)
        {
            switch (niveau)
            {
            case 1:
                pion = Coup_Ordi_Hasard(C, nlig, ncol, pion);
                break;
            case 2:
                if (Hasard(1, 3) == 2)
                    pion = Coup_Ordi_Gagnant(C, nlig, ncol, pion);
                else
                    pion = Coup_Ordi_Hasard(C, nlig, ncol, pion);
                break;
            case 3:
                if (Hasard(1, 3) == 2)
                    pion = Coup_Ordi_Hasard(C, nlig, ncol, pion);
                else
                    pion = Coup_Ordi_Gagnant(C, nlig, ncol, pion);
                break;
            case 4:
                pion = Coup_Ordi_Gagnant(C, nlig, ncol, pion);
                break;
            default:
                pion = Coup_Ordi_Gagnant(C, nlig, ncol, pion);
                break;
            }
            next--;
        }

        if (pion.x == nlig - 1 && pion.y == ncol - 1)
        {
            printf("Fin de partie\n");
            printf("Souhaitez-vous relancez la partie ? (1 = Oui /0 = Non)\n");
            loop = Lire_Entier("   ---> ", 0, 1);
            pion.x = 0;
            pion.y = 0;
            if (start == 1)
            {
                start = 2;
                next = 2;
            }
            else if (next == 2)
            {
                start = 1;
                next = 1;
            }
        }
    }

    return 0;
}

//      - Fonctions -      //

int Lire_Entier(char p[], int min, int max)
{
    int n;
    n = VerifInt(p, "Votre entrée n'est pas un entier, veuillez la corrigez.\n");
    while ((n < min) || (max < n))
    {
        printf("Veuillez saisir un entier compris entre %d et %d :\n", min, max);
        n = VerifInt(p, "Votre entrée n'est pas un entier, veuillez la corrigez.\n");
    }
    return n;
}

void Parametres(int *nlig, int *ncol, int *niveau, int *next, int *rules)
{
    *nlig = Lire_Entier("Veuillez entrez un nombre de ligne\n", VMIN, VMAX);
    *ncol = Lire_Entier("Veuillez entrez un nombre de colonnes\n", VMIN, VMAX);
    *niveau = Lire_Entier("Veuillez entrez niveau :\n       Débutant (1)      Moyen (2)       Expert (3)        Virtuose (4)\n", 1, 4);
    *next = Lire_Entier("Souhaitez-vous commencez en premier (1) ou laissez l'ordinateur (2) ?\n", 1, 2);
    *rules = Lire_Entier("Souhaitez-vous lire le règlement ? :      Non (1)     Oui (0)\n", 0, 1);
}

int VerifInt(char p[], char e[])
{
    int n;
    int input;
    printf("%s", p);
    input = scanf("%d", &n);
    while (input == 0)
    {
        while (getchar() != '\n')
            ;
        printf("%s", e);
        input = scanf("%d", &n);
    }
    return n;
}

int Hasard(int min, int max)
{
    int num = (rand() % (max - min + 1)) + min;
    return num;
}

void init_grille(int cm[][VMAX], int nlig, int ncol)
{
    int i, j;
    int nim = 0;
    T_Case c = {nlig - 1, ncol - 2};
    cm[nlig - 1][ncol - 1] = 0;

    for (i = nlig - 1; i >= 0; i--)
    {
        c.x = i;
        c.y = ncol - 1;
        Nimber(cm, c, nlig, ncol, &nim);
        cm[i][ncol - 1] = nim;
    }

    for (i = nlig - 1; i >= 0; i--)
    {
        for (j = ncol - 2; j >= 0; j--)
        {
            c.x = i;
            c.y = j;
            Nimber(cm, c, nlig, ncol, &nim);
            cm[i][j] = nim;
        }
    }
}

void Nimber(int cm[][VMAX], T_Case c, int nlig, int ncol, int *nim)
{
    int n, y;
    T_Tab_Case vois[VOISMAX];
    Voisine(vois, cm, c, nlig, ncol, &n);

    for (y = 0; y < n; y++)
    {
        if (cm[vois[y].z.x][vois[y].z.y] == 0)
        {
            *nim = 1;
            break;
        }
        else
        {
            *nim = 0;
        }
    }
}

void Voisine(T_Tab_Case vm[VOISMAX], int cm[][VMAX], T_Case c, int nlig, int ncol, int *n)
{
    int nb_vois = 0, i, j;
    for (i = (c.x) + 1; i <= (c.x) + 2 && i < nlig; i++)
    {
        T_Case vois = {i, c.y};
        vm[nb_vois].z = vois;
        nb_vois++;
    }

    for (j = (c.y) + 1; j <= (c.y) + 2 && j < ncol; j++)
    {
        T_Case vois = {c.x, j};
        vm[nb_vois].z = vois;
        nb_vois++;
    }
    *n = nb_vois;
}

void print_grille(int cm[][VMAX], int nlig, int ncol, T_Case pion)
{
    system("clear");
    int i, j;
    char space[] = " ";

    // printf("Coordonnées Pion : %d %d\n", pion.x, pion.y);
        char separator[VMAX * 6] = "     +";
    for (j = 0; j < ncol; j++)
    {
        strcat(separator, "---+");
    }

    int nb_space3=((w.ws_col+1)-strlen(separator)+6)/2;
    int nb_space4=((w.ws_col)+3)/2;

    printf("%*s Grille :\n\n", nb_space4, space);
    printf("     ");
    printf("%*s", nb_space3, space);
    for (j = 1; j <= ncol; j++)
    {
        printf("%4d", j);
    }
    printf("\n");

    for (i = 0; i < nlig; i++)
    {
        printf("%*s %s", nb_space3, space, separator);
        printf("\n");
        printf("%*s %-5d|",nb_space3, space, i + 1);
        for (j = 0; j < ncol; j++)
        {
            if (pion.x == i && pion.y == j)
            {
                printf(" X |");
            }
            else if (i == nlig - 1 && j == ncol - 1)
            {
                printf(" O |");
            }
            else
            {
                printf("   |");
            }
        }

        printf("\n");
    }
    printf("%*s%s\n\n", nb_space3+1, space, separator);
    //

    // printf("Grille Nimber :\n");
    // for (i = 0; i < nlig; i++)
    // {
    //     for (j = 0; j < ncol; j++)
    //     {
    //         printf("%5d", cm[i][j]);
    //     }
    //     printf("\n");
    // }
}

void print_rules(int nb_space, int nb_space2, char space[]){
system("clear");
printf("\n%*s" BHWHT "Règlement :" reset "\n", nb_space, space);
    printf("\n");
printf("Deux joueurs s’affrontent autour d’une grille quadrillée composée de "YEL"nlig"reset" lignes et de "YEL"ncol"reset" colonnes.");
printf("À tour de rôle, chaque joueur doit déplacer le pion, soit de "CYN"une"reset" ou de "CYN"deux"reset" cases vers la droite, soit de "CYN"une"reset" ou de "CYN"deux"reset" cases vers le bas. Les déplacements autorisés sont tels que le pion termine irrémédiablement sa course en case (nlig, ncol) appelée "BLU"puits"reset" où il se retrouve bloqué.");
printf(RED " Celui qui atteint le puit est le gagnant de la partie." reset);
printf("\n%*s" BHWHT "Bonne chance !" reset "\n", nb_space2, space);

Lire_Entier("Entrez un chiffre pour passez à la suite : ", 0,9);
}

T_Case Coup_Joueur(int cm[][VMAX], int nlig, int ncol, T_Case pion)
{
    int n, y, choix;
    char arrow[13];
    T_Case nvx;
    T_Tab_Case vois[VOISMAX];
    Voisine(vois, cm, pion, nlig, ncol, &n);
    printf("Veuillez choisir un deplacement possible : \n");
    for (y = 0; y < n; y++)
    {
        if ((vois[y].z.x) - 1 == pion.x)
            strcpy(arrow, "\u2B63");
        else if ((vois[y].z.x) - 2 == pion.x)
            strcpy(arrow, "\u2B63 \u2B63");
        else if ((vois[y].z.y) - 1 == pion.y)
            strcpy(arrow, "\u2b62");
        else if ((vois[y].z.y) - 2 == pion.y)
            strcpy(arrow, "\u2b62 \u2b62");
        else
            strcpy(arrow, "O");
        printf("        %d (%d,%d) %s      ", y + 1, (vois[y].z.x) + 1, (vois[y].z.y) + 1, arrow);
    }
    printf("\n");
    choix = Lire_Entier("   ---> ", 1, n);
    nvx.x = vois[choix - 1].z.x;
    nvx.y = vois[choix - 1].z.y;
    return nvx;
}

T_Case Coup_Ordi_Hasard(int cm[][VMAX], int nlig, int ncol, T_Case pion)
{
    int n, rdm;
    T_Case nvx;
    T_Tab_Case vois[VOISMAX];
    Voisine(vois, cm, pion, nlig, ncol, &n);
    rdm = Hasard(0, n - 1);
    nvx.x = vois[rdm].z.x;
    nvx.y = vois[rdm].z.y;
    printf("L'ordinateur a joué le pion en case %d,%d !\n", (nvx.x) + 1, (nvx.y) + 1);
    sleep(1);
    return nvx;
}

T_Case Coup_Ordi_Gagnant(int cm[][VMAX], int nlig, int ncol, T_Case pion)
{
    int n, i, rdm, vf = 0;
    T_Case nvx;
    T_Tab_Case vois[VOISMAX];
    Voisine(vois, cm, pion, nlig, ncol, &n);
    for (i = 0; i < n; i++)
    {
        if ((cm[vois[i].z.x][vois[i].z.y]) == 0)
        {
            nvx.x = vois[i].z.x;
            nvx.y = vois[i].z.y;
            vf = 1;
            break;
        }
    }

    if (vf == 0)
    {
        rdm = Hasard(0, n - 1);
        nvx.x = vois[rdm].z.x;
        nvx.y = vois[rdm].z.y;
    }

    printf("L'ordinateur a joué le pion en case %d,%d !\n", (nvx.x) + 1, (nvx.y) + 1);
    sleep(1);
    return nvx;
}