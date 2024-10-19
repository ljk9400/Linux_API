## 一、Linux API 说明:

### code
- base_file 模块
file 读写相关测试用例基础：  

- base_resource 模块
内存、资源、环境、系统日志等模块：  

- base_process 模块
进程、线程、信号等模块：  

- base_network模块
网络模块模块：  

- clock 模块
时钟模块：

- tony 模块
diy 功能应用

### common 模块
通用测试、log、界面等模块：  

### system 模块
脚本模块、系统模块：  

### monitor 性能工具
ebpf  
perftto  
heaptrack  


## 二、提交类型
- feat: 新功能（feature） 
- fix: 修补bug   
- refactor: 重构   


## 三、C++规范
定义了一些C++的编码规范  

### C++编码规则
- 文件夹名 命名规则  
使用全小写字母。  
多个单词可以使用下划线分隔。  

- 文件名 命名规则  
头文件使用 .h 或 .hpp 后缀，源文件使用 .cpp 后缀。  
使用全小写字母。  
多个单词可以使用下划线分隔。  

- 函数名 命名规则  
小驼峰  

- 变量名 命名规则  
变量类型: 大驼峰  
全局变量: 小写+下划线  
局部变量: 小写+下划线，结尾可加_g  

- 类名、结构体名 命名规则  
类名：   大驼峰，可加Class  
类对象： 小写+下划线  

结构体名：大驼峰, 可加Stu  
结构体对象：小写+下划线，结尾可加_t  


### C++编程规则
- 函数规则 
(1)、  
void fun() {

 }

- 其他命名规则


### 配置文件规则
- json配置规则

- xml配置规则

- yaml配置规则

- idl配置规则

### 生成产物规则
- 生成执行文件规则  
- 生成库文件规则  



## 四、其他说明
### 脚本win/lin字符转换
  脚本出现windows字符转化到unix/linux下
  dos2unix build.sh

