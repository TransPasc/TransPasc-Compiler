# Pre-commit

`pre-commit` 是一种代码质量保障工具，可以在代码提交前自动运行一系列检查工具（如格式化、静态分析等），以确保代码符合团队的规范。

## 为何使用
- **提高代码质量**：通过自动化工具检测代码中的潜在问题。
- **统一代码风格**：避免因代码风格不一致导致的代码审查问题。
- **节省时间**：在提交前发现问题，减少后续修复成本。**- **提高代码质量**：通过自动化工具检测代码中的潜在问题。
- **统一代码风格**：避免因代码风格不一致导致的代码审查问题。
- **节省时间**：在提交前发现问题，减少后续修复成本。
## 环境准备

### python 3
确保安装了 Python 3，并可以通过以下命令验证：
```sh
python3 --version
```
### clang-format
安装 clang-format、clang-tidy 和 cppcheck，用于 C/C++ 代码的格式化和静态分析。
```sh
# Ubuntu/Debian
sudo apt-get install clang-format clang-tidy cppcheck

# macOS
brew install llvm cppcheck

# Windows (通过 Chocolatey)
choco install llvm cppcheck
```

### pre-commit
安装 pre-commit 工具：
```sh
pip install pre-commit
```

## 如何使用

1. **初始化 pre-commit 配置文件**
   在项目根目录创建 `.pre-commit-config.yaml` 文件，并添加需要的钩子。例如：
   ```yaml
   repos:
     - repo: https://github.com/pre-commit/pre-commit-hooks
       rev: v4.4.0
       hooks:
         - id: trailing-whitespace
         - id: end-of-file-fixer
         - id: check-yaml
   ```

2. **安装 pre-commit 钩子**
   在项目中启用 `pre-commit`：
   ```sh
   pre-commit install
   ```

3. **运行 pre-commit**
   安装成功后，会在提交前自动运行，当然也可以手动运行 `pre-commit` 检查：
   ```sh
   pre-commit run --all-files
   ```

## 常见问题
- 如果某些钩子未通过，可以根据提示修复代码后重新提交。
- 如果需要跳过 `pre-commit` 检查，可以使用以下命令强制提交：
  ```sh
  git commit --no-verify
  ```

## 参考链接
- [pre-commit 官方文档](https://pre-commit.com/)
- [clang-format 官方文档](https://clang.llvm.org/docs/ClangFormat.html)
