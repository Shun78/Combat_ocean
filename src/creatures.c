#include <stdio.h>
#include "creatures.h"

CreatureMarine creer_creature() {
    CreatureMarine c;
    c.points_de_vie_max = 80;
    c.points_de_vie_actuels = 80;
    c.attaque_min = 10;
    c.attaque_max = 20;
    c.defense = 5;
    c.vitesse = 6;
    return c;
}

void afficher_etat_creature(CreatureMarine c) {
    printf("%s : %d/%d PV\n", c.nom, c.points_de_vie_actuels, c.points_de_vie_max);
}
