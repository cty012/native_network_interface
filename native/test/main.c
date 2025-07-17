#include <stdio.h>
#include <stdlib.h>
#include "network_interface.h"

/**
 * @brief A helper function to print the MAC address in a standard format.
 * @param mac The 6-byte array containing the MAC address.
 */
void print_mac_address(const uint8_t mac[6]) {
    // Check if the MAC address is all zeros (i.e., not available)
    bool is_null = true;
    for (int i = 0; i < 6; i++) {
        if (mac[i] != 0) {
            is_null = false;
            break;
        }
    }

    if (is_null) {
        printf("N/A\n");
    } else {
        printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
}

int main(void) {
    NetworkInterface *interfaces = NULL;
    size_t count = 0;

    // Call the main library function to get all network interfaces
    printf("Fetching network interfaces...\n");
    if (get_network_interfaces(&interfaces, &count) == -1) {
        perror("Failed to get network interfaces");
        return EXIT_FAILURE;
    }

    printf("Found %zu network interfaces.\n\n", count);

    // Iterate through the array of interfaces and print their details
    for (size_t i = 0; i < count; i++) {
        NetworkInterface *iface = &interfaces[i];

        printf("----------------------------------------\n");
        printf(" Interface: %s\n", iface->name);
        printf("----------------------------------------\n");

        printf("  Index:       %d\n", iface->index);
        printf("  Description: %s\n", iface->description);
        printf("  MTU:         %d\n", iface->mtu);

        printf("  MAC Address: ");
        print_mac_address(iface->mac_addr);

        printf("\n  Flags:\n");
        printf("    Is Up:       %s\n", iface->is_up ? "Yes" : "No");
        printf("    Is Running:  %s\n", iface->is_running ? "Yes" : "No");
        printf("    Is Loopback: %s\n", iface->is_loopback ? "Yes" : "No");
        printf("    Broadcast:   %s\n", iface->is_broadcast ? "Yes" : "No");
        printf("    Multicast:   %s\n", iface->is_multicast ? "Yes" : "No");

        printf("\n  IPv4 Addresses (%zu):\n", iface->ipv4_count);
        if (iface->ipv4_count == 0) {
            printf("    (None)\n");
        }
        for (size_t j = 0; j < iface->ipv4_count; j++) {
            printf("    - %s/%d\n", iface->ipv4[j].ipv4_addr, iface->ipv4[j].prefix_length);
        }

        printf("\n  IPv6 Addresses (%zu):\n", iface->ipv6_count);
        if (iface->ipv6_count == 0) {
            printf("    (None)\n");
        }
        for (size_t j = 0; j < iface->ipv6_count; j++) {
            printf("    - %s/%d\n", iface->ipv6[j].ipv6_addr, iface->ipv6[j].prefix_length);
        }
        printf("\n");
    }

    // IMPORTANT: Free the memory allocated by the library
    printf("Cleaning up memory...\n");
    free_network_interfaces(interfaces, count);
    printf("Done.\n");

    return EXIT_SUCCESS;
}
