#version 330 core 
// estamos usando la versión 3.3 de OpenGL 
// core vs. compatibility - acceso a lo actual vs acceso a lo obsolecido

// especificación de parámetro de entrada
layout(location = 0) in vec3 vertex_modelspace;

// aquí puede haber variables a las que denominamos uniform
uniform float desplazamiento;
uniform mat4 mvp;

// PREGUNTA - cuántas veces se ejecuta esto por frame?
// RESPUESTA - tantos vertices como tengas

// los shaders en GLSL necesitan un main
// GLSL - gl shading language
void main() {

    // en gl_Position guardamos la posición objetivo
    vec4 transformado =  mvp * vec4(
        vertex_modelspace.x,
        vertex_modelspace.y,
        vertex_modelspace.z,
        1.0
    );

    gl_Position = vec4(
       transformado.x,
       transformado.y,
       transformado.z,
       transformado.w 
    );
}