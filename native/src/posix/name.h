#pragma once

#include <ifaddrs.h>
#include <stddef.h>

/**
 * @brief A node in a linked list of strings.
 */
typedef struct NameNode {
    struct NameNode *next;
    char *name;
    size_t ipv4_count; // Count of IPv4 addresses associated with this name
    size_t ipv6_count; // Count of IPv6 addresses associated with this name
} NameNode;

/**
 * @brief Extracts a linked list of unique interface names from an ifaddrs list.
 *
 * @param head Pointer to the head of the linked list to be filled.
 * @param count Pointer to store the count of unique names.
 * @param ifaddr The head of the linked list from getifaddrs().
 * @return 0 on success, -1 on error.
 * @note The caller is responsible for freeing the list using free_name_list().
 */
int get_unique_interface_names_list(NameNode **head, size_t *count, struct ifaddrs *ifaddr);

/**
 * @brief Frees the memory allocated for a NameNode linked list.
 *
 * @param head The head of the linked list to free.
 */
void free_name_list(NameNode *head);

/**
 * @brief Finds a node in the linked list by name.
 *
 * @param head The head of the linked list.
 * @param name The name to search for.
 * @return Pointer to the found node, or NULL if not found.
 */
NameNode *find_name_node_by_name(NameNode *head, const char *name);
