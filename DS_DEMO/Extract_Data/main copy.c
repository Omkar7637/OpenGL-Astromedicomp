#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    float u, v;
} TexCoord;

typedef struct {
    float nx, ny, nz;
} Normal;

typedef struct {
    int v[3];   // vertex indices
    int vt[3];  // texture indices
    int vn[3];  // normal indices
} Face;

Vertex* vertices = NULL;
TexCoord* texCoords = NULL;
Normal* normals = NULL;
Face* faces = NULL;

int vCount = 0, vtCount = 0, vnCount = 0, fCount = 0;
int vCap = 0, vtCap = 0, vnCap = 0, fCap = 0;

void ensureCapacity() {
    if (vCount >= vCap) {
        vCap = (vCap == 0) ? 256 : vCap * 2;
        vertices = realloc(vertices, vCap * sizeof(Vertex));
    }
    if (vtCount >= vtCap) {
        vtCap = (vtCap == 0) ? 256 : vtCap * 2;
        texCoords = realloc(texCoords, vtCap * sizeof(TexCoord));
    }
    if (vnCount >= vnCap) {
        vnCap = (vnCap == 0) ? 256 : vnCap * 2;
        normals = realloc(normals, vnCap * sizeof(Normal));
    }
    if (fCount >= fCap) {
        fCap = (fCap == 0) ? 256 : fCap * 2;
        faces = realloc(faces, fCap * sizeof(Face));
    }
}

void parseOBJ(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("File open failed");
        exit(1);
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        ensureCapacity();
        if (strncmp(line, "v ", 2) == 0) {
            sscanf(line + 2, "%f %f %f", &vertices[vCount].x, &vertices[vCount].y, &vertices[vCount].z);
            vCount++;
        } else if (strncmp(line, "vt ", 3) == 0) {
            sscanf(line + 3, "%f %f", &texCoords[vtCount].u, &texCoords[vtCount].v);
            vtCount++;
        } else if (strncmp(line, "vn ", 3) == 0) {
            sscanf(line + 3, "%f %f %f", &normals[vnCount].nx, &normals[vnCount].ny, &normals[vnCount].nz);
            vnCount++;
        } else if (strncmp(line, "f ", 2) == 0) {
            int vi[3], ti[3], ni[3];
            int parsed = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                                &vi[0], &ti[0], &ni[0],
                                &vi[1], &ti[1], &ni[1],
                                &vi[2], &ti[2], &ni[2]);
            if (parsed == 9) {
                for (int i = 0; i < 3; i++) {
                    faces[fCount].v[i] = vi[i] - 1;
                    faces[fCount].vt[i] = ti[i] - 1;
                    faces[fCount].vn[i] = ni[i] - 1;
                }
                fCount++;
            }
        }
    }

    fclose(fp);
}

void writeHeader(const char* outname) {
    FILE* out = fopen(outname, "w");
    if (!out) {
        perror("Header write failed");
        exit(1);
    }

    fprintf(out, "// Auto-generated from .obj\n\n");

    fprintf(out, "typedef struct { float x, y, z; } Vertex;\n");
    fprintf(out, "typedef struct { float u, v; } TexCoord;\n");
    fprintf(out, "typedef struct { float nx, ny, nz; } Normal;\n\n");

    fprintf(out, "static const Vertex vertices[%d] = {\n", vCount);
    for (int i = 0; i < vCount; i++)
        fprintf(out, "    { %.6ff, %.6ff, %.6ff },\n", vertices[i].x, vertices[i].y, vertices[i].z);
    fprintf(out, "};\n\n");

    fprintf(out, "static const TexCoord texCoords[%d] = {\n", vtCount);
    for (int i = 0; i < vtCount; i++)
        fprintf(out, "    { %.6ff, %.6ff },\n", texCoords[i].u, texCoords[i].v);
    fprintf(out, "};\n\n");

    fprintf(out, "static const Normal normals[%d] = {\n", vnCount);
    for (int i = 0; i < vnCount; i++)
        fprintf(out, "    { %.6ff, %.6ff, %.6ff },\n", normals[i].nx, normals[i].ny, normals[i].nz);
    fprintf(out, "};\n\n");

    fprintf(out, "void draw_model() {\n");
    fprintf(out, "    glBegin(GL_TRIANGLES);\n");
    for (int i = 0; i < fCount; i++) {
        for (int j = 0; j < 3; j++) {
            fprintf(out, "    glTexCoord2f(texCoords[%d].u, texCoords[%d].v);\n", faces[i].vt[j], faces[i].vt[j]);
            fprintf(out, "    glNormal3f(normals[%d].nx, normals[%d].ny, normals[%d].nz);\n", faces[i].vn[j], faces[i].vn[j], faces[i].vn[j]);
            fprintf(out, "    glVertex3f(vertices[%d].x, vertices[%d].y, vertices[%d].z);\n", faces[i].v[j], faces[i].v[j], faces[i].v[j]);
        }
    }
    fprintf(out, "    glEnd();\n");
    fprintf(out, "}\n");

    fclose(out);
}

int main() {
    parseOBJ("O_Rabbit.obj");           // Use your actual .obj filename here
    writeHeader("O_Rabbit.h");    // Output header file
    printf("Header created: O_Rabbit.h\n");

    // Free memory
    free(vertices);
    free(texCoords);
    free(normals);
    free(faces);

    return 0;
}
