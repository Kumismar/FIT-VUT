#!/bin/bash
PYTHON_SCRIPT="arrow_sat.py"
OUTPUT_CSV="results.csv"

echo "N,M,Mode,MissingAxioms,Profiles,GenTime_s,Clauses,Result,SolveTime_s,Dictator" > $OUTPUT_CSV

declare -a experiments=(
"2 3" 
"3 3" 
"2 4"
"4 3"
"5 3"
)

run_single() {
    local n=$1
    local m=$2
    local mode_label=$3
    local flags=$4
    local missing_axioms=$5
    
    echo -n "Running N=$n M=$m [$mode_label] ... "
    
    cmd="python3 $PYTHON_SCRIPT --voters $n --alternatives $m $flags"
    
    output=$($cmd)
    
    profiles=$(echo "$output" | grep "Number of profiles:" | awk '{print $4}')
    gen_time=$(echo "$output" | grep "Generation time" | awk '{print $5}')
    clauses=$(echo "$output" | grep "Number of clauses:" | awk '{print $4}')
    result=$(echo "$output" | grep "Result:" | awk '{print $2}')
    solve_time=$(echo "$output" | grep "Solving time:" | awk '{print $3}')
    dictator=$(echo "$output" | grep "Dictator:" | awk '{print $3}')
    
    # If dictator is empty, use dash for CSV
    if [ -z "$dictator" ]; then
        dictator="-"
    fi
    
    echo "$n,$m,$mode_label,$missing_axioms,$profiles,$gen_time,$clauses,$result,$solve_time,$dictator" >> $OUTPUT_CSV
    echo "Done"
}

for exp in "${experiments[@]}"; do
    set -- $exp
    n=$1
    m=$2
    
    run_single $n $m "FullTheorem" "" "None"
    
    run_single $n $m "NoPareto" "--no-pareto" "Pareto"
    
    run_single $n $m "NoIIA" "--no-iia" "IIA"
    
    run_single $n $m "NoDictator" "--no-dictator" "NonDict"
done

echo "=== DONE ==="
echo "Results saved to $OUTPUT_CSV"