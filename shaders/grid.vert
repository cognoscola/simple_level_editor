#version 410

uniform mat4 view, proj;
in vec3 vertex_points;

void main () {

    gl_Position = proj * view * vec4 (vertex_points, 1.0);
}