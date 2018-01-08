#!/bin/bash
#$ -N NG_agg
#$ -cwd

TMPDIR=/tmp/NG_results/$1
JOBDIR=/home/vtrianni/swarm-lingustics/argos3-language/experiments

mkdir -p $TMPDIR
mv $1.argos $TMPDIR/tmp.argos

cd $TMPDIR
sh /home/vtrianni/Work/argos3/build_simulator/setup_env.sh

pr="$(nproc)"
pr="$((pr-2))"

for i in $(seq 1 1 20)
do
    sed -e 's/output="[^\"]*"/output="'"$2"'\/'"$i"'.txt"/'  tmp.argos > $1.argos
    
    /home/vtrianni/Work/argos3/build_simulator/core/argos3 -c tmp.argos &> /dev/null

    T="$(pgrep -c argos3)"
    while [ "$T" -gt "$pr" ]; 
    do
        sleep 10
        T="$(pgrep -c argos3)"
    done
done

while [ "$(pgrep -c argos3)" -gt 0 ]
do
    sleep 10
done

rm *.argos
mkdir -p $JOBDIR/results/$1
mv $TMPDIR/* $JOBDIR/results/$1/
cd $JOBDIR
    
