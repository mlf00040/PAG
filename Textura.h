//
// Created by Suspr on 17/12/2025.
//

#ifndef PAG_TEXTURA_H
#define PAG_TEXTURA_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "lodepng.h"

#include "ControllerMensajes.h"


class Textura {
private:
    std::string nombre="";
    GLuint idTextura =0;
public:
    Textura();



    virtual ~Textura();

    void cargar(std::string& ruta);

    void enlazar(unsigned int id) const;

    GLuint getIdGl() const;

    void setIdGl(GLuint idGl);

    explicit Textura(const std::string &nombre);

    const std::string &getNombre() const;

    void setNombre(const std::string &nombre);

};


#endif //PAG_TEXTURA_H
