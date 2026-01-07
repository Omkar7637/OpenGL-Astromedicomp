// SimpleOBJLoader.h
#ifndef SIMPLE_OBJ_LOADER_H
#define SIMPLE_OBJ_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>

#define MAX_VERTICES 10000
#define MAX_NORMALS  10000

typedef struct {
    float x, y, z;
} Vec3;

static Vec3 obj_vertices[MAX_VERTICES];
static Vec3 obj_normals[MAX_NORMALS];
static int vertexCount = 0;
static int normalCount = 0;

// Load vertices and normals
static void loadOBJ(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open OBJ file: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            Vec3 v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            if (vertexCount < MAX_VERTICES)
                obj_vertices[vertexCount++] = v;
        } else if (strncmp(line, "vn ", 3) == 0) {
            Vec3 n;
            sscanf(line, "vn %f %f %f", &n.x, &n.y, &n.z);
            if (normalCount < MAX_NORMALS)
                obj_normals[normalCount++] = n;
        }
    }

    fclose(file);
}

// Draw triangles only (format: f v//vn v//vn v//vn)
static void drawOBJ(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open OBJ file: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "f ", 2) == 0) {
            int v[3], n[3];
            if (sscanf(line, "f %d//%d %d//%d %d//%d", 
                &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]) == 6) {
                
                glBegin(GL_TRIANGLES);
                for (int i = 0; i < 3; ++i) {
                    Vec3 norm = obj_normals[n[i] - 1];
                    Vec3 vert = obj_vertices[v[i] - 1];
                    glNormal3f(norm.x, norm.y, norm.z);
                    glVertex3f(vert.x, vert.y, vert.z);
                }
                glEnd();
            }
        }
    }

    fclose(file);
}

#endif // SIMPLE_OBJ_LOADER_H
