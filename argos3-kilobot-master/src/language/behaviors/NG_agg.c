#include <kilolib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WORD 42

#define NO_LINK 0
#define STRONG_LINK 1
#define EVO_LINK 2

//FIXED VALUES
#define PI_TURN 132
#define PI 3.14159265359

//PARAMETERS TO SET
#define MUTATIONS 0.01
#define WALK_TURNS 330
#define STAY_TURNS WALK_TURNS + PI_TURN/2

#define LINK EVO_LINK

// kilobot state
typedef enum {
    WALK = 0,
    TURN = 1,
    STAY = 2
} state_t;

// word struct 
struct Word{
    uint8_t aByte;
    uint8_t bByte;
    uint8_t cByte;
};

uint32_t next_decision;
state_t state;

// message to send (twice per second)
message_t message;
uint8_t broadcast; 

double a,b,c;
double m;

struct Word lexicon[100];
uint8_t ids[100];
struct Word msgs[100];
uint8_t lexicon_size, ids_size, n;

void setup() {    
    if(LINK==EVO_LINK) {
        a = 0.0;
        b = 0.0;
        c = 0.0;
        m = MUTATIONS;
    } else {
        a = 1.70;
        b = 3.89;
        c = 0.49;
        m = 0;
    }

    // no msgs in the buffer at the beginning
    ids_size = 0;

    // initialize random seed
    uint8_t seed = rand_hard();
    rand_seed(seed);
    srand(seed);

    // randomly initialize word
    struct Word init_word = {rand_soft(),rand_soft(),rand_soft()};
    lexicon[0] = init_word;
    lexicon_size = 1;
    
    // do not send messages
    broadcast = 0;

    // first motion is go straight
    state = WALK;
    set_color(RGB(3, 0, 0));
    spinup_motors();
    set_motors(kilo_straight_left, kilo_straight_right);
    // asynchronous
    next_decision = kilo_ticks + rand_soft();
}

void reset() {
    setup();
}

void convertParams(struct Word val) {
    a = 0.02*(val.aByte);
    b = 0.02*(val.bByte);
    c = 0.0039*(val.cByte);
}

// add random noise to the word
struct Word noise(struct Word w) {
    uint8_t i;
    for(i=0; i<24; ++i) {
        double r = ((rand_soft()<<8)+rand_soft())/65535.0;
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

// generate message for sharing a word in the dictionary
void speak(unsigned char activate) {
    if(activate) {
        message.type = WORD;
        uint8_t	r = rand_soft()%lexicon_size;
        // set current kilo_uid
        message.data[0] = kilo_uid;
        // share the word
        message.data[1] = lexicon[r].aByte;
        message.data[2] = lexicon[r].bByte;
        message.data[3] = lexicon[r].cByte;
        // compute the crc (mandatory)
        message.crc = message_crc(&message);
        // signal to send the message
        broadcast = 1;
    } else {
        // do not send the message
        broadcast = 0;
    }
}

// listen to received messages
// get one from the buffer
void hear() {    
    if(ids_size>0) {
        uint8_t inside = 0;
        struct Word w=msgs[rand_soft()%ids_size];
        w=noise(w);

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

// sets the behavior when moving straight
void forward() {
    state = WALK;
    spinup_motors();
    set_motors(kilo_straight_left, kilo_straight_right);
    next_decision = kilo_ticks + WALK_TURNS;
}

// sets the behaviour when turning
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

// sets the behaviour in the WALK state
void leave() {
    set_color(RGB(3, 0, 0));
    speak(0);
    turn();
}

// sets the behaviour in the STAY state
void join() {
    if(n==0) {
        set_color(RGB(0,0,0));
    } else if(n<=2) {
        set_color(RGB(0,3,0));
    } else if (n <= 5) {
        set_color(RGB(0,0,3));
    } else {
        set_color(RGB(3,3,3));
    }

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
            n = ids_size;
            hear();
            ids_size = 0; //delete messages;
            //UPDATE PARAMETERS
            if(LINK==EVO_LINK) {
                if(lexicon_size>=1) {
                    convertParams(lexicon[rand_soft()%lexicon_size]);
                }
            }
            
            if(state==STAY) {
                //TRANSITION
                //double p = exp(-b*n);
                uint8_t p = round(exp(-b*n)*255);
                // generates random number on 16 bit of accuracy
                //double r = ((rand_soft()<<8)+rand_soft())/65535.0;
                uint8_t r = rand_soft();
                if(r < p) {
                    leave();
                } else {
                    join();
                }
            } else if(state==WALK) {
                //TRANSITION
                //double p;
                uint8_t p;
                if(LINK==EVO_LINK) {
                    if(n==0) {
                        //p = 0.01;
                        p = 3; // approx 1%
                    } else {
                        //p = (1-c)+c*(1-exp(-a*n));
                        p = round(((1-c)+c*(1-exp(-a*n)))*255);
                    }
                } else {
                    //p = 0.03+c*(1-exp(-a*n));
                    p = round((0.03+c*(1-exp(-a*n)))*255);
                }
                // generates random number on 16 bit of accuracy
                //double r = ((rand_soft()<<8)+rand_soft())/65535.0;
                uint8_t r = rand_soft();
                if(r < p) {
                    join();
                } else {
                    leave();
                }
            }
        }
    }
}

message_t *message_tx() {
    if(broadcast)
        return &message;
    else
        return 0;
}

// get unique id to avoid duplicate messages
uint8_t idxOf(uint8_t* ids, uint8_t id, uint8_t ids_size) {
    uint8_t i;
    for(i=0; i<ids_size; ++i) {
        if(ids[i]==id)
            return i;
    }
    return i+1;
}

void message_rx(message_t *m, distance_measurement_t *d) {
    // if correct type and
    // not turning and
    // tthree seconds to next decision
    // and within 12cm
    if(m->type == WORD &&
            state!=TURN &&
            kilo_ticks>=(next_decision-99) &&
            estimate_distance(d) <= 120) {
        uint8_t idx = idxOf(ids,m->data[0],ids_size);
        struct Word word;
        word.aByte = m->data[1];
        word.bByte = m->data[2];
        word.cByte = m->data[3];
        msgs[idx]=word;
        ids[idx]=m->data[0];
        if(idx >= ids_size)
            ++ids_size;
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
