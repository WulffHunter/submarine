// Jared Rand
// 500683609

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
    #include <glut/glut.h>
#else
    #include <gl/glut.h>
#endif
#include "Util.h"
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float randomRange(float min, float max) {
    //If the min and max are the same, return that: else, return the random number
    return (min != max)
        ? ((float )(floorf(fmodf(rand(), (max - min))) + min))
        : min;
}

Texture* generateTexture(const char* filename, GLenum glTexture) {
    Texture* texture = malloc(sizeof(texture));
    
    unsigned char* data = stbi_load(
        filename,
        &texture->width,
        &texture->height,
        &texture->nrChannels,
        0
    ); 

    // glActiveTexture(glTexture);
    glGenTextures(1, &texture->textureId);

    glBindTexture(GL_TEXTURE_2D, texture->textureId);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte
    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //mix with light

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        texture->width,
        texture->height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data
    ); // the pixels

    stbi_image_free(data);

    return texture;
}