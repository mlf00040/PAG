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
        //glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        //glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
        //glUseProgram ( idSP );

        glUseProgram(programIDActivo);

        //Obtenemos los uniforms
        GLint mVision = glGetUniformLocation(programIDActivo, "mVision");
        GLint mProjeccion = glGetUniformLocation(programIDActivo, "mProjeccion");
        GLint mModelado = glGetUniformLocation(programIDActivo,"mModelado");
        GLint mMVP = glGetUniformLocation(programIDActivo,"mMVP");
        GLint uColorDifuso = glGetUniformLocation(programIDActivo,"uColorDifuso");

        // Enviar matrices al shader
        glUniformMatrix4fv(mVision, 1, GL_FALSE, glm::value_ptr(camara.matrizVision()));
        glUniformMatrix4fv(mProjeccion, 1, GL_FALSE, glm::value_ptr(camara.matrizProyeccion()));



        //Pinta todos los modelos que haya cargados en el vector en el momento de la escena
        for (const auto& modelo : modelos) {
            if (modelo && modelo->getIdVao()) {
                glBindVertexArray(modelo->getIdVao());
                glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, modelo->getIdIbo() );

                //enviamos la matriz de modelado y la mvp al shader para cada modelo
                glUniformMatrix4fv(mModelado, 1, GL_FALSE, glm::value_ptr(modelo->getMatrizModelado()));

                glm::mat4 mvp = camara.matrizProyeccion() * camara.matrizVision() * modelo->getMatrizModelado();
                glUniformMatrix4fv(mMVP, 1, GL_FALSE, glm::value_ptr(mvp));

                if(modelo->getMRenderizado()==MetodoRenderizado::ALAMBRE){
                    glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
                    GLuint aux = glGetSubroutineIndex ( programIDActivo, GL_VERTEX_SHADER
                            , "colorRGB");
                    glUniformSubroutinesuiv ( GL_VERTEX_SHADER, 1, &aux );
                }
                if(modelo->getMRenderizado()==MetodoRenderizado::SOLIDO){
                    glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );

                    glm::vec3 colores = materiales.find(modelo->getNombreMaterial())->second.getColorDifuso();
                    glUniform3f(uColorDifuso,colores.x,colores.y,colores.z);

                    GLuint aux = glGetSubroutineIndex ( programIDActivo, GL_VERTEX_SHADER
                            ,"colorMaterial");
                    glUniformSubroutinesuiv ( GL_VERTEX_SHADER, 1, &aux );
                }

                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(modelo->getIndices().size()), GL_UNSIGNED_INT, nullptr);
                glBindVertexArray(0);
            }
        }
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

 /*
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
/*
        glGenVertexArrays ( 1, &idVAO );
        glBindVertexArray ( idVAO );

        glGenBuffers ( 1, &idVBO );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBO );
        glBufferData ( GL_ARRAY_BUFFER, 18*sizeof(GLfloat), vertices, GL_STATIC_DRAW );
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0 );
        glEnableVertexAttribArray ( 0 );
*/
/*
        //VBO de los colores
        glGenBuffers ( 1, &idVBOC );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBOC );
        glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colores, GL_STATIC_DRAW );
        glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr );
        glEnableVertexAttribArray ( 1 );
*/
/*
        //VBO Entrelazado
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        glGenBuffers ( 1, &idIBO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indices, GL_STATIC_DRAW );
    }
*/

/**
  * Método que añade un modelo al vector de modelos y lo carga
  * @note No se incluye ninguna comprobación de errores
  */
    void PAG::Renderer::creaModelo (std::string& ruta){
        Modelo* modelo = new Modelo();
        modelo->cargarModelo(ruta);
        modelos.push_back(std::unique_ptr<Modelo>(modelo));
    }

    void PAG::Renderer::borrarModelo(size_t indice) {
        if (indice < modelos.size()) {
            modelos.erase(modelos.begin() + indice);
        }
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

    const std::vector<std::unique_ptr<Modelo>> &Renderer::getModelos() const {
        return modelos;
    }

    const std::map<std::string, Material>& Renderer::getMateriales() const {
        return materiales;
    }

    void Renderer::addMaterial(const std::string &nombre, Material &mat) {
        materiales[nombre] = mat;
    }

    void Renderer::borrarMaterial(std::string& nombre){
        auto it = materiales.find(nombre);
        if (it != materiales.end()) {
            materiales.erase(it);
        }
    }
}