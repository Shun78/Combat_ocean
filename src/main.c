#include "plongeur.h"
#include "creatures.h"
#include "combat.h"
#include <stdio.h>

int main() {
    Plongeur p = creer_plongeur();
    CreatureMarine c = creer_creature();

    printf("=== COMBAT ===\n");
    tour_combat(&p, &c);

    printf("\nFin du combat.\n");
    return 0;
}
