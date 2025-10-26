#include "../include/plongeur.h"
#include "../include/creatures.h"
#include "../include/combat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int est_nom_valide(const char *nom);
CreatureMarine *trier_creatures(CreatureMarine *creatures, int nbr_mobs);

int main()
{   
    srand(time(NULL));
    int correct = 0;
    Plongeur p = creer_plongeur();
    int profondeur = 0;

    while (!correct)
    {
        printf("Entrez la Profondeur (1, 2, 3, ..):  ");
        scanf("%d", &profondeur);

        if (profondeur == 0 || profondeur > 3)
        {
            printf("Profondeur invalide !\n");
        }else{
            correct = 1;
        }
        
    }

    int nbr_mobs;
    CreatureMarine *creatures = cree_creatures(profondeur);
     if (profondeur == 1) {
        nbr_mobs = 1;
    }
    else if (profondeur == 2) {
        nbr_mobs = 2;
    }
    else {
        nbr_mobs = 3;
    }
    printf("=== Creatures generees pour Profondeur %d ===\n", profondeur);
    for (int i = 0; i < nbr_mobs; i++) {
        printf("%d - %s (PV: %d, ATK: %d-%d, DEF: %d, VITESSE: %d, EFFET_SPECIAL: %s)\n",
               i + 1,
               creatures[i].nom,
               creatures[i].points_de_vie_max,
               creatures[i].attaque_min,
               creatures[i].attaque_max,
               creatures[i].defense,
               creatures[i].vitesse,
               creatures[i].effet_special);
    }
    printf("\n");
    printf("#################### COMBAT ####################\n");

    CreatureMarine *creatures_triees = trier_creatures(creatures, nbr_mobs);
    tour_combat(&p, creatures_triees, nbr_mobs);

    printf("\nFin du combat.\n");

    free(creatures); // liberer la memoire
    free(creatures_triees);
    return 0;
}

int est_nom_valide(const char *nom)
{
    const char *noms_valides[] = {
        "Meduse", "Poisson-Epee", "Requin", "Kraken", "CrabeGeant"};
    int nb_noms = sizeof(noms_valides) / sizeof(noms_valides[0]);
    for (int i = 0; i < nb_noms; i++)
    {
        if (strcmp(nom, noms_valides[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

CreatureMarine *trier_creatures(CreatureMarine *creatures, int nbr_mobs)
{
    for (int i = 0; i < nbr_mobs - 1; i++)
    {
        for (int j = i + 1; j < nbr_mobs; j++)
        {
            if (creatures[j].vitesse > creatures[i].vitesse)
            {
                // On échange les deux éléments
                CreatureMarine temp = creatures[i];
                creatures[i] = creatures[j];
                creatures[j] = temp;
            }
        }
    }

    return creatures;
}