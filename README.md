# README

A Renderer is based on ray tracing.

## How to use

### Run [CMakeLists.txt](CMakeLists.txt)

````shell
cmake -B build      # 生成构建目录
cmake --build build # 执行构建
````
### Qt

The UI is based on Qt 5. Before use [CMakeLists.txt](CMakeLists.txt), please change Qt's path here

```` cmake
#CMakeLists.txt
....
#Change Qt Path
set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64")
....
````
### Example

If you don't want to use Qt as framework, just go to [example](./example) and run `run.bat`
in WINDOWS OS.

### OpenMP

The Render is accelerated by OpenMP. Make sure your compiler support it.



## reference

- [raytracing in one week](https://raytracing.github.io/)
- [PBRT3](https://pbr-book.org/3ed-2018/contents)