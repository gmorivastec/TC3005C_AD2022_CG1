
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

using namespace std;

GLFWwindow* window;

// hagamos un método aparte para carga de shader
// el resultado es un program que vamos a utlizar
GLuint CargarShaders(const char * ruta_vertex, const char * ruta_fragment){

    // crear shaders 
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // leer el código y cargamnos en shader
    // empezamos con fragment
    string VertexShaderCode;
    
    // stream de entrada 
    ifstream VertexShaderStream(ruta_vertex, std::ios::in);

    // abrimos stream y leemos contenido
    if(VertexShaderStream.is_open()){

        stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    } else {
        // no se pudo leer archivo
        cout << "NO SE PUDO LEER VERTEX SHADER" << endl;
        getchar();
        return 0;
    }

    // cargando fragment a un string 
    string FragmentShaderCode;
    ifstream FragmentShaderStream(ruta_fragment, ios::in);
    if(FragmentShaderStream.is_open()){

        stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    } else {
        // no se pudo leer archivo
        cout << "NO SE PUDO LEER FRAGMENT SHADER" << endl;
        getchar();
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // compilar vertex shader
    cout << "compilando vertex shader" << endl;
    char const * VertexSource = VertexShaderCode.c_str();
    glShaderSource(vertexShaderId, 1, &VertexSource, NULL);
    glCompileShader(vertexShaderId);

    // checar vertex shader
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0){
        vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(vertexShaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        cout << &VertexShaderErrorMessage[0] << endl;
    }

    // compilar fragment shader
    cout << "compilando fragment shader" << endl;
    char const * FragmentSource = FragmentShaderCode.c_str();
    glShaderSource(fragmentShaderId, 1, &FragmentSource, NULL);
    glCompileShader(fragmentShaderId);

    // checar fragment shader
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0){
        vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderId, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        cout << &FragmentShaderErrorMessage[0] << endl;
    }

    // hacer link en program
    cout << "haciendo link de programa + shaders" << endl;
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vertexShaderId);
    glAttachShader(ProgramID, fragmentShaderId);
    glLinkProgram(ProgramID);

    // checar programa
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0){
        vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        cout << &ProgramErrorMessage[0] << endl;
    }

    // limpiar shaders
    glDetachShader(ProgramID, vertexShaderId);
    glDetachShader(ProgramID, fragmentShaderId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    // regresar id de programa
    return ProgramID;
}

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

    GLuint vertexBuffer;

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);    

    // loop que va a correr mientras no se detenga explícitamente la ejecución
    do {

        // limpiar la ventana en el buffer de color
        // cada loop re-colorea con el color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        // utilizar programa de shaders
        glUseProgram(programID);

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