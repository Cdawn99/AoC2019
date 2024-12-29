#include "day6.h"
#include "../dawn_utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

struct GraphElem {
    uint64_t name;

    size_t length;
    size_t capacity;
    uint64_t *items;
};

typedef struct {
    uint64_t name;
    size_t depth;
} Depth;

typedef struct {
    size_t length;
    size_t capacity;
    Depth *items;
} DepthList;

static uint64_t get_name(char *s) {
    uint64_t name = 0;
    while (*s != '\0') {
        name *= 36;
        if ('0' <= *s && *s <= '9') {
            name += *s - '0';
        } else {
            name += *s - 'A' + 10;
        }
        s++;
    }
    return name;
}

static bool graph_contains(Graph g, uint64_t name) {
    for (size_t i = 0; i < g.length; i++) {
        if (g.items[i].name == name) return true;
    }
    return false;
}

Graph graph_init(char *filepath) {
    Graph g = {0};

    DawnStringBuilder info = {0};
    if (!dawn_read_entire_file(filepath, &info)) return g;
    DAWN_SB_APPEND_BUF(&info, "", 1);

    char delim[] = ")\n";
    char *token = strtok(info.items, delim);
    while (token) {
        uint64_t name = get_name(token);
        if (!graph_contains(g, name)) {
            GraphElem e = {.name = name};
            DAWN_DA_APPEND(&g, e);
        }
        token = strtok(NULL, delim);
        uint64_t adj_name = get_name(token);
        if (!graph_contains(g, adj_name)) {
            GraphElem e = {.name = adj_name};
            DAWN_DA_APPEND(&g, e);
        }
        token = strtok(NULL, delim);

        GraphElem *e = g.items;
        while (e->name != name) e++;
        DAWN_DA_APPEND(e, adj_name);

        e = g.items;
        while (e->name != adj_name) e++;
        DAWN_DA_APPEND(e, name);
    }

    DAWN_SB_FREE(info);

    return g;
}

void graph_free(Graph g) {
    if (g.items) {
        for (size_t i = 0; i < g.length; i++) {
            DAWN_DA_FREE(g.items[i]);
        }
    }
    DAWN_DA_FREE(g);
}

static GraphElem get_elem(Graph g, uint64_t name) {
    for (size_t i = 0; i < g.length; i++) {
        if (g.items[i].name == name) {
            return g.items[i];
        }
    }
    return (GraphElem){0};
}

static bool depth_list_contains(DepthList dl, uint64_t name) {
    for (size_t i = 0; i < dl.length; i++) {
        if (dl.items[i].name == name) return true;
    }
    return false;
}

size_t get_orbit_checksum(Graph g) {
    size_t checksum = 0;

    DepthList dl = {0};

    GraphElem ge = get_elem(g, get_name("COM"));
    Depth d = {.name = ge.name};
    DAWN_DA_APPEND(&dl, d);

    for (size_t i = 0; i < dl.length; i++) {
        d = dl.items[i];
        ge = get_elem(g, d.name);
        for (size_t j = 0; j < ge.length; j++) {
            Depth nd = {.name = ge.items[j], .depth = d.depth + 1};
            if (!depth_list_contains(dl, nd.name)) {
                DAWN_DA_APPEND(&dl, nd);
            }
        }
        checksum += d.depth;
    }

    DAWN_DA_FREE(dl);

    return checksum;
}

size_t get_orbital_transfer_count(Graph g) {
    uint64_t you = get_name("YOU");
    uint64_t san = get_name("SAN");

    DepthList dl = {0};

    GraphElem ge = get_elem(g, you);
    Depth d = {.name = ge.name};
    DAWN_DA_APPEND(&dl, d);

    size_t santa_distance = 0;
    bool santa_found = false;
    for (size_t i = 0; i < dl.length; i++) {
        d = dl.items[i];
        ge = get_elem(g, d.name);
        for (size_t j = 0; j < ge.length; j++) {
            Depth nd = {.name = ge.items[j], .depth = d.depth + 1};
            if (nd.name == san) {
                santa_found = true;
                santa_distance = nd.depth;
                break;
            }
            if (!depth_list_contains(dl, nd.name)) {
                DAWN_DA_APPEND(&dl, nd);
            }
        }
        if (santa_found) break;
    }

    DAWN_DA_FREE(dl);
    assert(santa_found && "Something went very wrong; check that you (YOU) and santa (SAN) are in the input");

    return santa_distance - 2;
}
