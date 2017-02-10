#ifndef _PACKET_H_
#define _PACKET_H_

/* Layer 2 Macro */
#define MD_IPV4                 0x0800
#define MD_IPV6                 0x86DD
#define MD_MPLS                 0x8847

/* pkt protocol definitions */
#define MD_PROTO_TCP            6
#define MD_PROTO_UDP            17

/* TCP Flags */
#define MD_TCPFLG_NONE          0x00
#define MD_TCPFLG_FIN           0x01
#define MD_TCPFLG_SYN           0x02
#define MD_TCPFLG_RST           0x04
#define MD_TCPFLG_PSH           0x08
#define MD_TCPFLG_ACK           0x10
#define MD_TCPFLG_URG           0x20


#endif /* _PACKET_H_ */
