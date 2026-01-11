//
// Created by Suspr on 06/10/2025.
//

#ifndef PAG_CONTROLLERSHADERS_H
#define PAG_CONTROLLERSHADERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>


class ControllerShaders {
private:
    struct shaderProgram {
        GLint id;               //id del shader program
        std::string nombre;     //nombre del shaderProgram
        GLuint idVs;           //puntero al id que se guarda en los vertex
        GLuint idFs;           //puntero al id que se guarda en los fragment
    };

    static  ControllerShaders* instancia;

    std::vector<shaderProgram> Programas;

    ControllerShaders();

    GLuint cargaShader(const std::string &ruta,GLenum tiposhader);
public:

    static ControllerShaders& getInstancia ();
    virtual ~ControllerShaders ();

    std::string crearPrograma (const std::string &nombre,const std::string rutaV,const std::string rutaF );
    std::string usarPrograma (const std::string &nombre);
    std::string eliminarPrograma(const std::string &nombre);

    GLuint getProgramId(const std::string& nombre);

    std::vector<std::string > listaShadersPrograms();

    void limpiar();

    bool existePrograma(const std::string &nombre);

};


#endif //PAG_CONTROLLERSHADERS_H
