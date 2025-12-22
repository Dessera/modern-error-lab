# 现代 C++ 实验室: 错误处理

本仓库内实现了一个基于`std::expected`和`concept`的简易错误处理框架`Flex`。

## 开发环境

- 编译器：GCC 14
- 工程管理：CMake 4.1.2, Ninja 1.13.1

> `Ninja`不是必须选项，可以使用`Makefile`

## 构建

```shell
# 创建构建文件夹
mkdir build
cd build

# 生成构建脚本
cmake .. -G Ninja

# 构建
ninja
```

## 测试

- `Flex`性能测试：`./build/test/test_flex_perf`
- 基于'Flex'的简易`Tokenizer`：`./build/token`
