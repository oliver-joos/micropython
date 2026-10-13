#ifndef LISA_INCLUDED_STM32_LWIP_LWIPOPTS_H
#define LISA_INCLUDED_STM32_LWIP_LWIPOPTS_H


#include <stdint.h>

// This is needed to access `next_timeout` via `sys_timeouts_get_next_timeout()`.
#define LWIP_TESTMODE                   1

// This protection is not needed, instead we execute all lwIP code at PendSV priority
#define SYS_ARCH_DECL_PROTECT(lev) do { } while (0)
#define SYS_ARCH_PROTECT(lev) do { } while (0)
#define SYS_ARCH_UNPROTECT(lev) do { } while (0)

#define NO_SYS                          1
#define SYS_LIGHTWEIGHT_PROT            1
#define MEM_ALIGNMENT                   4

#define LWIP_CHKSUM_ALGORITHM           3
#define LWIP_CHECKSUM_CTRL_PER_NETIF    1

#define LWIP_ARP                        1
#define LWIP_ETHERNET                   1
#define LWIP_RAW                        1
#define LWIP_NETCONN                    0
#define LWIP_SOCKET                     0
#ifdef MODULE_LWIP_STATS_ENABLED
#define LWIP_STATS                      1
#define LWIP_STATS_DISPLAY              1
#define MIB2_STATS                      1
#endif
#define LWIP_NETIF_HOSTNAME             1
#define LWIP_NETIF_EXT_STATUS_CALLBACK  1

#define LWIP_IPV6                       0
#define LWIP_DHCP                       1
#define LWIP_DHCP_CHECK_LINK_UP         1
#define LWIP_DHCP_DOES_ACD_CHECK        0 // 0 to speed DHCP up
#define LWIP_DNS                        1
#define LWIP_DNS_SUPPORT_MDNS_QUERIES   0
#define LWIP_MDNS_RESPONDER             0
#define LWIP_IGMP                       1

#define LWIP_NUM_NETIF_CLIENT_DATA      LWIP_MDNS_RESPONDER
#define MEMP_NUM_SYS_TIMEOUT            (LWIP_NUM_SYS_TIMEOUT_INTERNAL + LWIP_MDNS_RESPONDER)

#define SO_REUSE                        1
#define TCP_LISTEN_BACKLOG              1

extern uint32_t mp_hal_get_hw_random_u32(void);
#define LWIP_RAND() mp_hal_get_hw_random_u32()

// lwip takes 26700 bytes; TCP dl/ul are around 750/600 k/s on local network
#define MEM_SIZE (8000)
#define TCP_MSS (800)
#define TCP_WND (8 * TCP_MSS)
#define TCP_SND_BUF (8 * TCP_MSS)
#define MEMP_NUM_TCP_SEG (32)

#define TCP_TMR_INTERVAL                64
#define MEMP_NUM_RAW_PCB                1
#define MEMP_NUM_TCP_PCB                30
#define MEMP_NUM_TCP_PCB_LISTEN         4
#define MEMP_NUM_UDP_PCB                (8 + LWIP_MDNS_RESPONDER)

typedef uint32_t sys_prot_t;


#if LWIP_DEBUG
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_OFF    //  show this level and above
#endif

// #define ETHARP_DEBUG                    LWIP_DBG_ON
#define NETIF_DEBUG                     LWIP_DBG_ON
// #define PBUF_DEBUG                      LWIP_DBG_ON
// #define API_LIB_DEBUG                   LWIP_DBG_ON
// #define API_MSG_DEBUG                   LWIP_DBG_ON

#define SOCKETS_DEBUG                   LWIP_DBG_ON
// #define ICMP_DEBUG                      LWIP_DBG_ON
// #define IGMP_DEBUG                      LWIP_DBG_ON
// #define INET_DEBUG                      LWIP_DBG_ON
// #define IP_DEBUG                        LWIP_DBG_ON
// #define IP_REASS_DEBUG                  LWIP_DBG_ON
// #define RAW_DEBUG                       LWIP_DBG_ON
// #define MEM_DEBUG                       LWIP_DBG_ON
// #define MEMP_DEBUG                      LWIP_DBG_ON
// #define SYS_DEBUG                       LWIP_DBG_ON
// #define TIMERS_DEBUG                    LWIP_DBG_ON

#define UDP_DEBUG                       LWIP_DBG_ON
// #define TCP_DEBUG                       LWIP_DBG_ON
#define TCP_INPUT_DEBUG                 LWIP_DBG_ON
// #define TCP_FR_DEBUG                    LWIP_DBG_ON
// #define TCP_RTO_DEBUG                   LWIP_DBG_ON
// #define TCP_CWND_DEBUG                  LWIP_DBG_ON
// #define TCP_WND_DEBUG                   LWIP_DBG_ON
#define TCP_OUTPUT_DEBUG                LWIP_DBG_ON
// #define TCP_RST_DEBUG                   LWIP_DBG_ON
// #define TCP_QLEN_DEBUG                  LWIP_DBG_ON

// #define TCPIP_DEBUG                     LWIP_DBG_ON
// #define SLIP_DEBUG                      LWIP_DBG_ON
// #define DHCP_DEBUG                      LWIP_DBG_ON
// #define AUTOIP_DEBUG                    LWIP_DBG_ON
// #define DNS_DEBUG                       LWIP_DBG_ON
// #define IP6_DEBUG                       LWIP_DBG_ON
// #define DHCP6_DEBUG                     LWIP_DBG_ON


#endif // LISA_INCLUDED_STM32_LWIP_LWIPOPTS_H
