#include <iostream>
#include <cstring>
#include <vector>
#include "glm/gtc/matrix_transform.hpp"

void ParseOBJ(
    const char * ruta, // ruta del obj a abrir
    std::vector<glm::vec3> & vertices, // vector de vertices
    std::vector<glm::vec3> & resultado // vector donde va el resultado
){

    std::cout << "invocando metodo" << std::endl;
    // 1er paso - abrir archivo
    FILE * file = fopen(ruta, "r");

    // sin archivo truena
    if(file == NULL){
        std::cout << "ARCHIVO NO ENCONTRADO" << std::endl;
        return;
    }

    int caras = 0;

    
    while(true){


        // mientras no llegemos al final
        int caracteresLeidos;

        // buffercito para guardar caracteres
        char linea[1];

        caracteresLeidos = fscanf(file, "%s", linea);

        if(caracteresLeidos == EOF)
            break;

        // 3 posibilidades
        if(linea[0] == 'v'){
            // es un vertice

            // vamos a leer la linea actual 
            glm::vec3 actual;
            fscanf(file, "%f %f %f\n", &actual.x, &actual.y, &actual.z);

            std::cout << "V: " << actual.x << " " << actual.y << " " << actual.z << std::endl;

            // lo agregamos a índice de vertices
            vertices.push_back(actual);

        }else if(linea[0] == 'f'){
            
            // es una cara
            int caraActual[3];

            fscanf(file, "%d %d %d\n", &caraActual[0], &caraActual[1], &caraActual[2]);

            std::cout << "F: " << caraActual[0] << " " << caraActual[1] << " " << caraActual[2] << std::endl;

            std::cout << "******************" << std::endl;
            for(int i = 0; i < vertices.size(); i++){

                std::cout << "VERTICE" << std::endl;
            }
            // recorremos los 3 índices
            for(int i = 0; i < 3; i++){

                // agregamos el vec3 que corresponda al índice que nos mandaron
                // MENOS UNO! (MUY IMPORTANTE)
                resultado.push_back(vertices[caraActual[i] - 1]);
            }
            caras++;
        }
    }
}
