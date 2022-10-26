filetest.tar.gz是用于测试的文件，测试前先解压

```shell
tar -zxvf filetest.tar.gz
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