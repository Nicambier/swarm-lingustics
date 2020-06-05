#include <kilolib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../config.h"

//FIXED VALUES
#define PI_TURN 40
#define PI 3.14159265358979323846
#define TWO_PI 2.0*PI


//PARAMETERS TO SET
#define TIMESTEP 100
#define MUTATIONS 0.01
#define RHO 0
#define JOIN_TURN 0 //set to 1 if you want the robots to automatically turn around when they are joining an aggregate

#define LINK EVO_LINK

struct Word{
    uint8_t aByte;
    uint8_t bByte;
    uint8_t cByte;
};

int t, turn_steps;
unsigned short int STAY, TURN;

message_t message;
double a,b,c;
double m;

struct Word lexicon[200];
uint8_t ids[200];
struct Word msgs[200];
int lexicon_size, msgs_size, n;

void setup()
{    
    if(LINK==EVO_LINK) {
        a = 0;
        b = 0;
        c = 0;
        m = MUTATIONS;
    }
    else {
        a = 1.70;
        b = 3.89;
        c = 0.49;
        m = 0;
    }    
    
    msgs_size = 0;
    message.data[0] = NONE;
    message.data[1] = kilo_uid;
    message.data[2] = 0;
    message.data[3] = 0;
    message.data[4] = 0;
    
    rand_seed(rand_hard());
    
    struct Word init_word = {rand_soft(),rand_soft(),rand_soft()};
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
    /*double val, theta, u, q;
    //double c = RHO;
    q = 0.5;
    u = ((rand_soft()<<8)+rand_soft())/65536.0;
    val = (1.0 - RHO) / (1.0 + RHO);
    theta = 2 * atan(val * tan(PI * (u - q)));
    
    return  theta*PI_TURN/PI;*/
    int sign = rand_soft()%2?-1:1;
    return sign * (rand_soft()%PI_TURN);
}

void convertParams(struct Word val)
{
    a = 0.02*(val.aByte);
    b = 0.02*(val.bByte);
    c = 0.0039*(val.cByte);
    
    //activate the following line if LEDS are working
    set_color(RGB((int) (val.cByte/85), (int) (val.aByte/85), (int) (val.bByte/85)));
}

struct Word noise(struct Word w)
{
    uint8_t i;
    for(i=0; i<24; ++i)
    {
        double r = ((rand_soft()<<8)+rand_soft())/65535.0;;
        if(r < m) {
            if(i<8)
                w.aByte = w.aByte ^ (uint8_t) pow(2,i);
            else if(i<16)
                w.bByte = w.bByte ^ (uint8_t) pow(2,i%8);
            else
                w.cByte = w.cByte ^ (uint8_t) pow(2,i%8);
        }
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
        message.data[2] = lexicon[r].aByte;
        message.data[3] = lexicon[r].bByte;
        message.data[4] = lexicon[r].cByte;
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
        struct Word w=msgs[rand_soft()%msgs_size];
        w=noise(w);

        int j = 0;
        while(j<lexicon_size && !inside) {
            if(lexicon[j].aByte==w.aByte && lexicon[j].bByte==w.bByte && lexicon[j].cByte==w.cByte) {
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
    //set_color(RGB(0, 3, 0));
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
            if(LINK==EVO_LINK) {
                if(lexicon_size>=1) {
                    convertParams(lexicon[rand_soft()%lexicon_size]);
                }
            }
            
            if(STAY) { 
                //TRANSITION
                double p = exp(-b*n);
                double r = ((rand_soft()<<8)+rand_soft())/65535.0; //generates random number on 16 bits for accuracy
                if(r < p) {
                    leave(n>1);
                } else
                    join(0);
            }
            else
            {
                //TRANSITION
                double p;
                if(LINK==EVO_LINK)
                    p = (1-c)+c*(1-exp(-a*n));
                else
                    p = 0.03+c*(1-exp(-a*n));
                double r = ((rand_soft()<<8)+rand_soft())/65535.0; //generates random number on 16 bits for accuracy
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
        struct Word word;
        word.aByte = m->data[2];
        word.bByte = m->data[3];
        word.cByte = m->data[4];
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
