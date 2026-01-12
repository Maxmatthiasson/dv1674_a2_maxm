#!/bin/bash

echo "Running pearson_par on all datasets with 1, 2, 4, 8, 16, 32 threads."

# Dataset files
DATASETS=("128.data" "256.data" "512.data" "1024.data")
THREADS=(1 2 4 8 16 32)

echo "dataset,threads,time_sec,max_rss_kb,cpu_util_percent"

for ds in "${DATASETS[@]}"; do
    for t in "${THREADS[@]}"; do

        # Runs pearson_par and captures time info
        /usr/bin/time -v ./pearson_par "data/$ds" out.data $t 2> tmp_time.txt

        # Extracts wall-clock time in seconds
        TIME=$(grep "Elapsed (wall clock) time" tmp_time.txt | awk '{print $8}')

        # Extracts maximum resident set size in KB
        RSS=$(grep "Maximum resident set size" tmp_time.txt | awk '{print $6}')

        # Extracts CPU utilization and removes the percent sign
        CPU=$(grep "Percent of CPU this job got" tmp_time.txt | awk -F: '{gsub(/%/,"",$2); print $2}')

        echo "$ds,$t,$TIME,$RSS,$CPU"
    done
done

# Cleanup
rm -f tmp_time.txt
