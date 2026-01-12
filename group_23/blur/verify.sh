#!/bin/bash

echo "NOTE: this script relies on the binaries blur and blur_par to exist"

status=0
red=$(tput setaf 1)
reset=$(tput sgr0)

for image in im1 im2 im3 im4
do
    ./blur 15 "data/$image.ppm" "./data_o/blur_${image}_par.ppm"

    if ! cmp -s "./data_o/${image}_seq.ppm" "./data_o/blur_${image}_par.ppm"
    then
        echo "${red}Error: Incongruent output data detected when blurring image $image.ppm ${reset}"
        status=1
    fi

    rm "./data_o/blur_${image}_par.ppm"
done

exit $status