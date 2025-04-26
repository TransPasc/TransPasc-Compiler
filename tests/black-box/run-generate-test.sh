#!/bin/bash
set -euo pipefail

# 参数解析
if [[ $# -ne 2 ]]; then
    echo "Usage: $0 test_path output_path"
    exit 1
fi

TEST_PATH="$1"
OUTPUT_PATH="$2"
FAILED_LOG_DIR="$OUTPUT_PATH/failed/log"
TIME_OUT=3

# 初始化目录
mkdir -p "$FAILED_LOG_DIR" "$OUTPUT_PATH"

# 收集测试用例
mapfile -t CASES < <(find "$TEST_PATH" -name '*.pas' -exec sh -c '
    for pas; do
        name=$(basename "$pas" .pas)
        in_file=$(dirname "$pas")/${name}.in
        [ -f "$in_file" ] || in_file="/dev/null"
        echo "${name}:${pas}:${in_file}"
    done
' sh {} +)

echo "找到 ${#CASES[@]} 个测试用例"

# 单个测试用例处理函数
process_case() {
    local IFS=":"
    local name pas_path in_path
    read -r name pas_path in_path <<< "$1"
    local status="success" log=""

    # 定义路径
    fpc_dir="$OUTPUT_PATH/fpc_tmp-$name"
    mkdir -p "$fpc_dir"
    executable="$fpc_dir/$name"
    c_source="$OUTPUT_PATH/$name.c"
    cc_out="$OUTPUT_PATH/$name.gcc_out"
    ans_file="$OUTPUT_PATH/$name.ans"
    out_file="$OUTPUT_PATH/$name.out"

    # 编译Pascal
    if ! fpc "$pas_path" -FE"$fpc_dir" > "$FAILED_LOG_DIR/${name}_fpc.log" 2>&1 1>/dev/null; then
        log="Pascal编译失败"
        status="failed"
    # 生成C代码
    elif ! ./build/kpc -i "$pas_path" -o "$c_source" > "$FAILED_LOG_DIR/${name}_kpc.log" 2>&1 1>/dev/null; then
        log="C代码生成失败"
        status="failed"
    # 编译C
    elif ! timeout "$TIME_OUT" cc "$c_source" -o "$cc_out" > "$FAILED_LOG_DIR/${name}_cc.log" 2>&1 1>/dev/null; then
        log="C编译失败"
        status="failed"
    else
        # 运行并比较输出
        if ! "$executable" < "$in_path" > "$ans_file" 2>"$FAILED_LOG_DIR/${name}_pas_run.log"; then
            log="Pascal运行错误"
            status="failed"
        elif ! timeout "$TIME_OUT" "$cc_out" < "$in_path" > "$out_file" 2>"$FAILED_LOG_DIR/${name}_c_run.log"; then
            log="C运行错误"
            status="failed"
        elif ! diff -q "$ans_file" "$out_file" >/dev/null; then
            log="输出不一致"
            status="failed"
            diff "$ans_file" "$out_file" > "$FAILED_LOG_DIR/${name}_diff.log" 2>&1
        fi
    fi

    # 写入状态文件
    echo "name:$name|status:$status|log:$log" > "$OUTPUT_PATH/${name}.status"
}

# 并发执行
pids=()
for case in "${CASES[@]}"; do
    process_case "$case" &
    pids+=($!)
    echo "正在处理测试用例：$case"
done

# 等待所有进程
wait "${pids[@]}" >/dev/null 2>&1

# 统计结果
success=0
failed=0
for case in "${CASES[@]}"; do
    IFS=":" read -r name pas_path in_path <<< "$case"
    status_file="$OUTPUT_PATH/${name}.status"
    if [[ -f "$status_file" ]]; then
        if grep -q "status:success" "$status_file"; then
            ((success++))
        else
            ((failed++))
        fi
    else
        ((failed++))
        echo "警告：状态文件未找到：$status_file"
    fi
done

echo "统计结果: 成功 $success, 失败 $failed"
