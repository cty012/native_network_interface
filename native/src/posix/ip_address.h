#pragma once

#include <netinet/in.h>
#include "network_interface.h"

// Helper function to calculate prefix length from an IPv4 netmask
int count_prefix_length_ipv4(in_addr_t mask);

// Helper function to calculate prefix length from an IPv6 netmask
int count_prefix_length_ipv6(struct in6_addr mask);

/**
 * Get the IPv4 address from a struct ifaddrs.
 *
 * @param ifa The ifaddrs structure.
 * @param ipv4 The IPv4 structure to populate.
 * @return 0 on success, -1 on error.
 */
int get_ipv4_address(struct ifaddrs *ifa, IPv4 *ipv4);

/**
 * Get the IPv6 address from a struct ifaddrs.
 *
 * @param ifa The ifaddrs structure.
 * @param ipv6 The IPv6 structure to populate.
 * @return 0 on success, -1 on error.
 */
int get_ipv6_address(struct ifaddrs *ifa, IPv6 *ipv6);
