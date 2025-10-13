#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "combat.h"

int degats_infliges(int attaque_min, int attaque_max, int defense) {
    int base = attaque_min + rand() % (attaque_max - attaque_min + 1);
    int degats = base - defense;
    if (degats < 1) degats = 1;
    return degats;
}

void attaquer_creature(Plongeur *p, CreatureMarine *c, int type) {
    int degats;

    if (type == 1) { // attaque légère
        degats = degats_infliges(8, 14, c->defense);
        printf("Vous effectuez une attaque legere.\n");
    } else { // attaque lourde
        degats = degats_infliges(15, 25, c->defense);
        printf("Vous effectuez une attaque lourde.\n");
    }

    c->points_de_vie_actuels -= degats;
    if (c->points_de_vie_actuels < 0) c->points_de_vie_actuels = 0;

    printf("Vous infligez %d degats à %s !\n", degats, c->nom);
}

void attaquer_plongeur(CreatureMarine *c, Plongeur *p) {
    int degats = degats_infliges(c->attaque_min, c->attaque_max, 0);
    p->points_de_vie -= degats;
    if (p->points_de_vie < 0) p->points_de_vie = 0;

    printf("%s vous attaque et inflige %d degats !\n", c->nom, degats);
}

void tour_combat(Plongeur *p, CreatureMarine *c) {
    srand(time(NULL));
    int choix;

    printf("=== Début du combat contre %s ===\n", c->nom);

    while (p->points_de_vie > 0 && c->points_de_vie_actuels > 0) {
        printf("\n--- etat actuel ---\n");
        printf("Plongeur : %d/%d PV (Vitesse : %d)\n", p->points_de_vie, p->points_de_vie_max, p->vitesse);
        printf("%s : %d/%d PV (Vitesse : %d)\n", c->nom, c->points_de_vie_actuels, c->points_de_vie_max, c->vitesse);

        printf("\n1 - Attaque legere\n2 - Attaque lourde\n> ");
        scanf("%d", &choix);
        if (choix != 1 && choix != 2) {
            printf("Choix invalide, vous perdez votre tour !\n");
            choix = 0;
        }

        //Détermination de qui attaque en premier
        int joueur_premier = 0;
        if (p->vitesse >= c->vitesse)
            joueur_premier = 1; // le plongeur attaque d’abord en cas d’égalité
        else
            joueur_premier = 0;

        // Phase d’attaque
        if (joueur_premier) {
            printf("\nVous attaquez en premier !\n");
            Sleep(1000);
            if (choix) attaquer_creature(p, c, choix);

            if (c->points_de_vie_actuels > 0) {
            Sleep(1000);
                printf("\n%s contre-attaque !\n", c->nom);
                Sleep(1000);
            attaquer_plongeur(c, p);
            }
        } else {
            printf("\n%s attaque en premier !\n", c->nom);
            Sleep(1000);
            attaquer_plongeur(c, p);
            if (p->points_de_vie > 0 && choix) {
                Sleep(1000);
                printf("\n Vous contre-attaquez !\n");
                Sleep(1000);
                attaquer_creature(p, c, choix);
            }
        }

        Sleep(1000);  //pause entre les tours
    }

    // -----------------
    // Fin du combat
    // -----------------

    if (p->points_de_vie <= 0)
        printf("\nVous avez ete vaincu...\n");
    else
        printf("\nVous avez demoli %s !\n", c->nom);
}
