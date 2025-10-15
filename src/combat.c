#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>  //pour les sleeps pour les temps de pauses entre plusieurs actions

#include "combat.h"  // le fichier combat.h

#define FATIGUE_MAX 100
#define COUT_ATT_LEGERE 10
#define COUT_ATT_LOURDE 20
#define FATIGUE_REPOS 30
#define LIMITE_FATIGUE 90

//-------------------------------------------
//Fonction pour les calculs des degats
//----------------------------------------
int degats_infliges(int attaque_min, int attaque_max, int defense) {
    int base = attaque_min + rand() % (attaque_max - attaque_min + 1);
    int degats = base - defense;
    if (degats < 1) {
        degats = 1;
    }
    return degats;
}

//---------------------------------------------------
//Fonction pour l'attaque du joueur sur creature
//---------------------------------------------------
void attaquer_creature(Plongeur *p, CreatureMarine *c, int type) {
    int degats;
    int cout_fatigue;

    if (type==1){
        cout_fatigue= COUT_ATT_LEGERE;
    }
    else{
        cout_fatigue= COUT_ATT_LOURDE;
    }

    if (type == 1) { // attaque légère
        degats = degats_infliges(8, 14, c->defense);
        printf("Vous effectuez une attaque legere.\n");
    } else { // attaque lourde
        degats = degats_infliges(15, 25, c->defense);
        printf("Vous effectuez une attaque lourde.\n");
    }

    p->fatigue +=cout_fatigue; //ajout à chaque tour de la fatigue en fonction du type d'attaque (+10% si attaque legere ou +20% si attaque lourde)

    if (p->fatigue > FATIGUE_MAX) {
        p->fatigue = FATIGUE_MAX;
    }

    c->points_de_vie_actuels -= degats;
    if (c->points_de_vie_actuels < 0) {
        c->points_de_vie_actuels = 0;
    }
    printf("Vous infligez %d degats a %s !\n", degats, c->nom);
}

//------------------------------------------------------------
// Fonction pour s'économiser pour recuperer de la fatigue
//-------------------------------------------------------------
void seconomiser(Plongeur *p) {
    printf("\nVous decidez de vous economiser ce tour\n");
    p->fatigue -= FATIGUE_REPOS;  //enlève -30% de fatigue
    if (p->fatigue < 0) {
        p->fatigue = 0;
    }
    Sleep(1000);
    printf(" Votre fatigue diminue a %d%%\n", p->fatigue);
}

//----------------------------------------------------
// Fonction de l'attaque de la créature sur joueur
//----------------------------------------------------
void attaquer_plongeur(CreatureMarine *c, Plongeur *p) {
    int degats = degats_infliges(c->attaque_min, c->attaque_max, 0);
    p->points_de_vie -= degats;
    if (p->points_de_vie < 0) {
        p->points_de_vie = 0;
    }

    printf("%s vous attaque et inflige %d degats !\n", c->nom, degats);
}

//-------------------
//Phases du combat
//-------------------
void tour_combat(Plongeur *p, CreatureMarine *c) {
    srand(time(NULL));
    int choix;

    printf("=== Debut du combat contre %s ===\n", c->nom);
    p->fatigue=0;  // notre fatigue initiale

    while (p->points_de_vie > 0 && c->points_de_vie_actuels > 0) {
        printf("\n------------------- ETAT ACTUEL -------------------\n");
        printf("Plongeur : %d/%d PV (Vitesse : %d, Fatigue: %d%%)\n", p->points_de_vie, p->points_de_vie_max, p->vitesse, p->fatigue);
        printf("%s : %d/%d PV (Vitesse : %d)", c->nom, c->points_de_vie_actuels, c->points_de_vie_max, c->vitesse);
        printf("\n---------------------------------------------------\n");

    //Choix des actions
        if (p->fatigue + COUT_ATT_LEGERE >= FATIGUE_MAX && p->fatigue + COUT_ATT_LOURDE >= FATIGUE_MAX) {
            printf("\nVous etes trop creve pour attaquer \n");
            printf("1 - S'economiser (-%d%% de fatigue)\n> ", FATIGUE_REPOS);
            scanf("%d", &choix);
            if (choix == 1)
                seconomiser(p);
            else
                printf("Vous hesiter et perdez votre tour\n");
                choix=0;
        }
        else {
            printf("\n1 - Attaque legere (+%d%% fatigue)\n", COUT_ATT_LEGERE);
            printf("2 - Attaque lourde (+%d%% fatigue)\n", COUT_ATT_LOURDE);
            printf("3 - S'economiser (-%d%% fatigue)\n", FATIGUE_REPOS);
            printf("Quel est votre choix ?");
            scanf("%d", &choix);

            if (choix < 1 || choix > 3) {
                printf("Vous hesiter et perdez votre tour !\n");
                choix = 0;
            }
        }

        //Détermination de qui attaque en premier
        int joueur_premier = 0;
        if (p->vitesse >= c->vitesse) {
            joueur_premier = 1; // le plongeur attaque d’abord en cas d’égalité
        }
        else {
            joueur_premier = 0;
        }

        // Phase d’attaque qui dépend du choix et vitesse
        if (joueur_premier) {
            printf("\nVous etes le plus rapide, vous prenez l'initiative !\n");
            Sleep(1000);
            if (choix == 3) {
                seconomiser(p);
            }
           else if (choix == 1 || choix ==2) {
                attaquer_creature(p, c, choix);
            }

           // La creature riposte si elle est encore en vie
            if (c->points_de_vie_actuels > 0) {
            Sleep(1000);
                printf("\n%s contre-attaque !\n", c->nom);
                Sleep(1000);
            attaquer_plongeur(c, p);
            }
        } else {  //si le monstre est plus rapide
            printf("\n%s attaque en premier !\n", c->nom);
            Sleep(1000);
            attaquer_plongeur(c, p);

            // Contre-attaque du plongeur s'il est encore en vie
            if (p->points_de_vie > 0) {
                Sleep(1000);
                if (choix == 3){
                    printf("\n Vous vous reposez !\n");
                    Sleep(1000);
                    seconomiser(p);
                }
                else if (choix == 1 || choix == 2) {
                    Sleep(1000);
                    printf("\n Vous contre-attaquez !\n");
                    Sleep(1000);
                    attaquer_creature(p, c, choix);
                }
            }
        }

        Sleep(1000);  //pause entre les tours
    }

    // Fin du combat
    if (p->points_de_vie <= 0)
        printf("\nVous avez ete vaincu...\n");
    else
        printf("\nVous avez demoli %s !\n", c->nom);
}
