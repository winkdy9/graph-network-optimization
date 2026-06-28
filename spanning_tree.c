// Функция для построения минимального остовного дерева из ориентированного графа
#include <math.h>
#include "Graph/graph.h"
#include "disjoint_set.h"
#include "doubly_list/generic.h"
#include <stdlib.h>

#define MAX_LINE 256

typedef struct NodeWithDistance {
    long node_id;
    double distance;
} NodeWithDistance;

typedef struct {
    long from;
    long to;
    double weight;
} SEdge;


Vector* get_all_edges(Graph* g) {
    Vector* edges = createVector(sizeof(SEdge));
    if (!edges) {
        return NULL;
    }

    for (size_t i = 0; i < g->nodes->capacity; ++i) {
        Vector **bucket_ptr = get_bucket_ptr(g->nodes, i);
        Vector *bucket = *bucket_ptr;

        if (!bucket) {
            continue;
        }

        for (size_t j = 0; j < bucket->size; ++j) {
            void* elem = getVectorItem(bucket, j);
            GNode* node = (GNode*)((char*)elem + sizeof(long long));

            GEdge* e = node->edges;

            while (e) {
                SEdge ed;
                ed.from = e->from;
                ed.to = e->to;
                ed.weight = e->lenght;

                appendVectorItem(edges, &ed);

                e = e->next;
            }
        }
    }

    return edges;
}

int cmp_edges(const void* a, const void* b) {
    SEdge* ea = (SEdge*)a;
    SEdge* eb = (SEdge*)b;

    if (ea->weight < eb->weight) {
        return -1;
    }
    if (ea->weight > eb->weight) {
        return 1;
    }
    return 0;
}


void write_edges_to_file(Vector* edges, char* path) {
    FILE* f = fopen(path, "wb");
    if (!f) {
        return;
    }

    fprintf(f, "node_id_from\tnode_id_to\tweight\n");

    for (size_t i = 0; i < edges->size; i++) {
        SEdge* e = (SEdge*)getVectorItem(edges, i);

        fprintf(f, "%ld\t%ld\t%lf\n", e->from, e->to, e->weight);
    }


    fclose(f);
}

Graph* read_buildings(char* path) {
    Graph* g = createGraph();
    if (!g) {
        return NULL;
    }

    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Error: cannot open file %s\n", path);
        return NULL;
    }

    char buffer[512];

    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file)) {
        char* token;

        token = strtok(buffer, "\t");
        if (!token) {
            continue;
        }
        long long id = atoll(token);

        token = strtok(NULL, "\t");
        if (!token) {
            continue;
        }

        token = strtok(NULL, "\t");
        if (!token) {
            continue;
        }
        double lat = atof(token);

        token = strtok(NULL, "\t\n");
        if (!token) {
            continue;
        }
        double lon = atof(token);
        addNode(g, id, lat, lon);
    }

    fclose(file);
    return g;
}

int cmp_nd(const void* a, const void* b) {
    const NodeWithDistance* x = (const NodeWithDistance*)a;
    const NodeWithDistance* y = (const NodeWithDistance*)b;

    if (x->distance < y->distance) {
        return -1;
    }
    if (x->distance > y->distance) {
        return 1;
    }
    return 0;
}

void build_edges(Graph* g, int k) {
    if (!g) {
        return;
    }

    Vector* nodes = createVector(sizeof(GNode*));

    for (size_t i = 0; i < g->nodes->capacity; i++) {
        Vector **bucket_ptr = get_bucket_ptr(g->nodes, i);
        Vector *bucket = *bucket_ptr;

        if (!bucket) {
            continue;
        }

        for (size_t j = 0; j < bucket->size; j++) {
            void* elem = getVectorItem(bucket, j);
            GNode* node = (GNode*)((char*)elem + sizeof(long long));
            appendVectorItem(nodes, &node);
        }
    }

    for (size_t i = 0; i < nodes->size; i++) {

        GNode* a = *(GNode**)getVectorItem(nodes, i);
        Vector* best = createVector(sizeof(NodeWithDistance));

        for (size_t j = 0; j < nodes->size; j++) {
            if (i == j) {
                continue;
            }

            GNode* b = *(GNode**)getVectorItem(nodes, j);
            NodeWithDistance nd;

            nd.node_id = b->node_id;
            double dx = a->lat - b->lat;
            double dy = a->lon - b->lon;
            nd.distance = (dx * dx + dy * dy);

            appendVectorItem(best, &nd);
        }

        qsort(best->data, best->size, sizeof(NodeWithDistance), cmp_nd);

        for (int t = 0; t < k && t < (int)best->size; t++) {
            NodeWithDistance* nd = (NodeWithDistance*)getVectorItem(best, t);
            addEdge(g, a->node_id, nd->node_id, (nd->distance), 0, "X");
        }

        vectorFree(best);
    }

    vectorFree(nodes);
}

Vector* kruskal_mst(Graph* g) {
    if (!g) {
        return NULL;
    }

    Vector* edges = get_all_edges(g);
    if (!edges) {
        return NULL;
    }

    qsort(edges->data, edges->size, sizeof(SEdge), cmp_edges);

    DisjointSet* dsu = dsu_create_from_hash_table(g->nodes);

    Vector* mst = createVector(sizeof(SEdge));

    for (size_t i = 0; i < edges->size; i++) {
        SEdge* e = getVectorItem(edges, i);

        DSNode* a = find_set_from_key(dsu, e->from);
        DSNode* b = find_set_from_key(dsu, e->to);

        if (find_set(a) != find_set(b)) {
            union_sets(a, b);
            appendVectorItem(mst, e);
        }
    }

    freeDisjointSet(dsu);
    vectorFree(edges);

    return mst;
}

double total_mst_weight(Vector* mst) {
    if (!mst) {
        return 0.0;
    }

    double total = 0.0;

    for (size_t i = 0; i < mst->size; i++) {
        SEdge* e = (SEdge*)getVectorItem(mst, i);
        total += e->weight;
    }

    return total;
}

/*
int main() {
    char path_input[] = "data/buildings.csv";
    char path_output[] = "data/spanning_tree.csv";

    Graph* graph = read_buildings("data/buildings.csv");
    build_edges(graph, 3);

    Vector* edges = kruskal_mst(graph);
    double total = total_mst_weight(edges);
    printf("Total MST weight = %.6lf\n", total);

    write_edges_to_file(edges, path_output);

}*/