> 来源：[Build your own Shell | CodeCrafters](https://app.codecrafters.io/courses/shell/overview)
>
> 完成日期：2026 / 2 / 21

环境：Linux 24.04  （Windows 下大部分功能不可用）

依赖：GNU Readline 库

```shell
sudo apt install libreadline-dev
```

构建：

```shell
$ cd ./src/make/      # 切换目录
$ ./build.sh          # 构建
$ ./build/bin/myshell # 运行
```

codecrafters-shell-c 目录下的 CmakeLists.txt 为网站评测所用

[![progress-banner](https://backend.codecrafters.io/progress/shell/ae79f394-8776-4ae8-a66c-dbf617186d5e)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

This is a starting point for C solutions to the
["Build Your Own Shell" Challenge](https://app.codecrafters.io/courses/shell/overview).

In this challenge, you'll build your own POSIX compliant shell that's capable of
interpreting shell commands, running external programs and builtin commands like
cd, pwd, echo and more. Along the way, you'll learn about shell command parsing,
REPLs, builtin commands, and more.

**Note**: If you're viewing this repo on GitHub, head over to
[codecrafters.io](https://codecrafters.io) to try the challenge.

# Passing the first stage

The entry point for your `shell` implementation is in `src/main.c`. Study and
uncomment the relevant code, and push your changes to pass the first stage:

```sh
git commit -am "pass 1st stage" # any msg
git push origin master
```

Time to move on to the next stage!

# Stage 2 & beyond

Note: This section is for stages 2 and beyond.

1. Ensure you have `cmake` installed locally
1. Run `./your_program.sh` to run your program, which is implemented in
   `src/main.c`.
1. Commit your changes and run `git push origin master` to submit your solution
   to CodeCrafters. Test output will be streamed to your terminal.
