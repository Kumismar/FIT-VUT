from scapy.all import Ether, IP, UDP, BOOTP, DHCP, wrpcap

ADDRESSES1 = [f"192.168.0.{i}" for i in range(255)]
ADDRESSES2 = [f"192.168.0.{i//2}" for i in range(300) if i % 2 != 0]
ADDRESSES3 = [f"10.0.1.{i}" for i in range(20)]
ADDRESSES4 = [f"10.0.1.{i}" for i in [1, 4, 9, 16]]
ADDRESSES5 = [f"10.0.1.{i}" for i in [9]]
ADDRESSES6 = [f"192.168.0.{i}" for i in range(100, 150)]


def create_dhcprelease_packet(address):
    eth = Ether(dst="ff:ff:ff:ff:ff:ff", src="00:11:22:33:44:55")
    ip = IP(src="192.168.1.1", dst="255.255.255.255")
    udp = UDP(sport=68, dport=67)
    bootp = BOOTP(op=1, ciaddr=address, chaddr="00:11:22:33:44:55")
    dhcp = DHCP(options=[("message-type", "release"), "end"])

    packet = eth / ip / udp / bootp / dhcp
    return packet


def create_dhcpack_packet(address):
    eth = Ether(dst="ff:ff:ff:ff:ff:ff", src="00:11:22:33:44:55")
    ip = IP(src="192.168.1.1", dst="255.255.255.255")
    udp = UDP(sport=67, dport=68)
    bootp = BOOTP(op=2, yiaddr=address, chaddr="00:11:22:33:44:55")
    dhcp = DHCP(options=[("message-type", "ack"), "end"])

    packet = eth / ip / udp / bootp / dhcp
    return packet


if __name__ == "__main__":
    ackpackets1 = [create_dhcpack_packet(address) for address in ADDRESSES1]
    releasepackets1 = [create_dhcprelease_packet(address) for address in ADDRESSES2]
    ackpackets2 = [create_dhcpack_packet(address) for address in ADDRESSES3]
    releasepackets2 = [create_dhcprelease_packet(address) for address in ADDRESSES4]
    ackpackets3 = [create_dhcpack_packet(address) for address in ADDRESSES5]
    ackpackets4 = [create_dhcpack_packet(address) for address in ADDRESSES6]

    packets = ackpackets1 + releasepackets1 + ackpackets2 + releasepackets2 + ackpackets3 + ackpackets4
    wrpcap('dhcp.pcap', packets)
