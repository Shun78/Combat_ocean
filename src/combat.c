#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "armure.h"
#include "armure.c"
#include <windows.h> //pour les sleeps pour les temps de pauses entre plusieurs actions

#include "combat.h" // le fichier combat.h

#define FATIGUE_MAX 100
#define COUT_ATT_LEGERE 10
#define COUT_ATT_LOURDE 20
#define COUT_ATT_COMPETENCE 30
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
int degats_infliges(int attaque_min, int attaque_max, int defense, char effet_mob[])
{

    int chance_effet = rand() % 100; // 0 a 99
    int effet_active = 0;

    if (chance_effet < 50)
    {
        effet_active = 1;
    }

    int base = attaque_min + rand() % (attaque_max - attaque_min + 1);

    if (strcmp(effet_mob, "Carapace") == 0 && effet_active == 1)
    { // Réduit tous les dégâts subis de 20%
        base = base - (int)(base * .2);
        printf("EFFET SPECIALE UTILISER: CARAPACE\n");
        printf("Reduit tous les degats subis de 20 \n");
    }
    else if (strcmp(effet_mob, "Charge") == 0 && effet_active == 1)
    {
        printf("EFFET SPECIALE UTILISER: Charge perforante\n");
        printf("Ignore 2 points de defense\n");
        defense = defense - 2;
        if (defense < 0)
            defense = 0;
    }

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
void attaquer_creature(Plongeur *p, CreatureMarine *c, int type, char arm[])
{
    // recuperer arme
    printf("DEBUG: arm reçue dans attaquer_creature = '%s'\n", arm);

    // type = 4 (attaque competence)

    int degats;
    int cout_fatigue;

    if (type == 1)
    {
        cout_fatigue = COUT_ATT_LEGERE;
    }
    else if (type == 2)
    {
        cout_fatigue = COUT_ATT_LOURDE;
    }
    else if (type == 4)
    {
        cout_fatigue = COUT_ATT_COMPETENCE;
    }

    if (type == 1)
    {                                                                   // attaque légère
        degats = degats_infliges(20, 30, c->defense, c->effet_special); // 8, 14
        printf("Vous effectuez une attaque legere.\n");
    }
    else if (type == 2)
    {                                                                   // attaque lourde
        degats = degats_infliges(30, 50, c->defense, c->effet_special); // a ala base 15, 25
        printf("Vous effectuez une attaque lourde.\n");
    }
    else
    {
        degats = degats_infliges(40, 60, c->defense, c->effet_special);
        printf("Vous effectuez une attaque competence.\n");
    }

    //===========================================
    if (strcmp(arm, "gold") == 0)
    {
        printf("degats avant: %d\n", degats);
        printf("Plongeur est equiper d'une armure gold\n");
        degats += 10;
        printf("degats apres: %d\n", degats);
    }
    else if (strcmp(arm, "argent") == 0)
    {
        degats += 5;
    }
    else if (strcmp(arm, "bronze") == 0)
    {
        degats += 2;
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

//-----------------------------------------------------
// Fonction pour calculer la consommation de l'O2
//-----------------------------------------------------
void consommation_o2(Plongeur *p, int type_attack, int profondeur)
{
    int consommation = 0;

    // Base selon type d'attaque
    if (type_attack == 1 || type_attack == 2)
    {
        // Attaque normale : -2 à -4 selon profondeur
        consommation = 2 + (rand() % 3); // 2, 3 ou 4
    }
    else if (type_attack == 4)
    {
        // Compétence spéciale : -5 à -8
        consommation = 5 + (rand() % 4); // 5, 6, 7, 8
    }

    // Plus on descend, plus on consomme d’oxygène
    if (profondeur == 2)
    {
        consommation += 1;
    }
    else if (profondeur == 3)
    {
        consommation += 2;
    }
    else if (profondeur >= 4)
    {
        consommation += 3;
    }

    p->niveau_oxygene -= consommation;
    if (p->niveau_oxygene < 0)
        p->niveau_oxygene = 0;
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
void attaquer_plongeur(CreatureMarine *c, Plongeur *p, char armure[])
{
    // {"paralysie", "Charge", "Frenesie", "Etreinte", "Carapace"};
    int degats;
    int chance_effet = rand() % 100; // 0 a 99
    int effet_active = 0;

    if (chance_effet < 50)
    {
        effet_active = 1;
    }

    // Requin : "Frénésie sanguinaire" → +30% dégâts si PV < 50%
    if (strcmp(c->effet_special, "Frenesie") == 0 && effet_active == 1)
    {
        if (c->points_de_vie_actuels < (c->points_de_vie_max / 2))
        {
            printf("EFFET SPECIALE UTILISER: FRENESIE (+30%% degats)\n");
            degats = degats_infliges(c->attaque_min, c->attaque_max, 0, c->effet_special);
            degats = degats + (int)(degats * .3);
        }
        else
        {
            degats = degats_infliges(c->attaque_min, c->attaque_max, 0, c->effet_special);
        }
    }
    else
    {
        degats = degats_infliges(c->attaque_min, c->attaque_max, 0, c->effet_special);
        if (strcmp(armure, "gold") == 0)
        {
            printf("degats avant: %d\n", degats);
            printf("Plongeur est equiper d'une armure gold\n");
            degats -= 10;
            printf("degats apres: %d\n", degats);
        }
        else if (strcmp(armure, "argent") == 0)
        {
            degats -= 5;
        }
        else if (strcmp(armure, "bronze") == 0)
        {
            degats -= 2;
        }
    }

    p->points_de_vie -= degats;
    if (p->points_de_vie < 0)
    {
        p->points_de_vie = 0;
    }

    // attaque subie fait perdre 1-2 oxygene (stress)
    int stress = rand() % 2 + 1;
    p->niveau_oxygene -= stress;
    printf("STRESS: -%d O2\n", stress);

    printf("%s vous attaque et inflige %d degats !\n", c->nom, degats);
}

void creatures_restants(CreatureMarine *creatures, int nbr_mobs)
{
    printf("CREATURES RESTANTS :\n");
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
}

void afficher_stats(Plongeur *p)
{
    printf("\n==============================================================\n");
    printf("VOS STATS : PV = %d | FATIGUE = %d%% | VITESSE = %d | O2 = %d\n", p->points_de_vie, p->fatigue, p->vitesse, p->niveau_oxygene);
    printf("================================================================\n");
    if (p->niveau_oxygene <= 10)
    {
        printf("ALERTE: O2 < 10\n");
        printf("@@@@@@@@@@@@@@@@@@@\n");
    }
}
//-------------------
// Phases du combat
//-------------------
void tour_combat(Plongeur *p, CreatureMarine *creatures, int nbr_mobs, int profondeur)
{

    int choix;
    int choix_mob;
    int index_mob = 0;
    int toutes_mortes = 0;

    while (p->points_de_vie > 0 && !toutes_mortes)
    {
        // afficher les stats du plongeur
        afficher_stats(p);
        // afficher les creatures restants
        creatures_restants(creatures, nbr_mobs);

        // choix d'actions
        if (p->fatigue + COUT_ATT_LEGERE >= FATIGUE_MAX && p->fatigue + COUT_ATT_LOURDE >= FATIGUE_MAX)
        {
            printf("\nVous etes trop creve pour attaquer !\n");
            printf("1 - S'economiser (-%d%% de fatigue)\n> ", FATIGUE_REPOS);
            scanf("%d", &choix);
            if (choix == 1)
            {
                seconomiser(p);
            }
            else
            {
                printf("Vous hesitez et perdez votre tour.\n");
            }
        }
        else
        {
            printf("\n1 - Attaque legere (+%d%% fatigue)\n", COUT_ATT_LEGERE);
            printf("2 - Attaque lourde (+%d%% fatigue)\n", COUT_ATT_LOURDE);
            printf("3 - S'economiser (-%d%% fatigue)\n", FATIGUE_REPOS);
            printf("4- Competence speciale (+%d%% fatigue)\n", COUT_ATT_COMPETENCE);
            printf("Quel est votre choix ? ");
            scanf("%d", &choix);

            if (choix < 1 || choix > 4)
            {
                printf("Choix invalide, vous perdez votre tour !\n");
                choix = 0;
            }
            if (choix == 1 || choix == 2 || choix == 4)
            {
                // afficher les mobs
                printf("\nChoisissez la creature a attaquer :\n");
                creatures_restants(creatures, nbr_mobs);
                printf("> ");
                scanf("%d", &choix_mob);
                while (choix_mob < 1 || choix_mob > nbr_mobs || !creatures[choix_mob - 1].est_vivant)
                {
                    printf("Choix invalide, entrez a nouveau : ");
                    scanf("%d", &choix_mob);
                }

                // Recuperer l'effet de la creature a attaquer
                char effet_mob_att[20];
                strcpy(effet_mob_att, creatures[choix_mob - 1].effet_special);

                // attaquer la creature
                attaquer_creature(p, &creatures[choix_mob - 1], choix, p->arm);

                if (creatures[choix_mob - 1].points_de_vie_actuels <= 0)
                {
                    creatures[choix_mob - 1].est_vivant = 0;
                    printf("\n%s est vaincu !\n", creatures[choix_mob - 1].nom);
                }
                // consommer O2
                consommation_o2(p, choix, profondeur);
            }
            else if (choix == 3)
            {
                seconomiser(p);
            }

            Sleep(1000);
        }

        // Phase: attack des creatures

        int chance_effet = rand() % 100; // 0 a 99
        int effet_active = 0;

        if (chance_effet < 50)
        {
            effet_active = 1;
        }
        while (creatures[index_mob].est_vivant == 0)
        {
            index_mob++;
            if (index_mob > nbr_mobs - 1)
            {
                index_mob = 0;
            }
        }
        if (creatures[index_mob].est_vivant)
        {
            // recuperer l'effet speciale de la creature en defense
            //{"paralysie", "Charge", "Frenesie", "Etreinte", "Carapace"};
            char effet_mob_def[20];
            strcpy(effet_mob_def, creatures[index_mob].effet_special);

            printf("\n%s attaque!\n", creatures[index_mob].nom);
            Sleep(1000);

            // si mob = Kraken => 2 attaques consecutives
            if (strcmp(effet_mob_def, "Etreinte") == 0 && effet_active == 1)
            {
                printf("EFFET SPECIALE UTILISER: 2 attaques consecutives: \n");
                for (int j = 0; j < 2; j++)
                {
                    printf("Attaque numero: %d \n", j + 1);
                    attaquer_plongeur(&creatures[index_mob], p, p->armure); // armure
                    Sleep(3000);
                }
                // si meduse en reduit le nombre d'attaque de prochaine tour
            }
            else if (strcmp(effet_mob_def, "paralysie") == 0 && effet_active == 1)
            {
                printf("EFFET SPECIALE : PARALYSIE\n");
                printf("Fatigue Reduit de 10\n");
                p->fatigue -= 10;
                attaquer_plongeur(&creatures[index_mob], p, p->armure);
                Sleep(3000);
            }
            else
            {
                attaquer_plongeur(&creatures[index_mob], p, p->armure);
            }
            index_mob++;
            if (index_mob > nbr_mobs - 1)
            {
                index_mob = 0;
            }
        }

        if (p->points_de_vie <= 0)
        {
            printf("\nVous avez ete vaincu...\n");
            break;
        }

        Sleep(1000);

        // Verifier si toutes les mobs sont morts
        toutes_mortes = 1;
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
    }
}
