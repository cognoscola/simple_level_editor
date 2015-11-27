//
// Created by alvaregd on 27/11/15.
//

#ifndef SIMPLE_LEVEL_EDITOR_GLFW_H
#define SIMPLE_LEVEL_EDITOR_GLFW_H

#include <utils/log_utils/log.h>
#include <stdio.h>
#include <main.h>


extern Hardware hardware;

bool start_gl ();

//glfw callbacks
void glfw_error_callback(int error, const char* description);
void glfw_window_size_callback(GLFWwindow* window, int width, int height);


#endif //SIMPLE_LEVEL_EDITOR_GLFW_H
