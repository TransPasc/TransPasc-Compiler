import enum
from typing import List,Tuple
from dataclasses import dataclass
import os
import sys
import subprocess
import multiprocessing
import time
from collections import defaultdict

DIFF = "diff"
FPC = "fpc"
CC = "cc"
TIMEOUT = 60
class Platform(enum.Enum):
  LINUX = "linux"
  WINDOWS = "windows"
  MACOS = "macos"

@dataclass
class Config:
  @dataclass
  class Skip:
    reason : str
    platform : List[Platform]
    path : str
  skips: List[Skip]

  @staticmethod
  def shouldSkip(config: "Config", pascal_path: str) -> Tuple[bool, 'Config.Skip|None']:
    for skip in config.skips:
        if skip.path in pascal_path and get_platform() in skip.platform:
            return True, skip
    return False, None

PLATFORM = None
def get_platform() -> Platform:
  import platform
  global PLATFORM
  if PLATFORM is not None:
      return PLATFORM
  plat = platform.system()
  if plat == "Linux":
      PLATFORM = Platform.LINUX
  elif plat == "Windows":
      PLATFORM = Platform.WINDOWS
  elif plat == "Darwin":
      PLATFORM = Platform.MACOS
  else:
      raise ValueError(f"Unsupported platform: {plat}")
  return PLATFORM

def load_config(json_path: str) -> Config:
  import json
  with open(json_path, "r") as f:
      data = json.load(f)
  config = Config(skips=[])
  for item in data['skip']:
      reason = item["reason"]
      if "all" in item["platform"]:
          platform = [Platform.LINUX, Platform.WINDOWS, Platform.MACOS]
      else:
          platform = [Platform(p) for p in item["platform"]]
      path = item["path"]
      config.skips.append(
          Config.Skip(
              reason=reason,
              platform=platform,
              path=path,
          )
      )
  return config

def process_case(args) -> dict[str, str]:
    name, pas_path, in_path, output_base, config = args
    result = {
        "name": name,
        "status": "success",
        "log": "",
        "pas_path": pas_path,
        "short_report": "",
        "time_usage": None,
    }

    output_label = result["pas_path"].split("generate/")[-1]
    result["name"] = output_label

    # calculate all the path needed
    log_dir = os.path.join(output_base, "failed/log")
    fpc_working_dir = os.path.join(output_base, f"fpc_tmp-{name}")
    fpc_output_executable = os.path.join(fpc_working_dir, name)
    fpc_output_executable_better_name = fpc_output_executable
    kpc_path = "./build/kpc"
    c_source = os.path.join(output_base, f"{name}.c")
    cc_output_executable = os.path.join(output_base, f"{name}.gcc_out")
    testcase_input = os.path.abspath(in_path) if in_path else os.devnull
    testcase_output = os.path.join(output_base, f"{name}.out")
    testcase_answer = os.path.join(output_base, f"{name}.ans")
    testcase_diff = os.path.join(output_base, f"{name}.diff")

    os.makedirs(log_dir, exist_ok=True)
    os.makedirs(fpc_working_dir, exist_ok=True)

    start_time = time.time()
    # os.rename(fpc_output_executable, fpc_output_executable_better_name)

    # print all the path needed
    # print(f"Pascal: {pas_path}")
    # print(f"FPC: {fpc_working_dir}")
    # print(f"FPC output: {fpc_output_executable}")
    # print(f"KPC: {kpc_path}")
    # print(f"C source: {c_source}")
    # print(f"GCC output: {cc_output_executable}")
    # print(f"Input: {in_path}")
    # print(f"Output base: {output_base}")
    # print(f"Log dir: {log_dir}")
    # print(f"Name: {name}")
    # print("!" + "-" * 50)

    try:
        # check if the pas_path is in the skip list
        should_skip, skip_config = config.shouldSkip(config, pas_path)
        if should_skip:
            result["status"] = "skipped"
            result["log"] = f"Skipped: {skip_config.reason} on {get_platform()}"
            result["short_report"] = skip_config.reason
            print(f"\033[93m{result['name']}: {result['status']} Because: {result['short_report']}\033[0m")
            return result

        err_msg_list = []
        # 编译Pascal
        # print the command
        # create a directory for fpc
        fpc_res = subprocess.run(
            [FPC, "-Mdelphi", pas_path, f"-FE{fpc_working_dir}"],
            capture_output=True,
            text=True,
        )
        if fpc_res.returncode != 0:
            err_msg_list.append(f"Pascal编译失败:\n{fpc_res.stderr}")
            err_msg_list.append(f"Pascal编译失败:\n{fpc_res.stdout}")

        with open(testcase_answer, "w") as f_ans:
            subprocess.run(
                [fpc_output_executable_better_name],
                stdin=open(testcase_input),
                stdout=f_ans,
                stderr=f_ans,
            )

        # 生成C文件
        kpc_res = subprocess.run(
            [kpc_path, "-i", pas_path, "-o", c_source],
            capture_output=True,
            timeout=TIMEOUT,
        )
        if kpc_res.returncode != 0:
            err_msg_list.append(f"C代码生成失败:\n{kpc_res.stderr}")

        # 编译C文件
        cc_res = subprocess.run(
            [CC, c_source, "-o", cc_output_executable],
            capture_output=True,
        )
        if cc_res.returncode != 0:
            err_msg_list.append(f"C编译失败:\n{cc_res.stderr.decode()}")
        if err_msg_list:
            err_msg = ""
            for msg in err_msg_list:
                err_msg += f"{msg}\n"
            raise Exception(err_msg)
        with open(testcase_output, "w") as f_out:
            subprocess.run(
                [cc_output_executable],
                stdin=open(testcase_input),
                stdout=f_out,
                stderr=f_out,
                timeout=TIMEOUT,
            )

        # 结果比对

        diff = subprocess.run(
            [DIFF, "-b", testcase_answer, testcase_output],
            capture_output=True,
            text=True,
        )
        if diff.returncode != 0:
            try:
                with open(testcase_answer, 'r') as f_ans, open(testcase_output, 'r') as f_out:
                    ans_content = f_ans.read().strip()
                    out_content = f_out.read().strip()

                ans_parts = ans_content.split()
                out_parts = out_content.split()

                if len(ans_parts) != len(out_parts):
                    raise ValueError("输出字段数量不一致")

                ans_floats = []
                for part in ans_parts:
                    ans_floats.append(float(part))

                out_floats = []
                for part in out_parts:
                    out_floats.append(float(part))

                for a, o in zip(ans_floats, out_floats):
                    if abs(a - o) > 1e-6:
                        raise ValueError("浮点误差超过1e-6")
            except Exception as e:
                with open(testcase_diff, "w") as f:
                    f.write(diff.stdout)
                    raise Exception("输出不一致")

    except Exception as e:
        result["status"] = "failed"
        result["log"] = str(e)
        result["short_report"] = str(e).split(":")[0]
        # write the log to the file
        with open(os.path.join(log_dir, f"{name}.log"), "w") as f:
            f.write(result["log"])
    end_time = time.time()
    result["time_usage"] = end_time - start_time



    text_message = (
        f"\033[92m{result['name']}: {result['status']}\033[0m"
        if result["status"] == "success"
        else f"\033[91m{result['name']}: {result['status']}\033[0m : {result['short_report']}"
    )

    print(text_message)
    return result


def main():
    if len(sys.argv) != 3:
        print("Usage: python script.py test_path output_path")
        sys.exit(1)

    test_path, output_path = sys.argv[1], sys.argv[2]

    # if directory not exists, create it
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    cases: list[tuple[str, str, str | None, str, Config]] = []
    config = load_config("tests/black-box/run-gencode-config.json")
    # 递归查找所有.pas文件[9,10](@ref)
    for root, _, files in os.walk(test_path):
        for file in files:
            if file.endswith(".pas"):
                name = os.path.splitext(file)[0]
                pas_file = os.path.join(root, file)

                # 查找对应的.in文件
                in_file = os.path.join(root, f"{name}.in")
                if not os.path.exists(in_file):
                    in_file = None
                cases.append((name, pas_file, in_file, output_path, config))
    print(f"找到 {len(cases)} 个测试用例\n")

    start_time = time.time()
    print("开始处理测试用例...\n")

    # with ThreadPoolExecutor() as executor:  # 可以自定义线程数
    #     results = list(executor.map(process_case, cases))

    with multiprocessing.Pool() as pool:
        results = pool.map(process_case, cases)

    # results = []
    # for case in cases:
    #     result = process_case(case)
    #     results.append(result)

    end_time = time.time()
    print(f"处理完成, 耗时 {end_time - start_time:.2f} 秒\n")

    #
    # print(f"处理结果: {result['name']} - {result['status']}")

    # sort results by name
    results.sort(key=lambda x: x["name"])

    # 统计结果
    stats = defaultdict(int)
    for res in results:
        stats[res["status"]] += 1
    print(f"\n统计结果: 成功 {stats['success']}, 失败 {stats['failed']}, 跳过 {stats['skipped']}")
    print(f"错误率: {stats['failed'] / len(cases) * 100:.2f}%")


if __name__ == "__main__":
    main()
