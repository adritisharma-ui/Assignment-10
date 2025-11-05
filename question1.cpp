#include <iostream>
#include <climits>
using namespace std;

struct MinHeapNode {
    int vertex;
    int dist;
};

struct MinHeap {
    int size;
    int capacity;
    int *pos;
    MinHeapNode **array;
};

MinHeapNode* newMinHeapNode(int v, int dist) {
    MinHeapNode* minHeapNode = new MinHeapNode;
    minHeapNode->vertex = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = new MinHeap;
    minHeap->pos = new int[capacity];
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = new MinHeapNode*[capacity];
    return minHeap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;
    if (smallest != idx) {
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];
        minHeap->pos[smallestNode->vertex] = idx;
        minHeap->pos[idxNode->vertex] = smallest;
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

bool isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

MinHeapNode* extract_min(MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;
    MinHeapNode* root = minHeap->array[0];
    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->pos[root->vertex] = minHeap->size - 1;
    minHeap->pos[lastNode->vertex] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}

void decrease_key(MinHeap* minHeap, int v, int dist) {
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        minHeap->pos[minHeap->array[i]->vertex] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->vertex] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(MinHeap *minHeap, int v) {
    return minHeap->pos[v] < minHeap->size;
}

void build_heap(MinHeap* minHeap) {
    int n = minHeap->size;
    for (int i = (n - 1) / 2; i >= 0; i--)
        minHeapify(minHeap, i);
}

void dijkstra(int **graph, int V, int src) {
    int *dist = new int[V];
    MinHeap* minHeap = createMinHeap(V);
    for (int v = 0; v < V; ++v) {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    dist[src] = 0;
    minHeap->array[src]->dist = 0;
    minHeap->size = V;
    build_heap(minHeap);
    while (!isEmpty(minHeap)) {
        MinHeapNode* minHeapNode = extract_min(minHeap);
        int u = minHeapNode->vertex;
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && isInMinHeap(minHeap, v) &&
                dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                decrease_key(minHeap, v, dist[v]);
            }
        }
    }
    cout << "\nVertex\tDistance from Source (" << src << ")\n";
    for (int i = 0; i < V; ++i)
        cout << i << "\t\t" << dist[i] << endl;
}

int main() {
    int V;
    cout << "Enter number of vertices: ";
    cin >> V;
    int **graph = new int*[V];
    for (int i = 0; i < V; i++)
        graph[i] = new int[V];
    cout << "Enter the adjacency matrix (0 for no edge):\n";
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            cin >> graph[i][j];
    int src;
    cout << "Enter the source vertex: ";
    cin >> src;
    dijkstra(graph, V, src);
    return 0;
}
