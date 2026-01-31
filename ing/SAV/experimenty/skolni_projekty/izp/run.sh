#!/bin/bash

TIMEOUT_DURATION="900s"
TIMEOUT_SECONDS=900
# Zkusíme jít až do 8, ale očekáváme, že 7 a 8 možná skončí timeoutem
MAX_SIZE=8

run_cbmc_single() {
    local size=$1
    local extra_flags=$2
    local output_file=$3
    
    
    UNWIND_VAL=$((size * 2 + 2))

    rm -f temp.dimacs
    
    # 1. Generovani formule (DIMACS)
    CMD_FLAGS="-DCBMC_VERIFICATION -DTEST_SET_SIZE=$size $extra_flags"
    
    echo "Processing Size $size (Flags: $extra_flags)..."

    start_gen=$(date +%s.%N)
    timeout $TIMEOUT_DURATION cbmc setcal.c $CMD_FLAGS \
         --unwind $UNWIND_VAL --dimacs > temp.dimacs 2>/dev/null
    
    gen_exit_code=$?
    end_gen=$(date +%s.%N)
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
        echo "  -> Timeout during generation."
        return
    fi

    # 2. Reseni (Verification)
    start_total=$(date +%s.%N)
    
    # Pro korektni verifikaci potrebujeme memory checky
    cbmc_output=$(timeout $TIMEOUT_DURATION cbmc setcal.c $CMD_FLAGS \
                  --unwind $UNWIND_VAL --pointer-check --bounds-check --memory-leak-check 2>&1)
    
    cbmc_exit_code=$?
    end_total=$(date +%s.%N)
    total_time=$(echo "$end_total - $start_total" | bc)

    result="UNKNOWN"
    
    if [ $cbmc_exit_code -eq 124 ]; then
        result="TIMEOUT"
        total_time=$TIMEOUT_SECONDS
    elif echo "$cbmc_output" | grep -q "VERIFICATION SUCCESSFUL"; then
        result="SUCCESS"
    elif echo "$cbmc_output" | grep -q "VERIFICATION FAILED"; then
        result="FAILURE"
    else
        # Pokud dojde k unwind assertion error, zaznamenáme to jako ERROR
        if echo "$cbmc_output" | grep -q "unwinding assertion"; then
             result="UNWIND_ERR"
        else
             result="ERROR"
        fi
    fi

    echo "  -> Result: $result (Total: ${total_time}s, Clauses: $clauses)"
    echo "$size,$total_time,$gen_time,$vars,$clauses,$result" >> $output_file
    
    rm -f temp.dimacs
}

run_batch() {
    local name=$1
    local filename=$2
    local flags=$3
    
    echo "=========================================="
    echo "Batch: $name -> $filename"
    echo "Size,TotalTime,GenTime,Variables,Clauses,Result" > $filename

    for ((s=2; s<=MAX_SIZE; s++)); do
        run_cbmc_single $s "$flags" "$filename"
    done
}

# --- SPUSTENI EXPERIMENTU ---

# run_batch "Set Union" "results_set_union.csv" "-DTEST_CASE_UNION"

run_batch "Set Intersect" "results_set_intersect.csv" "-DTEST_CASE_INTERSECT"

run_batch "Symmetric Closure" "results_closure_sym.csv" "-DTEST_CASE_CLOSURE_SYM"

run_batch "Relation Domain" "results_rel_domain.csv" "-DTEST_CASE_DOMAIN"

echo "Hotovo."