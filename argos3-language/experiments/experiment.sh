#!/bin/bash
mkdir -p /tmp/NG_results
mkdir results

pr="$(nproc)"
pr="$((pr-1))"
    
FILE='aggregation_auto.argos'

for version in 'n' 'w' 's'
do
    sed -e 's/library="..\/build\/controllers\/[^\"]*"/library="..\/build\/controllers\/footbot_aggregation_'"$version"'NG\/libfootbot_aggregation_'"$version"'NG.so"/' $FILE > tmp1
    sed -e 's/footbot_aggregation.*controller/footbot_aggregation_'"$version"'NG_controller/' tmp1 > tmp2
    
    for swarm in 30 100 200
    do
        sed -e 's/<entity quantity=".*" max_trials="100">/<entity quantity="'"$swarm"'" max_trials="100">/' tmp2 > tmp3
    
        a=1
        for aParam in 4.6 2.3 1.5 1.15 0.9 0.75 0.65
        do
            sed -e 's/aParam="[^\"]*"/aParam="'"$aParam"'"/' tmp3 > tmp4
            
            b=1
            for bParam in 4.6 2.3 1.5 1.15 0.9 0.75 0.65
            do
                sed -e 's/bParam="[^\"]*"/bParam="'"$bParam"'"/' tmp4 > tmp5
                
                mkdir /tmp/NG_results/"$version"NG_s"$swarm"_a"$a"_b"$b"
                cp tmp5 /tmp/NG_results/"$version"NG_s"$swarm"_a"$a"_b"$b"/tmp
                qsub run.sh -N "$version"NG_s"$swarm"_a"$a"_b"$b"
                done                
                b="$((b+1))"
            done
        a="$((a+1))"
        done
    done
done