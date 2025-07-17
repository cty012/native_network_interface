#define _GNU_SOURCE

#include <ifaddrs.h>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ip_address.h"
#include "mac_address.h"
#include "name.h"
#include "network_interface.h"

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#define AF_MAC AF_LINK // For Darwin/BSD compatibility
#else
#define AF_MAC AF_PACKET // For Linux compatibility
#endif

NetworkInterface *find_network_interface_by_name(NetworkInterface *interfaces, size_t count, const char *name) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(interfaces[i].name, name) == 0) {
            return &interfaces[i];
        }
    }
    return NULL; // Not found
}

int get_network_interfaces(NetworkInterface **interfaces, size_t *count) {
    *interfaces = NULL;
    *count = 0;

    // Get the list of network interfaces
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        return -1; // Error retrieving network interfaces
    }
    
    // Get the list of unique interface names
    NameNode *name_list = NULL;
    size_t n = 0;
    if (get_unique_interface_names_list(&name_list, &n, ifaddr) == -1) {
        freeifaddrs(ifaddr);
        return -1; // Error retrieving unique interface names
    }

    // If no interfaces found, return early
    if (n == 0) {
        free_name_list(name_list);
        freeifaddrs(ifaddr);
        return 0; // Success, but no interfaces found
    }

    // Allocate memory for the interfaces
    *interfaces = malloc(n * sizeof(NetworkInterface));
    if (*interfaces == NULL) {
        free_name_list(name_list);
        freeifaddrs(ifaddr);
        return -1; // Memory allocation error
    }
    *count = n;

    // Create a socket to use for ioctl calls
    int sock = -1;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        // Error creating socket
        free_network_interfaces(*interfaces, 0);
        free_name_list(name_list);
        freeifaddrs(ifaddr);
        return -1;
    }

    // Fill the interfaces
    size_t curr_count = 0;
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_name == NULL) {
            continue;
        }

        // Find the corresponding name node (must exist)
        NameNode *name_node = find_name_node_by_name(name_list, ifa->ifa_name);
        NetworkInterface *iface = find_network_interface_by_name(*interfaces, curr_count, ifa->ifa_name);
        
        // If the interface is not found, create a new one
        if (iface == NULL) {
            iface = &(*interfaces)[curr_count++];

            // Index, name, and description
            iface->index = if_nametoindex(ifa->ifa_name);
            iface->name = strdup(ifa->ifa_name);
            iface->description = strdup(ifa->ifa_name); // POSIX does not provide a description, using name as a placeholder

            // IPv4 and IPv6 addresses
            iface->ipv4 = malloc(name_node->ipv4_count * sizeof(IPv4));
            iface->ipv4_count = 0;
            iface->ipv6 = malloc(name_node->ipv6_count * sizeof(IPv6));
            iface->ipv6_count = 0;
            if (iface->ipv4 == NULL || iface->ipv6 == NULL) {
                close(sock);
                free_network_interfaces(*interfaces, curr_count);
                free_name_list(name_list);
                freeifaddrs(ifaddr);
                return -1; // Memory allocation error
            }

            // Clear MAC address
            memset(iface->mac_addr, 0, sizeof(iface->mac_addr));

            // Interface flags
            iface->is_up = (ifa->ifa_flags & IFF_UP) != 0;
            iface->is_running = (ifa->ifa_flags & IFF_RUNNING) != 0;
            iface->is_loopback = (ifa->ifa_flags & IFF_LOOPBACK) != 0;
            iface->is_broadcast = (ifa->ifa_flags & IFF_BROADCAST) != 0;
            iface->is_multicast = (ifa->ifa_flags & IFF_MULTICAST) != 0;

            // MTU
            struct ifreq ifr;
            strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ-1);
            ifr.ifr_name[IFNAMSIZ-1] = '\0';
            if (ioctl(sock, SIOCGIFMTU, &ifr) == -1) {
                iface->mtu = 0; // Unknown MTU
            } else {
                iface->mtu = ifr.ifr_mtu;
            }
        }

        // Add the IPv4 or IPv6 address if available
        if (ifa->ifa_addr != NULL) {
            if (ifa->ifa_addr->sa_family == AF_INET) {
                // IPv4 address
                get_ipv4_address(ifa, &iface->ipv4[iface->ipv4_count++]);
            } else if (ifa->ifa_addr->sa_family == AF_INET6) {
                // IPv6 address
                get_ipv6_address(ifa, &iface->ipv6[iface->ipv6_count++]);
            } else if (ifa->ifa_addr->sa_family == AF_MAC) {
                // MAC address (Linux uses AF_PACKET, BSD/Darwin uses AF_LINK)
                if (!is_valid_mac_address(iface->mac_addr)) {
                    get_mac_address(ifa, iface->mac_addr);
                }
            }
        }

    }

    // Clean up
    close(sock);
    free_name_list(name_list);
    freeifaddrs(ifaddr);
    return 0; // Success
}

void free_ipv4(IPv4 *ipv4, size_t count) {
    if (ipv4 == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(ipv4[i].ipv4_addr);
    }
    free(ipv4);
}

void free_ipv6(IPv6 *ipv6, size_t count) {
    if (ipv6 == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(ipv6[i].ipv6_addr);
    }
    free(ipv6);
}

void free_network_interfaces(NetworkInterface *interfaces, size_t count) {
    if (interfaces == NULL) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        NetworkInterface *iface = &interfaces[i];
        free(iface->name);
        free(iface->description);
        free_ipv4(iface->ipv4, iface->ipv4_count);
        free_ipv6(iface->ipv6, iface->ipv6_count);
    }

    free(interfaces);
}
