#ifndef CREATURES_H
#define CREATURES_H

typedef struct {
    char nom[30];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_min;
    int attaque_max;
    int defense;
    int vitesse;
} CreatureMarine;

CreatureMarine creer_creature();
void afficher_etat_creature(CreatureMarine c);

#endif