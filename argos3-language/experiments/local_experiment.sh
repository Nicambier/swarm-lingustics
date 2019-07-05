#!/bin/bash
    
FILE='aggregation_EE.argos'
FOLDER='weak_EmbEvo'
mkdir -p $FOLDER

for version in 'c'
do
    sed -e 's/library="..\/build\/controllers\/[^\"]*"/library="..\/build\/controllers\/footbot_aggregation_'"$version"'NG\/libfootbot_aggregation_'"$version"'NG.so"/' $FILE > tmp1
    sed -e 's/footbot_aggregation.*controller/footbot_aggregation_'"$version"'NG_controller/' tmp1 > tmp2
    
    for swarm in 25 50 100
    do
        sed -e 's/nBots=".*" out/nBots="'"$swarm"'" out/' tmp2 > tmp3
        if [ $swarm -eq 25 ] 
        then 
            sed -e 's/arena size="20, 20, 1"/arena size="20, 20, 1"/' tmp3 > tmp4
            #sed -e 's/aParam="1" bParam="2" leave/aParam="2.25" bParam="3.5" leave/' tmp4 > tmp5
        fi
        if [ $swarm -eq 50 ]
        then 
            sed -e 's/arena size="20, 20, 1"/arena size="20, 20, 1"/' tmp3 > tmp4
            #sed -e 's/aParam="1" bParam="2" leave/aParam="1.25" bParam="2" leave/' tmp4 > tmp5
        fi
        if [ $swarm -eq 100 ]
        then 
            sed -e 's/arena size="20, 20, 1"/arena size="20, 20, 1"/' tmp3 > tmp4
            #sed -e 's/aParam="1" bParam="2" leave/aParam="1.25" bParam="1.25" leave/' tmp4 > tmp5
        fi
        sleep 0.1 
        a=1
        for aParam in 1
        do
            sed -e 's/aParam="[^\"]*"/aParam="'"$aParam"'"/' tmp4 > tmpa
            for bParam in 1
            do
                sed -e 's/bParam="[^\"]*"/bParam="'"$bParam"'"/' tmpa > tmpb
                sleep 0.1 
                DIREC=$FOLDER/$version'NG_s'$swarm'_a'$a'_b'$b
                mkdir -p $DIREC
                for i in $(seq 1 1 20)
                do
                    sleep 1
                    sed -e 's/output=".*" words=".*" min/output="'"$FOLDER\/$version"'NG_s'"$swarm"'_a'"$a"'_b'"$b"'\/out_n1_'"$i"'" words="'"$FOLDER\/$version"'NG_s'"$swarm"'_a'"$a"'_b'"$b"'\/words_n1_'"$i"'" min/' tmpb > tmp.argos
                    sleep 1
                    while [ "$(pgrep -c argos3)" -gt 6 ] 
                    do
                        sleep 1
                    done
                    argos3 -c  tmp.argos &
                done
                b="$((b+1))"
            done
            a="$((a+1))"
        done
    done
done

sleep 2
rm -f tmp*
