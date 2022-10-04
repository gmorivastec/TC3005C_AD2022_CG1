#version 330 core 
// estamos usando la versión 3.3 de OpenGL 
// core vs. compatibility - acceso a lo actual vs acceso a lo obsolecido

// especificación de parámetro de entrada
layout(location = 0) in vec3 vertex_modelspace;


// PREGUNTA - cuántas veces se ejecuta esto por frame?
// RESPUESTA - tantos vertices como tengas

// los shaders en GLSL necesitan un main
// GLSL - gl shading language
void main() {

    // en gl_Position guardamos la posición objetivo
    gl_Position.xyz = vertex_modelspace;
    gl_Position.w = 1.0;
}