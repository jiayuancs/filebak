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
      --type arg     过滤文件类型: n普通文件,d目录文件,l符号链接,p管道 文件
      --name arg     过滤文件名：正则表达式
      --atime arg    文件的访问时间区间, 例"2000-11-11 23:20:21 2022-10-11 20:10:51"
      --mtime arg    文件的修改时间区间, 格式同atime
      --ctime arg    文件的改变时间区间, 格式同atime
  -m, --message arg  添加备注信息

 Restore options:
  -a, --metadata  恢复文件的元数据
```

### 实例

备份：将当前目录下的`myfile`目录树打包压缩备份到`bakdir`目录下，文件名为`mybak`

```
filebak -bcv -i myfile -o bakdir/mybak
```

恢复：将`mybak.pak.cps`中的数据恢复到`restore_file`目录下

```
filebak -rv -i mybak.pak.cps -o restore_file/
```

自定义备份：指定备份路径的正则表达式

```
filebak -bv -i myfile3 -o bakdir/myfile3 --path "^myfile3/test1/(1.txt|test4)"
```

自定义备份：指定备份文件的修改时间范围

```
filebak -bv -i myfile6 -o bakdir/myfile6 --mtime "2022-10-24 12:00:00 2022-10-25 22:00:00"
```
