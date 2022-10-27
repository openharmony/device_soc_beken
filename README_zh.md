# device_soc_beken

-	[介绍](#介绍)
-	[目录框架](#目录框架)
-	[编译环境搭建](#编译环境搭建)
-	[编译流程](#编译流程)
-	[烧录流程](#烧录流程)
-	[相关仓](#相关仓)

## 介绍

该仓库托管博通集成旗下soc芯片相关代码，包含hal模块、wifi配网、ble配网，以及各个模块等相关代码：

## 目录框架

```
device_soc_beken/
├── bk7235									# BK7235芯片平台目录
│  		├── liteos_m						# 基于liteos_m的适配目录
│   	│   ├── components					# 组件服务层代码目录
│   	│   └── bk_sdk_armino				# sdk模块的目录
│   	└── hal
│			├── communication				# wifi/ble适配层代码目录
│   	    └── iot_hardware				# hardware的目录
│			├── update						# hota的目录
│   	    └── utils						# hal_file适配的目录
│
├── Kconfig.liteos_m.defconfig				# Kconfig配置
├── Kconfig.liteos_m.series					# 系列soc配置宏
└── Kconfig.liteos_m.soc					# soc kconfig配置宏
```

## 编译环境搭建

* 安装必要的库和工具

```
	sudo apt-get install build-essential gcc g++ make zlib* libffi-dev e2fsprogs pkg-config flex bison perl bc openssl libssl-dev libelf-dev libc6-dev binutils binutils-dev libdwarf-dev u-boot-tools mtd-utils gcc-arm-linux-gnueabi cpio device-tree-compiler git git-lfs ruby ccache
```

* 安装python3

```
	1.sudo apt-get install python3.8
	2.安装pip3，更换下载源
	sudo apt install python3-pip
```

* 安装hb

```
	1.python3 -m pip install --user ohos-build
	2.安装固定hb版本
	pip3 install ohos-build==0.4.6
```

* 安装risc-v

```
	1.解压risc-v.tar.bz2
	2.设置环境变量
	export PATH=/opt/risc-v/nds32le-elf-mculib-v5/bin:$PATH
```


## 编译流程

1. 选择芯片

```
	hb set
```
使用键盘方向键进行板卡demo选择：

```
	beken
 		> wifi_bk7235
 		> xts_demo
```
正常启动选wifi_bk7235,xts测试选择xts_demo

2. 编译
	
```
 	  hb build -f
```

## 烧录流程
打开烧录工具，选择openharmony\device\board\beken\bk7235x\liteos_m\build\all_2M.1220.bin，选择串口点击烧录
开发板重新上电

## 相关仓

[vendor_beken](https://gitee.com/openharmony-sig/vendor_beken)

[device_board_beken](https://gitee.com/openharmony-sig/device_board_beken)
