#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include "mac_address.h"

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)

#include <net/if_dl.h>

void get_mac_address(struct ifaddrs *ifa, unsigned char mac[6]) {
    struct sockaddr_dl *sdl = (struct sockaddr_dl *)ifa->ifa_addr;
    if (sdl->sdl_alen == 6) { // MAC addresses are 6 bytes
        memcpy(mac, LLADDR(sdl), 6);
    }
}

#else

#include <netpacket/packet.h>

void get_mac_address(struct ifaddrs *ifa, unsigned char mac[6]) {
    struct sockaddr_ll *sll = (struct sockaddr_ll *)ifa->ifa_addr;
    if (sll->sll_halen == 6) { // MAC addresses are 6 bytes
        memcpy(mac, sll->sll_addr, 6);
    }
}

#endif

bool is_valid_mac_address(const unsigned char mac[6]) {
    if (mac == NULL) {
        return false;
    }
    // A simple check: MAC address should not be all zeros
    for (int i = 0; i < 6; i++) {
        if (mac[i] != 0) {
            return true;
        }
    }
    return false;
}
