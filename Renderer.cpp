//
// Created by Suspr on 23/09/2025.
//


#include "Renderer.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>


namespace PAG {

    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    PAG::Renderer::Renderer() {}

    /**
     * Destructor
     */
    PAG::Renderer::~Renderer() {


        if ( idVBO != 0 )
        {  glDeleteBuffers ( 1, &idVBO );
        }

        if ( idIBO != 0 )
        {  glDeleteBuffers ( 1, &idIBO );
        }

        if ( idVAO != 0 )
        {  glDeleteVertexArrays ( 1, &idVAO );
        }
    }

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
        glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        //glUseProgram ( idSP );
        glBindVertexArray ( idVAO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glDrawElements ( GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr );

        //Obtenemos los uniforms
        GLint mVision = glGetUniformLocation(programIDActivo, "mVision");
        GLint mProjeccion = glGetUniformLocation(programIDActivo, "mProjeccion");

        // Enviar matrices al shader
        glUniformMatrix4fv(mVision, 1, GL_FALSE, glm::value_ptr(camara.matrizVision()));
        glUniformMatrix4fv(mProjeccion, 1, GL_FALSE, glm::value_ptr(camara.matrizProyeccion()));
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
     * Método para crear el VAO para el modelo a renderizar
     * @note No se incluye ninguna comprobación de errores
     */
    void PAG::Renderer::creaModelo ()
    {  GLfloat vertices[] = { -.5, -.5, 0,      1.0f, 0.0f, 0.0f,
                              .5, -.5, 0,       0.0f, 1.0f, 0.0f,
                              .0,  .5, 0,    0.0f, 0.0f, 1.0f};
        GLuint indices[] = { 0, 1, 2 };
        GLuint idVBOC;
/*

        GLfloat colores[] = {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
        };
*/
        glGenVertexArrays ( 1, &idVAO );
        glBindVertexArray ( idVAO );

        glGenBuffers ( 1, &idVBO );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBO );
        glBufferData ( GL_ARRAY_BUFFER, 18*sizeof(GLfloat), vertices, GL_STATIC_DRAW );
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0 );
        glEnableVertexAttribArray ( 0 );

/*
        //VBO de los colores
        glGenBuffers ( 1, &idVBOC );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBOC );
        glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colores, GL_STATIC_DRAW );
        glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr );
        glEnableVertexAttribArray ( 1 );
*/

        //VBO Entrelazado
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        glGenBuffers ( 1, &idIBO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indices, GL_STATIC_DRAW );
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

    void Renderer::obtenerDatosContexto() {
        // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
        // 3D construido.
        std::ostringstream oss;
        oss << glGetString(GL_RENDERER) << "\n"
            << glGetString(GL_VENDOR) << "\n"
            << glGetString(GL_VERSION) << "\n"
            << glGetString(GL_SHADING_LANGUAGE_VERSION);
        std::string info = oss.str();
        ControllerMensajes::getInstancia().anadirMensaje(info);
    }

    GLuint Renderer::getProgramIdActivo() const {
        return programIDActivo;
    }

    void Renderer::setProgramIdActivo(GLuint programIdActivo) {
        programIDActivo = programIdActivo;
    }

    Camara &Renderer::getCamara()  {
        return camara;
    }

    void Renderer::setCamara(const Camara &camara) {
        Renderer::camara = camara;
    }
}