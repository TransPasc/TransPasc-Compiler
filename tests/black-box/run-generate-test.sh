#!/bin/bash
echo "当前目录："
pwd
echo "开始测试..."

output_dir="./tmp"
mkdir -p "$output_dir"

success_count=0
failure_count=0

cleanup() {
  # 清理当前测试用例生成的所有临时文件
  rm -f "$output_dir/fpc.out" \
      "$output_dir/$base_name.c" \
      "$output_dir/res.out" \
      "$output_dir/ans" \
      "$output_dir/res" \
      "$output_dir/${base_name}.o" \
      "$output_dir/${base_name}.s" 2>/dev/null
}

# 使用进程替换确保在父shell中执行循环
while IFS= read -r pas_file; do
  base_name=$(basename "$pas_file" .pas)
  dir_name=$(dirname "$pas_file")
  input_file="$dir_name/$base_name.in"

  # FPC编译
  if ! fpc "$pas_file" -o"$output_dir/fpc.out" &>/dev/null; then
    echo "[31mFPC 编译失败: $pas_file[0m"
    ((failure_count++))
    cleanup
    continue
  fi

  # KPC编译
  if ! kpc "$pas_file" -o"$output_dir/$base_name.c" &>/dev/null; then
    echo "[31mKPC 编译失败: $pas_file[0m"
    ((failure_count++))
    cleanup
    continue
  fi

  # GCC编译
  if ! gcc "$output_dir/$base_name.c" -o "$output_dir/res.out" &>/dev/null; then
    echo "[31mGCC 编译失败: $output_dir/$base_name.c[0m"
    ((failure_count++))
    cleanup
    continue
  fi

  # 执行测试（带或不带输入）
  if [[ -f "$input_file" ]]; then
    timeout 5s "$output_dir/fpc.out" < "$input_file" > "$output_dir/ans" 2>/dev/null
    timeout 5s "$output_dir/res.out" < "$input_file" > "$output_dir/res" 2>/dev/null
  else
    timeout 5s "$output_dir/fpc.out" > "$output_dir/ans" 2>/dev/null
    timeout 5s "$output_dir/res.out" > "$output_dir/res" 2>/dev/null
  fi

  # 比较结果
  if diff -q "$output_dir/ans" "$output_dir/res" &>/dev/null; then
    ((success_count++))
  else
    echo "[31m测试失败: $pas_file[0m"
    ((failure_count++))
  fi

  cleanup
done < <(find ../tests/black-box/generate -name "*.pas")

# 输出结果
echo "================================="
echo "测试成功: [32m$success_count[0m 项"
echo "测试失败: [31m$failure_count[0m 项"
echo "================================="

exit $((failure_count > 0 ? 1 : 0))
