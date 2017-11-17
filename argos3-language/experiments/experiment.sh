#!/bin/bash
mkdir /tmp/vtrianni/swarm_results
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
                
                mkdir /tmp/vtrianni/swarm_results/"$version"NG_s"$swarm"_a"$a"_b"$b"
                
                for i in $(seq 1 1 50)
                do
                    sed -e 's/output="[^\"]*"/output="/tmp/vtrianni/swarm_results\/'"$version"'NG_s'"$swarm"'_a'"$a"'_b'"$b"'\/'"$i"'.txt"/'  tmp5 > tmp.argos
                    qsub run.sh -N "$version"NG_s"$swarm"_a"$a"_b"$b"_"$i"
        
                    #T="$(pgrep -c argos3)"
                    #while [ "$T" -gt "$pr" ]; 
                    #do
                    #    sleep 10
                    #    T="$(pgrep -c argos3)"
                    #done
                done                
                b="$((b+1))"
            done
        a="$((a+1))"
        done
    done
done

#if [ "$(pgrep -c argos3)" -lt 1 ]
#then 
#    cp -rf /tmp/swarm_results results/
#    echo "done"
#    rm tmp*
#fi