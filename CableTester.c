// Prototypes
void stepByStep();       // Mode 1
void cycle();            // Mode 2 & 3
// Variables globales
int output;              // Variable to manage the LED output
int mode;                // Manages the mode in which the CableTester is in
int stop;                // Stop condition for while loop
int faster;              // Used to define which automatic mode is used
void main() {
     TRISA = 0b00000010; 
     PORTA = 0b11001000; 
     TRISB = 0b00000001; 
     PORTB = 0;          

     pcon.OSCF = 1;      
     CMCON = 0b00000111; 

     output = 256;                       
     
     RA2_bit = 1;
     delay_ms(500);
     RA2_bit = 0;
	
	// When the CableTester is ON, one quick button press will make it go to the next LED.
	// A 1 second button press will make it enter mode 2, which will circle through all LEDs automatically.
	// A 1.5 second press will enter mode 3, which is the same as mode 2, but without delay so it will go at 4MHz
	// So it will look like all the LEDs are lit at the same moment (but they aren't).

     while(1){
         if(RB0_bit == 0){               
             RA3_bit = 0;                
             mode = 0;                   
             delay_ms(500);

             if(RB0_bit == 0){           
                 RA6_bit = 0;            
                 mode += 1;              
                 delay_ms(500);
             }
             if(RB0_bit == 0){           
                 RA7_bit = 0;            
                 mode += 1;              
                 delay_ms(500);
             }
             if(mode >= 3){             
                 mode = 0;
                 delay_ms(500);
             }
         }
         switch(mode){                  
             case 0: RA3_bit = 0;       
                     RA6_bit = 1;
                     RA7_bit = 1;
                     stepByStep();
                     mode = -1;         
                     break;
             case 1: RA3_bit = 1;       
                     RA6_bit = 0;
                     RA7_bit = 1;
                     faster = 0;        
                     cycle();
                     break;
             case 2: RA3_bit = 1;       
                     RA6_bit = 1;
                     RA7_bit = 0;
                     faster = 1;        
                     cycle();
                     break;
         }
     }
}
/* Functions */
// Mode 1
void stepByStep(){
    output = output * 2;             
    PORTB = output;                  

    RA0_bit = 0;                     
    delay_ms(500);                   

    if(output >= 256){               
        output = 1;                  
        RA0_bit = 1;                 
        delay_ms(500);               
    }
}
// Mode 2 & 3
void cycle(){
    delay_ms(500);                   
    stop = 0;                        
    while(stop != 1){                
        output = output * 2;         
        PORTB = output;              

        RA0_bit = 0;                 
        if(faster == 0){             
            delay_ms(250);           
        }

        if(output >= 256){           
            output = 1;              
            RA0_bit = 1;             
            if(faster == 0){         
                delay_ms(250);       
            }
        }

        if(RB0_bit == 0){            
            mode = 0;                
            stop = 1;                
        }
    }
}