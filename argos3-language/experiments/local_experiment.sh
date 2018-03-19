#!/bin/bash
    
FILE='aggregation_EE.argos'
FOLDER='EmbEvo_random'
mkdir $FOLDER
    
for swarm in 100
do
    sed -e 's/<entity quantity=".*" max_trials="100">/<entity quantity="'"$swarm"'" max_trials="100">/' $FILE > tmp1
    sleep 0.1 
    for ntype in 'shannon'
    do
        mkdir $FOLDER/$ntype
        n=1
        for noise in 0.008
        do
            sed -e 's/noise=".*" ntype=".*" fitness/noise="'"$noise"'" ntype="'"$ntype"'" fitness/' tmp1 > tmp2
            sleep 0.1 
            for i in $(seq 1 1 20)
            do
                a="$( (seq 1.5 0.25 5) | shuf -n1)"
                b="$( (seq 1.5 0.25 5) | shuf -n1)"
                sed -e 's/aParam=".*" bParam=".*" leave/aParam="'"$a"'" bParam="'"$b"'" leave/' tmp2 > tmp3
                echo $a
                echo $b
                sleep 0.1
                sed -e 's/output=".*" words=".*" min/output="'"$FOLDER\/$ntype"'\/out_n'"$n"'_'"$i"'" words="'"$FOLDER\/$ntype"'\/words_n'"$n"'_'"$i"'" min/' tmp2 > tmp.argos
                sleep 1
                while [ "$(pgrep -c argos3)" -gt 6 ] 
                do
                    sleep 1
                done
                argos3 -c  tmp.argos &
            done
            n="$((n+1))"
        done
    done
done

sleep 2
rm -f tmp*