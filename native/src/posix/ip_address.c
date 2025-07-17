#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <stddef.h>
#include <string.h>
#include "ip_address.h"

int count_prefix_length_ipv4(in_addr_t mask) {
    mask = ntohl(mask); // Convert from network to host byte order
    int count = 0;
    while (mask > 0) {
        mask &= (mask - 1); // This clever trick clears the least significant set bit
        count++;
    }
    return count;
}

int count_prefix_length_ipv6(struct in6_addr mask) {
    int count = 0;
    for (int i = 0; i < 16; i++) { // Iterate over 16 bytes
        uint8_t byte = mask.s6_addr[i];
        while (byte > 0) {
            byte &= (byte - 1);
            count++;
        }
    }
    return count;
}

int get_ipv4_address(struct ifaddrs *ifa, IPv4 *ipv4) {
    // Get the address string
    struct sockaddr_in *sa_in = (struct sockaddr_in *)ifa->ifa_addr;
    char addr_buffer[NI_MAXHOST];
    inet_ntop(AF_INET, &sa_in->sin_addr, addr_buffer, sizeof(addr_buffer));
    ipv4->ipv4_addr = strdup(addr_buffer);

    // Get the prefix length from the netmask
    if (ifa->ifa_netmask != NULL) {
        struct sockaddr_in *netmask_in = (struct sockaddr_in *)ifa->ifa_netmask;
        ipv4->prefix_length = count_prefix_length_ipv4(netmask_in->sin_addr.s_addr);
    } else {
        ipv4->prefix_length = 0;
    }

    return 0;
}

int get_ipv6_address(struct ifaddrs *ifa, IPv6 *ipv6) {
    // Get the address string
    struct sockaddr_in6 *sa_in6 = (struct sockaddr_in6 *)ifa->ifa_addr;
    char addr_buffer[NI_MAXHOST];
    inet_ntop(AF_INET6, &sa_in6->sin6_addr, addr_buffer, sizeof(addr_buffer));
    ipv6->ipv6_addr = strdup(addr_buffer);

    // Get the prefix length from the netmask
    if (ifa->ifa_netmask != NULL) {
        struct sockaddr_in6 *netmask_in6 = (struct sockaddr_in6 *)ifa->ifa_netmask;
        ipv6->prefix_length = count_prefix_length_ipv6(netmask_in6->sin6_addr);
    } else {
        ipv6->prefix_length = 0;
    }

    return 0;
}
