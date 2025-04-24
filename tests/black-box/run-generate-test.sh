#!/bin/bash
pwd
echo "begin test"

output_dir="./tmp/"

mkdir -p "$output_dir"

# 初始化计数器
success_count=0
failure_count=0

# 递归查找 .pas 文件
find tests/black-box/generate -name "*.pas" | while read -r pas_file; do
  # 获取文件名和路径
  base_name=$(basename "$pas_file" .pas)
  dir_name=$(dirname "$pas_file")

  # 检查是否存在对应的 .in 文件
  input_file="$dir_name/$base_name.in"
  if [[ -f "$input_file" ]]; then
    # 编译 .pas 文件
    fpc "$pas_file" -o"$output_dir/fpc.out" &>/dev/null
    if [[ $? -ne 0 ]]; then
      echo "FPC 编译失败: $pas_file"
      ((failure_count++))
      continue
    fi

    # 使用 kpc 编译为 .c 文件
    kpc "$pas_file" -o"$output_dir/$base_name.c" &>/dev/null
    if [[ $? -ne 0 ]]; then
      echo "KPC 编译失败: $pas_file"
      ((failure_count++))
      continue
    fi

    # 使用 gcc 编译为可执行文件
    gcc "$output_dir/$base_name.c" -o "$output_dir/res.out" &>/dev/null
    if [[ $? -ne 0 ]]; then
      echo "GCC 编译失败: $output_dir/$base_name.c"
      ((failure_count++))
      continue
    fi

    # 执行测试并生成输出
    cat "$input_file" | "$output_dir/fpc.out" > "$output_dir/ans" 2>/dev/null
    cat "$input_file" | "$output_dir/res.out" > "$output_dir/res" 2>/dev/null

    # 比较结果
    if diff -q "$output_dir/ans" "$output_dir/res" &>/dev/null; then
      ((success_count++))
    else
      echo "测试失败: $pas_file"
      ((failure_count++))
    fi

    # 清理生成的文件
    rm -f "$output_dir/fpc.out" "$output_dir/$base_name.c" "$output_dir/res.out" "$output_dir/ans" "$output_dir/res"
  else
    echo "缺少输入文件: $input_file"
    ((failure_count++))
  fi
done

# 输出统计结果
echo "测试成功: $success_count"
echo "测试失败: $failure_count"
