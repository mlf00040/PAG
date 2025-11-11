//
// Created by Suspr on 23/09/2025.
//

#ifndef PAG_RENDERER_H
#define PAG_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "ControllerMensajes.h"
#include "Camara.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "Modelo.h"

#define GLM_ENABLE_EXPERIMENTAL

/**
 * Espacio de nombres para las prácticas de Programación de Aplicaciones
 * Gráficas
 */
namespace PAG {

    class Renderer {
    private:
        static  Renderer* instancia;
        Camara camara;
        float red=0;
        float green=0;
        float blue=0;
        Renderer();

        std::vector<std::unique_ptr<Modelo>> modelos;

        GLuint programIDActivo =0;

    public:
        static Renderer& getInstancia ();
        virtual ~Renderer ();
        void refrescar ();
        void pintarColores();
        void inicializaOpenGL ();
        void resizeViewPort(int w,int h);

        void creaModelo(std::string& ruta);

        void borrarModelo(size_t indice);

        float getGreen() const;

        void setGreen(float green);

        float getRed() const;

        void setRed(float red);

        float getBlue() const;

        void setBlue(float blue);

        Camara &getCamara() ;

        void setCamara(const Camara &camara);

        GLuint getProgramIdActivo() const;

        void setProgramIdActivo(GLuint programIdActivo);

        const std::vector<std::unique_ptr<Modelo>> &getModelos() const;

        void obtenerDatosContexto ();
    };

} // PAG

#endif //PAG_RENDERER_H
