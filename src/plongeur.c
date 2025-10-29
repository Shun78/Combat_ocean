#include <stdio.h>
#include "plongeur.h"
#include <stdlib.h>
#include <string.h>

Plongeur creer_plongeur() {
    Plongeur p;
    p.points_de_vie_max = 100;
    p.points_de_vie = 100;
    p.vitesse = 5;
    p.niveau_oxygene_max = 100;
    p.niveau_oxygene = 100;
    p.fatigue = 0;
    strcpy(p.armure, "aucun");
    strcpy(p.arm, "gold");
    return p;
}

void afficher_etat_plongeur(Plongeur p) {
    printf("Plongeur : %d/%d PV | O2 : %d/%d | Fatigue : %d\n",
           p.points_de_vie, p.points_de_vie_max,
           p.niveau_oxygene, p.niveau_oxygene_max,
           p.fatigue);
}
