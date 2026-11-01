*
 * File System as a Graph (Adjacency Matrix) with BFS and DFS search.
 * Each file/directory is a node; a connection (directory -> child) is an edge.
 */
#include <stdio.h>
#include <string.h>

#define MAX_NODES 20
#define NAME_LEN  32

char names[MAX_NODES][NAME_LEN];       /* node names           */
int  adj[MAX_NODES][MAX_NODES];        /* adjacency matrix     */
int  nodeCount = 0;

/* ---------- Step 1: build the graph ---------- */
int addNode(const char *name) {
    if (nodeCount >= MAX_NODES) return -1;
    strncpy(names[nodeCount], name, NAME_LEN - 1);
    names[nodeCount][NAME_LEN - 1] = '\0';
    return nodeCount++;
}

void addEdge(int parent, int child) {
    adj[parent][child] = 1;
    adj[child][parent] = 1;            /* undirected connection */
}

void printMatrix(void) {
    printf("Adjacency Matrix (%d nodes):\n     ", nodeCount);
    for (int i = 0; i < nodeCount; i++) printf("%2d ", i);
    printf("\n");
    for (int i = 0; i < nodeCount; i++) {
        printf("%2d | ", i);
        for (int j = 0; j < nodeCount; j++) printf("%2d ", adj[i][j]);
        printf("  %s\n", names[i]);
    }
    printf("\n");
}

/* ---------- Step 2: BFS ---------- */
int bfs(int start, const char *target) {
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES], front = 0, rear = 0;
    int found = 0;

    printf("BFS order: ");
    visited[start] = 1;
    queue[rear++] = start;

    while (front < rear) {
        int u = queue[front++];
        printf("%s ", names[u]);
        if (strcmp(names[u], target) == 0) { found = 1; break; }
        for (int v = 0; v < nodeCount; v++) {
            if (adj[u][v] && !visited[v]) {
                visited[v] = 1;
                queue[rear++] = v;
            }
        }
    }
    printf("\n");
    return found;
}

/* ---------- Step 2: DFS (recursive) ---------- */
int dfsUtil(int u, const char *target, int visited[]) {
    visited[u] = 1;
    printf("%s ", names[u]);
    if (strcmp(names[u], target) == 0) return 1;
    for (int v = 0; v < nodeCount; v++) {
        if (adj[u][v] && !visited[v]) {
            if (dfsUtil(v, target, visited)) return 1;
        }
    }
    return 0;
}

int dfs(int start, const char *target) {
    int visited[MAX_NODES] = {0};
    printf("DFS order: ");
    int found = dfsUtil(start, target, visited);
    printf("\n");
    return found;
}

/* ---------- Step 3: test ---------- */
void runSearch(int start, const char *target) {
    printf("--- Searching for \"%s\" starting from \"%s\" ---\n", target, names[start]);
    int b = bfs(start, target);
    printf("BFS result: %s\n", b ? "File FOUND" : "File NOT found");
    int d = dfs(start, target);
    printf("DFS result: %s\n\n", d ? "File FOUND" : "File NOT found");
}

int main(void) {
    /* Sample file system:
     * root
     * |-- home
     * |   |-- user1 (notes.txt, photo.jpg)
     * |   `-- user2 (report.pdf)
     * |-- etc  (config.cfg)
     * `-- var  (log.txt)
     */
    int root   = addNode("root");
    int home   = addNode("home");
    int etc    = addNode("etc");
    int var    = addNode("var");
    int user1  = addNode("user1");
    int user2  = addNode("user2");
    int notes  = addNode("notes.txt");
    int photo  = addNode("photo.jpg");
    int report = addNode("report.pdf");
    int config = addNode("config.cfg");
    int logf   = addNode("log.txt");

    addEdge(root, home);   addEdge(root, etc);    addEdge(root, var);
    addEdge(home, user1);  addEdge(home, user2);
    addEdge(user1, notes); addEdge(user1, photo);
    addEdge(user2, report);
    addEdge(etc, config);
    addEdge(var, logf);

    printMatrix();

    runSearch(root, "report.pdf");   /* exists, deep in tree  */
    runSearch(root, "log.txt");      /* exists, other branch  */
    runSearch(root, "missing.doc");  /* does not exist        */
    runSearch(home, "config.cfg");   /* start from a sub-node */

    return 0;
