#include "main.h"

static void updateCursorXZPosition(Cursor* cursor);



void createVertexBufferObject(GLuint *name, size_t size, GLfloat *data){

    glGenBuffers (1, name);
    glBindBuffer (GL_ARRAY_BUFFER, *name);
    glBufferData (GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void createVertexArrayObjet(GLuint* name, GLuint* bufferObject, GLint dimensions){

    glGenVertexArrays (1, name);
    glBindVertexArray (*name);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, *bufferObject);
    glVertexAttribPointer (0, dimensions, GL_FLOAT, GL_FALSE, 0, NULL);
}


int main () {

    //initialize window variables
    hardware = {};

    //start logger system
    assert(restart_gl_log());

    //create our main window
    assert(start_gl());

    cursor = {};
    GLfloat points[]  = {
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
    cursor.data = points;

    grid = {};
    grid.numberOfLines = 100;
    grid.heightValue = -0.5f;

    /**Triangle Coordinates*/

    //Create our gridPoints coordinates
    grid.data = new GLfloat[grid.numberOfLines * 6];
    for (int i = 0; i < grid.numberOfLines; ++i) {
        //draw the lines parallel to the x axis
        if (i < 50) {
            grid.data[i * 6     ] = i - 25;  //
            grid.data[i * 6 + 1 ] = grid.heightValue;
            grid.data[i * 6 + 2 ] = -100.f;
            grid.data[i * 6 + 3 ] = i - 25;  //
            grid.data[i * 6 + 4 ] = grid.heightValue;
            grid.data[i * 6 + 5 ] = 100.0f;
        }
        //draw the lines parallel to the z axis;
        if (i >= 50) {
            grid.data[i * 6     ] =  -100.0f;  //
            grid.data[i * 6 + 1 ] =  grid.heightValue;
            grid.data[i * 6 + 2 ] = i - 50 - 25.0f;
            grid.data[i * 6 + 3 ] =  100.0f;  //
            grid.data[i * 6 + 4 ] =  grid.heightValue;
            grid.data[i * 6 + 5 ] = i - 50 - 25.0f;
        }
    }

    GLuint shader_program = create_programme_from_files(VERTEX_SHADER, FRAGMENT_SHADER);

    glfwSetCursorPosCallback(hardware.window,cursor_position_callback);
    glfwSetInputMode(hardware.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(hardware.window, key_callback);
    glfwSetInputMode(hardware.window,GLFW_STICKY_KEYS, 1);

    /* get version info */

    glEnable (GL_DEPTH_TEST); /* enable depth-testing */
    glDepthFunc (GL_LESS);

    createVertexBufferObject(&grid.vbo, grid.numberOfLines * 6 * sizeof(GLfloat), grid.data);
    createVertexArrayObjet(&grid.vao, &grid.vbo, 3);
    createVertexBufferObject(&cursor.vbo, 36 * sizeof(GLfloat), cursor.data);
    createVertexArrayObjet(&cursor.vao, &cursor.vbo, 3);

    // camera stuff
#define PI 3.14159265359
#define DEG_TO_RAD (2.0 * PI) / 360.0

    float near = 0.1f;
    float far = 100.0f;
    double fov = 67.0f * DEG_TO_RAD;
    float aspect = (float)hardware.vmode->width /(float)hardware.vmode->height;

    // matrix components
    double range = tan (fov * 0.5f) * near;
    double Sx = (2.0f * near) / (range * aspect + range * aspect);
    double Sy = near / range;
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

    glUseProgram(shader_program);

    camera.view_mat_location = glGetUniformLocation(shader_program, "view");
    camera.proj_mat_location = glGetUniformLocation(shader_program, "proj");

    glUniformMatrix4fv(camera.view_mat_location, 1, GL_FALSE, camera.viewMatrix.m);
    glUniformMatrix4fv(camera.proj_mat_location, 1, GL_FALSE, proj_mat);

    while (!glfwWindowShouldClose (hardware.window)) {
        updateMovement(&camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, hardware.vmode->width, hardware.vmode->height);
        glUseProgram(shader_program);

        glBindVertexArray(cursor.vao);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        glBindVertexArray(grid.vao);
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

    //reduce signal
    camera.yaw += position_x_difference *camera.signal_amplifier;
    camera.pitch += position_y_difference *camera.signal_amplifier;

    //calculate rotation sequence
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

    switch (key) {
        case GLFW_KEY_W:
            input.wPressed = action == GLFW_PRESS ? true: ((action == GLFW_RELEASE) ? false: input.wPressed);
            camera.move_angle =  action == GLFW_PRESS ? 0:camera.move_angle;
            break;
        case GLFW_KEY_S:
            input.sPressed = action == GLFW_PRESS ? true: ((action == GLFW_RELEASE) ? false: input.sPressed);
            camera.move_angle =  action == GLFW_PRESS ? 180:camera.move_angle;
            break;
        case GLFW_KEY_A:
            input.aPressed = action == GLFW_PRESS ? true: ((action == GLFW_RELEASE) ? false: input.aPressed);
            camera.move_angle =  action == GLFW_PRESS ? -90:camera.move_angle;
            break;
        case GLFW_KEY_D:
            input.dPressed = action == GLFW_PRESS ? true: ((action == GLFW_RELEASE) ? false: input.dPressed);
            camera.move_angle =  action == GLFW_PRESS ? 90:camera.move_angle;
            break;
        case GLFW_KEY_PAGE_UP:
            if (action == GLFW_PRESS) { grid.heightValue += 1.0f;updateGridHeight(&grid, &cursor);}
            break;
        case GLFW_KEY_PAGE_DOWN:
            if (action == GLFW_PRESS) { grid.heightValue -= 1.0f; updateGridHeight(&grid, &cursor);}
            break;
        case GLFW_KEY_UP:
            if(action == GLFW_PRESS || action == GLFW_REPEAT)
            { cursor.editZValue = 1.0f;cursor.editXValue = 0;updateCursorXZPosition(&cursor); }
            break;
        case GLFW_KEY_DOWN:
            if(action == GLFW_PRESS || action == GLFW_REPEAT)
            { cursor.editZValue = -1.0f;cursor.editXValue = 0;updateCursorXZPosition(&cursor); }
            break;
        case GLFW_KEY_LEFT:
            if(action == GLFW_PRESS || action == GLFW_REPEAT)
            { cursor.editXValue = 1.0f;cursor.editZValue = 0;updateCursorXZPosition(&cursor);}
            break;
        case GLFW_KEY_RIGHT:
            if(action == GLFW_PRESS || action == GLFW_REPEAT)
            { cursor.editXValue = -1.0f;cursor.editZValue = 0;updateCursorXZPosition(&cursor);}
            break;
    }
}

static void updateCursorXZPosition(Cursor* cursor){
    glBindBuffer(GL_ARRAY_BUFFER, cursor->vbo);
    GLfloat *data = (GLfloat *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (data != (GLfloat *) NULL) {
        data[0]  += cursor->editXValue; data[3]  += cursor->editXValue; data[6]  += cursor->editXValue;
        data[9]  += cursor->editXValue; data[12]  += cursor->editXValue; data[15]  += cursor->editXValue;
        data[18]  += cursor->editXValue; data[21]  += cursor->editXValue; data[24]  += cursor->editXValue;
        data[27]  += cursor->editXValue; data[30]  += cursor->editXValue; data[33]  += cursor->editXValue;
        data[2]  += cursor->editZValue; data[5]  += cursor->editZValue; data[8]  += cursor->editZValue;
        data[11] += cursor->editZValue; data[14] += cursor->editZValue; data[17] += cursor->editZValue;
        data[20] += cursor->editZValue; data[23] += cursor->editZValue; data[26] += cursor->editZValue;
        data[29] += cursor->editZValue; data[32] += cursor->editZValue; data[35] += cursor->editZValue;
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
}

/**
 * Change the height of the floor grid
 */
static void updateGridHeight(Grid* grid, Cursor* cursor){

    //Modify the value
    glBindBuffer(GL_ARRAY_BUFFER, grid->vbo);
    GLfloat *data = (GLfloat *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (data != (GLfloat *) NULL) {
        for (int i = 0; i < (grid->numberOfLines*2); ++i) {
            data[i * 3 + 1 ] =grid->heightValue ;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, cursor->vbo);
    GLfloat *data2 = (GLfloat *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (data2 != (GLfloat *) NULL) {
        data2[7] = data2[4] = grid->heightValue;
        data2[1]   =  data2[4] + 0.2f;
        data2[10]  =  data2[13] = data2[4]; data2[16] = data2[4] + 0.2f;
        data2[19]  =   data2[22]= data2[4] ; data2[25] = data2[4] + 0.2f;
        data2[28]  = grid->heightValue + 0.2f; data2[31] = data2[34]  = grid->heightValue ;
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
}

/**
 * calculate a new View Matrix
 */
static void calculateViewMatrix(Camera* camera){
    camera->T = translate (identity_mat4 (), vec3 (-camera->pos[0], -camera->pos[1], -camera->pos[2]));
    camera->viewMatrix = camera->Rpitch * camera->Ryaw * camera->T;
}

/**
 * Calculate the player's kinematics and render it
 * When we detect a keypress, push becomes 1 (positive acceleration) until we release the key.
 * When key releases pushing becomes -1 (negative acceleration) to indicate that we are slowing down.
 */
static void updateMovement(Camera* camera) {

    //if any of the WASD keys are pressed, the camera will move
    if(input.wPressed || input.sPressed || input.aPressed || input.dPressed) {
        camera->pushing = 1;
    }

    //while we push,
    if (camera->pushing) {

        //set linear motion
        const double maxVelocity = 0.1 * (camera->pushing> 0);
        const double acceleration= camera->pushing>0 ? 0.2:0.1;

        if(camera->move_angle == 90.0f || camera->move_angle == -90.0f) {

            //Player has pressed either straf left or straf right, calculate the direction Vector using cross product
            //of actor's heading and the up direction
            vec3 left = cross(vec3(camera->viewMatrix.m[2],camera->viewMatrix.m[6],camera->viewMatrix.m[10]),
                              vec3(camera->viewMatrix.m[1],camera->viewMatrix.m[5],camera->viewMatrix.m[9]));

            //update the camera's velocity accordingly
            camera->velocity.v[0] =(float)(camera->velocity.v[0] * (1-acceleration) +
                                           ( left.v[0]) * ((camera->move_angle == 90 )? 1:-1) * (acceleration *maxVelocity));
            camera->velocity.v[2] =(float)(camera->velocity.v[2] * (1-acceleration) +
                                           ( left.v[2]) * ((camera->move_angle == 90 )? 1:-1) * (acceleration *maxVelocity));
        }else{

            //player has hit forward (w) or backwards (s). update the velocity in these directions
            camera->velocity.v[0] =(float)(camera->velocity.v[0] * (1-acceleration) +
                                           ( camera->viewMatrix.m[2]) * ((camera->move_angle == 180 )? -1:1) * (acceleration *maxVelocity));
            camera->velocity.v[1] =(float)(camera->velocity.v[1] * (1-acceleration) +
                                           ( camera->viewMatrix.m[6]) * ((camera->move_angle == 180 )? -1:1) * (acceleration *maxVelocity));
            camera->velocity.v[2] =(float)(camera->velocity.v[2] * (1-acceleration) +
                                           ( camera->viewMatrix.m[10]) * ((camera->move_angle == 180 )? -1:1) * (acceleration *maxVelocity));
        }
        camera->moving = true;
    }

    //while we are moving (velocity is nonzero), update the camera's position
    if (camera->moving) {
        camera->pos[0] += -camera->velocity.v[0] *0.02f;
        camera->pos[2] += -camera->velocity.v[2] *0.02f;
        camera->pos[1] += -camera->velocity.v[1] *0.02f;
        if(dot(camera->velocity,camera->velocity) < 1e-9) {
            camera->velocity.v[0] = camera->velocity.v[2] = camera->velocity.v[1] = 0.0f;
            camera->pushing = 0;
            camera->moving = false;
        }
    }
    if(camera->pushing){
        camera->pushing = -1;
    }

    calculateViewMatrix(camera);
    //set the new view matrix @ the shader level
    glUniformMatrix4fv(camera->view_mat_location, 1, GL_FALSE, camera->viewMatrix.m);

}




