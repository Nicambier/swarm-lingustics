#!/bin/bash

cd results/
for i in 1 2 3 4 5 6 7 8 9 10
do
    mkdir "$i"/
    cd "$i"/
    
    for swarm in 20 100
    do
        for arena in 2
        do
            for proba in 14 17 20
            do
                argos3 -c ../../aggregation_NG_s"$swarm"_a"$arena"_p"$proba".argos &
    
                T="$(pgrep -c argos3)"
                while [ "$T" -gt 6 ]; do
                    sleep 10
                    T="$(pgrep -c argos3)"
                done
            done
        done
    done    
    
    cd ../
done
cd ../