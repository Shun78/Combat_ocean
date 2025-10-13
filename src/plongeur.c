#include <stdio.h>
#include "plongeur.h"

Plongeur creer_plongeur() {
    Plongeur p;
    p.points_de_vie_max = 100;
    p.points_de_vie = 100;
    p.vitesse = 5;
    p.niveau_oxygene_max = 100;
    p.niveau_oxygene = 100;
    p.niveau_fatigue = 0;
    return p;
}

void afficher_etat_plongeur(Plongeur p) {
    printf("Plongeur : %d/%d PV | O2 : %d/%d | Fatigue : %d\n",
           p.points_de_vie, p.points_de_vie_max,
           p.niveau_oxygene, p.niveau_oxygene_max,
           p.niveau_fatigue);
}
