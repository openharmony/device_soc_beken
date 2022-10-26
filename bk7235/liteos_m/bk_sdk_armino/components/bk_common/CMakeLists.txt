set(incs include)
set(srcs)

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	# REQUIRES freertos_v9 app bk_event bk_rtos driver soc lwip_intf_v2_0 lwip_intf_v2_1 hal soc bk_log bk_ps bk_init
	REQUIRES release bk_startup os_source bk_event bk_os driver lwip_intf_v2_1 bk_log bk_system bk_ps bk_init bk_wifi bk_cli
	)
#app
