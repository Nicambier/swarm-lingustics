#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define MANTIS 4

uint8_t double_to_uint8(double d) 
{
    uint8_t dCode = 0;
    double temp = d;
    int i = 0, j = -1; //we don't encode the first bit
    uint8_t encoding = 0;
    while(j<MANTIS)
    {
        if(temp >= pow(2,-(1+i))) {
            if(encoding)
                dCode += pow(2, 7-j);
            else
                encoding = 1;
            temp -= pow(2,-(1+i));
        }
        ++i;
        if(encoding)
            ++j;
    }
    if(i-(j+1) <= pow(2,8-MANTIS)-1)
        dCode += i-(j+1); //mantis + exp
    else
        dCode = pow(2,8-MANTIS)-1; //this is the smallest value we can represent
    return dCode;
}

double uint8_to_double(uint8_t u) 
{    
    short int i;
    int exp = u%(int)pow(2,8-MANTIS);
    double m = pow(2,-(1+exp));
    for(i=0; i<MANTIS; ++i)
        if((u & (uint8_t) pow(2,7-i)) > 0)
            m+= pow(2,-(2+i+exp));
    return m;
}

void display_uint8(uint8_t u)
{
    short int i;
    for(i=0; i<8; ++i) {
        if((u & (uint8_t) pow(2,7-i)) > 0)
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

/*int main() {
    double m = 1;
    printf("%f \n",m);
    uint8_t mutation = double_to_uint8(m);
    display_uint8(mutation);
    printf("%f \n",uint8_to_double(mutation));    
}*/
    
