#include <stdio.h>
#include "creatures.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

//Le mot-clé static limite la portée de cette variable au fichier courant
static int prochain_id = 1;

CreatureMarine cree_creature(const char *nom) {
    CreatureMarine c;
    c.est_vivant = 1;
    c.id = prochain_id++;

    strcpy(c.nom, nom);

    // On adapte les stats selon le nom
    if (strcmp(nom, "Meduse") == 0) {
        c.points_de_vie_max = rand() % 21 + 20;
        c.attaque_min = 8;
        c.attaque_max = 15;
        c.vitesse = 6;
        strcpy(c.effet_special, "paralysie");//Réduit les attaques du joueur de 1 au prochain tour
    }
    else if (strcmp(nom, "Poisson-Epee") == 0) {
        c.points_de_vie_max = rand() % 21 + 70;
        c.attaque_min = 18;
        c.attaque_max = 28;
        c.vitesse = 3;
        strcpy(c.effet_special, "Charge"); //Ignore 2 points de défense
    }
    else if (strcmp(nom, "Requin") == 0) {
        c.points_de_vie_max = rand() % 41 + 60;
        c.attaque_min = 15;
        c.attaque_max = 25;
        c.vitesse = 5;
        strcpy(c.effet_special, "Frenesie ");//+30% dégâts si PV < 50%
    }
    else if (strcmp(nom, "Kraken") == 0) {
        c.points_de_vie_max = rand() % 61 + 120;
        c.attaque_min = 25;
        c.attaque_max = 40;
        c.vitesse = 2;
        strcpy(c.effet_special, "Etreinte");//2 attaques consécutives
    }
    else if (strcmp(nom, "CrabeGeant") == 0) {
        c.points_de_vie_max = rand() % 41 + 80;
        c.attaque_min = 12;
        c.attaque_max = 20;
        c.vitesse = 1;
        strcpy(c.effet_special, "Carapace"); //Réduit tous les dégâts subis de 20%
    }

    c.points_de_vie_actuels = c.points_de_vie_max;
    c.defense = 5;

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
    }else if (Profondeur ==3){
        // combat 1v3
        nbr_mobs = 3;
        dificulte = 0.5;

    }else if(Profondeur >= 4){
        nbr_mobs = 4;
        dificulte = 0.95;
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
