// Prototypes
void stepByStep();       // Mode pas par pas
void cycle();            // Mode continu
// Variables globales
int output;              // G�re les diff�rentes sorties
int mode;                // G�re le mode de fonctionnement du testeur de c�ble
int stop;                // Condition d'arr�t de boucle dans la proc�dure cycle
int faster;              // G�re la vitesse du mode continu
void main() {
     TRISA = 0b00000010; // Ports A en sortie sauf RA1
     PORTA = 0b11001000; // Toutes les sorties � 0 sauf RA3/6/7
     TRISB = 0b00000001; // Ports B en  sortie sauf RB0
     PORTB = 0;          // Toutes les sorties � 0

     pcon.OSCF = 1;      // Configurer la fr�quence
     CMCON = 0b00000111; // D�sactiver les comparateurs sur RA0

     output = 256;                       // On initialise output � 256 afin d'avoir la premi�re LED allum�e quand on lance le mode pas par pas pour la 1ere fois
     
     RA2_bit = 1;
     delay_ms(500);
     RA2_bit = 0;


     while(1){
         if(RB0_bit == 0){               // Si l'on appuie bri�vement sur le bouton, on passe � la prochaine led du mode pas par pas
             RA3_bit = 0;                // 1ere LED allum�e = mode pas � pas s�lectionn�
             mode = 0;                   // mode 0 = mode pas � pas
             delay_ms(500);

             if(RB0_bit == 0){           // Si apr�s avoir appuyer sur le bouton, et que celui-ci est toujours appuy� 0.5 secondes plus tard, on passe au mode continu
                 RA6_bit = 0;            // 1ere et 2e LEDs allum�es = mode continu s�lectionn�
                 mode += 1;              // mode 1 = mode continu
                 delay_ms(500);
             }
             if(RB0_bit == 0){           // Si apr�s avoir appuyer sur le bouton, et que celui-ci est toujours appuy� 1 secondes plus tard, on passe au mode continu "rapide"
                 RA7_bit = 0;            // Les 3 LEDs allum�es = mode continu "rapide" s�lectionn�
                 mode += 1;              // mode 2 = mode continu "rapide"
                 delay_ms(500);
             }
             if(mode >= 3){             // Si la variable mode devrait augmenter au dessus de '2', on la r�initialise � '0' (= mode pas � pas)
                 mode = 0;
                 delay_ms(500);
             }
         }
         switch(mode){                  // Apr�s avoir d�termin� le mode, le programme va continuer
             case 0: RA3_bit = 0;       // Mode en cours d'utilisation, ici RA3 allum�e = mode pas � pas
                     RA6_bit = 1;
                     RA7_bit = 1;
                     stepByStep();
                     mode = -1;         // On passe le mode � -1, ainsi � pression br�ve sur le bouton, on rentre et sort de la proc�dure pas � pas
                     break;
             case 1: RA3_bit = 1;       // Mode en cours d'utilisation, ici RA6 allum�e = mode continu
                     RA6_bit = 0;
                     RA7_bit = 1;
                     faster = 0;        // Mode continu � vitesse normale;
                     cycle();
                     break;
             case 2: RA3_bit = 1;       // Mode en cours d'utilisation, ici RA7 allum�e = mode continu rapide
                     RA6_bit = 1;
                     RA7_bit = 0;
                     faster = 1;        // Mode continu � vitesse rapide
                     cycle();
                     break;
         }
     }
}
/* Sous-programmes */
// Mode pas � pas
void stepByStep(){
    output = output * 2;             // On double la valeur de la variable qui g�re les sortie
    PORTB = output;                  // Le port B prends la valeur de output, et selon la valeur d'output, on passe � 1 une sortie pr�cise (1, 2, 4, 8, 16, 32, 64, 128)

    RA0_bit = 0;                     // La premi�re sortie est sur un port diff�rent, donc on la d�sactive manuellement
    delay_ms(500);                   // le programme attends 0.5 seconde avant de continuer

    if(output >= 256){               // Si la variable qui g�re nos sorties atteins la valeur de 256 ou plus on ex�cute les lignes suivantes
        output = 1;                  // on r�initialise notre variable qui g�re les sorties
        RA0_bit = 1;                 // on passe manuellement la premi�re sortie � 1, car celle-ci n'est pas g�r�e par la variable output
        delay_ms(500);               // le programme attends 0.5 secondes avant de continuer
    }
}
// Mode continu
void cycle(){
    delay_ms(500);                   // le programme attends 0.5 secondes avant de continuer sinon il reviendras dans le mode pas � pas
    stop = 0;                        // on initialise la condition d'arr�t de la boucle qui est � la ligne suivante
    while(stop != 1){                // tant que la condition d'arr�t (stop) est diff�rente de 1, on ex�cute les lignes suivantes
        output = output * 2;         // On double la valeur de la variable qui g�re les sortie
        PORTB = output;              // Le port B prends la valeur de output, et selon la valeur d'output, on passe � 1 une sortie pr�cise (1, 2, 4, 8, 16, 32, 64, 128)

        RA0_bit = 0;                 // La premi�re sortie est sur un port diff�rent, donc on la d�sactive manuellement
        if(faster == 0){             // Si la condition est v�rifi�e, le programme feras une pause de 0.25s avant de continuer et on est donc en mode normal, si elle n'est pas v�rifi�e, il n'y a pas de pause et on est donc en mode rapide
            delay_ms(250);           // Pause de 0.25s
        }

        if(output >= 256){           // Si la variable qui g�re nos sorties atteins la valeur de 256 ou plus on ex�cute les lignes suivantes
            output = 1;              // on r�initialise notre variable qui g�re les sorties
            RA0_bit = 1;             // on passe manuellement la premi�re sortie � 1, car celle-ci n'est pas g�r�e par la variable output
            if(faster == 0){         // Si la condition est v�rifi�e, le programme feras une pause de 0.25s avant de continuer et on est donc en mode normal, si elle n'est pas v�rifi�e, il n'y a pas de pause et on est donc en mode rapide
                delay_ms(250);       // Pause de 0.25s
            }
        }

        if(RB0_bit == 0){            // Une pression br�ve de bouton dans le mode continue normal ou rapide qui permet de revenir au mode pas � pas
            mode = 0;                // On s�lectionne le mode pas � pas
            stop = 1;                // On assigne une valeur � la condition d'arr�t de notre boucle afin que celle s'arr�te et nous laisse repasser au mode pas � pas.
        }
    }
}