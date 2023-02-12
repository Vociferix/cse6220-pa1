#!/bin/bash

make
rm -f results.csv
touch results.csv
qsub -l nodes=1:ppn=24 ./run.pbs -o /dev/null
watch cat results.csv
