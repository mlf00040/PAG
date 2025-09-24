//
// Created by Suspr on 23/09/2025.
//

#include "Renderer.h"
#include <GL/gl.h>

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
}