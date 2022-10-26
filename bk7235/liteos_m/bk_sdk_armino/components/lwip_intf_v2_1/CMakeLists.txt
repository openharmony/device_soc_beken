set(incs)
set(srcs)

if(CONFIG_HARMONY_LWIP)
	set(lwip_dir "../../../../../../../../third_party/lwip")
	set(liteos_dir "../../../../../../../../kernel/liteos_m")
	set(extra_dir "../../../../../../../../device/soc/beken/bk7235/liteos_m")
else()
	set(lwip_dir "lwip-2.1.2")
endif()

if (CONFIG_LWIP_V2_1)
	list(APPEND incs
	${lwip_dir}/src
	lwip-2.1.2/port
	${lwip_dir}/src/include
	${lwip_dir}/src/include/netif
	${lwip_dir}/src/include/lwip
	${liteos_dir}/arch/risc-v/riscv32/gcc 
	${liteos_dir}/arch/risc-v/riscv32/gcc/asm 
	${liteos_dir}/arch/include 
	${liteos_dir}/kernel/include
	${liteos_dir}/kal/cmsis
	${liteos_dir}/kal/posix/include 
	${liteos_dir}/components/power 
	${liteos_dir}/components/signal 
	${liteos_dir}/utils 
	${extra_dir}/inc 
	)

	list(APPEND srcs
	lwip-2.1.2/port/ethernetif.c
	lwip-2.1.2/port/net.c
	lwip-2.1.2/port/sys_arch.c
	lwip-2.1.2/port/sockets.c
	#lwip-2.1.2/port/ping.c
	${lwip_dir}/src/api/api_lib.c
	${lwip_dir}/src/api/api_msg.c
	${lwip_dir}/src/api/err.c
	${lwip_dir}/src/api/netbuf.c
	${lwip_dir}/src/api/netdb.c
	${lwip_dir}/src/api/netifapi.c
	#${lwip_dir}/src/api/sockets.c
	${lwip_dir}/src/api/tcpip.c
	${lwip_dir}/src/apps/mdns/mdns.c
	#lwip-2.1.2/src/apps/ping/ping.c
	${lwip_dir}/src/core/def.c
	${lwip_dir}/src/core/dns.c
	${lwip_dir}/src/core/inet_chksum.c
	${lwip_dir}/src/core/init.c
	${lwip_dir}/src/core/ip.c
	${lwip_dir}/src/core/ipv4/autoip.c
	${lwip_dir}/src/core/ipv4/dhcp.c
	${lwip_dir}/src/core/ipv4/etharp.c
	${lwip_dir}/src/core/ipv4/icmp.c
	${lwip_dir}/src/core/ipv4/igmp.c
	${lwip_dir}/src/core/ipv4/ip4_addr.c
	${lwip_dir}/src/core/ipv4/ip4.c
	${lwip_dir}/src/core/ipv4/ip4_frag.c
	${lwip_dir}/src/core/ipv6/dhcp6.c
	${lwip_dir}/src/core/ipv6/ethip6.c
	${lwip_dir}/src/core/ipv6/icmp6.c
	${lwip_dir}/src/core/ipv6/inet6.c
	${lwip_dir}/src/core/ipv6/ip6_addr.c
	${lwip_dir}/src/core/ipv6/ip6.c
	${lwip_dir}/src/core/ipv6/ip6_frag.c
	${lwip_dir}/src/core/ipv6/mld6.c
	${lwip_dir}/src/core/ipv6/nd6.c
	${lwip_dir}/src/core/mem.c
	${lwip_dir}/src/core/memp.c
	${lwip_dir}/src/core/netif.c
	${lwip_dir}/src/core/pbuf.c
	${lwip_dir}/src/core/raw.c
	${lwip_dir}/src/core/stats.c
	${lwip_dir}/src/core/sys.c
	${lwip_dir}/src/core/tcp.c
	${lwip_dir}/src/core/tcp_in.c
	${lwip_dir}/src/core/tcp_out.c
	${lwip_dir}/src/core/timeouts.c
	${lwip_dir}/src/core/udp.c
	${lwip_dir}/src/netif/ethernet.c
	dhcpd/dhcp-server.c
	dhcpd/dhcp-server-main.c
	)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES driver
	PRIV_REQUIRES bk_common bk_wifi
	)
