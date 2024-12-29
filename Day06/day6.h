#ifndef DAY6_H_
#define DAY6_H_

#include <stddef.h>

typedef struct GraphElem GraphElem;

typedef struct {
    size_t length;
    size_t capacity;
    GraphElem *items;
} Graph;

Graph graph_init(char *filepath);
void graph_free(Graph g);

size_t get_orbit_checksum(Graph g);
size_t get_orbital_transfer_count(Graph g);

#endif // DAY6_H_
