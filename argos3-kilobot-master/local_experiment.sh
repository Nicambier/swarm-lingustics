#!/bin/bash
    
FILE='src/language/experiments/kilobot_naming_game.argos'
FOLDER='new_comp'

VERSIONS="0 2"
SIZES="25 50 100"
ITER="20"

#THESE WILL NOT APPLY FOR VERSIONS=2
A_PARAM="1.75"
B_PARAM="4"

#THESE WILL ONLY APPLY FOR VERSIONS=2
MUTATION="0.001"

mkdir -p $FOLDER

echo "VERSIONS $VERSIONS" > $FOLDER/params.txt
echo "SIZES $SIZES" >> $FOLDER/params.txt
echo "A PARAMETERS $A_PARAM" >> $FOLDER/params.txt
echo "B_PARAMETERS $B_PARAM" >> $FOLDER/params.txt
echo "MUTATIONS $MUTATION" >> $FOLDER/params.txt
echo "ITER $ITER" >> $FOLDER/params.txt

sed -e 's/<visualization>/<!--visualization>/' $FILE > tmp1
sed -i 's/<\/visualization>/<\/visualization-->/' ./tmp1

for version in $VERSIONS
do
    sed -e 's/link="[^\"]*" mutation/link="'"$version"'" mutation/' tmp1 > tmp2
    
    for swarm in $SIZES
    do
        sed -e 's/nBots=".*" out/nBots="'"$swarm"'" out/' tmp2 > tmp3
        if [ $swarm -eq 25 ] 
        then 
            sed -e 's/arena size="1, 1, 1"/arena size="1, 1, 1"/' tmp3 > tmp4
            #sed -e 's/aParam="1" bParam="2" leave/aParam="2.25" bParam="3.5" leave/' tmp4 > tmp5
        elif [ $swarm -eq 50 ]
        then 
            sed -e 's/arena size="1, 1, 1"/arena size="1.4, 1.4, 1"/' tmp3 > tmp4
            #sed -e 's/aParam="1" bParam="2" leave/aParam="1.25" bParam="2" leave/' tmp4 > tmp5
        elif [ $swarm -eq 100 ]
        then 
            sed -e 's/arena size="1, 1, 1"/arena size="2, 2, 1"/' tmp3 > tmp4
            #sed -e 's/aParam="1" bParam="2" leave/aParam="1.25" bParam="1.25" leave/' tmp4 > tmp5
        fi
        sleep 0.1 
        
        if [ $version -eq 2 ]
        then
            m=1
            for mut in $MUTATION
            do
                sed -i 's/mutation="[^\"]*"/mutation="'"$mut"'"/' ./tmpb
                DIREC=$version'NG_s'$swarm'_m'$m
                mkdir -p $FOLDER/$DIREC
                for i in $(seq 1 1 $ITER)
                do
                    sleep 0.5
                    sed -e 's/output=".*" aParam/output="'"$FOLDER\/$DIREC"'\/out_'"$i"'" aParam/' tmpb > tmp.argos
                    sleep 0.5
                    while [ "$(pgrep -c argos3)" -gt 6 ] 
                    do
                        sleep 0.5
                    done
                    argos3 -c  tmp.argos &
                done
            done        
        else
            for aParam in $A_PARAM
            do
                sed -e 's/aParam="[^\"]*"/aParam="'"$aParam"'"/' tmp4 > tmpa
                a=$(echo $aParam*100/1 | bc)
                for bParam in $B_PARAM
                do
                    b=$(echo $bParam*100/1 | bc)
                    sed -e 's/bParam="[^\"]*"/bParam="'"$bParam"'"/' tmpa > tmpb
                    sleep 0.1 
                    
                    DIREC=$version'NG_s'$swarm'_a'$a'_b'$b
                    mkdir -p $FOLDER/$DIREC
                    for i in $(seq 1 1 $ITER)
                    do
                        sleep 0.5 
                        sed -e 's/output=".*" aParam/output="'"$FOLDER\/$DIREC"'\/out_'"$i"'" aParam/' tmpb > tmp.argos
                        sleep 0.5
                        while [ "$(pgrep -c argos3)" -gt 6 ] 
                        do
                            sleep 0.5
                        done
                        argos3 -c  tmp.argos &
                    done
                    m="$((m+1))"
                done
            done
        fi
    done
done

while [ "$(pgrep -c argos3)" -gt 0 ] 
do
    sleep 1
done
rm -f tmp*
