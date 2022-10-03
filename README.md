# 数据备份软件

软件开发综合实验-数据备份软件(filebak)



## 实现功能

- 数据备份

- 数据还原

- 文件类型支持

- 元数据支持

- 自定义备份

- 压缩解压

- 打包解包

- 加密备份

- 实时备份



## 命令使用方法

```Bash
Usage:  filebak [OPTIONS] COMMAND

Options:
    -v, --version   版本信息
    -h, --help      帮助文档

Command:
    backup          设置备份任务
    info            输出备份任务的信息
    stop            停止指定的备份任务
    start           继续指定的备份任务
    rm              删除指定的备份任务

Run 'filebak COMMAND --help' for more information on a command.
```

### backup

```Bash
Usage:  filebak backup [OPTIONS] [<args>]

设置备份任务

Options:
    # 元数据支持
    -m --metadata 保留元数据

    # 自定义备份
    --filter    记录规则的文本文件
    --path      正则表达式(仿照.gitignore)
    --name      正则表达式
    --type      普通文件(r, regular),目录文件(d, directory),
                管道文件(f, FIFO),链接文件(s, symbolic link)
    --time      "起始时间 终止时间"
                备份给定时间区间内的文件
    --task      "起始时间 终止时间 间隔"或"起始时间"

    # 打包解包
    始终打包，打包为.tar后缀

    # 解压压缩
    -d --dcompress 	解压	解压xxxx.tar.cps文件
    -c --compress 	压缩	压缩为.cps后缀的文件，即xxxx.tar.cps

    # 加密解密
    -p --password 	加密与解密时输入密码

    # 实时备份
    --auto-backup	感知文件变化，自动进行备份

    -m	--message	备份说明

    -v	--verbose 显示指令执行过程
```

### info

```Bash
Usage:  filebak info [OPTIONS]

显示任务信息

Options:
    -a	--all	    显示所有任务(默认)
    -s	--stop	    显示所有停止状态的任务
    -f	--finished	所有完成的任务
    -r	--running	正在运行的任务
```
### stop

```Bash
Usage:  filebak stop [task_id]

停止编号为task_id的备份任务
```

### start

```Bash
Usage:  filebak start [task_id]

继续编号为task_id的备份任务
```

### rm

```Bash
Usage:  filebak rm [task_id]

删除编号为task_id的备份任务
```

