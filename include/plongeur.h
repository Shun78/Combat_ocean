#ifndef PLONGEUR_H
#define PLONGEUR_H

typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int vitesse;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int fatigue; // 0-100%
} Plongeur;

Plongeur creer_plongeur();
void afficher_etat_plongeur(Plongeur p);

#endif