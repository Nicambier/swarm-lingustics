#include <kilolib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int nt;
int ns;
int nm;
message_t message;
short int sending_turn;
char inventory[100][9];
int inventory_size;

void setup()
{
    // Set the message.
    message.type = NORMAL;
    int i=0;
    for(i=0;i<9;++i)
        message.data[i] = '\0';
    message.crc = message_crc(&message);
    sending_turn = 0;
    
    inventory_size = 0;
    for(i=0;i<100;++i)
        inventory[i][0] = '\0';
    
    rand_seed(kilo_uid);
    delay(10*rand_soft());
    
    nt = 0;
    ns = 6;
    nm = 25;
}

void createWord() 
{
    int i;
    int length = rand_soft() % 7 + 1;
    for(i=0;i<length;++i) {
        message.data[i] = rand_soft() % 26 + 97;
    }
    message.data[length] = '\0';
}

void updateInventory(char *w)
{
    short int inside = 0;
    int i = 0;
    while(i<inventory_size && !inside) {
        if(!strcmp(inventory[i],w))
            inside = 1;
        ++i;
    }
    
    
    if(inside) {
        i=0;
        while(i<1 || w[i] != '\0') {
            inventory[0][i] = w[i];
            ++i;
        }
        inventory[0][i] = '\0';
        inventory_size = 1;
    } else {
        i=0;
        while(i<1 || w[i] != '\0') {
            inventory[inventory_size][i] = w[i];
            ++i;
        }
        inventory[inventory_size][i] = '\0';
        ++inventory_size;
    }	
}

void loop()
{
    set_color(RGB(0, 0, 1));
    ++nt;
    if(nt%nm==0) {
        // Spinup the motors to overcome friction.
        spinup_motors();
        set_motors(kilo_turn_left, 0);
        delay(20*rand_soft());
    }
    // Spinup the motors to overcome friction.
    spinup_motors();
    set_motors(kilo_straight_left, kilo_straight_right);

    if(nt%ns!=0) { //listener
        // Set the LED green.
        set_color(RGB(0, 1, 0));
        if(inventory_size==1)
            printf("%d: %s\n",nt,inventory[0]);
        else
            fprintf(stderr,"%d: no convergence\n",nt);
    
    } else{ //speaker
        // Set the LED red.
        set_color(RGB(1, 0, 0));
        message.type = NORMAL;
        if(inventory_size>0) {
            int w = rand_soft()%inventory_size;
            int i = 0;
            while(i<1 || inventory[w][i]!='\0'){
                message.data[i] = inventory[w][i];
                ++i;
            }
            message.data[i] = '\0';
            
        } else
            createWord();

        // It's important that the CRC is computed after the data has been set;
        // otherwise it would be wrong and the message would be dropped by the
        // receiver.
        message.crc = message_crc(&message);
        sending_turn = 1;
    }
}

message_t *message_tx()
{
    if(sending_turn) {
        sending_turn = 0;
        return &message;
    } else
        return 0;
}

void message_rx(message_t *m, distance_measurement_t *d)
{
    char w[9];
    int i;
    for(i=0;i<9;++i)
        w[i] = m->data[i];
    if(w[0] != 0)
        updateInventory(w);
}

int main()
{
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
    kilo_start(setup, loop);
    
    return 0;
}
