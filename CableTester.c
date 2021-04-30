// Prototypes
void stepByStep();       // Mode pas par pas
void cycle();            // Mode continu
// Variables globales
int output;              // Gère les différentes sorties
int mode;                // Gère le mode de fonctionnement du testeur de câble
int stop;                // Condition d'arrêt de boucle dans la procédure cycle
int faster;              // Gère la vitesse du mode continu
void main() {
     TRISA = 0b00000010; // Ports A en sortie sauf RA1
     PORTA = 0b11001000; // Toutes les sorties à 0 sauf RA3/6/7
     TRISB = 0b00000001; // Ports B en  sortie sauf RB0
     PORTB = 0;          // Toutes les sorties à 0

     pcon.OSCF = 1;      // Configurer la fréquence
     CMCON = 0b00000111; // Désactiver les comparateurs sur RA0

     output = 256;                       // On initialise output à 256 afin d'avoir la première LED allumée quand on lance le mode pas par pas pour la 1ere fois
     
     RA2_bit = 1;
     delay_ms(500);
     RA2_bit = 0;


     while(1){
         if(RB0_bit == 0){               // Si l'on appuie brièvement sur le bouton, on passe à la prochaine led du mode pas par pas
             RA3_bit = 0;                // 1ere LED allumée = mode pas à pas sélectionné
             mode = 0;                   // mode 0 = mode pas à pas
             delay_ms(500);

             if(RB0_bit == 0){           // Si après avoir appuyer sur le bouton, et que celui-ci est toujours appuyé 0.5 secondes plus tard, on passe au mode continu
                 RA6_bit = 0;            // 1ere et 2e LEDs allumées = mode continu sélectionné
                 mode += 1;              // mode 1 = mode continu
                 delay_ms(500);
             }
             if(RB0_bit == 0){           // Si après avoir appuyer sur le bouton, et que celui-ci est toujours appuyé 1 secondes plus tard, on passe au mode continu "rapide"
                 RA7_bit = 0;            // Les 3 LEDs allumées = mode continu "rapide" sélectionné
                 mode += 1;              // mode 2 = mode continu "rapide"
                 delay_ms(500);
             }
             if(mode >= 3){             // Si la variable mode devrait augmenter au dessus de '2', on la réinitialise à '0' (= mode pas à pas)
                 mode = 0;
                 delay_ms(500);
             }
         }
         switch(mode){                  // Après avoir déterminé le mode, le programme va continuer
             case 0: RA3_bit = 0;       // Mode en cours d'utilisation, ici RA3 allumée = mode pas à pas
                     RA6_bit = 1;
                     RA7_bit = 1;
                     stepByStep();
                     mode = -1;         // On passe le mode à -1, ainsi à pression brève sur le bouton, on rentre et sort de la procédure pas à pas
                     break;
             case 1: RA3_bit = 1;       // Mode en cours d'utilisation, ici RA6 allumée = mode continu
                     RA6_bit = 0;
                     RA7_bit = 1;
                     faster = 0;        // Mode continu à vitesse normale;
                     cycle();
                     break;
             case 2: RA3_bit = 1;       // Mode en cours d'utilisation, ici RA7 allumée = mode continu rapide
                     RA6_bit = 1;
                     RA7_bit = 0;
                     faster = 1;        // Mode continu à vitesse rapide
                     cycle();
                     break;
         }
     }
}
/* Sous-programmes */
// Mode pas à pas
void stepByStep(){
    output = output * 2;             // On double la valeur de la variable qui gère les sortie
    PORTB = output;                  // Le port B prends la valeur de output, et selon la valeur d'output, on passe à 1 une sortie précise (1, 2, 4, 8, 16, 32, 64, 128)

    RA0_bit = 0;                     // La première sortie est sur un port différent, donc on la désactive manuellement
    delay_ms(500);                   // le programme attends 0.5 seconde avant de continuer

    if(output >= 256){               // Si la variable qui gère nos sorties atteins la valeur de 256 ou plus on exécute les lignes suivantes
        output = 1;                  // on réinitialise notre variable qui gère les sorties
        RA0_bit = 1;                 // on passe manuellement la première sortie à 1, car celle-ci n'est pas gérée par la variable output
        delay_ms(500);               // le programme attends 0.5 secondes avant de continuer
    }
}
// Mode continu
void cycle(){
    delay_ms(500);                   // le programme attends 0.5 secondes avant de continuer sinon il reviendras dans le mode pas à pas
    stop = 0;                        // on initialise la condition d'arrêt de la boucle qui est à la ligne suivante
    while(stop != 1){                // tant que la condition d'arrêt (stop) est différente de 1, on exécute les lignes suivantes
        output = output * 2;         // On double la valeur de la variable qui gère les sortie
        PORTB = output;              // Le port B prends la valeur de output, et selon la valeur d'output, on passe à 1 une sortie précise (1, 2, 4, 8, 16, 32, 64, 128)

        RA0_bit = 0;                 // La première sortie est sur un port différent, donc on la désactive manuellement
        if(faster == 0){             // Si la condition est vérifiée, le programme feras une pause de 0.25s avant de continuer et on est donc en mode normal, si elle n'est pas vérifiée, il n'y a pas de pause et on est donc en mode rapide
            delay_ms(250);           // Pause de 0.25s
        }

        if(output >= 256){           // Si la variable qui gère nos sorties atteins la valeur de 256 ou plus on exécute les lignes suivantes
            output = 1;              // on réinitialise notre variable qui gère les sorties
            RA0_bit = 1;             // on passe manuellement la première sortie à 1, car celle-ci n'est pas gérée par la variable output
            if(faster == 0){         // Si la condition est vérifiée, le programme feras une pause de 0.25s avant de continuer et on est donc en mode normal, si elle n'est pas vérifiée, il n'y a pas de pause et on est donc en mode rapide
                delay_ms(250);       // Pause de 0.25s
            }
        }

        if(RB0_bit == 0){            // Une pression brève de bouton dans le mode continue normal ou rapide qui permet de revenir au mode pas à pas
            mode = 0;                // On sélectionne le mode pas à pas
            stop = 1;                // On assigne une valeur à la condition d'arrêt de notre boucle afin que celle s'arrête et nous laisse repasser au mode pas à pas.
        }
    }
}