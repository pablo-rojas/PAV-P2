#!/bin/bash

# Be sure that this file has execution permissions:
# Use the nautilus explorer or chmod +x run_vad.sh

# Write here the name and path of your program and database
DB=db.v4
alpha1=2
alpha2=2
nInit=10
nChange=8
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

exit 0
