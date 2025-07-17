#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "name.h"

int get_unique_interface_names_list(NameNode **head, size_t *count, struct ifaddrs *ifaddr) {
    *head = NULL;
    *count = 0;

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_name == NULL) {
            continue;
        }

        // Check if the name already exists in our list
        NameNode *name_node = find_name_node_by_name(*head, ifa->ifa_name);

        // If the name is not found, create a new node for the unique name
        if (name_node == NULL) {
            name_node = malloc(sizeof(NameNode));
            if (name_node == NULL) {
                // Handle memory allocation failure
                free_name_list(*head);
                *head = NULL;
                *count = 0;
                return -1;
            }
            
            // Add the new node to the head of the list
            name_node->name = strdup(ifa->ifa_name);
            if (name_node->name == NULL) {
                // Handle memory allocation failure
                free(name_node);
                free_name_list(*head);
                *head = NULL;
                *count = 0;
                return -1;
            }
            name_node->next = *head;
            name_node->ipv4_count = 0;
            name_node->ipv6_count = 0;
            *head = name_node;
            (*count)++;
        }

        // Increment the count of IPv4 or IPv6 addresses associated with this name
        if (ifa->ifa_addr != NULL) {
            if (ifa->ifa_addr->sa_family == AF_INET) {
                name_node->ipv4_count++;
            } else if (ifa->ifa_addr->sa_family == AF_INET6) {
                name_node->ipv6_count++;
            }
        }
    }

    return 0; // Success
}

void free_name_list(NameNode* head) {
    while (head != NULL) {
        NameNode* next = head->next;
        free(head->name);
        free(head);
        head = next;
    }
}

NameNode *find_name_node_by_name(NameNode *head, const char *name) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}
