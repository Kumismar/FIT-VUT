#!/bin/bash


# Configuration for the experiment
TIMEOUT_DURATION="900s"
TIMEOUT_SECONDS=900
MAX_SIZE=20

run_cbmc_single() {
    local size=$1
    local extra_flags=$2
    local output_file=$3
    
    UNWIND_VAL=$(echo "$size + 1" | bc -l)

    # Clean up temporary files
    rm -f temp.dimacs
    
    start_gen=$(date +%s.%N)
    
    # Run CBMC to generate DIMACS (SAT formula) statistics
    CMD_GEN="timeout $TIMEOUT_DURATION cbmc merge_sort.c -DSIZE=$size $extra_flags --unwind $UNWIND_VAL --dimacs"
    echo "Executing (DIMACS): $CMD_GEN"
    timeout $TIMEOUT_DURATION cbmc merge_sort.c -DSIZE=$size $extra_flags \
         --unwind $UNWIND_VAL --dimacs > temp.dimacs 2>/dev/null
    
    gen_exit_code=$?
    end_gen=$(date +%s.%N)
    
    # Calculate approximate generation duration
    gen_time=$(echo "$end_gen - $start_gen" | bc)
    
    vars=0
    clauses=0
    dimacs_status="OK"

    if [ $gen_exit_code -eq 124 ]; then
        dimacs_status="TIMEOUT"
        gen_time=$TIMEOUT_SECONDS
    else
        read vars clauses <<< $(grep "^p cnf" temp.dimacs | awk '{print $3 " " $4}')
        if [ -z "$vars" ]; then vars=0; clauses=0; fi
    fi

    if [ "$dimacs_status" == "TIMEOUT" ]; then
        echo "$size,$TIMEOUT_SECONDS,$TIMEOUT_SECONDS,0,0,TIMEOUT" >> $output_file
        return
    fi

    start_total=$(date +%s.%N)
    
    # Run CBMC for actual verification
    CMD_VERIFY="timeout $TIMEOUT_DURATION cbmc merge_sort.c -DSIZE=$size $extra_flags --unwind $UNWIND_VAL"
    echo "Executing (Verify): $CMD_VERIFY"
    cbmc_output=$(timeout $TIMEOUT_DURATION cbmc merge_sort.c -DSIZE=$size $extra_flags --unwind $UNWIND_VAL 2>&1)
    cbmc_exit_code=$?
    
    end_total=$(date +%s.%N)
    total_time=$(echo "$end_total - $start_total" | bc)

    result="UNKNOWN"
    
    # Analyze the result code and output
    if [ $cbmc_exit_code -eq 124 ]; then
        result="TIMEOUT"
        total_time=$TIMEOUT_SECONDS
    elif echo "$cbmc_output" | grep -q "VERIFICATION SUCCESSFUL"; then
        result="SUCCESS"
    elif echo "$cbmc_output" | grep -q "VERIFICATION FAILED"; then
        result="FAILURE"
    else
        result="ERROR"
    fi

    echo "  -> Size $size: $result (Total: ${total_time}s, Gen: ${gen_time}s, Clauses: $clauses)"

    echo "$size,$total_time,$gen_time,$vars,$clauses,$result" >> $output_file
    
    rm -f temp.dimacs
}

run_batch() {
    local name=$1
    local filename=$2
    local flags=$3
    
    echo "Starting Batch: $name -> $filename"
    echo "Size,TotalTime,GenTime,Variables,Clauses,Result" > $filename

    for ((s=2; s<=MAX_SIZE; s++)); do
        run_cbmc_single $s "$flags" "$filename"
    done
}

# run_batch "Correct Version" "results_correct.csv" ""
# run_batch "Condition Bug" "results_cond.csv" "-DBUG_CONDITION"
run_batch "Missing Rest Copy" "results_missing_copy.csv" "-DBUG_MISSING_REST_COPY"