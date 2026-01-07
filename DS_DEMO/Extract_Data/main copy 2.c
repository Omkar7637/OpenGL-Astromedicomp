#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { float x, y, z; } Vertex;
typedef struct { float u, v; } TexCoord;
typedef struct { float x, y, z; } Normal;

typedef struct {
    int v[3];   // vertex index
    int vt[3];  // texcoord index
    int vn[3];  // normal index
} Face;

Vertex* v = NULL;
TexCoord* vt = NULL;
Normal* vn = NULL;
Face* faces = NULL;

int vCount = 0, vtCount = 0, vnCount = 0, fCount = 0;
int vCap = 0, vtCap = 0, vnCap = 0, fCap = 0;

void push_vertex(float x, float y, float z) {
    if (vCount >= vCap) {
        vCap = vCap == 0 ? 1024 : vCap * 2;
        v = realloc(v, vCap * sizeof(Vertex));
    }
    v[vCount++] = (Vertex){ x, y, z };
}

void push_texcoord(float u, float vval) {
    if (vtCount >= vtCap) {
        vtCap = vtCap == 0 ? 1024 : vtCap * 2;
        vt = realloc(vt, vtCap * sizeof(TexCoord));
    }
    vt[vtCount++] = (TexCoord){ u, vval };
}

void push_normal(float x, float y, float z) {
    if (vnCount >= vnCap) {
        vnCap = vnCap == 0 ? 1024 : vnCap * 2;
        vn = realloc(vn, vnCap * sizeof(Normal));
    }
    vn[vnCount++] = (Normal){ x, y, z };
}

void push_face(Face f) {
    if (fCount >= fCap) {
        fCap = fCap == 0 ? 1024 : fCap * 2;
        faces = realloc(faces, fCap * sizeof(Face));
    }
    faces[fCount++] = f;
}

void parse_obj(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) { perror("Failed to open OBJ"); exit(1); }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            float x, y, z;
            sscanf(line + 2, "%f %f %f", &x, &y, &z);
            push_vertex(x, y, z);
        } else if (strncmp(line, "vt ", 3) == 0) {
            float u, vval;
            sscanf(line + 3, "%f %f", &u, &vval);
            push_texcoord(u, vval);
        } else if (strncmp(line, "vn ", 3) == 0) {
            float x, y, z;
            sscanf(line + 3, "%f %f %f", &x, &y, &z);
            push_normal(x, y, z);
        } else if (strncmp(line, "f ", 2) == 0) {
            Face f;
            int ret = sscanf(line + 2,
                "%d/%d/%d %d/%d/%d %d/%d/%d",
                &f.v[0], &f.vt[0], &f.vn[0],
                &f.v[1], &f.vt[1], &f.vn[1],
                &f.v[2], &f.vt[2], &f.vn[2]);

            if (ret == 9) {
                for (int i = 0; i < 3; i++) {
                    f.v[i] -= 1;
                    f.vt[i] -= 1;
                    f.vn[i] -= 1;
                }
                push_face(f);
            }
        }
    }

    fclose(file);
}

void generate_header(const char* out_file) {
    FILE* out = fopen(out_file, "w");
    if (!out) { perror("Write failed"); exit(1); }

    fprintf(out, "// Auto-generated draw_model() from OBJ\n");
    fprintf(out, "#include <GL/gl.h>\n\n");
    fprintf(out, "void draw_model() {\n");
    fprintf(out, "    glBegin(GL_TRIANGLES);\n");

    for (int i = 0; i < fCount; i++) {
        for (int j = 0; j < 3; j++) {
            TexCoord t = vt[faces[i].vt[j]];
            Normal n = vn[faces[i].vn[j]];
            Vertex p = v[faces[i].v[j]];

            fprintf(out, "    glTexCoord2f(%.6ff, %.6ff);\n", t.u, t.v);
            fprintf(out, "    glNormal3f(%.6ff, %.6ff, %.6ff);\n", n.x, n.y, n.z);
            fprintf(out, "    glVertex3f(%.6ff, %.6ff, %.6ff);\n", p.x, p.y, p.z);
        }
    }

    fprintf(out, "    glEnd();\n");
    fprintf(out, "}\n");

    fclose(out);
}

int main() {
    parse_obj("O_Rabbit.obj");            // replace with your OBJ file
    generate_header("O_Rabbit.h");  // output .h file

    // cleanup
    free(v);
    free(vt);
    free(vn);
    free(faces);

    printf("O_Rabbit.h generated successfully.\n");
    return 0;
}
