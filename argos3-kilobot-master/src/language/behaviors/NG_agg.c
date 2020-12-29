#include <kilolib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NONE 0
#define WORD 42
#define CONFIG 201

#define NO_LINK 0
#define EVO_LINK 2

//FIXED VALUES
#define PI_TURN 136
#define PI 3.14159265359

//PARAMETERS TO SET
#define MUTATIONS 0.01
#define WALK_TURNS 330
#define STAY_TURNS WALK_TURNS + PI_TURN/2

#define LINK NO_LINK

typedef enum {
              WALK = 0,
              TURN = 1,
              STAY = 2
} state_t;

// a word of the lexicon
struct Word {
    uint8_t aByte;
    uint8_t bByte;
    uint8_t cByte;
};

uint32_t next_decision;
state_t state;

// a message to send
message_t message;
// if true, then share a word
uint8_t broadcast = 0;

// internatl lexicon parameters
double a,b,c;
double m;

struct Word lexicon[255];
uint8_t ids[255];
struct Word msgs[255];
uint8_t lexicon_size, msgs_size, n;


void setup() {    
    if(LINK==EVO_LINK) {
        a = 0;
        b = 0;
        c = 0;
        m = MUTATIONS;
    } else {
        a = 1.70;
        b = 3.89;
        c = 0.49;
        m = 0;
    }    
    
    // setup initial message
    msgs_size = 0; // keeps count of received messages
    message.type = WORD; // only accepting WORD type
    message.data[0] = kilo_uid; // this stays like this forever
    
    // initialize seed
    uint8_t seed = rand_hard();
    rand_seed(seed);
    srand(seed);
    
    // random initialize a word
    struct Word init_word = {rand_soft(),rand_soft(),rand_soft()};
    lexicon[0] = init_word; // our only word at first
    lexicon_size = 1; // our dictionary has size 1
    
    // walk at the begin
    state = WALK;
    spinup_motors();
    set_motors(kilo_straight_left, kilo_straight_right);
    // asynchronous execution
    next_decision = kilo_ticks + rand_soft();
    
    // red color when walking
    set_color(RGB(3,0,0));
}

void reset() {
    setup();
}

// convert parameters according to the chosen word
void convertParams(struct Word val) {
    a = 0.02*(val.aByte);
    b = 0.02*(val.bByte);
    c = 0.0039*(val.cByte);
}

// add some random noise to the word
struct Word noise(struct Word w) {
    uint8_t i;
    for(i=0; i<24; ++i) {
        // generates random number on 16 bit of accuracy
        uint16_t r1 = rand_soft();
        r1 = r1 << 8 | rand_soft();
        double r = (double)r1/65535.0;

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

void speak(unsigned char activate) {
    if(lexicon_size == 0)
        return;

    if(activate) {
        int r = rand_soft()%lexicon_size;
        // kilo_uid and msg type alraedy set in setup
        // share the word
        message.data[1] = lexicon[r].aByte;
        message.data[2] = lexicon[r].bByte;
        message.data[3] = lexicon[r].cByte;
        // mandatory to compute the crc
        message.crc = message_crc(&message);
        broadcast = 1;
    }
    else {
        broadcast = 0;
    }    
}

void hear() {    
    if(msgs_size>0) {
        uint8_t inside = 0;
        struct Word w = msgs[rand_soft()%msgs_size];
        w = noise(w);

        uint8_t j = 0;
        while(j<lexicon_size && !inside) {
            if(lexicon[j].aByte==w.aByte && lexicon[j].bByte==w.bByte && lexicon[j].cByte==w.cByte) {
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

// sets the forward behavior
void forward() {
    // red when walking
    set_color(RGB(3,0,0));

    state = WALK;
    spinup_motors();
    set_motors(kilo_straight_left, kilo_straight_right);
    next_decision = kilo_ticks + WALK_TURNS;
}

//sets the behavior when turning
void turn() {
    int8_t turn_steps = (rand_soft()-127)%PI_TURN;
    
    if(turn_steps>0) {
        state = TURN;
        spinup_motors();
        set_motors(0, kilo_turn_right);
        next_decision = kilo_ticks + turn_steps;
    } else if(turn_steps<0) {
        state = TURN;
        spinup_motors();
        set_motors(kilo_turn_left, 0);
        next_decision = kilo_ticks - turn_steps;
    } else {
        forward();
    }
}

//sets the behaviour in the WALK state
void leave() {
    // red when walking
    set_color(RGB(3, 0, 0));
    speak(0);
    turn();
}

//sets the behaviour in the STAY state
void join() {
    // blue when staying
    set_color(RGB(0,0,3));
    state = STAY;
    speak(1);
    set_motors(0,0);
    next_decision = kilo_ticks + STAY_TURNS;
}

void loop() {
    if(kilo_ticks>=next_decision) {
        //TURNING
        if(state==TURN) {
            forward();
        } else {
            n = msgs_size;
            // take a random word from those received 
            hear();
            //delete messages;   
            msgs_size = 0; 

            //UPDATE PARAMETERS
            if(LINK==EVO_LINK) {
                if(lexicon_size>=1) {
                    convertParams(lexicon[rand_soft()%lexicon_size]);
                }
            }
            
            if(state==STAY) { 
                //TRANSITION
                double p = exp(-b*n);
                // generates random number on 16 bit of accuracy
                uint16_t r1 = rand_soft();
                r1 = r1 << 8 | rand_soft();
                double r = (double)r1/65535.0;
        
                if(r < p) {
                    leave();
                } else
                    join();
            } else if(state==WALK) {
                //TRANSITION
                double p;
                if(LINK==EVO_LINK) {
                    if(n==0) {
                        p = 0.01;
                    } else {
                        p = (1-c)+c*(1-exp(-a*n));
                    }
                } else {
                    p = 0.03+c*(1-exp(-a*n));
                }

                // generates random number on 16 bit of accuracy
                uint16_t r1 = rand_soft();
                r1 = r1 << 8 | rand_soft();
                double r = (double)r1/65535.0;

                if(r < p) {
                    join();
                } else {
                    leave();
                }
            }
        }
    }    
}

message_t *message_tx(){
    if (broadcast)
        return &message;
    else 
        return 0;
}

uint8_t idxOf(uint8_t* ids, uint8_t id, uint8_t ids_size){
    short int i;
    for(i=0; i<ids_size; ++i) {
        if(ids[i]==id)
            break;
    }
    return i;
}

void message_rx(message_t *m, distance_measurement_t *d) {
    if(m->type == WORD && 
    state!=TURN && 
    kilo_ticks>=(next_decision-40) &&
    estimate_distance(d) <= 100) {
        uint8_t idx = idxOf(ids,m->data[0],msgs_size);
        struct Word word;
        word.aByte = m->data[1];
        word.bByte = m->data[2];
        word.cByte = m->data[3];
        msgs[idx]=word;
        ids[idx]=m->data[1];
        if(idx >= msgs_size)
            ++msgs_size;
    }
}

int main() {
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
    kilo_start(setup, loop);
    
    return 0;
}
