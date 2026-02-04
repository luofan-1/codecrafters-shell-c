#!/usr/bin/env zsh

# 定义要操作的目录
BUILD_DIR="build"

# 检查并删除 build 目录（对应原批处理的 rd /s /q）
if [ -d "$BUILD_DIR" ]; then
    echo "removing build"
    rm -rf "$BUILD_DIR"  # -r 递归删除目录，-f 强制删除（无提示）
    echo "build removed"
else
    echo "build not found"
fi

# 创建 build 目录（对应原批处理的 mkdir）
mkdir -p "$BUILD_DIR"  # -p 确保目录不存在时创建，存在时不报错

# 进入 build 目录并执行 cmake + make（对应原批处理的 cd + cmake + make）
cd "$BUILD_DIR" || exit  # 若 cd 失败（比如目录创建失败），直接退出脚本
cmake ..
make
