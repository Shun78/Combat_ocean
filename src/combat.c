#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>  //pour les sleeps pour les temps de pauses entre plusieurs actions

#include "combat.h"

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

    if (type == 1) { // attaque légère
        degats = degats_infliges(8, 14, c->defense);
        printf("Vous effectuez une attaque legere.\n");
        p->fatigue += 10; //fatigue +10%
    } else { // attaque lourde
        degats = degats_infliges(15, 25, c->defense);
        printf("Vous effectuez une attaque lourde.\n");
        p->fatigue += 20;  //fatigue +20%
    }

    if (p->fatigue > 100) {
        p->fatigue = 100;
    }

    c->points_de_vie_actuels -= degats;
    if (c->points_de_vie_actuels < 0) {
        c->points_de_vie_actuels = 0;
    }
    printf("Vous infligez %d degats à %s !\n", degats, c->nom);
}

//------------------------------------------------------------
// Fonction pour s'économiser pour recuperer de la fatigue
//-------------------------------------------------------------
void seconomiser(Plongeur *p) {
    printf("\nVous decidez de vous economiser ce tour\n");
    p->fatigue -= 30;  //enlève -30% de fatigue
    if (p->fatigue < 0) {
        p->fatigue = 0;
    }
    Sleep(1000);
    printf(" Votre fatigue diminue a %d%%\n", p->fatigue);
}

//-----------------------------
// Fonction de l'attaque de la créature sur joueur
//-------------------------------
void attaquer_plongeur(CreatureMarine *c, Plongeur *p) {
    int degats = degats_infliges(c->attaque_min, c->attaque_max, 0);
    p->points_de_vie -= degats;
    if (p->points_de_vie < 0) {
        p->points_de_vie = 0;
    }

    printf("%s vous attaque et inflige %d degats !\n", c->nom, degats);
}

//-----------------
//Phases du combat
//-----------------
void tour_combat(Plongeur *p, CreatureMarine *c) {
    srand(time(NULL));
    int choix;

    printf("=== Debut du combat contre %s ===\n", c->nom);
    p->fatigue=0;  // notre fatigue initiale

    while (p->points_de_vie > 0 && c->points_de_vie_actuels > 0) {
        printf("\n--- etat actuel ---\n");
        printf("Plongeur : %d/%d PV (Vitesse : %d, Fatigue: %d%%)\n", p->points_de_vie, p->points_de_vie_max, p->vitesse, p->fatigue);
        printf("%s : %d/%d PV (Vitesse : %d)\n", c->nom, c->points_de_vie_actuels, c->points_de_vie_max, c->vitesse);


    //Choix des actions
        if (p->fatigue >= 90) {
            printf("\nVous etes trop creve pour attaquer \n");
            printf("1 - S'économiser (-30%% de fatigue)\n> ");
            scanf("%d", &choix);
            if (choix == 1)
                seconomiser(p);
            else
                printf("Vous hesiter et perdez votre tour\n");
                choix=0;
        }
        else {
            printf("\n1 - Attaque legere (+10%% fatigue)\n");
            printf("2 - Attaque lourde (+20%% fatigue)\n");
            printf("3 - S'economiser (-30%% fatigue)\n");
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
