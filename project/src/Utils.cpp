#include "Utils.h"

// Перевод изображения в текстуру
void mat_to_texture(GLuint texture, cv::Mat pic, bool with_alpha) {
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    cv::flip(pic, pic, 0);

    if (with_alpha) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pic.cols, pic.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, pic.ptr());
    } else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pic.cols, pic.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, pic.ptr());
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

