
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// glew - gl extension wrangler
// librería para manejo de extensiones en OpenGL
#include <GL/glew.h>

// glfw - librería para creación de ventanas 
// y manejo de input para OpenGL / Vulkan
#include <GLFW/glfw3.h>

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

    // loop que va a correr mientras no se detenga explícitamente la ejecución
    do {

        // limpiar la ventana en el buffer de color
        // cada loop re-colorea con el color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        // aquí ira la dibujada

        // intercambio de buffers
        glfwSwapBuffers(window);

        // solicitar actualización de eventos en ventana
        glfwPollEvents();

    }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && 
        glfwWindowShouldClose(window) == 0);

    
    // esto siempre sí se queda
    // cerrar recursos utilizados por glfw
    glfwTerminate();

    return 0;
}