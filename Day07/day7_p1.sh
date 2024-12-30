#!/usr/bin/bash

# set -xe
set -e

run() {
    ../build/day7_p1 program.txt <<< "${1}
${2}
"
}

max_signal=0

# File permutations.csv generated using:
# python3 -c "from itertools import permutations as p; print([item for item in p(range(5))])" > permutations.csv
# The file's contents were properly rearranged by hand.
while IFS=, read -r c1 c2 c3 c4 c5; do
    signal=0
    for p in $c1 $c2 $c3 $c4 $c5; do
        signal=$(run ${p} ${signal})
    done
    if [[ $max_signal -lt $signal ]]; then
        max_signal=$signal
    fi
done < permutations.csv

echo ${max_signal}
