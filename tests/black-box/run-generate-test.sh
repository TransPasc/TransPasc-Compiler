#!/bin/bash

# Initialization
echo "Current directory:"
pwd
echo "Starting test sequence..."

# Directory configuration
output_dir="./tmp"
log_dir="$output_dir/log"
failure_dir="$output_dir/failures"
mkdir -p "$output_dir" "$log_dir" "$failure_dir"
rm -rf "$failure_dir"/* 2>/dev/null

# Global counters
success_count=0
failure_count=0

# Logging functions
log_error() {
    local timestamp=$(date "+%Y-%m-%d %H:%M:%S")
    echo -e "[$timestamp] $1" >> "$log_dir/error.log"
    echo -e "\033[31m$1\033[0m" # Red text for errors
}

log_info() {
    local timestamp=$(date "+%Y-%m-%d %H:%M:%S")
    echo -e "[$timestamp] $1" >> "$log_dir/runtime.log"
}

# Cleanup temporary files (preserve failure records)
cleanup() {
    rm -f "$output_dir/fpc.out" \
          "$output_dir/$base_name.c" \
          "$output_dir/res.out" \
          "$output_dir"/{ans,res} \
          "$output_dir"/{fpc,kpc,gcc}_error.log 2>/dev/null
}

# Main test loop
while IFS= read -r pas_file; do
    base_name=$(basename "$pas_file" .pas)
    dir_name=$(dirname "$pas_file")
    input_file="$dir_name/$base_name.in"
    test_case_dir="$failure_dir/$base_name"

    log_info "Processing test case: $base_name"

    # Compile with FPC and capture errors
    if ! fpc "$pas_file" -o"$output_dir/fpc.out" 2>"$output_dir/fpc_error.log"; then
        mkdir -p "$test_case_dir"
        cp "$output_dir/fpc_error.log" "$test_case_dir/fpc_compile.log"
        log_error "FPC compilation failed: $pas_file (see $test_case_dir/fpc_compile.log)"
        ((failure_count++))
        cleanup
        continue
    fi

    # Convert to C with KPC and capture errors
    if ! ./kpc -i "$pas_file" -o "$output_dir/$base_name.c" 2>"$output_dir/kpc_error.log"; then
        mkdir -p "$test_case_dir"
        cp "$output_dir/kpc_error.log" "$test_case_dir/kpc_compile.log"
        log_error "KPC conversion failed: $pas_file (see $test_case_dir/kpc_compile.log)"
        ((failure_count++))
        cleanup
        continue
    fi

    # Compile C code with GCC and capture errors
    if ! gcc "$output_dir/$base_name.c" -o "$output_dir/res.out" 2>"$output_dir/gcc_error.log"; then
        mkdir -p "$test_case_dir"
        cp "$output_dir/gcc_error.log" "$test_case_dir/gcc_compile.log"
        log_error "GCC compilation failed: $base_name.c (see $test_case_dir/gcc_compile.log)"
        ((failure_count++))
        cleanup
        continue
    fi

    # Execute test with/without input
    if [[ -f "$input_file" ]]; then
        log_info "Using input file: $input_file"
        timeout 5s "$output_dir/fpc.out" < "$input_file" > "$output_dir/ans" 2>>"$log_dir/runtime.log"
        timeout 5s "$output_dir/res.out" < "$input_file" > "$output_dir/res" 2>>"$log_dir/runtime.log"
    else
        log_info "No input file detected, executing directly"
        timeout 5s "$output_dir/fpc.out" > "$output_dir/ans" 2>>"$log_dir/runtime.log"
        timeout 5s "$output_dir/res.out" > "$output_dir/res" 2>>"$log_dir/runtime.log"
    fi

    # Result comparison and failure recording
    if diff -q "$output_dir/ans" "$output_dir/res" &>/dev/null; then
        ((success_count++))
        log_info "Test passed: $base_name"
    else
        mkdir -p "$test_case_dir"
        cp "$output_dir/ans" "$test_case_dir/expected_output"
        cp "$output_dir/res" "$test_case_dir/actual_output"
        diff -u "$output_dir/ans" "$output_dir/res" > "$test_case_dir/diff.diff"
        log_error "Result mismatch: $base_name (details in $test_case_dir/)"
        [[ -f "$input_file" ]] && cp "$input_file" "$test_case_dir/test_input"
        ((failure_count++))
    fi

    cleanup
done < <(find ../tests/black-box/generate -name "*.pas")

# Final report
echo -e "\n=============================================="
echo -e "Test summary:"
echo -e "  Successful tests: \033[32m$success_count\033[0m"
echo -e "  Failed tests:     \033[31m$failure_count\033[0m"
echo -e "Debug artifacts:"
echo -e "  Failure details:  file://$(realpath "$failure_dir")"
echo -e "  System logs:      file://$(realpath "$log_dir")"
echo -e "=============================================="

exit $((failure_count > 0 ? 1 : 0))
