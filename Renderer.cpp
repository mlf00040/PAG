//
// Created by Suspr on 23/09/2025.
//

#include "Renderer.h"
#include "glad/glad.h"
#include <GL/gl.h>
#include <string>


namespace PAG {

    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    PAG::Renderer::Renderer() {}

    /**
     * Destructor
     */
    PAG::Renderer::~Renderer() {}

    /**
    * Consulta del objeto único de la clase
    * @return La dirección de memoria del objeto
    */
    PAG::Renderer &PAG::Renderer::getInstancia() {
        if (!instancia)  // Lazy initialization: si aún no existe, lo crea
        {
            instancia = new Renderer();
        }
        return *instancia;
    }

    /**
    * Método para hacer el refresco de la escena
    */
    void Renderer::refrescar ()
    {  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /**
     * Cambia el color de fondo pro los valores actuales de rgb
     */
    void Renderer::pintarColores() {

        glClearColor(red, green, blue, 1);
    }

    /**
    * Método para inicializar los parámetros globales de OpenGL
    */
    void PAG::Renderer::inicializaOpenGL ( )
    {  glClearColor ( red, green, blue, 1 );
        glEnable ( GL_DEPTH_TEST );
        glEnable ( GL_MULTISAMPLE );
    }

    void PAG::Renderer::resizeViewPort(int w,int h) {
        glViewport ( 0, 0, w, h );
    }

    /**
     * Getters and setters de los colores
     * @return
     */
    float Renderer::getGreen() const {
        return green;
    }

    void Renderer::setGreen(float green) {
        Renderer::green = green;
    }

    float Renderer::getRed() const {
        return red;
    }

    void Renderer::setRed(float red) {
        Renderer::red = red;
    }

    float Renderer::getBlue() const {
        return blue;
    }

    void Renderer::setBlue(float blue) {
        Renderer::blue = blue;
    }
}