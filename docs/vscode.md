# Vscode 配置示例
## launch.json
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug GTest (Auto)",
      "type": "cppdbg",
      "request": "launch",
      "program": "${command:cmake.launchTargetPath}",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "lldb",
      "preLaunchTask": "cmake: build"
    },
    {
      "name": "Launch Main Program",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/kpc",
      "args": [
        "-i",
        "/Users/songjian/code/trans_pascal/TransPasc-Compiler/build/test.pas",
      ],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}/build",
      "environment": [],
      "externalConsole": false,
      "MIMode": "lldb",
      "preLaunchTask": "cmake: build"
    },
    {
      "name": "Launch Python Script",
      "type": "debugpy",
      "request": "launch",
      "program": "${file}",
      "cwd": "${workspaceFolder}/build",
      "args": [],
      "console": "integratedTerminal",
      "justMyCode": true,
    },
    {
      "name": "Run Integrated Test",
      "type": "debugpy",
      "request": "launch",
      "program": "${workspaceFolder}/tests/black-box/run-gencode.py",
      "cwd": "${workspaceFolder}",
      "args": [
        "${workspaceFolder}/tests/black-box/generate/",
        "${workspaceFolder}/build/tmp"
      ],
      "console": "integratedTerminal",
      "justMyCode": true
    }
  ]
}
```
## tasks.son
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "shell",
            "label": "cmake: build",
            "command": "cmake --build build --config Debug --target all -j 14",
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```
