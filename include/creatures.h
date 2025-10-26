#ifndef CREATURES_H
#define CREATURES_H

typedef struct {
    int id;
    char nom[30];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_min;
    int attaque_max;
    int defense;
    int vitesse;
    char effet_special[20];
    int est_vivant;
} CreatureMarine;
 
CreatureMarine cree_creature(const char *nom);
CreatureMarine *cree_creatures(int Profondeur);
void afficher_etat_creature(CreatureMarine c);

#endif