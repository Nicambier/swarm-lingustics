#!/bin/bash

for i in 21
do
    mkdir results/"$i"/
    
    for version in 1 2 #1=NG 2=vanilla
    do
        case $version in
            1)
            V='NG'
            sed -e 's/library="..\/build\/controllers\/[^\"]*"/library="..\/build\/controllers\/footbot_aggregation_NG\/libfootbot_aggregation_NG.so"/' aggregation_auto.argos > tmp1
            sed -e 's/footbot_aggregation.*controller/footbot_aggregation_NG_controller/' tmp1 > tmp2
            ;;
            2)
            V='V'
            sed -e 's/library="..\/build\/controllers\/[^\"]*"/library="..\/build\/controllers\/footbot_aggregation\/libfootbot_aggregation.so"/' aggregation_auto.argos > tmp1
            sed -e 's/footbot_aggregation.*controller/footbot_aggregation_controller/' tmp1 > tmp2
            ;;
        esac
        
        for swarm in 20 100
        do
            cat tmp2 > tmp3
            
            for arena in 1
            do
                sed -e 's/<entity quantity=".*" max_trials="100">/<entity quantity="'"$swarm"'" max_trials="100">/' tmp3 > tmp4
                
                for proba in 14 17 20
                do
                    sed -e 's/baseProba="[^\"]*"/baseProba="0.'"$proba"'"/' tmp4 > tmp5
                    
                    sed -e 's/output="[^\"]*"/output="results\/'"$i"'\/'"$V"'_s'"$swarm"'_a'"$arena"'_p'"$proba"'.txt"/'  tmp5 > aggregation_"$V"_s"$swarm"_a"$arena"_p"$proba".argos
                    argos3 -c aggregation_"$V"_s"$swarm"_a"$arena"_p"$proba".argos &
        
                    T="$(pgrep -c argos3)"
                    while [ "$T" -gt 5 ]; do
                        sleep 10
                        T="$(pgrep -c argos3)"
                    done
                done
            done
        done
    done
    if [ "$(pgrep -c argos3)" -lt 1 ]
    then 
        rm aggregation_*_s*_a*_p*.argos
        rm tmp*
    fi
    
done