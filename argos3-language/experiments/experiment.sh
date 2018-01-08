#!/bin/bash
mkdir results
    
FILE='aggregation_auto.argos'

for version in 'n' 'w' 's'
do
    sed -e 's/library="..\/build\/controllers\/[^\"]*"/library="..\/build\/controllers\/footbot_aggregation_'"$version"'NG\/libfootbot_aggregation_'"$version"'NG.so"/' $FILE > tmp1
    sed -e 's/footbot_aggregation.*controller/footbot_aggregation_'"$version"'NG_controller/' tmp1 > tmp2
    
    for swarm in 30 100 200
    do
        sed -e 's/<entity quantity=".*" max_trials="100">/<entity quantity="'"$swarm"'" max_trials="100">/' tmp2 > tmp3
    
        a=1
        for aParam in $(seq 0.2 0.4 3)
        do
            sed -e 's/aParam="[^\"]*"/aParam="'"$aParam"'"/' tmp3 > tmp4
            
            b=1
            for bParam in $(seq 0.2 0.4 3)
            do
                sed -e 's/bParam="[^\"]*"/bParam="'"$bParam"'"/' tmp4 > $version"NG_s"$swarm"_a"$a"_b"$b".argos
                qsub run.sh -N "$version"NG_s"$swarm"_a"$a"_b"$b"
                                
                b="$((b+1))"
            done
        a="$((a+1))"
        done
    done
done

rm -f tmp*