#version 330 core
// default es core

// definimos variable de salida
out vec3 color;

// PREGUNTA - cuántas veces corre este?
// RESPUESTA - 1 vez por fragmento

// Tambien necesitamos un main
void main() {

    color = vec3(0, 0, 1);
}