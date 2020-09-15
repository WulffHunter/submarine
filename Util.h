// Jared Rand
// 500683609

#ifndef UTIL_H
#define UTIL_H

typedef struct Texture {
    GLuint textureId;

    int width;
    int height;
    int nrChannels;

    unsigned char* data;
} Texture;

float randomRange(float min, float max);

Texture* generateTexture(const char* filename, GLenum glTexture);

#endif