# device_soc_beken

## 介绍

该仓库托管博通集成旗下SoC芯片BK7235相关代码。包含HAL模块、WiFi配网、BLE配网，以及各个模块的相关代码。

| 芯片    | 能力介绍			|   
| -------------- | -------------|
| BK7235	| [WiFi数据传输](http://www.bekencorp.com/index/goods/detail/cid/40.html)|

## 目录框架

```
device_soc_beken/
├── bk7235									# BK7235芯片平台目录
│   ├── BUILD.gn							# GN构建脚本
│   ├── hals								# 代码适配目录
│   ├── Kconfig.liteos_m.defconfig.bk7235	# BK7235 Kconfig默认配置
│   ├── Kconfig.liteos_m.defconfig.series	# Series Kconfig默认配置
│   ├── Kconfig.liteos_m.series				# Series Kconfig配置项
│   ├── Kconfig.liteos_m.soc				# SoC Kconfig配置项
│   └── liteos_m							# 基于LiteOS-M的适配目录
├── BUILD.gn								# GN构建脚本
├── EULA									# 最终用户许可协议
├── Kconfig.liteos_m.defconfig				# Kconfig默认配置
├── Kconfig.liteos_m.series					# Series配置项
├── Kconfig.liteos_m.soc					# Series配置项
├── LICENSE									# 证书文件
├── OAT.xml									# 开源仓审查规则配置文件
└── README_zh.md							# 中文README文件
```

## 编译环境搭建
* 安装python3

```
	1.sudo apt-get install python3.8
	2.安装pip3，更换下载源
	sudo apt install python3-pip
```

* 安装hb

```
	1.删除旧版本
	python -m pip uninstall ohos-build
	2.安装hb1.0.0版本
	python -m pip install --user build/hb
	3.设置环境变量
	echo 'export PATH=~/.local/bin:$PATH' | tee -a ~/.bashrc
	source ~/.bashrc
```

* 安装 [toolchain_v5.1.1.tgz](http://dl.bekencorp.com/tools/toolchain/riscv/toolchain_v5.1.1.tgz)

```
	1.解压缩
	tar xvf toolchain_v5.1.1.tgz -C
	2.设置环境变量
	echo 'export PATH=/opt/risc-v/nds32le-elf-mculib-v5f/bin:$PATH' | tee -a ~/.bashrc
	source ~/.bashrc

```

## 编译流程

1. 选择芯片

```
	hb set
```
使用键盘方向键进行板卡demo选择：

```
	OHOS Which os_level do you need?  (Use arrow keys)
		> mini
		> small
		> standard
```
选择mini

```
	beken
 		> wifi_bk7235
 		> xts_demo
```
正常启动选wifi_bk7235，XTS测试选择xts_demo

2. 编译
	
```
	rm -rf out/
 	hb build -f  --gn-args build_xts=true
```
编译成功界面：
```
 	[OHOS INFO] c targets overlap rate statistics
	[OHOS INFO] subsystem           files NO.       percentage      builds NO.      percentage      overlap rate
	[OHOS INFO] commonlibrary              3        0.6%           3        0.6%    1.00
	[OHOS INFO] hdf                       54        11.5%         54        11.5%   1.00
	[OHOS INFO] hiviewdfx                 12        2.5%          12        2.5%    1.00
	[OHOS INFO] kernel                    32        6.8%          32        6.8%    1.00
	[OHOS INFO] securec                   39        8.3%          39        8.3%    1.00
	[OHOS INFO] security                  54        11.5%         54        11.5%   1.00
	[OHOS INFO] startup                    6        1.3%           6        1.3%    1.00
	[OHOS INFO] systemabilitymgr          15        3.2%          15        3.2%    1.00
	[OHOS INFO] test                      60        12.7%         60        12.7%   1.00
	[OHOS INFO] third_party              215        45.6%        215        45.6%   1.00
	[OHOS INFO] thirdparty               215        45.6%        215        45.6%   1.00
	[OHOS INFO] updater                    4        0.8%           4        0.8%    1.00
	[OHOS INFO] xts                       60        12.7%         60        12.7%   1.00
	[OHOS INFO] 
	[OHOS INFO] c overall build overlap rate: 1.00
	[OHOS INFO] 
	[OHOS INFO] 
	[OHOS INFO] xts_demo build success
	[OHOS INFO] cost time: 0:00:57
```

## 烧录流程

-
	打开烧录工具[BEKEN_WRITER_V1.6.38](http://dl.bekencorp.com/tools/flash/)。
-
	烧录文件选择openharmony\device\board\beken\bk7235x\liteos_m\build\all_2M.1220.bin。
-
	选择串口，点击烧录。
-
	等待出现“下载成功”字样。
-
	开发板重新上电。


## 相关仓

[vendor_beken](https://gitee.com/openharmony-sig/vendor_beken)

[device_board_beken](https://gitee.com/openharmony-sig/device_board_beken)
