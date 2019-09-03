#include <kilolib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../config.h"
#include "../conversion.c"

//FIXED VALUES
#define PI_TURN 40
#define PI 3.14159265358979323846
#define TWO_PI 2.0*PI


//PARAMETERS TO SET
#define TIMESTEP 80
#define MUTATIONS 0.01
#define RHO 0
#define JOIN_TURN 0 //set to 1 if you want the robots to automatically turn around when they are joining an aggregate

//#define MAX_DIST 2

int t, turn_steps;
unsigned short int STAY, TURN;

message_t message;
double a,b,c;
double m;

uint32_t lexicon[200];
uint8_t ids[200];
uint32_t msgs[200];
int lexicon_size, msgs_size, n;

unsigned char link;

void setup()
{    
    a = 0;
    b = 0;
    c = 0;
    m = MUTATIONS;
    link = EVO_LINK;
    
    msgs_size = 0;
    message.data[0] = NONE;
    message.data[1] = kilo_uid;
    message.data[2] = 0;
    message.data[3] = 0;
    message.data[4] = 0;
    
    rand_seed(rand_hard());
    
    uint32_t init_word = rand_soft();
    init_word+= rand_soft() << 8;
    init_word+= rand_soft() << 16;
    lexicon[0] = init_word;
    lexicon_size = 1;
    //lastWord=lexicon[0];
    
    STAY=0;
    TURN=0;
    t=rand_soft()%TIMESTEP;
    spinup_motors(); // Spinup the motors to overcome friction.
    set_motors(kilo_straight_left, kilo_straight_right);
    turn_steps=0;
}

void reset()
{
    setup();
}

int random_turn()
{
    double val, theta, u, q;
    //double c = RHO;
    q = 0.5;
    u = (double) ((rand_soft()<<8)+rand_soft())/(double) 65536;
    val = (1.0 - RHO) / (1.0 + RHO);
    theta = 2 * atan(val * tan(PI * (u - q)));
    
    return  theta*PI_TURN/PI;
}

void convertParams(uint32_t val)
{
    a = 0.02*((val>>8) & 255);
    b = 0.02*(val & 255);
    c = 0.0039*(val>> 16);
    
    //activate the following line if LEDS are working
    //set_color(RGB((int) (3*c), (int) (a/1.7), (int) (b/1.7)));
}

uint32_t noise(uint32_t w)
{
    uint8_t i;
    for(i=0; i<24; ++i)
    {
        double r = ((double) ((rand_soft()<<8)+rand_soft()))/(double) 65535;
        if(r < m)
            w = w ^ (uint32_t) pow(2,i);
    }
    return w;
}

void speak(unsigned char activate) 
{
    message.data[5] = lexicon_size;
    message.data[6] = n;
    if(activate) {
        message.type = NORMAL;
        int r = rand_soft()%lexicon_size;
        message.data[0] = WORD;
        message.data[2] = (lexicon[r] >> 8) & 255;
        message.data[3] = lexicon[r] & 255;
        message.data[4] = lexicon[r] >> 16;
        // It's important that the CRC is computed after the data has been set;
        // otherwise it would be wrong and the message would be dropped by the
        // receiver.
        message.crc = message_crc(&message);
    }
    else {
        message.data[0] = NONE;
        message.data[2] = 0;
        message.data[3] = 0;
        message.data[4] = 0;
        message.crc = message_crc(0);
    }    
}

void hear()
{    
    if(msgs_size>0) {
        uint8_t inside = 0;
        uint32_t w=msgs[rand_soft()%msgs_size];
        if(link==EVO_LINK)
            w=noise(w);

        int j = 0;
        while(j<lexicon_size && !inside) {
            if(lexicon[j]==w) {
                //++scores[j];
                inside = 1;
            }
            ++j;
        }
        
        if(inside) { //success
            lexicon[0] = w;
            lexicon_size = 1;
        }
        else { //failure
            lexicon[lexicon_size] = w;
            ++lexicon_size;
        }
    }
}

//sets the behaviour when turning
void turn(unsigned char activate) {
    if(activate) {
        turn_steps=random_turn();
        TURN=1;
        spinup_motors(); // Spinup the motors to overcome friction.
        if(turn_steps>0) {
            set_motors(0, kilo_turn_right);
        } else {
            set_motors(kilo_turn_left, 0);
            turn_steps = -turn_steps;
        }
    }
    else {
        TURN=0;
        turn_steps=0;
    }
}

//sets the behaviour in the WALK state
void leave(uint8_t first_time) {
    STAY=0;
    speak(0);
    if(first_time && JOIN_TURN) { //don't turn when leaving an aggregate as a turn was already performed when joining
        spinup_motors(); // Spinup the motors to overcome friction.
        set_motors(kilo_straight_left, kilo_straight_right);
    }
    else
        turn(1);
}

//sets the behaviour in the STAY state
void join(uint8_t first_time) {
    STAY=1;
    speak(1);
    set_color(RGB(0, 3, 0));
    if(first_time && JOIN_TURN) {
        turn(1);
        turn_steps=PI_TURN;
    } else
        set_motors(0,0);
}

void loop()
{
    //TURNING
    if(TURN && t%TIMESTEP==turn_steps) {
        turn(0);
        if(STAY)
            set_motors(0,0);
        else {
            spinup_motors(); // Spinup the motors to overcome friction.
            set_motors(kilo_straight_left, kilo_straight_right);
            t=0;}
    }
    //behaviour changes every TIMESTEP (as set above)
    else {
        if(t%TIMESTEP==0) {
            n = msgs_size;
            hear();
            msgs_size = 0; //delete messages;   
            //UPDATE PARAMETERS
            if(link==EVO_LINK) {
                if(lexicon_size>=1) {
                    convertParams(lexicon[rand_soft()%lexicon_size]);
                }
            }
            
            if(STAY) { 
                //TRANSITION
                double p = exp(-b*n);
                double r = ((double) ((rand_soft()<<8)+rand_soft()))/(double) 65535; //generates random number on 16 bits for accuracy
                if(r < p) {
                    leave(n>1);
                } else
                    join(0);
            }
            else
            {
                //TRANSITION
                double p = (1-c)+c*(1-exp(-a*n));
                double r = ((double) ((rand_soft()<<8)+rand_soft()))/(double) 65535; //generates random number on 16 bits for accuracy
                if(r < p) {
                    join(n>0);
                } else
                    leave(0);

            }
        }
    }
    ++t;        
}

message_t *message_tx()
{
    return &message;
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

void message_rx(message_t *m, distance_measurement_t *d)
{
    if(m->data[0] == WORD && t%TIMESTEP>=(TIMESTEP-10)) //receive only during the last 10 timesteps before the tick
    {
        short int idx = idxOf(ids,m->data[1],msgs_size);
        uint32_t word = m->data[2]<<8;
        word += m->data[3];
        word += m->data[4]<<16;
        msgs[idx]=word;
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
