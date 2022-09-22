# Extrapid 
## Extrapid是什么  
extrapid是一个**简单易用**、**拓展性高**、**客制化能力强**的网络服务器框架，其全部功能均以模块化的方式实现，加入http模块，它就是一个HTTP服务器，加入rtmp模块，它又是一个rtmp服务器，需要什么功能就可以添加什么功能。  
extrapid是一个致力于让普通用户可以更简单的搭建各种服务器，同时又尽可能少的限制开发者的操作空间的框架

## Extrapid怎么用
下载Extrapid直接运行后会自动在当前目录下创建新的目录conf/，并在该目录下创建文件config.json  
config的结构如下
```json
{
        "LogPath":      "./logs",
        "ModulePath":   "Modules"
}
```
LogPath指示日志存放目录  
ModulePath指示模块存放目录  
### 如何安装模块  
将模块放入config.json中ModulePath所指示的目录下运行extrapid即可自动加载  
## 如何编译本程序
克隆本仓库，在包含Makefile的目录下输入`make`即可编译，生成的文件在当前目录。也可以使用`xmake`命令编译  
## 如何开发模块
开发模块详情请看[SDK文档](https://github.com/extrapid/extrapid/blob/main/sdk/README_zh.md "SDK文档")页面。
