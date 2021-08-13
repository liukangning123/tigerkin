# tigerkin

## 开发环境

* CentOS Linux release 8.4.2105
* gcc version 8.4.1
* cmake version 3.18.2
* libboost_random.so.1.66.0
  ```sh
    yum install boost
    yum install boost-devel
  ```
* libyaml-cpp.so.0.7.0
  ```sh
    git clone https://github.com/jbeder/yaml-cpp.
    cd yaml-cpp
    cmake . -DYAML_BUILD_SHARED_LIBS=ON
    make
    make install
  ```

## 日志系统

    Logger(定义日志类别)
      |
      |
    Appender(日志输出地)
      |
      |-- Formatter(日志格式)

## 配置系统

> 约定优于配置

