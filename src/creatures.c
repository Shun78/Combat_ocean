#include <stdio.h>
#include "creatures.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

CreatureMarine cree_creature(const char *nom) {
    CreatureMarine c;
    c.est_vivant = 1;
    c.id = 0;

    strcpy(c.nom, nom);

    // On adapte les stats selon le nom
    if (strcmp(nom, "Meduse") == 0) {
        c.points_de_vie_max = rand() % 21 + 20;
        c.attaque_min = 8;
        c.attaque_max = 15;
        c.vitesse = 4;
    }
    else if (strcmp(nom, "Poisson-Epee") == 0) {
        c.points_de_vie_max = rand() % 21 + 70;
        c.attaque_min = 18;
        c.attaque_max = 28;
        c.vitesse = 3;
    }
    else if (strcmp(nom, "Requin") == 0) {
        c.points_de_vie_max = rand() % 41 + 60;
        c.attaque_min = 15;
        c.attaque_max = 25;
        c.vitesse = 5;
    }
    else if (strcmp(nom, "Kraken") == 0) {
        c.points_de_vie_max = rand() % 61 + 120;
        c.attaque_min = 25;
        c.attaque_max = 30;
        c.vitesse = 2;
    }
    else if (strcmp(nom, "CrabeGeant") == 0) {
        c.points_de_vie_max = rand() % 41 + 80;
        c.attaque_min = 12;
        c.attaque_max = 20;
        c.vitesse = 1;
    }

    c.points_de_vie_actuels = c.points_de_vie_max;
    c.defense = 5;
    strcpy(c.effet_special, "Aucun");

    return c;
}

CreatureMarine *cree_creatures(int Profondeur){

    const char *listes_creatures[] = {"Meduse", "Poisson-Epee", "Requin", "Kraken", "CrabeGeant"};
    int nbr_mobs;
    double dificulte;
    char nom[30];
    
    if (Profondeur == 1){
        // combat 1v1
        nbr_mobs = 1;
        dificulte = 0.2;
    
    }else if(Profondeur == 2){
        // combat 1v2
        nbr_mobs = 2;
        dificulte = 0.4;
    }else if (Profondeur >=3){
        // combat 1v3
        nbr_mobs = 3;
        dificulte = 0.9;

    }
    
    CreatureMarine *creatures = malloc(sizeof(CreatureMarine) * (nbr_mobs));

    for (int i = 0; i < nbr_mobs; i++) {
        double r = (double)rand() / RAND_MAX;
        int random_number;

        if (r > dificulte) {
            // mob facile
            random_number = rand() % 2; // 0 or 1
            strcpy(nom, listes_creatures[random_number]);
        } else {
            // mob difficile
            random_number = 2 + rand() % 3; // 2, 3, 4
            strcpy(nom, listes_creatures[random_number]);
        }

        creatures[i] = cree_creature(nom);
    }
    return creatures;
}

void afficher_etat_creature(CreatureMarine c) {
    printf("%s : %d/%d PV\n", c.nom, c.points_de_vie_actuels, c.points_de_vie_max);
}
