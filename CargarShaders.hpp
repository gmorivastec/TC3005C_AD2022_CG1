#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <stdlib.h>

using namespace std;

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