#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> //pour les sleeps pour les temps de pauses entre plusieurs actions

#include "combat.h" // le fichier combat.h

#define FATIGUE_MAX 100
#define COUT_ATT_LEGERE 10
#define COUT_ATT_LOURDE 20
#define FATIGUE_REPOS 30
#define LIMITE_FATIGUE 90

// le joueur attaque en premier
// puis les montres attaque en fonction de leurs vitesse

// pour la selection du monstre a attaquer :

// d'abord le joueur selectionne une attaque, puis il choisi le monstre a attaquer
//(et si il selectionne une attaque passive, pas besoin de selection un ennemie a attaquer)

//-------------------------------------------
// Fonction pour les calculs des degats
//----------------------------------------
int degats_infliges(int attaque_min, int attaque_max, int defense)
{
    int base = attaque_min + rand() % (attaque_max - attaque_min + 1);
    int degats = base - defense;
    if (degats < 1)
    {
        degats = 1;
    }
    return degats;
}

//---------------------------------------------------
// Fonction pour l'attaque du joueur sur creature
//---------------------------------------------------
void attaquer_creature(Plongeur *p, CreatureMarine *c, int type)
{
    int degats;
    int cout_fatigue;

    if (type == 1)
    {
        cout_fatigue = COUT_ATT_LEGERE;
    }
    else
    {
        cout_fatigue = COUT_ATT_LOURDE;
    }

    if (type == 1)
    { // attaque légère
        degats = degats_infliges(8, 14, c->defense);
        printf("Vous effectuez une attaque legere.\n");
    }
    else
    { // attaque lourde
        degats = degats_infliges(15, 25, c->defense);
        printf("Vous effectuez une attaque lourde.\n");
    }

    p->fatigue += cout_fatigue; // ajout à chaque tour de la fatigue en fonction du type d'attaque (+10% si attaque legere ou +20% si attaque lourde)

    if (p->fatigue > FATIGUE_MAX)
    {
        p->fatigue = FATIGUE_MAX;
    }

    c->points_de_vie_actuels -= degats;
    if (c->points_de_vie_actuels < 0)
    {
        c->points_de_vie_actuels = 0;
    }
    printf("Vous infligez %d degats a %s !\n", degats, c->nom);
}

//------------------------------------------------------------
// Fonction pour s'économiser pour recuperer de la fatigue
//-------------------------------------------------------------
void seconomiser(Plongeur *p)
{
    printf("\nVous decidez de vous economiser ce tour\n");
    p->fatigue -= FATIGUE_REPOS; // enlève -30% de fatigue
    if (p->fatigue < 0)
    {
        p->fatigue = 0;
    }
    Sleep(1000);
    printf(" Votre fatigue diminue a %d%%\n", p->fatigue);
}

//----------------------------------------------------
// Fonction de l'attaque de la créature sur joueur
//----------------------------------------------------
void attaquer_plongeur(CreatureMarine *c, Plongeur *p)
{
    int degats = degats_infliges(c->attaque_min, c->attaque_max, 0);
    p->points_de_vie -= degats;
    if (p->points_de_vie < 0)
    {
        p->points_de_vie = 0;
    }

    printf("%s vous attaque et inflige %d degats !\n", c->nom, degats);
}

//-------------------
// Phases du combat
//-------------------
void tour_combat(Plongeur *p, CreatureMarine *creatures, int nbr_mobs)
{
    srand(time(NULL));
    int choix;
    int choix_mob;

    while (p->points_de_vie > 0)
    {
        printf("\n=========================================\n");
        printf("VOS STATS : PV = %d | FATIGUE = %d%% | VITESSE = %d\n", p->points_de_vie, p->fatigue, p->vitesse);
        printf("=========================================\n");
        printf("CREATURES ENNEMIES :\n");
        for (int i = 0; i < nbr_mobs; i++)
        {
            if (creatures[i].est_vivant)
            {
                printf("%d - %s (VITESSE: %d) (PV: %d)\n",
                       i + 1,
                       creatures[i].nom,
                       creatures[i].vitesse,
                       creatures[i].points_de_vie_actuels);
            }
        }
        printf("=========================================\n");

        // choix d'actions
        if (p->fatigue + COUT_ATT_LEGERE >= FATIGUE_MAX && p->fatigue + COUT_ATT_LOURDE >= FATIGUE_MAX)
        {
            printf("\nVous etes trop creve pour attaquer !\n");
            printf("1 - S'economiser (-%d%% de fatigue)\n> ", FATIGUE_REPOS);
            scanf("%d", &choix);
            if (choix == 1)
                seconomiser(p);
            else
                printf("Vous hesitez et perdez votre tour.\n");
        }
        else
        {
            printf("\n1 - Attaque legere (+%d%% fatigue)\n", COUT_ATT_LEGERE);
            printf("2 - Attaque lourde (+%d%% fatigue)\n", COUT_ATT_LOURDE);
            printf("3 - S'economiser (-%d%% fatigue)\n", FATIGUE_REPOS);
            printf("Quel est votre choix ? ");
            scanf("%d", &choix);

            if (choix < 1 || choix > 3)
            {
                printf("Choix invalide, vous perdez votre tour !\n");
                choix = 0;
            }
        }

        if (choix == 1 || choix == 2)
        {
            printf("\nChoisissez la creature a attaquer :\n");
            for (int i = 0; i < nbr_mobs; i++)
            {
                if (creatures[i].est_vivant)
                {
                    printf("%d - %s (VITESSE: %d) (PV: %d)\n",
                           i + 1,
                           creatures[i].nom,
                           creatures[i].vitesse,
                           creatures[i].points_de_vie_actuels);
                }
            }
            printf("> ");
            scanf("%d", &choix_mob);
            while (choix_mob < 1 || choix_mob > nbr_mobs || !creatures[choix_mob - 1].est_vivant)
            {
                printf("Choix invalide, entrez a nouveau : ");
                scanf("%d", &choix_mob);
            }

            attaquer_creature(p, &creatures[choix_mob - 1], choix);
            if (creatures[choix_mob - 1].points_de_vie_actuels <= 0)
            {
                creatures[choix_mob - 1].est_vivant = 0;
                printf("\n%s est vaincu !\n", creatures[choix_mob - 1].nom);
            }
        }
        else if (choix == 3)
        {
            seconomiser(p);
        }

        Sleep(1000);

        // attack des creatures
        for (int i = 0; i < nbr_mobs; i++)
        {
            if (creatures[i].est_vivant)
            {
                printf("\n%s attaque en premier !\n", creatures[i].nom);
                Sleep(1000);
                attaquer_plongeur(&creatures[i], p);
                break; 
            }
        }

        if (p->points_de_vie <= 0)
        {
            printf("\nVous avez ete vaincu...\n");
            break;
        }

        // Check if all creatures are dead
        int toutes_mortes = 1;
        for (int i = 0; i < nbr_mobs; i++)
        {
            if (creatures[i].est_vivant)
            {
                toutes_mortes = 0;
                break;
            }
        }
        if (toutes_mortes)
        {
            printf("\nToutes les creatures ont ete vaincues !\n");
            break;
        }

        Sleep(1000); 
    }
}

