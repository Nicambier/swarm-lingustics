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
        for aParam in 0.2 1.4 2.6
        do
            sed -e 's/aParam="[^\"]*"/aParam="'"$aParam"'"/' tmp3 > tmp4
            
            b=1
            for bParam in 1.2 2.0 2.4
            do
                sed -e 's/bParam="[^\"]*"/bParam="'"$bParam"'"/' tmp4 > "$version"NG_s"$swarm"_a"$a"_b"$b".argos
                
                for i in $(seq 1 1 30)
                do
                    qsub run.sh "$version"NG_s"$swarm"_a"$a"_b"$b" $i
                done
                                
                b="$((b+1))"
            done
        a="$((a+1))"
        done
    done
done

#rm -f tmp*