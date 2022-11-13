# 软件开发综合实验-数据备份软件(filebak)

## 开发环境

- 操作系统：Ubuntu22.04 LTS
- 开发工具：Visual Studio Code
- g++ 11.3.0
- cmake 3.22.1
- OpenSSL 1.1.1n



## 编译安装

### 方法一：cmake

安装编译工具和依赖库
```shell
apt install -y build-essential cmake libssl-dev
```
切换到项目根目录下，依次执行如下命令
```shell
mkdir build && cd build
cmake ..
make
sudo make install
```

### 方法二：使用Dockerfile

切换到项目根目录下，执行如下命令构建镜像

```
docker build -t filebak .
```

启动容器

```
docker run -it --name filebak_test filebak /bin/bash
```



## 实现的功能

- 数据备份
- 数据还原
- 文件类型支持
  - 普通文件

  - 目录文件

  - 管道文件

  - 软链接文件

  - 硬链接文件
- 元数据支持
- 自定义备份
  - 路径
  - 类型
  - 名字
  - 时间
- 压缩解压
- 打包解包
- 加密备份





## 使用方法

```
Usage:
  filebak [OPTION...]

  -b, --backup        备份
  -r, --restore       恢复
  -l, --list arg      查看指定备份文件的信息
  -v, --verbose       输出执行过程信息
  -i, --input arg     程序输入文件路径
  -o, --output arg    程序输出文件路径
  -p, --password arg  指定密码
  -h, --help          查看帮助文档

 Backup options:
  -c, --compress     备份时压缩文件
  -e, --encrypt      备份时加密文件
      --path arg     过滤路径：正则表达式
      --type arg     过滤文件类型: n普通文件,d目录文件,l链接文件,p管道文件
      --name arg     过滤文件名：正则表达式
      --atime arg    文件的访问时间区间, 例"2000-11-11 23:20:21 2022-10-11 20:10:51"
      --mtime arg    文件的修改时间区间, 格式同atime
      --ctime arg    文件的改变时间区间, 格式同atime
  -m, --message arg  添加备注信息

 Restore options:
  -a, --metadata  恢复文件的元数据
```

> `test`文件夹下存放了测试用的文件

**查看帮助文档**
```
filebak -h
```

**查看备份文件信息**
```
filebak -l filetest.pak.cps.ept
```


**普通备份(仅打包)**

`-b`表示备份，
`-v`表示输出备份过程信息
`-m`用于给出备份说明信息

```shell
filebak -bv -i filetest -o bakdir/filetest -m "这里是备份说明"
```

**备份(启用压缩和加密)**

`-c`表示对打包文件进行压缩，
`-e`表示对文件进行加密，
`-p`用于指定加密密码

```shell
filebak -bvce -i filetest -o bakdir/filetest -p 123456 
```

**自定义备份**

`--path`指定**路径**的匹配规则(正则表达式)，
下面的指令只备份`filetest/include`和`filetest/assets/images`两个目录树

```
filebak -bv -i filetest -o bakdir/filetest --path "^filetest/(include|(assets/images))"
```

`--name`指定**文件名**的匹配规则(正则表达式)，
下面的指令只备份以`.h`或`.png`为后缀的文件

```
filebak -bv -i filetest -o bakdir/filetest --name "\.(h|png)$"
```

`--type`指定**文件类型**，`n`普通文件,`d`目录文件,`l`链接文件,`p`管道文件

下面的指令只备份普通文件和链接文件

```
filebak -bv -i filetest -o bakdir/filetest --type "nl"
```

`--atime`,`--mtime`, `--ctime`分别用于指定**文件的时间戳范围**，
下面的指令只备份在`2022-11-10 08:00:00`到`2022-11-18 22:00:00`之间修改过的文件
```
filebak -bv -i filetest -o bakdir/filetest --mtime "2022-11-10 08:00:00 2022-11-18 22:00:00"
```

**恢复文件**

`-r`表示恢复文件，
`-a`表示需要恢复文件元数据，
`-i`指定备份文件，
`-o`指定文件恢复到哪个位置
`-p`用于指定解密用的密码

```
filebak -rva -i filetest.pak.cps.ept -o restore_file/ -p 123456
```

