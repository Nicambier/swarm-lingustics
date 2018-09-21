#include <kilolib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"


#define COMPLETE_TURN 53
#define SIGMA 50
#define HEAR_TIME 8

int next_turn, turn_turn;
char walk;

message_t message;
unsigned char sending_turn;
float a,b;
double m = 0.01;

uint8_t inventory[100];
unsigned char ids[100];
message_t* msgs[100];
int inventory_size, msgs_size, n;

float closest;
unsigned char link;

double rayleigh()
{
    double u;
    double res = 1001;
    while(res > 1000) {
        u = (float) rand_soft()/(float) 255;
        res = SIGMA * sqrt(-2 * log(u));
    }
    return res;
    //return 100;
}

int random_turn()
{
    double res = rand_soft() % COMPLETE_TURN;
    int half_turn = COMPLETE_TURN/2;
    return res - half_turn;
}

void setup()
{
    sending_turn = 0;
    
    a = 0;
    b = 0;
    link = 0;
    inventory_size = 0;
    msgs_size = 0;
    
    message.data[1] = kilo_uid;
    message.data[0] = 0;
    message.crc = message_crc(&message);
    
    rand_seed(rand_hard());
    
    walk = 1;
    next_turn=rayleigh();
    turn_turn=100; //flag value
    closest = -1;
}

void convertParams(uint8_t val)
{
    a = 1.25+0.25*(val>>4);
    b = 1.25+0.25*(val&(unsigned char) 15);
}

void config(uint8_t params, uint8_t l)
{
    convertParams(params);
    link = l;        
}

void speak(char activate) 
{
    if(activate) {
        if(sending_turn==0) { //set a new message only at the start of broadcasting
            message.type = NORMAL;
            if(inventory_size==0) {
                inventory_size=1;
                inventory[0] = rand_soft();
            }
            int r = rand_soft()%inventory_size;
            message.data[0] = inventory[r];
            // It's important that the CRC is computed after the data has been set;
            // otherwise it would be wrong and the message would be dropped by the
            // receiver.
            message.crc = message_crc(&message);
        }
        sending_turn=1;
    }
    else {
        message.data[0] = 0;
        message.crc = message_crc(0);
        sending_turn=0;
    }      
}

unsigned char noise(uint8_t w)
{
    uint8_t n = 0;
    short int i;
    for(i=0; i<8; ++i)
    {
        float r = (float) rand_soft()/(float) 255;
        if(r < m)
            n += pow(2,i);
    }
    return w ^ n;
}

short int alreadyReceived(unsigned char* ids, unsigned char id, int ids_size)
{
    int i;
    short int inside = 0;
    for(i=0; i<ids_size; ++i) {
        if(ids[i]==id)
        {
            inside = 1;
            break;
        }
    }
    return inside;
}

void hear()
{
    int i;
    if(kilo_ticks%HEAR_TIME==0) { //else we keep the same value for n
        n=0;
        unsigned char neigh_ids[20];
        short int nSize = 0;
        for(i=0; i<msgs_size; ++i) {
            char inside = 0, w=noise(msgs[i]->data[0]);
            if(!alreadyReceived(neigh_ids,msgs[i]->data[1],nSize)) {
                neigh_ids[nSize] = msgs[i]->data[1];
                ++nSize;
                int j = 0;
                while(j<inventory_size && !inside) {
                    if(inventory[j]==w)
                        inside = 1;
                    ++j;
                }
                
                if(inside) {
                    ++n;
                    inventory[0] = w;
                    inventory_size = 1;
                } else {
                    n=0;
                    inventory[inventory_size] = w;
                    ++inventory_size;
                }
            }
        }
        if(link==NO_LINK)
            n = msgs_size; //temp -> this line erases previous operation in order to perform regular aggregation
        msgs_size = 0; //delete messages;
    }
}

void loop()
{
    if(walk)
    {
        //MOVES
        set_color(RGB(0, 0, 3));
        --next_turn;
        if(next_turn<=0) {
            if(turn_turn==100)
                turn_turn=random_turn();            
            else if(turn_turn==0) {
                turn_turn = 100; //reset
                next_turn=rayleigh();
            }
            
            if(turn_turn<100) {
                spinup_motors(); // Spinup the motors to overcome friction.
                if(turn_turn>0) {
                    set_motors(0, kilo_turn_right);
                    --turn_turn;
                } else if(turn_turn<0) {
                    set_motors(kilo_turn_left, 0);
                    ++turn_turn;
                }
                set_color(RGB(3, 0, 0));
            }
        
        }
        else {
            spinup_motors(); // Spinup the motors to overcome friction.
            set_motors(kilo_straight_left, kilo_straight_right);}
    
        //NAMING GAME
        hear();
        speak(0);
        
        //TRANSITION
        if(link==EVO_LINK) {
            a = 0;//1.7;
            if(inventory_size==1) {
                convertParams(inventory[0]);
            }
        }
        
        float p = 0.03+0.48*(1-exp(-a*n));
        float r = (float) rand_soft()/(float) 255;
        if(kilo_ticks%HEAR_TIME==0 && r < p) {
            walk = 0;
        }
        

    }
    else//STAY
    {
        //MOVES
        set_color(RGB(0, 3, 0));
        set_motors(0, 0);
        
        //NAMING GAME
        hear();
        speak(1);
            
        
        //TRANSITION
        if(link==EVO_LINK) {
            b = 0;//3.9;
            if(inventory_size==1) {
                convertParams(inventory[0]);
            }
        }
        float p = exp(-b*n);
        float r = (float) rand_soft()/(float) 255;
        if(kilo_ticks%HEAR_TIME==0 && r < p) {
            walk = 1;
        }
    }
}

message_t *message_tx()
{
    if(sending_turn) {
        return &message;
    } else
        return 0;
}

void message_rx(message_t *m, distance_measurement_t *d)
{
    if(m->data[2] == CONFIG)
        config(m->data[0],m->data[1]);
    else if(m->data[0] != 0 && !alreadyReceived(ids,m->data[1],msgs_size)) {
        msgs[msgs_size]=m;
        ids[msgs_size]=m->data[1];
        ++msgs_size;
    }
}

int main()
{
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
    kilo_start(setup, loop);
    
    return 0;
}
