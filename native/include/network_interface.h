#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT
#endif

typedef struct IPv4 {
    char *ipv4_addr;               // IPv4 address
    int prefix_length;             // Prefix length for the IPv4 address
} IPv4;

typedef struct IPv6 {
    char *ipv6_addr;               // IPv6 address
    int prefix_length;             // Prefix length for the IPv6 address
} IPv6;

typedef struct NetworkInterface {
    int index;                     // Index of the network interface
    char *name;                    // Name of the network interface
    char *description;             // Description of the network interface

    IPv4 *ipv4;                    // IPv4 address of the interface
    size_t ipv4_count;             // Number of IPv4 addresses
    IPv6 *ipv6;                    // IPv6 address of the interface
    size_t ipv6_count;             // Number of IPv6 addresses
    uint8_t mac_addr[6];           // MAC address of the interface (all zeros if not available)

    bool is_up;                    // Whether the interface is up
    bool is_running;               // Whether the interface is running
    bool is_loopback;              // Whether the interface is a loopback interface
    bool is_broadcast;             // Whether the interface is a broadcast interface
    bool is_multicast;             // Whether the interface supports multicast
    int mtu;                       // Maximum Transmission Unit size of the interface (0: unknown)
} NetworkInterface;

FFI_PLUGIN_EXPORT int get_network_interfaces(NetworkInterface **interfaces, size_t *count);
FFI_PLUGIN_EXPORT void free_ipv4(IPv4 *ipv4, size_t count);
FFI_PLUGIN_EXPORT void free_ipv6(IPv6 *ipv6, size_t count);
FFI_PLUGIN_EXPORT void free_network_interfaces(NetworkInterface *interfaces, size_t count);
