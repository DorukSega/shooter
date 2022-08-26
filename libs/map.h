// Example for loop for objects
// for (int i = 0; i < m_MAP_size; i++) {
//     m_MAP[i];
// }

#ifndef __MAP_H__
#define __MAP_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getline.h"

enum m_keys {
    m_empty,
    m_type,
    m_posX,
    m_posY,
    m_posZ,
    m_width,
    m_height,
    m_length,
    m_texture,  // texture name
    m_R,
    m_G,
    m_B,
    m_A,
};

enum t_types {
    t_EMPTY,
    t_MESH,
    t_MESH_T,  // textured
};

struct s_MESH {
    int type;
    float posX, posY, posZ;
    float width;
    float height;
    float length;
    int R, G, B, A;
};

struct m_KV {
    int key;
    void* value;
};

#define M_LIMIT 5000
const int M_KEY_LIMIT = 20;
const char M_DELIMETER[] = "␟";

struct m_KV m_MAP[M_LIMIT][20];
int m_MAP_size = 0;

void loadMap(const char* fileName);

void unloadMap();

void* getMObjectValue(struct m_KV* MapObject, int key) {
    for (int i = 0; i < M_KEY_LIMIT; i++) {
        if (MapObject[i].key == key)
            return MapObject[i].value;
    }
    return NULL;
}

struct s_MESH getsMesh(struct m_KV* MapObject) {
    int type = *(int*)MapObject[0].value;
    if (type != t_MESH)
        exit(3);

    const float posX = *(float*)MapObject[1].value;
    const float posY = *(float*)MapObject[2].value;
    const float posZ = *(float*)MapObject[3].value;
    const float width = *(float*)MapObject[4].value;
    const float height = *(float*)MapObject[5].value;
    const float length = *(float*)MapObject[6].value;
    const int R = *(int*)MapObject[7].value;
    const int G = *(int*)MapObject[8].value;
    const int B = *(int*)MapObject[9].value;
    const int A = *(int*)MapObject[10].value;

    return (struct s_MESH){type,   posX, posY, posZ, width, height,
                           length, R,    G,    B,    A};
}

void loadMap(const char* fileName) {
    FILE* fp;
    char* line = NULL;  // line
    size_t len = 0;
    ssize_t read;  // line length

    fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("File not found");
        exit(2);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        const int type = atoi(&line[0]);

        if (type == t_MESH) {
            // printf("%s %zd %d\n", line, read, type);

            const char* data[M_KEY_LIMIT];  // not heap :)

            int toknum = 0;

            char* token = strtok(line, M_DELIMETER);
            while (token != NULL) {
                data[toknum++] = token;
                token = strtok(NULL, M_DELIMETER);
            }
            free(token);

            m_MAP[m_MAP_size][0].key = m_type;
            m_MAP[m_MAP_size][1].key = m_posX;
            m_MAP[m_MAP_size][2].key = m_posY;
            m_MAP[m_MAP_size][3].key = m_posZ;
            m_MAP[m_MAP_size][4].key = m_width;
            m_MAP[m_MAP_size][5].key = m_height;
            m_MAP[m_MAP_size][6].key = m_length;
            m_MAP[m_MAP_size][7].key = m_R;
            m_MAP[m_MAP_size][8].key = m_G;
            m_MAP[m_MAP_size][9].key = m_B;
            m_MAP[m_MAP_size][10].key = m_A;

            m_MAP[m_MAP_size][0].value = malloc(sizeof(data[0]));
            m_MAP[m_MAP_size][1].value = malloc(sizeof(data[1]));
            m_MAP[m_MAP_size][2].value = malloc(sizeof(data[2]));
            m_MAP[m_MAP_size][3].value = malloc(sizeof(data[3]));
            m_MAP[m_MAP_size][4].value = malloc(sizeof(data[4]));
            m_MAP[m_MAP_size][5].value = malloc(sizeof(data[5]));
            m_MAP[m_MAP_size][6].value = malloc(sizeof(data[6]));
            m_MAP[m_MAP_size][7].value = malloc(sizeof(data[7]));
            m_MAP[m_MAP_size][8].value = malloc(sizeof(data[8]));
            m_MAP[m_MAP_size][9].value = malloc(sizeof(data[9]));
            m_MAP[m_MAP_size][10].value = malloc(sizeof(data[10]));

            *(int*)m_MAP[m_MAP_size][0].value = atoi(data[0]);
            *(float*)m_MAP[m_MAP_size][1].value = atof(data[1]);
            *(float*)m_MAP[m_MAP_size][2].value = atof(data[2]);
            *(float*)m_MAP[m_MAP_size][3].value = atof(data[3]);
            *(float*)m_MAP[m_MAP_size][4].value = atof(data[4]);
            *(float*)m_MAP[m_MAP_size][5].value = atof(data[5]);
            *(float*)m_MAP[m_MAP_size][6].value = atof(data[6]);
            *(int*)m_MAP[m_MAP_size][7].value = atoi(data[7]);
            *(int*)m_MAP[m_MAP_size][8].value = atoi(data[8]);
            *(int*)m_MAP[m_MAP_size][9].value = atoi(data[9]);
            *(int*)m_MAP[m_MAP_size][10].value = atoi(data[10]);

            m_MAP_size++;
        } else if (type == t_MESH_T) {
            m_MAP_size++;
        }
    }

    fclose(fp);

    if (line)
        free(line);
}

void unloadMap() {
    for (int i = 0; i < M_LIMIT; i++) {
        for (int j = 0; j < M_KEY_LIMIT; j++) {
            free(m_MAP[i][j].value);
            m_MAP[i][j].key = m_empty;
        }
    }
}

#endif  // __MAP_H__