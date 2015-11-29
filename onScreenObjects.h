//
// Created by alvaregd on 27/11/15.
//

#ifndef SIMPLE_LEVEL_EDITOR_ONSCREENOBJECTS_H
#define SIMPLE_LEVEL_EDITOR_ONSCREENOBJECTS_H

#include <GL/glew.h>


struct Grid{

    GLuint vao;
    GLuint vertexVbo;
    GLuint colorVbo;
    GLuint colorAttributeIndex;
    int numberOfLines;
    GLfloat heightValue;
};

struct Cursor{


    GLuint vao;
    GLuint vertexVbo;
    GLuint colorVbo;
    GLfloat*vertexData;

    GLuint colorAttributeIndex;

    GLfloat Z;
    GLfloat X;
    GLfloat Y;

    GLfloat Xs = 1.0f;
    GLfloat Ys = 1.0f;
    GLfloat Zs = 1.0f;

    GLfloat pitch;
    GLfloat yaw;
    GLfloat roll;

    mat4 T;
    mat4 Ryaw;
    mat4 Rpitch;
    mat4 Rroll;
    mat4 viewMatrix;
};


#endif //SIMPLE_LEVEL_EDITOR_ONSCREENOBJECTS_H
