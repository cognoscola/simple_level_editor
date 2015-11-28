#include "main.h"

int main () {

    hardware = {};

    //start logger system
    assert(restart_gl_log());

    //create our main window
    assert(start_gl());


    GLuint vao;
    GLuint vbo;

    grid = {};
    grid.numberOfLines = 100;
    grid.heightValue = -0.5f;
    GLuint gridVao;

    /**Triangle Coordinates*/
    GLfloat points[] = {
            0.0f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            0.5f, -0.5f, -0.5f,
            0.5, -0.5f, 0.5f,
            0.5f, 0.5f, 0.0f,

            -0.5f, -0.5f, 0.5f,
            -0.5f,-0.5f, -0.5f,
            -0.5f,0.5f, 0.0f,

            0.0f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
    };


    //Create our gridPoints coordinates
    GLfloat gridPoints[grid.numberOfLines * 6];
    for (int i = 0; i < grid.numberOfLines; ++i) {
        //draw the lines parallel to the x axis
        if (i < 50) {
            gridPoints[i * 6     ] = i - 25;  //
            gridPoints[i * 6 + 1 ] = grid.heightValue;
            gridPoints[i * 6 + 2 ] = -100.f;
            gridPoints[i * 6 + 3 ] = i - 25;  //
            gridPoints[i * 6 + 4 ] = grid.heightValue;
            gridPoints[i * 6 + 5 ] = 100.0f;
        }
        //draw the lines parallel to the z axis;
        if (i >= 50) {
            gridPoints[i * 6     ] =  -100.0f;  //
            gridPoints[i * 6 + 1 ] =  grid.heightValue;
            gridPoints[i * 6 + 2 ] = i - 50 - 25.0f;
            gridPoints[i * 6 + 3 ] =  100.0f;  //
            gridPoints[i * 6 + 4 ] =  grid.heightValue;
            gridPoints[i * 6 + 5 ] = i - 50 - 25.0f;
        }
    }

    grid.gridData = gridPoints;

    /*Shader Stuff*/
    const char* vertex_shader =
            "#version 410\n"
                    "uniform mat4 view, proj;"
                    "in vec3 vertex_points;"

                    "void main () {"
                    "	gl_Position = proj * view * vec4 (vertex_points, 1.0);"
                    "}";
    const char* fragment_shader =
            "#version 410\n"
                    "out vec4 fragment_colour;"
                    "void main () {"
                    "	fragment_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
                    "}";

    GLuint vs, fs;
    GLuint shader_programme;

    glfwSetCursorPosCallback(hardware.window,cursor_position_callback);
    glfwSetInputMode(hardware.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(hardware.window, key_callback);
    glfwSetInputMode(hardware.window,GLFW_STICKY_KEYS, 1);

    /* get version info */

    glEnable (GL_DEPTH_TEST); /* enable depth-testing */
    glDepthFunc (GL_LESS);

    //generate buffers for our stuff
    glGenBuffers (1, &grid.gridVbo);
    glBindBuffer (GL_ARRAY_BUFFER, grid.gridVbo);
    glBufferData (GL_ARRAY_BUFFER, grid.numberOfLines* 6 * sizeof (GLfloat), gridPoints,
                  GL_STATIC_DRAW);

    glGenVertexArrays (1, &gridVao);
    glBindVertexArray (gridVao);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, grid.gridVbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glLineWidth((GLfloat) 2.5f);

    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, 36 * sizeof (GLfloat), points,
                  GL_STATIC_DRAW);

    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);
    fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);
    shader_programme = glCreateProgram ();
    glAttachShader (shader_programme, fs);
    glAttachShader (shader_programme, vs);
    glLinkProgram (shader_programme);

    // camera stuff
#define PI 3.14159265359
#define DEG_TO_RAD (2.0 * PI) / 360.0

    float near = 0.1f;
    float far = 100.0f;
    float fov = 67.0f * DEG_TO_RAD;
    float aspect = (float)hardware.vmode->width /(float)hardware.vmode->height;

    // matrix components
    float range = tan (fov * 0.5f) * near;
    float Sx = (2.0f * near) / (range * aspect + range * aspect);
    float Sy = near / range;
    float Sz = -(far + near) / (far - near);
    float Pz = -(2.0f * far * near) / (far - near);
    GLfloat proj_mat[] = {
            Sx, 0.0f, 0.0f, 0.0f,
            0.0f, Sy, 0.0f, 0.0f,
            0.0f, 0.0f, Sz, -1.0f,
            0.0f, 0.0f, Pz, 0.0f
    };

    camera = {};

    //create view matrix
    camera.pos[0] = 0.0f; // don't start at zero, or we will be too close
    camera.pos[1] = 0.0f; // don't start at zero, or we will be too close
    camera.pos[2] = 0.5f; // don't start at zero, or we will be too close
    camera.T = translate (identity_mat4 (), vec3 (-camera.pos[0], -camera.pos[1], -camera.pos[2]));
    camera.Rpitch = rotate_y_deg (identity_mat4 (), -camera.yaw);
    camera.Ryaw = rotate_y_deg (identity_mat4 (), -camera.yaw);
    camera.viewMatrix = camera.Rpitch * camera.T;

    glUseProgram(shader_programme);

    camera.view_mat_location = glGetUniformLocation(shader_programme, "view");
    camera.proj_mat_location = glGetUniformLocation(shader_programme, "proj");

    glUniformMatrix4fv(camera.view_mat_location, 1, GL_FALSE, camera.viewMatrix.m);
    glUniformMatrix4fv(camera.proj_mat_location, 1, GL_FALSE, proj_mat);

    while (!glfwWindowShouldClose (hardware.window)) {
        updateMovement(&camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, hardware.vmode->width, hardware.vmode->height);
        glUseProgram(shader_programme);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        glBindVertexArray(gridVao);
        glDrawArrays(GL_LINES, 0, grid.numberOfLines* 2);

        glfwPollEvents();
        if (GLFW_PRESS == glfwGetKey(hardware.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(hardware.window, 1);
        }
        glfwSwapBuffers(hardware.window);
    }

    /* close GL context and any other GLFW resources */
    glfwTerminate();
    return 0;
}

/**
 * Called every time the cursor moves. It is used to calculate the Camera's direction
 * in window - the window holding the cursor
 * in xpos   - the xposition of the cursor on the screen
 * in ypos   - the yposition of the curose on the screen
 */
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {


    //calculate pitch
    static double  previous_ypos = ypos;
    double position_y_difference = ypos - previous_ypos;
    previous_ypos = ypos;

    //calculate yaw
    static double previous_xpos = xpos;
    double position_x_difference = xpos - previous_xpos;
    previous_xpos = xpos;

    camera.yaw += position_x_difference *camera.signal_amplifier;
    camera.pitch += position_y_difference *camera.signal_amplifier;

    create_versor(camera.quatPitch, camera.pitch, 1.0f, 0.0f, 0.0f);
    create_versor(camera.quatYaw, camera.yaw, 0.0f, 1.0f, 0.0f);

    quat_to_mat4(camera.Rpitch.m, camera.quatPitch);
    quat_to_mat4(camera.Ryaw.m, camera.quatYaw);

//    mult_quat_quat(camera.resultQuat, camera.quatYaw, camera.quatPitch);
//    mult_quat_quat(camera.resultQuat, camera.quatYaw, camera.quatPitch);
//    quat_to_mat4(camera.R.m, camera.resultQuat);

}

/**
 * Called everytime we press a key on the keyboard
 * in window - the focused window
 * in key    - which key?
 * in scancode
 * in action - One of GFLW_PRESS, GLFW_REPEAT or GLFW_RELEASE
 */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_W &&  action == GLFW_PRESS) {
        printf("Pressed W\n");
        camera.move_angle = 0;
        input.wPressed = true;
    }

    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        input.wPressed = false;
    }

    if (key == GLFW_KEY_S &&  action == GLFW_PRESS ) {
        printf("Pressed s\n");
        input.sPressed = true;
        camera.move_angle = 180;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        input.sPressed = false;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        camera.move_angle = -90;
        input.aPressed = true;

    }

    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        input.aPressed = false;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        camera.move_angle = 90;
        input.dPressed = true;
    }

    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        input.dPressed = false;
    }

    if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS) {
        grid.heightValue  += 1.0f;
        updateGridHeight(&grid);
    }

    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS) {
        grid.heightValue  -= 1.0f;
        updateGridHeight(&grid);
    }
}

static void updateGridHeight(Grid* grid){

    //Modify the value
    glBindBuffer(GL_ARRAY_BUFFER, grid->gridVbo);
    GLfloat *data = (GLfloat *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

    if (data != (GLfloat *) NULL) {
        for (int i = 0; i < (grid->numberOfLines*2); ++i) {
            data[i * 3 + 1 ] =grid->heightValue ;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
}

/**
 * calculate a new View Matrix for our shader
 */
static void calculateViewMatrix(Camera* camera){
    camera->T = translate (identity_mat4 (), vec3 (-camera->pos[0], -camera->pos[1], -camera->pos[2]));
    camera->viewMatrix = camera->Rpitch * camera->Ryaw * camera->T;
}

/**
 * Calculate the player's kinematics and render it
 */
static void updateMovement(Camera* camera) {


    if(input.wPressed || input.sPressed || input.aPressed || input.dPressed) {
        camera->pushing = 1;
    }

    if (camera->pushing) {

        const double maxVelocity = 0.1 * (camera->pushing> 0);
        const double acceleration= camera->pushing>0 ? 0.2:0.1;

        if(camera->move_angle == 90.0f || camera->move_angle == -90.0f) {
            vec3 left = cross(vec3(camera->viewMatrix.m[2],camera->viewMatrix.m[6],camera->viewMatrix.m[10]),
                              vec3(camera->viewMatrix.m[1],camera->viewMatrix.m[5],camera->viewMatrix.m[9]));

            camera->velocity.v[0] =(float)(camera->velocity.v[0] * (1-acceleration) +
                                           ( left.v[0]) * ((camera->move_angle == 90 )? 1:-1) * (acceleration *maxVelocity));
            camera->velocity.v[2] =(float)(camera->velocity.v[2] * (1-acceleration) +
                                           ( left.v[2]) * ((camera->move_angle == 90 )? 1:-1) * (acceleration *maxVelocity));
        }else{
            camera->velocity.v[0] =(float)(camera->velocity.v[0] * (1-acceleration) +
                                           ( camera->viewMatrix.m[2]) * ((camera->move_angle == 180 )? -1:1) * (acceleration *maxVelocity));
            camera->velocity.v[1] =(float)(camera->velocity.v[1] * (1-acceleration) +
                                           ( camera->viewMatrix.m[6]) * ((camera->move_angle == 180 )? -1:1) * (acceleration *maxVelocity));
            camera->velocity.v[2] =(float)(camera->velocity.v[2] * (1-acceleration) +
                                           ( camera->viewMatrix.m[10]) * ((camera->move_angle == 180 )? -1:1) * (acceleration *maxVelocity));
        }
        camera->moving = true;
    }

    if (camera->moving) {

        camera->pos[0] += -camera->velocity.v[0] *0.02f;
        camera->pos[2] += -camera->velocity.v[2] *0.02f;
        camera->pos[1] += -camera->velocity.v[1] *0.02f;

        if(dot(camera->velocity,camera->velocity) < 1e-9) {
            printf("Stopping\n");
            camera->velocity.v[0] = camera->velocity.v[2] = camera->velocity.v[1] = 0.0f;
            camera->pushing = 0;
            camera->moving = false;
        }
    }
    if(camera->pushing){
        camera->pushing = -1.0f;
    }

    calculateViewMatrix(camera);
    glUniformMatrix4fv(camera->view_mat_location, 1, GL_FALSE, camera->viewMatrix.m);

}




