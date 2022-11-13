`filetest.tar.gz`是用于测试的文件，测试前先解压

`filetest.pak.cps.ept`是使用`filebak`对测试文件进行加密压缩备份后得到的备份文件，其内容与`filetest.tar.gz`相同，解密密码为123456

```shell
tar -zxvf filetest.tar.gz

# 或

filebak -rva -i filetest.pak.cps.ept -o . -p 123456
```

测试文件的目录树结构
```shell
filetest
├── assets
│   ├── filetest.tar
│   └── images
│       ├── class_diagram.png
│       └── use_case_diagram.png
├── CMakeLists.txt
├── fifo    # 管道文件
├── hard_link   # 硬链接 指向assets/filetest.tar
├── include
│   ├── aes.h
│   ├── argparser.h
│   ├── compressor.h
│   ├── cxxopts.hpp
│   ├── filebase.h
│   ├── filter.h
│   ├── packer.h
│   └── task.h
├── LICENSE
└── soft_link -> assets/filetest.tar    # 软链接
```