#!/bin/bash

# Be sure that this file has execution permissions:
# Use the nautilus explorer or chmod +x run_vad.sh

# Write here the name and path of your program and database
DB=db.v4

for nInit in {11..12}; do
    for nChange in {8..13}; do
        for alpha1 in $(seq 2.0 0.1 8.0); do
            for alpha2 in $(seq 4.0 0.1 8.0); do

                echo "alpha1="$alpha1$" alpha2="$alpha2$" nInit="$nInit$" nChange"$nChange

                CMD="bin/vad --alpha1="$alpha1" --alpha2="$alpha2" --nInit="$nInit" --nChange="$nChange

                for filewav in $DB/*/*wav; do
                #    echo
                    echo "**************** $filewav ****************"
                    if [[ ! -f $filewav ]]; then 
                        echo "Wav file not found: $filewav" >&2
                        exit 1
                    fi

                    filevad=${filewav/.wav/.vad}

                    $CMD -i $filewav -o $filevad || exit 1

                # Alternatively, uncomment to create output wave files
                #    filewavOut=${filewav/.wav/.vad.wav}
                #    $CMD $filewav $filevad $filewavOut || exit 1

                done
                echo $alpha1$'\t'$alpha2$'\t'$nInit$'\t'$nChange  >> total.txt
                scripts/vad_evaluation.pl $DB/*/*lab > evaluation.txt
                tail -n 2 evaluation.txt | awk '{ print $3 }' >> total.txt
            done
        done
    done
done
exit 0
