'''
description:
  找出在两个结果文件中，第一份文件的测试用例成功而第二份文件的测试用例失败的测试用例
'''
def parse_results(filename):
    result = {}
    with open(filename, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line or ':' not in line or '/' not in line:
                continue
            parts = line.split(':')
            testcase = parts[0].strip()
            status = parts[1].strip()
            if status.startswith('success'):
                result[testcase] = 'success'
            elif status.startswith('failed'):
                result[testcase] = 'failed'
    return result

import sys
if len(sys.argv) != 3:
    print("Usage: python find_failed.py <result_file1> <result_file2>")
    sys.exit(1)
file1 = sys.argv[1]
file2 = sys.argv[2]

res1 = parse_results(file1)
res2 = parse_results(file2)

for case in res1:
    if res1[case] == 'success' and res2.get(case) == 'failed':
        print(case)
