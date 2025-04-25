from email.mime import base
from math import log
import os
import threading
import subprocess
import shutil
import difflib
import queue
from datetime import datetime
from pathlib import Path

workspace_dir = "./"

# 全局共享数据结构（带线程锁）
result_lock = threading.Lock()
global_stats = {'success': 0, 'failure': 0}
failure_details = queue.Queue()

# 初始化目录结构
output_dir = Path("./tmp")
log_dir = output_dir/"log"
failure_dir = output_dir/"failures"

# 日志函数（线程安全）
def log_message(message, level="INFO"):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_entry = f"[{timestamp}] {message}"

    with result_lock:
        with open(log_dir/"runtime.log", "a") as f:
            f.write(log_entry + "\n")
        if level == "ERROR":
            with open(log_dir/"error.log", "a") as f:
                f.write(log_entry + "\n")
        #     print(f"\033[31m{log_entry}\033[0m")  # 红色错误
        # else:
        #     print(f"\033[34m{log_entry}\033[0m")  # 蓝色信息

# 测试用例处理器（线程执行单元）
def process_test_case(pas_file):
    dirname = os.path.dirname(pas_file)
    base_name = Path(pas_file).stem
    case_dir = failure_dir/base_name
    input_file = pas_file.replace(".pas", ".in")

    # 阶段1：Pascal编译
    try:
        fpc_out = output_dir/base_name
        # fpc_out = output_dir/"fpc.out"
        proc = subprocess.run(
            ["fpc", pas_file],
            stderr=subprocess.PIPE,
            check=True,
            stdout=subprocess.DEVNULL
        )
        # move the output file to the output directory
        # executable file is in ${dirname}/${base_name}
        # I will move it to ${output_dir}/${base_name}
        os.rename(
            os.path.join(dirname, base_name),
            fpc_out
        )
    except (subprocess.CalledProcessError, subprocess.TimeoutExpired) as e:
        case_dir.mkdir(parents=True, exist_ok=True)
        (case_dir/"fpc_compile.log").write_bytes(e.stderr)
        with result_lock:
            global_stats["failure"] += 1
            failure_details.put((base_name, "FPC编译失败"))
        return

    # 阶段2：KPC转换
    try:
        c_file = output_dir/f"{base_name}.c"
        proc = subprocess.run(
            ["./kpc", "-i", pas_file, "-o", c_file],
            stderr=subprocess.PIPE,
            timeout=15,
            check=True,
            stdout=subprocess.DEVNULL
        )
    except (subprocess.CalledProcessError, subprocess.TimeoutExpired) as e:
        case_dir.mkdir(parents=True, exist_ok=True)
        (case_dir/"kpc_convert.log").write_bytes(e.stderr)
        with result_lock:
            global_stats["failure"] += 1
            failure_details.put((base_name, "KPC转换失败"))
        return

    # 阶段3：GCC编译
    try:
        gcc_out = output_dir / f"{base_name}.gcc_out"
        proc = subprocess.run(
            ["gcc", c_file, "-o", str(gcc_out)],
            stderr=subprocess.PIPE,
            check=True
        )
    except (subprocess.CalledProcessError, subprocess.TimeoutExpired) as e:
        case_dir.mkdir(parents=True, exist_ok=True)
        (case_dir/"gcc_compile.log").write_bytes(e.stderr)
        with result_lock:
            global_stats["failure"] += 1
            failure_details.put((base_name, "GCC编译失败"))
        return

    # 阶段4：执行测试
    ans_file = output_dir/"ans"
    res_file = output_dir/"res"

    def run_executable(executable):
        try:
            input_handle = open(input_file) if Path(input_file).exists() else None
            with open(ans_file if "fpc" in str(executable) else res_file, "w") as f:
                return subprocess.run(
                    executable,
                    stdin=input_handle,
                    stdout=f,
                    stderr=subprocess.PIPE,
                    check=True
                )
        finally:
            if input_handle: input_handle.close()

    try:
        run_executable(fpc_out)
        run_executable(gcc_out)
    except (subprocess.CalledProcessError, subprocess.TimeoutExpired) as e:
        case_dir.mkdir(parents=True, exist_ok=True)
        (case_dir/"runtime_error.log").write_bytes(e.stderr)
        with result_lock:
            global_stats["failure"] += 1
            failure_details.put((base_name, "执行超时或错误"))
        return

    # 结果比对
    if ans_file.read_bytes() == res_file.read_bytes():
        with result_lock:
            global_stats["success"] += 1
    else:
        case_dir.mkdir(parents=True, exist_ok=True)
        shutil.copy(ans_file, case_dir/"expected")
        shutil.copy(res_file, case_dir/"actual")
        diff = difflib.unified_diff(
            ans_file.read_text().splitlines(),
            res_file.read_text().splitlines(),
            fromfile="expected",
            tofile="actual"
        )
        (case_dir/"diff.diff").write_text("\n".join(diff))
        with result_lock:
            global_stats["failure"] += 1
            failure_details.put((base_name, "输出不一致"))

    # 清理临时文件
    for f in [fpc_out, c_file, gcc_out, ans_file, res_file]:
        f.unlink(missing_ok=True)

# 主程序
def main():
    # 初始化目录
    shutil.rmtree(output_dir, ignore_errors=True)
    output_dir.mkdir()
    log_dir.mkdir()
    failure_dir.mkdir()

    # 获取测试用例队列
    test_cases = list(Path("../tests/black-box/generate").glob("**/*.pas"))
    log_message(f"发现 {len(test_cases)} 个测试用例")

    # 创建线程池（限制最大并发数）
    # max_workers = 20  # 根据CPU核心数调整[10](@ref)
    threads = []
    for pas_file in test_cases:
        # while threading.active_count() > max_workers:
        #     pass  # 简单限流
        t = threading.Thread(target=process_test_case, args=(str(pas_file),))
        t.start()
        threads.append(t)

    # 等待所有线程完成
    for t in threads:
        t.join()

    # 输出最终报告
    print("\n" + "="*50)
    print(f"成功案例: \033[32m{global_stats['success']}\033[0m")
    print(f"失败案例: \033[31m{global_stats['failure']}\033[0m")
    print("="*50)

    # 记录失败详情到日志
    if not failure_details.empty():
        log_message("失败案例详情:", level="ERROR")
        while not failure_details.empty():
            case, reason = failure_details.get()
            # print(f"  • {case}: {reason}")
            log_message(f"  • {case}: {reason}", level="ERROR")

    print(f"\n调试信息目录: file://{failure_dir.resolve()}")
    print("="*50)

if __name__ == "__main__":
    main()
