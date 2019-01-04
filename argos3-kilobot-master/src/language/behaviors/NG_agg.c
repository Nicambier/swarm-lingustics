#include <kilolib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../config.h"
#include "../conversion.c"

#define HEAR_TIME 8

#define PI_TURN 26
#define STRAIGHT 10
#define RHO 0.6
#define PI 3.14159265358979323846
#define TWO_PI 2.0*PI

int next_turn, turn_turn;
char walk;

message_t message;
unsigned char sending_turn;
float a,b;
double m;

uint8_t inventory[100];
uint8_t ids[100];
uint8_t msgs[100];
int inventory_size, msgs_size, n;

unsigned char link;

double rayleigh()
{
    /*double u;
    double res = 1001;
    while(res > 1000) {
        u = (float) rand_soft()/(float) 255;
        res = SIGMA * sqrt(-2 * log(u));
    }
    //return res;*/
    return STRAIGHT;
}

int random_turn()
{
    double val, theta, u, q;
    double c = RHO;
    q = 0.5;
    u = (double) rand_soft()/(double) 256;
    val = (1.0 - c) / (1.0 + c);
    theta = 2 * atan(val * tan(PI * (u - q)));
    
    return  theta*8;
}

void setup()
{
    sending_turn = 0;
    
    a = 0;
    b = 0;
    m = 0;
    link = 0;
    msgs_size = 0;
    
    message.data[0] = NONE;
    message.data[1] = kilo_uid;
    message.data[2] = 0;
    
    rand_seed(rand_hard());
    
    walk = 1;
    next_turn=rayleigh();
    turn_turn=100; //flag value
}

void convertParams(uint8_t val)
{
    if(walk)
        a = 1.25 + 0.25*(val>>4);
    else
        b = 1.25 + 0.25*(val%16);
}

void config(uint8_t l, uint8_t params, uint8_t mutation)
{
    link = l; 
    if(link != EVO_LINK) {
        convertParams(params);
    }
    
    if(link == EVO_LINK)
        m = uint8_to_double(mutation);
}

void speak(uint8_t activate) 
{
    if(activate) {
        if(sending_turn==0) { //set a new message only at the start of broadcasting
            message.type = NORMAL;
            if(inventory_size==0) {
                inventory_size=1;
                inventory[0] = rand_hard();//((rand_hard()%128) << 8) + (rand_hard()%128);
            }
            int r = rand_soft()%inventory_size;
            message.data[0] = WORD;
            message.data[2] = inventory[r];
            // It's important that the CRC is computed after the data has been set;
            // otherwise it would be wrong and the message would be dropped by the
            // receiver.
            message.crc = message_crc(&message);
        }
        sending_turn=1;
    }
    else {
        message.data[0] = NONE;
        message.data[2] = 0;
        message.crc = message_crc(0);
        sending_turn=0;
    }      
}

uint8_t noise(uint8_t w)
{
    uint8_t i;
    for(i=0; i<8; ++i)
    {
        double r = (double) rand_soft()/(double) 256;
        if(r < m)
            w = w ^ (uint8_t) pow(2,i);
    }
    return w;
}

short int idxOf(uint8_t* ids, uint8_t id, int ids_size)
{
    short int i;
    for(i=0; i<ids_size; ++i) {
        if(ids[i]==id)
            break;
    }
    return i;
}

void hear()
{
    n=0;
    int i;
    for(i=0; i<msgs_size; ++i) {
        uint8_t inside = 0;
        uint8_t w=msgs[i];
        if(link==EVO_LINK || link==STRONG_LINK)
            w=noise(w);

        int j = 0;
        while(j<inventory_size && !inside) {
            if(inventory[j]==w)
                inside = 1;
            ++j;
        }
        
        if(inside) {
            n+=1;
            inventory[0] = w;
            inventory_size = 1;
        } else {
            n=0;
            inventory[inventory_size] = w;
            ++inventory_size;
        }
    }
    if(link==NO_LINK)
        n = msgs_size; //this line erases previous operation in order to perform regular aggregation
    msgs_size = 0; //delete messages;
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
            set_motors(kilo_straight_left, kilo_straight_right);
        }
        //NAMING GAME
        speak(0);
        
        //TRANSITION
        if(link==EVO_LINK) {
            a = 0;//1.7;
            if(inventory_size==1) {
                convertParams(inventory[0]);
            }
        }
        
        if(kilo_ticks%HEAR_TIME==0)
        {
            hear();
            float p = 0.03+0.48*(1-exp(-a*n));
            float r = (float) rand_soft()/(float) 255;
            if(r < p) {
                walk = 0;
            }
        }

    }
    else//STAY
    {
        //MOVES
        set_color(RGB(0, 3, 0));
        set_motors(0, 0);
        
        //NAMING GAME
        speak(1);
            
        
        //TRANSITION
        if(link==EVO_LINK) {
            b = 0;//3.9;
            if(inventory_size==1) {
                convertParams(inventory[0]);
            }
        }
        
        if(kilo_ticks%HEAR_TIME==0)
        {
            hear();
            float p = exp(-b*n);
            float r = (float) rand_soft()/(float) 255;
            if(r < p) {
                walk = 1;
            }
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
    if(m->data[0] == CONFIG)
        config(m->data[1],m->data[2],m->data[4]);
    else if(m->data[0]==WORD) 
    {
        short int idx = idxOf(ids,m->data[1],msgs_size);
        msgs[idx]=m->data[2];
        ids[idx]=m->data[1];
        if(idx >= msgs_size)
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
