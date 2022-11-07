
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

// glew - gl extension wrangler
// librería para manejo de extensiones en OpenGL
#include <GL/glew.h>

// glfw - librería para creación de ventanas 
// y manejo de input para OpenGL / Vulkan
#include <GLFW/glfw3.h>

#include "CargarShaders.hpp"
#include "CalcularMVP.hpp"
#include "ParseOBJ.hpp"

using namespace std;

GLFWwindow* window;

int main(){

    cout << "HOLA MUNDO" << endl;

    // inicializamos GLFW
    if(!glfwInit()){

        // si regreso código de error terminamos aplicación
        cout << "ERROR AL INICIALIZAR GLFW" << endl;
        getchar();
        return -1;
    }

    // vamos a configurar la ventana
    // establecemos un montón de "hints" que son parámetros
    // para nuestra nueva ventana
    glfwWindowHint(GLFW_SAMPLES, 4);

    // version de opengl a utilizar
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // parámetro opcional indispensable para mac
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ancho
    // alto
    // nombre de la ventana
    // si es ventana o si es fullscreen el monitor a utilizar
    // si está compartiendo recursos
    window = glfwCreateWindow(800, 800, "VENTANA 1", NULL, NULL);

    // si la ventana fue nula hubo error en creacion
    if( window == NULL){

        cout << "ERROR EN CREACION DE VENTANA" << endl;
        getchar();
        glfwTerminate();
        return -1;
    }

    // hay que hacer el contexto de opengl el actual en el hilo 
    glfwMakeContextCurrent(window);

    // inicializar glew 
    if(glewInit() != GLEW_OK){

        cout << "ERROR AL INICIALIZAR GLEW" << endl;
        getchar();
        glfwTerminate();
        return -1;
    }

    // habilitar captura de input para ventana en GLFW
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // limpiando el fondo con opengl
    // el color se define con 4 canales (red, green, blue, alpha)
    // los valores van de 0 a 1
    glClearColor(1.0f, 179.0/255.0f, 0.0f, 0.0f);

    // obtener program con shaders incluidos
    GLuint programID = CargarShaders("shader.vert", "shader.frag");

    // preparar buffer de entrada
    // para dibujado se va a utilizar un buffer

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // vamos a generar la info de los vértices
    static const GLfloat vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f 
    };

    vector<vec3> vertices;
    vector<vec3> resultado;
    ParseOBJ("triangulo.obj", vertices, resultado);

    GLuint vertexBuffer;

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, resultado.size() * sizeof(vec3), &resultado[0], GL_STATIC_DRAW);    

    // para poder mandar un valor uniforme
    // necesitamos un índice que nos diga cuál es
    GLuint DesplazamientoID = glGetUniformLocation(programID, "desplazamiento");
    GLuint mvpID = glGetUniformLocation(programID, "mvp");

    mat4 mvp = CalcularMVP();

    int frames = 0;

    // update / tick / loop principal
    // loop que va a correr mientras no se detenga explícitamente la ejecución
    do {

        // limpiar la ventana en el buffer de color
        // cada loop re-colorea con el color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        // utilizar programa de shaders
        glUseProgram(programID);
        
        // AQUÍ MANDAMOS TODOS LOS UNIFORMS QUE QUERAMOS
        glUniform1f(DesplazamientoID, frames/1000.0f);
        glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

        // habilitar uso de array para dibujado 
        // mandamos un índice 
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        // parametrizar datos que se obtienen de buffer array
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // A DIBUJAR EL TRIÁNGULO!
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);


        // intercambio de buffers
        glfwSwapBuffers(window);

        // solicitar actualización de eventos en ventana
        glfwPollEvents();

        frames++;

    }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && 
        glfwWindowShouldClose(window) == 0);

    // limpieza de buffers
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    
    // esto siempre sí se queda
    // cerrar recursos utilizados por glfw
    glfwTerminate();

    return 0;
}