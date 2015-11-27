//
// Created by alvaregd on 27/11/15.
//


#include "glfw.h"


bool start_gl () {
    gl_log ("starting GLFW %s", glfwGetVersionString ());

    glfwSetErrorCallback (glfw_error_callback);
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    hardware.mon = glfwGetPrimaryMonitor ();
    const GLFWvidmode* vmode = glfwGetVideoMode (hardware.mon);
    hardware.window = glfwCreateWindow (hardware.vmode->width, hardware.vmode->height, "Extended GL Init", hardware.mon, NULL);

//    window = glfwCreateWindow (width, height, "Extended Init.", NULL, NULL  );
    if (!hardware.window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    glfwSetWindowSizeCallback (hardware.window, glfw_window_size_callback);
    glfwMakeContextCurrent (hardware.window);

    glfwWindowHint (GLFW_SAMPLES, 4);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit ();

    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);
    gl_log ("renderer: %s\nversion: %s\n", renderer, version);

    return true;
}

//callbacks

void glfw_error_callback(int error, const char* description) {
    gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
}