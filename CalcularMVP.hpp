#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

mat4 CalcularMVP(){

    // tenemos que regresar la multiplicación de las 3 matrices
    // cada matriz puede tener n transformaciones en ella

    // 1er paso - transformaciones de mundo 
    // mover el objeto en la escena
    mat4 model = mat4(1.0f);

    // si queremos aplicar más transformaciones al modelo con respecto a su posición en el
    // mundo lo hacemos en la model matrix

    mat4 view = lookAt(
        vec3(0, 0, 10), // posición de la cámara en el mundo
        vec3(0, 0, 0), // punto que está siendo observado
        vec3(0, 1, 0) // vector que indica en qué dirección es arriba del mundo
    );

    mat4 projection = perspective(
        radians(60.0f), // ángulo de apertura en FOV 
        1.0f, // relación de aspecto - width / height
        0.01f, // clipping plane cercano y lejano 
        1000.0f
    );

    return projection * view * model;
}