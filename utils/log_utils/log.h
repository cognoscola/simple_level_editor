//
// Created by alvaregd on 27/11/15.
//

#ifndef SIMPLE_LEVEL_EDITOR_LOG_H
#define SIMPLE_LEVEL_EDITOR_LOG_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GL_LOG_FILE "/home/alvaregd/Documents/Games/tutorial/gl.log"

bool restart_gl_log();
bool gl_log_err(const char* message, ...);
bool gl_log (const char* message,...);

void _print_shader_info_log(GLuint shader_index);
void _print_programme_info_log(GLuint program);
bool is_valid (GLuint programme);
void print_all(GLuint program);
void log_gl_params ();
void _update_fps_counter (GLFWwindow* window);



GLuint create_programme_from_files (
        const char* vert_file_name, const char* frag_file_name
);

#endif //SIMPLE_LEVEL_EDITOR_LOG_H
