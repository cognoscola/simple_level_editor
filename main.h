//
// Created by alvaregd on 27/11/15.
//

#ifndef SIMPLE_LEVEL_EDITOR_MAIN_H_H
#define SIMPLE_LEVEL_EDITOR_MAIN_H_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <utils/math_utils/maths_funcs.h>
#include <utils/math_utils/quat_funcs.h>
#include <utils/log_utils/logger.h>
#include <platform/glfw_launcher.h>
#include <platform/hardware.h>

struct Camera{

    float pos[3]; // don't start at zero, or we will be too close
    float yaw = 0.0f; // y-rotation in degrees
    float pitch = 0.0f;
    float signal_amplifier = 0.1f;
    mat4 T;
    mat4 Rpitch;
    mat4 Ryaw;
    mat4 viewMatrix;

    GLint view_mat_location;
    GLint proj_mat_location;

    float quatYaw[4];
    float quatPitch[4];

    int pushing; //-1 slowing down, +1 accelerating , 0 = idle
    bool moving; //velocity != 0
    double move_angle;
    double look_angle;

    vec3 velocity; //actor's velocity
};

struct Input{
    bool wPressed;
    bool sPressed;
    bool aPressed;
    bool dPressed;
};

struct Grid{
    GLuint gridVbo;
    int numberOfLines;
    GLfloat heightValue;
    GLfloat* gridData;
};

static Grid grid;
static Camera camera;
static Input input;
Hardware hardware;

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void calculateViewMatrix(Camera* camera);
static void updateMovement(Camera* camera);
static void updateGridHeight(Grid* grid);


#endif //SIMPLE_LEVEL_EDITOR_MAIN_H_H