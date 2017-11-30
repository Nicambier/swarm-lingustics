#!/bin/bash
#$ -cwd

TMPDIR=/tmp/NG_results/$2
JOBDIR=/home/vtrianni/swarm-lingustics/argos3-language/experiments

cd $TMPDIR
pr="$(nproc)"
pr="$((pr-2))"

for i in $(seq 1 1 1)
do
    sed -e 's/output="[^\"]*"/output="'"$2"'\/'"$i"'.txt"/'  tmp > tmp.argos
    argos3 -c tmp.argos &> /dev/null

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

mkdir $JOBDIR/results/$2
mv * $JOBDIR/results/$2/
rm -f *
cd $JOBDIR
    
