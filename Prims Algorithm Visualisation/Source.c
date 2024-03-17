#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX 1000
#define INF 999999
#define M_PI 3.14159265358979323846

typedef struct {
    int x, y;
} Point;

typedef struct {
    int u, v, weight;
} Edge;

int vertices, edges;
Point points[MAX];
Edge graph[MAX][MAX];
int parent[MAX];

enum ViewMode {
    ORIGINAL_GRAPH,
    MINIMUM_SPANNING_TREE,
    VIEW_TABLE,
    RANDOM_GRAPH
};

enum ViewMode currentViewMode = ORIGINAL_GRAPH;

void drawLine(Point p1, Point p2) {
    glBegin(GL_LINES);
    glVertex2i(p1.x, p1.y);
    glVertex2i(p2.x, p2.y);
    glEnd();
    glFlush();
}

void drawText(int x, int y, int value) {
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2i(x, y);
    char charValue = '0' + value;
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, charValue);
}

void drawCircle(Point center, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(center.x, center.y);
    for (int i = 0; i <= 360; i++) {
        float radian = i * (M_PI / 180.0);
        glVertex2f(center.x + radius * cos(radian), center.y + radius * sin(radian));
    }
    glEnd();
}

/*void printMST(int parent[]) {
    printf("Edge \tWeight\n");
    for (int i = 1; i < vertices; i++)
        printf("%d - %d\t%d\n", parent[i], i, graph[i][parent[i]].weight);
}*/

void drawMenu() {
    glColor3f(0.0, 0.0, 0.0);
    int x = 10;
    int y = 580;

    const char* message;
    if (currentViewMode == ORIGINAL_GRAPH) {
        message = "Current View: Original Graph";
    }
    else if (currentViewMode == MINIMUM_SPANNING_TREE) {
        message = "Current View: Minimum Spanning Tree";
    }
    else {
        message = "Current View: View Cost Table";
    }

    glRasterPos2i(x, y);
    for (int i = 0; message[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
    }
}


void drawGraph() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (currentViewMode == ORIGINAL_GRAPH) {
        for (int i = 0; i < vertices; i++) {
            glColor3f(1.0, 0.0, 0.0);
            drawCircle(points[i], 10.0);
            drawText(points[i].x - 5, points[i].y - 15, i);
        }

        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (i != j && graph[i][j].weight != INF) {
                    glColor3f(1.0, 0.0, 0.0);
                    drawLine(points[i], points[j]);

                    // Display the weight of the edge
                    int weight = graph[i][j].weight;
                    int midX = (points[i].x + points[j].x) / 2;
                    int midY = (points[i].y + points[j].y) / 2;
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2i(midX, midY);
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0' + weight / 10);
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0' + weight % 10);
                }
            }
        }
    }
    else if (currentViewMode == MINIMUM_SPANNING_TREE) {
        for (int i = 0; i < vertices; i++) {
            glColor3f(0.0, 1.0, 0.0);
            drawCircle(points[i], 10.0);
            drawText(points[i].x - 5, points[i].y - 15, i);
        }

        for (int i = 1; i < vertices; i++) {
            glColor3f(0.0, 1.0, 0.0);  // Highlight MST edges in green
            drawLine(points[parent[i]], points[i]);

            // Display the weight of the edge
            int weight = graph[i][parent[i]].weight;
            int midX = (points[parent[i]].x + points[i].x) / 2;
            int midY = (points[parent[i]].y + points[i].y) / 2;
            glColor3f(0.0, 0.0, 0.0);
            glRasterPos2i(midX, midY);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0' + weight / 10);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0' + weight % 10);
        }
    }
    else if (currentViewMode == VIEW_TABLE) {
        int startX = 250;  // Starting X-coordinate for the table
        int startY = 250;  // Starting Y-coordinate for the table
        int lineHeight = 15;  // Line height for each row

        // Draw table headers
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2i(startX, startY);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'U');
        glRasterPos2i(startX + 30, startY);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'V');
        glRasterPos2i(startX + 60, startY);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'W');

        // Draw table content
        for (int i = 1; i < vertices; i++) {
            int rowY = startY + (i + 1) * lineHeight;

            // Draw U value
            glRasterPos2i(startX, rowY);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0' + parent[i]);

            // Draw V value
            glRasterPos2i(startX + 30, rowY);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0' + i);

            // Draw Weight value
            glRasterPos2i(startX + 60, rowY);
            int weight = graph[i][parent[i]].weight;
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0' + weight / 10);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0' + weight % 10);
        }
    }

    drawMenu();

    glutPostRedisplay();
    glutSwapBuffers();
}

void generateRandomGraph() {
    for (int i = 0; i < vertices; i++) {
        points[i].x = rand() % 800;
        points[i].y = rand() % 600;
    }

    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i != j) {
                graph[i][j].weight = rand() % 100;
            }
            else {
                graph[i][j].weight = 0;
            }
        }
    }
}

int minKey(int key[], bool mstSet[]) {
    int min = INF, min_index = -1;

    for (int v = 0; v < vertices; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }

    return min_index;
}

void primMST(int value) {


    int key[MAX];
    bool mstSet[MAX];

    for (int i = 0; i < vertices; i++) {
        key[i] = INF;
        mstSet[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < vertices - 1; count++) {
        int u = minKey(key, mstSet);
        mstSet[u] = true;

        for (int v = 0; v < vertices; v++) {
            if (graph[u][v].weight && !mstSet[v] && graph[u][v].weight < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v].weight;
            }
        }


    }

    glutPostRedisplay();
    glutTimerFunc(1000, primMST, value + 1);
    //printMST(parent);
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800.0, 600.0, 0.0);
}

void processMenu(int option) {
    switch (option) {
    case 1:
        currentViewMode = ORIGINAL_GRAPH;
        break;
    case 2:
        currentViewMode = MINIMUM_SPANNING_TREE;
        break;
    case 3:
        currentViewMode = VIEW_TABLE;
        break;
    case 4:
        currentViewMode = RANDOM_GRAPH;
        generateRandomGraph();
        break;
    }
    glutPostRedisplay();
}

void createMenu() {
    int menu = glutCreateMenu(processMenu);
    glutAddMenuEntry("Original Graph", 1);
    glutAddMenuEntry("Minimum Spanning Tree", 2);
    glutAddMenuEntry("View Table", 3);
    glutAddMenuEntry("Random Graph", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    printf("Enter the number of vertices: ");
    scanf_s("%d", &vertices);

    printf("Enter the number of edges: ");
    scanf_s("%d", &edges);



    printf("Enter the coordinates of vertices (format: x y):\n");
    for (int i = 0; i < vertices; i++) {
        scanf_s("%d %d", &points[i].x, &points[i].y);
    }

    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i != j) {
                graph[i][j].weight = INF;
            }
            else {
                graph[i][j].weight = 0;  // Diagonal elements should be 0
            }
        }
    }

    printf("Enter the edges and weights (format: u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        scanf_s("%d %d %d", &u, &v, &weight);
        graph[u][v].weight = weight;
        graph[v][u].weight = weight; // Undirected graph

        // Assuming the graph is 0-indexed, update the weights in both directions
        graph[v][u].u = v;
        graph[v][u].v = u;
        graph[v][u].weight = weight;

        // Assuming the graph is 0-indexed, update the weights in both directions
        graph[u][v].u = u;
        graph[u][v].v = v;
        graph[u][v].weight = weight;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Prim's Algorithm Visualization");
    init();
    glutDisplayFunc(drawGraph);
    createMenu();  // Add menu creation
    glutTimerFunc(1000, primMST, 1);
    glutMainLoop();

    return 0;
}