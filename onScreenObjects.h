//
// Created by alvaregd on 27/11/15.
//

#ifndef SIMPLE_LEVEL_EDITOR_ONSCREENOBJECTS_H
#define SIMPLE_LEVEL_EDITOR_ONSCREENOBJECTS_H

#include <GL/glew.h>

struct Grid{

    GLuint vao;
    GLuint vbo;
    int numberOfLines;
    GLfloat heightValue;
    GLfloat* data;
};

struct Cursor{

    GLuint vao;
    GLuint vbo;
    GLfloat* data;
    GLfloat editZValue;
    GLfloat editXValue;
    GLfloat Z;
    GLfloat X;
    GLfloat Y;
};


#endif //SIMPLE_LEVEL_EDITOR_ONSCREENOBJECTS_H
