#pragma once

#include <ifaddrs.h>
#include <stdbool.h>

/**
 * Get the MAC address of a network interface. Assumes the ifaddrs contains a MAC address.
 *
 * @param ifaddr The ifaddrs structure containing the interface information.
 * @param mac The buffer to store the MAC address (6 bytes).
 */
void get_mac_address(struct ifaddrs *ifaddr, unsigned char mac[6]);

/**
 * Check if a MAC address is valid (not all zeros).
 *
 * @param mac The MAC address to check (6 bytes).
 * @return true if the MAC address is valid, false otherwise.
 */
bool is_valid_mac_address(const unsigned char mac[6]);
