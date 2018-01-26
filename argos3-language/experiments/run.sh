#!/bin/bash
#$ -N NG_agg
#$ -cwd

TMPDIR=/tmp/NG_results/$1
JOBDIR=/home/vtrianni/swarm-lingustics/argos3-language/experiments
I=$2

mkdir -p $TMPDIR
cp $1.argos $TMPDIR/tmp.argos

cd $TMPDIR
sh /home/vtrianni/Work/argos3/build_simulator/setup_env.sh

/home/vtrianni/Work/argos3/build_simulator/core/argos3 -c tmp.argos &> /dev/null

mkdir -p $JOBDIR/results/$1
mv $TMPDIR/res $JOBDIR/results/$1/$2
rm *
cd $JOBDIR
    
