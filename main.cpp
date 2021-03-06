#include "main.h"

static void getTransformationMatrix(Wall* wall);

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
    glEnableVertexAttribArray(0);

}

void setColourMesh(GLuint* vao, GLuint* bufferObject, GLint dimensions, GLuint* attributeIndex){

    glBindVertexArray (*vao);
    glBindBuffer (GL_ARRAY_BUFFER, *bufferObject);
    glVertexAttribPointer (*attributeIndex, dimensions, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
}


int main () {

    //start logger system
    assert(restart_gl_log());

    hardware = {}; //must initialize window before starting gl stuff

    //create our main window
    assert(start_gl());

    cursor = {};
    cursor.vertexData = new GLfloat[36 + (3 * 6)];
    setCursorCoordinates(cursor.vertexData, &cursor);
    GLfloat*cursorColourData = new GLfloat[36 + (3 * 6)];
    {
        for (int i = 0; i < 12; ++i) {
            cursorColourData[i * 3] = 0.5f;
            cursorColourData[i * 3 + 1] = 0.0f;
            cursorColourData[i * 3 + 2] = 0.5f;
        };
        for (int j = 12; j < 17; ++j) {
            cursorColourData[j * 3] = 0.5f;
            cursorColourData[j * 3 + 1] = 0.5f;
            cursorColourData[j * 3 + 2] = 0.5f;
        }
    }

    grid = {};
    grid.numberOfLines = 100;
    grid.heightValue = 0.0f;

    GLfloat *gridColourData = new GLfloat[100 * 2 * 3];
    {
        int totalVerteces = 100 * 2;
        for (int i = 0; i < totalVerteces; ++i) {
            gridColourData[i * 3  ]  = 0.5f;
            gridColourData[i * 3 + 1]  = 0.0f;
            gridColourData[i * 3 + 2]  = 0.5f;
        }
    }

    //Create our gridPoints coordinates
    GLfloat *gridVertexData = new GLfloat[grid.numberOfLines * 6];
    {
        for (int i = 0; i < grid.numberOfLines; ++i) {
            //draw the lines parallel to the x axis
            if (i < 50) {
                gridVertexData[i * 6] = i - 25;  //
                gridVertexData[i * 6 + 1] = grid.heightValue;
                gridVertexData[i * 6 + 2] = -100.f;
                gridVertexData[i * 6 + 3] = i - 25;  //
                gridVertexData[i * 6 + 4] = grid.heightValue;
                gridVertexData[i * 6 + 5] = 100.0f;
            }
            //draw the lines parallel to the z axis;
            if (i >= 50) {
                gridVertexData[i * 6] = -100.0f;  //
                gridVertexData[i * 6 + 1] = grid.heightValue;
                gridVertexData[i * 6 + 2] = i - 50 - 25.0f;
                gridVertexData[i * 6 + 3] = 100.0f;  //
                gridVertexData[i * 6 + 4] = grid.heightValue;
                gridVertexData[i * 6 + 5] = i - 50 - 25.0f;
            }
        }
    }

    //create our wall items //2 triangles , 3 points each, 3 coordinate



    Wall wall = {};
    wall.scale = vec3(1.0f, 0, 1.0f);
    wall.position = vec3(1.0f, 1.0f, 1.0f);
    GLfloat *wallVertexData = new GLfloat[2 * 3 * 3];
    {
        wallVertexData[0] = wall.scale.v[0] * 0.5f;
        wallVertexData[1] = 0.0f;
        wallVertexData[2] = - wall.scale.v[2] * 0.5f;
        wallVertexData[3] = -wall.scale.v[0] * 0.5f;
        wallVertexData[4] = 0.0f;
        wallVertexData[5] = + wall.scale.v[2] * 0.5f;
        wallVertexData[6] = - wall.scale.v[0] * 0.5f;
        wallVertexData[7] = 0.0f;
        wallVertexData[8] = - wall.scale.v[2] * 0.5f;
        wallVertexData[9] = + wall.scale.v[0] * 0.5f;
        wallVertexData[10] = 0.0f;
        wallVertexData[11] = - wall.scale.v[2] * 0.5f;
        wallVertexData[12] = + wall.scale.v[0] * 0.5f;
        wallVertexData[13] = 0.0f;
        wallVertexData[14] = + wall.scale.v[2] * 0.5f;
        wallVertexData[15] = - wall.scale.v[0] * 0.5f;
        wallVertexData[16] = 0.0f;
        wallVertexData[17] = + wall.scale.v[2] * 0.5f;
    }

    GLfloat *wallColourData = new GLfloat[2 * 3 * 3];
    {
        for (int i = 0; i < 6; ++i) {
            wallColourData[i * 3 + 0] = 0.8f;
            wallColourData[i * 3 + 1] = 0.8f;
            wallColourData[i * 3 + 2] = 0.8f;
        }
    }



    glfwSetCursorPosCallback(hardware.window,cursor_position_callback);
    glfwSetInputMode(hardware.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(hardware.window, key_callback);
    glfwSetInputMode(hardware.window,GLFW_STICKY_KEYS, 1);

    GLuint shader_program = create_programme_from_files(VERTEX_SHADER, FRAGMENT_SHADER);
    /* get version info */
    glEnable (GL_DEPTH_TEST); /* enable depth-testing */
    glDepthFunc (GL_LESS);

    createVertexBufferObject(&wallColourVbo, 3 * 3 * 2 * sizeof(GLfloat), wallColourData);
    createVertexBufferObject(&wallVertexVbo, 3 * 3 * 2 * sizeof(GLfloat), wallVertexData);

    createVertexBufferObject(&grid.vertexVbo, grid.numberOfLines * 6 * sizeof(GLfloat), gridVertexData);
    createVertexBufferObject(&grid.colourVbo, grid.numberOfLines * 6 * sizeof(GLfloat), gridColourData);

    createVertexBufferObject(&cursor.vertexVbo, (36 + (3 * 6)) * sizeof(GLfloat), cursor.vertexData);
    createVertexBufferObject(&cursor.colourVbo, (36 + (3 * 6)) * sizeof(GLfloat), cursorColourData);

    createVertexArrayObjet(&wallVao, &wallVertexVbo, 3);
    createVertexArrayObjet(&grid.vao, &grid.vertexVbo, 3);
    createVertexArrayObjet(&cursor.vao, &cursor.vertexVbo, 3);

    cursor.colourAttributeIndex = 1;
    grid.colourAttributeIndex = 1;
    wallColourAttributeIndex = 1;
    setColourMesh(&cursor.vao, &cursor.colourVbo, 3, &cursor.colourAttributeIndex);
    setColourMesh(&grid.vao, &grid.colourVbo, 3, &grid.colourAttributeIndex);
    setColourMesh(&wallVao, &wallColourVbo, 3, &wallColourAttributeIndex);

    free(cursor.vertexData);
    free(cursorColourData);
    free(wallVertexData);
    free(wallColourData);

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

    cursor.yaw = cursor.roll = cursor.pitch += 0.0f;
    calculateCursorRotations(&cursor);

    //create the viewmatrix of the wall
    wall.T= translate (identity_mat4 (), vec3 (2.0f, 2.0f, 2.0f));


    glUseProgram(shader_program);

    camera.view_mat_location = glGetUniformLocation(shader_program, "view");
    camera.proj_mat_location = glGetUniformLocation(shader_program, "proj");

    glUniformMatrix4fv(camera.view_mat_location, 1, GL_FALSE, camera.viewMatrix.m);
    glUniformMatrix4fv(camera.proj_mat_location, 1, GL_FALSE, proj_mat);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    walls.push_back(wall);

    while (!glfwWindowShouldClose (hardware.window)) {
        updateMovement(&camera);
        calculateViewMatrices(&camera, &cursor);
        //set the new view matrix @ the shader level

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, hardware.vmode->width, hardware.vmode->height);
        glUseProgram(shader_program);

        //draw the cursor
        glUniformMatrix4fv(camera.view_mat_location, 1, GL_FALSE, cursor.viewMatrix.m);
        glBindVertexArray(cursor.vao);
        glDrawArrays(GL_TRIANGLES, 0, 18);

        //draw the walls in place
        //for each wall, draw it.
        //get view matrix

        for ( std::vector<Wall>::iterator it = walls.begin(); it != walls.end(); ++it) {
            Wall tempWall = *it;
            getTransformationMatrix(&tempWall);
            glUniformMatrix4fv(camera.view_mat_location, 1, GL_FALSE, tempWall.transformationMatrix.m);
            glBindVertexArray(wallVao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }


        //draw the grid
        glUniformMatrix4fv(camera.view_mat_location, 1, GL_FALSE, camera.viewMatrix.m);
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
            if (action == GLFW_PRESS) {
                switch (state) {
                    case STATE_POSITION: cursor.Y = grid.heightValue += 1.0f;updateGridHeight(&grid, &cursor); break;
                    case STATE_SCALE:break;
                    case STATE_ORIENTATION:
                        cursor.roll += 45.0f;
                        calculateCursorRotations(&cursor);break;
                }
            }
            break;
        case GLFW_KEY_PAGE_DOWN:
            if (action == GLFW_PRESS) {
                switch (state) {
                    case STATE_POSITION:
                        cursor.Y = grid.heightValue -= 1.0f;
                        updateGridHeight(&grid, &cursor);
                        break;
                    case STATE_SCALE:break;
                    case STATE_ORIENTATION:
                        cursor.roll -= 45.0f;
                        calculateCursorRotations(&cursor);
                        break;
                }
            }
            break;

        case GLFW_KEY_UP:
            if(action == GLFW_PRESS || action == GLFW_REPEAT) {
                switch (state) {
                    case STATE_POSITION:
                        cursor.Z += 1.0f;
                        break;
                    case STATE_SCALE:
                        cursor.Xs += 0.2f;
                        updateScales(&cursor);
                        break;
                    case STATE_ORIENTATION:
                        cursor.pitch += 45.0f;
                        calculateCursorRotations(&cursor);
                        break;
                }
            }
            break;
        case GLFW_KEY_DOWN:
            if(action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                switch (state) {
                    case STATE_POSITION:
                        cursor.Z -= 1.0f;
                        break;
                    case STATE_SCALE:
                        cursor.Xs -= 0.2f;
                        updateScales(&cursor);
                        break;
                    case STATE_ORIENTATION:
                        cursor.pitch -= 45.0f;
                        calculateCursorRotations(&cursor);
                        break;
                }
            }
            break;
        case GLFW_KEY_LEFT:
            if(action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                switch (state) {
                    case STATE_POSITION:
                        cursor.X += 1.0f;
                        break;
                    case STATE_SCALE:
                        cursor.Zs += 0.2f;
                        updateScales(&cursor);
                        break;
                    case STATE_ORIENTATION:
                        cursor.yaw += 45.0f;
                        calculateCursorRotations(&cursor);
                        break;
                }
            }
            break;
        case GLFW_KEY_RIGHT:
            if(action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                switch (state) {
                    case STATE_POSITION:
                        cursor.X -= 1.0f;
                        break;
                    case STATE_SCALE:
                        cursor.Zs -= 0.2f;
                        updateScales(&cursor);
                        break;
                    case STATE_ORIENTATION:
                        cursor.yaw -= 45.0f;
                        calculateCursorRotations(&cursor);
                        break;
                }
            }
            break;
        case GLFW_KEY_ENTER: {

            //create a new wall and place it into walls vector.
            Wall wall = {};
            wall.position = vec3(cursor.X, cursor.Y, cursor.Z);
            wall.orientation = vec3(cursor.pitch, cursor.yaw, cursor.roll);
            wall.scale = vec3(cursor.Xs, cursor.Ys, cursor.Zs);
            wall.T = translate(identity_mat4(), vec3(-wall.position.v[0], wall.position.v[1], -wall.position.v[2]));
            walls.push_back(wall);
        }
            break;
        case GLFW_KEY_1:state = STATE_POSITION;   break;
        case GLFW_KEY_2:state = STATE_ORIENTATION;break;
        case GLFW_KEY_3:state = STATE_SCALE;      break;
    }
}


/**
 * Called every time the cursor moves. It is used to calculate the Camera's direction
 * in window - the window holding the cursor
 * in xpos   - the xposition of the cursor on the screen
 * in ypos   - the yposition of the curose on the screen
 */
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {

    GLfloat quat[] = {0.0f,0.0f,0.0f,0.0f};

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
    create_versor(quat, camera.pitch, 1.0f, 0.0f, 0.0f);
    quat_to_mat4(camera.Rpitch.m, quat);
    create_versor(quat, camera.yaw, 0.0f, 1.0f, 0.0f);
    quat_to_mat4(camera.Ryaw.m,quat);

//    mult_quat_quat(camera.resultQuat, camera.quatYaw, camera.quatPitch);
//    mult_quat_quat(camera.resultQuat, camera.quatYaw, camera.quatPitch);
//    quat_to_mat4(camera.R.m, camera.resultQuat);

}

/**
 * Change the height of the floor grid
 */
static void updateGridHeight(Grid* grid, Cursor* cursor){

    //Modify the value
    glBindBuffer(GL_ARRAY_BUFFER, grid->vertexVbo);
    GLfloat *data = (GLfloat *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (data != (GLfloat *) NULL) {
        for (int i = 0; i < (grid->numberOfLines*2); ++i) {
            data[i * 3 + 1 ] =grid->heightValue ;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
//    updateScales(cursor);
//
}

static void updateScales(Cursor *cursor){

    glBindBuffer(GL_ARRAY_BUFFER, cursor->vertexVbo);
    GLfloat *data = (GLfloat *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (data != (GLfloat *) NULL) {
        setCursorCoordinates(data, cursor);
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
}

static void setCursorCoordinates(GLfloat* data, Cursor* cursor){

    ///first triangle
    data[0] = 0.0f;
    data[1] = 0.2f;
    data[2] = - cursor->Zs*0.5f;
    data[3] = + cursor->Xs*0.5f;
    data[4] = 0.0f;
    data[5] = - cursor->Zs*0.5f;
    data[6] = - cursor->Xs*0.5f;
    data[7] = 0.0f;
    data[8] = - cursor->Zs*0.5f;

    //2nd triangle
    data[9]  =  + cursor->Xs*0.5f;
    data[10] =  + 0.2f;
    data[11] =  0.0f;
    data[12] =  + cursor->Xs*0.5f;
    data[13] =  0.0f;
    data[14] =  + cursor->Zs*0.5f;
    data[15] =  + cursor->Xs*0.5f;
    data[16] =  0.0f;
    data[17] =  - cursor->Zs* 0.5f;

    //third triangle
    data[18] = 0.0f;
    data[19] = + 0.2f;
    data[20] = +cursor->Zs*0.5f;
    data[21] = -cursor->Xs* 0.5f;
    data[22] = 0.0f;
    data[23] = + cursor->Zs* 0.5f;
    data[24] = + cursor->Xs*0.5f;
    data[25] = 0.0f;
    data[26] = + cursor->Zs* 0.5f;

    //fourth triangle
    data[27] = - cursor->Xs*0.5f;
    data[28] = + 0.2f;
    data[29] = 0.0f;
    data[30] = - cursor->Xs*0.5f;
    data[31] = 0.0f;
    data[32] =  - cursor->Zs*0.5f;
    data[33] =  - cursor->Xs*0.5f;
    data[34] = 0.0f;
    data[35] =  + cursor->Zs* 0.5f;

    //middle square
    data[36] = + cursor->Xs*0.5f;
    data[37] = 0.0f;
    data[38] = - cursor->Zs*0.5f;
    data[39] = -cursor->Xs* 0.5f;
    data[40] = 0.0f;
    data[41] = + cursor->Zs* 0.5f;
    data[42] = - cursor->Xs*0.5f;
    data[43] = 0.0f;
    data[44] = - cursor->Zs*0.5f;

    data[45] = + cursor->Xs*0.5f;
    data[46] = 0.0f;
    data[47] = - cursor->Zs*0.5f;
    data[48] = + cursor->Xs*0.5f;
    data[49] = 0.0f;
    data[50] = + cursor->Zs*0.5f;
    data[51] = - cursor->Xs* 0.5f;
    data[52] = 0.0f;
    data[53] = + cursor->Zs* 0.5f;
}

/**
 * calculate a new View Matrix
 */
static void calculateViewMatrices(Camera *camera, Cursor *cursor){
    camera->T = translate (identity_mat4 (), vec3 (-camera->pos[0], -camera->pos[1], -camera->pos[2]));
    camera->viewMatrix = camera->Rpitch * camera->Ryaw * camera->T;
    cursor->T = translate (identity_mat4 (), vec3 (-cursor->X, +cursor->Y, -cursor->Z));
    cursor->viewMatrix =   camera->viewMatrix * cursor->T *  cursor->Rpitch * cursor->Ryaw * cursor->Rroll ;
}

static void getTransformationMatrix(Wall* wall){
    wall->transformationMatrix = camera.viewMatrix * wall->T;
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


}

static void calculateCursorRotations(Cursor *cursor){
    GLfloat quat[] = {0.0f,0.0f,0.0f,0.0f};
    create_versor(quat, cursor->pitch, 1.0f, 0.0f, 0.0f);
    quat_to_mat4(cursor->Rpitch.m, quat);
    create_versor(quat, cursor->yaw, 0.0f, 1.0f, 0.0f);
    quat_to_mat4(cursor->Ryaw.m, quat);
    create_versor(quat, cursor->roll, 0.0f, 0.0f, 1.0f);
    quat_to_mat4(cursor->Rroll.m, quat);
}





