#!/bin/bash

echo "Running blur_par algorithm on all images with 1, 2, 4, 8, 16, 32 threads."

# Parameters
RADIUS=15
IMAGES=("im1.ppm" "im2.ppm" "im3.ppm" "im4.ppm")
THREADS=(1 2 4 8 16 32)

echo "image,threads,time_sec,max_rss_kb,cpu_util_percent"

# Loops over images and thread counts
for img in "${IMAGES[@]}"; do
    for t in "${THREADS[@]}"; do

        # Runs blur_par and captures /usr/bin/time output
        /usr/bin/time -v ./blur_par $RADIUS "data/$img" out.ppm $t 2> tmp_time.txt

        # Extracts wall-clock time in seconds
        TIME=$(grep "Elapsed (wall clock) time" tmp_time.txt | awk '{print $8}')

        # Extracts maximum resident set size in KB
        RSS=$(grep "Maximum resident set size" tmp_time.txt | awk '{print $6}')

        # Extracts CPU utilization and removes the percent sign
        CPU=$(grep "Percent of CPU this job got" tmp_time.txt | awk -F: '{gsub(/%/,"",$2); print $2}')

        echo "$img,$t,$TIME,$RSS,$CPU"
    done
done

# Clean up temporary file
rm -f tmp_time.txt
