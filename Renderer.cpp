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

        glUseProgram(programIDActivo);

        //Obtenemos los uniforms comunes
        GLint mVision = glGetUniformLocation(programIDActivo, "mVision");
        GLint mProyeccion = glGetUniformLocation(programIDActivo, "mProyeccion");
        //la de modelado la paso a la hora de dibujar los modelos abajo

        //Enviamos matrices al shader
        glUniformMatrix4fv(mVision, 1, GL_FALSE, glm::value_ptr(camara.matrizVision()));
        glUniformMatrix4fv(mProyeccion, 1, GL_FALSE, glm::value_ptr(camara.matrizProyeccion()));

        //rendering multipasada si hay luces
        if(!luces.empty()){
            //activamos el blending
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            //bucle para las luces
            for(size_t i = 0;i< luces.size();++i) {
                const auto &luz = *luces[i];

                //dependiendo de si es la primera luz o no
                if (i == 0) {
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                } else {
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                }

                //seleccionamos subrutina
                GLuint indices[2] = {0};



                //seleccionamos subrutina segun el tipo de luz y enviamos los datos relativos a las luces
                GLuint idSubrutina = 0;
                if (luz.getTLuz() == tipoLuz::AMBIENTE) {
                    indices[locMetodoLuzElegido] = indiceLuzAmbiente;
                }else if (luz.getTLuz() == tipoLuz::PUNTUAL) {
                    indices[locMetodoLuzElegido] = indiceLuzPuntual;
                    glUniform3f(glGetUniformLocation(programIDActivo, "uPosLuz"),
                                luz.getPos().x, luz.getPos().y, luz.getPos().z);
                    glUniform3f(glGetUniformLocation(programIDActivo, "uIntensidadDifusa"),
                                luz.getIDifusa().x, luz.getIDifusa().y, luz.getIDifusa().z);
                    glUniform3f(glGetUniformLocation(programIDActivo, "uIntensidadEspecular"),
                                luz.getIEspecular().x, luz.getIEspecular().y, luz.getIEspecular().z);


                }else if (luz.getTLuz() == tipoLuz::DIRECCIONAL) {
                    indices[locMetodoLuzElegido] = indiceLuzDireccional;
                    glUniform3f(glGetUniformLocation(programIDActivo, "uDireccionLuz"),
                                luz.getDireccion().x, luz.getDireccion().y, luz.getDireccion().z);
                    glUniform3f(glGetUniformLocation(programIDActivo, "uIntensidadDifusa"),
                                luz.getIDifusa().x, luz.getIDifusa().y, luz.getIDifusa().z);
                    glUniform3f(glGetUniformLocation(programIDActivo, "uIntensidadEspecular"),
                                luz.getIEspecular().x, luz.getIEspecular().y, luz.getIEspecular().z);

                }else if (luz.getTLuz() == tipoLuz::FOCO) {
                    indices[locMetodoLuzElegido] = indiceLuzFoco;
                    glUniform3f(glGetUniformLocation(programIDActivo, "uPosLuz"),
                                luz.getPos().x, luz.getPos().y, luz.getPos().z);
                    glUniform3f(glGetUniformLocation(programIDActivo, "uDireccionLuz"),
                                luz.getDireccion().x, luz.getDireccion().y, luz.getDireccion().z);
                    glUniform1f(glGetUniformLocation(programIDActivo, "uAnguloApertura"), luz.getAngulo());
                    glUniform3f(glGetUniformLocation(programIDActivo, "uIntensidadDifusa"),
                                luz.getIDifusa().x, luz.getIDifusa().y, luz.getIDifusa().z);
                    glUniform3f(glGetUniformLocation(programIDActivo, "uIntensidadEspecular"),
                                luz.getIEspecular().x, luz.getIEspecular().y, luz.getIEspecular().z);
                }

                //activamos la subrutina de iluminacion
                glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &indices[locMetodoLuzElegido]);

                //dibujamos todos los modelos
                dibujaTodosModelos(indices[locMetodoLuzElegido]);

            }

            glDisable(GL_BLEND);

        }else{
            glDisable(GL_BLEND);

            GLuint indices[2] = {0};
            indices[locFuenteColorBase] = indiceColorDesdeVertice;
            indices[locMetodoLuzElegido] = indiceColorRGB;

            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, indices);

            //dibujamos todos los modelos
            dibujaTodosModelos(indiceColorRGB);

        }

    }

    void Renderer::dibujaTodosModelos(GLuint &LuzActual){
        //Pinta todos los modelos que haya cargados en el vector en el momento de la escena para cada pasada de la luz
        for (const auto& modelo : modelos) {
            if (modelo && modelo->getIdVao()) {
                glBindVertexArray(modelo->getIdVao());
                glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, modelo->getIdIbo() );

                //enviamos la matriz de modelado y la mvp al shader para cada modelo
                GLint mModelado = glGetUniformLocation(programIDActivo,"mModelado");
                GLint mMVP = glGetUniformLocation(programIDActivo,"mMVP");

                glUniformMatrix4fv(mModelado, 1, GL_FALSE, glm::value_ptr(modelo->getMatrizModelado()));

                glm::mat4 mvp = camara.matrizProyeccion() * camara.matrizVision() * modelo->getMatrizModelado();
                glUniformMatrix4fv(mMVP, 1, GL_FALSE, glm::value_ptr(mvp));

                //seleccionamos modo del modelo
                if(modelo->getMRenderizado()==MetodoRenderizado::ALAMBRE){
                    glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );

                }else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }

                //seleccion de subrutinas por modelo
                GLuint indiceColorModelo = 0;
                GLuint indiceIluminacionModelo = 0;

                //seleccion de color
                if ((modelo->getMRenderizado() == MetodoRenderizado::TEXTURA || modelo->getMRenderizado() == MetodoRenderizado::NORMALMAPPING) && modelo->getTextura()) {
                    indiceColorModelo = indiceColorDesdeTextura;
                } else if(modelo->getNombreMaterial()!= ""){
                    indiceColorModelo = indiceColorDesdeMaterial;
                }else {
                    indiceColorModelo = indiceColorDesdeVertice;
                }


                //Seleccion de luz
                if (modelo->getMRenderizado() == MetodoRenderizado::ALAMBRE) {
                    indiceIluminacionModelo = indiceColorRGB;
                }
                else if (modelo->getMRenderizado() == MetodoRenderizado::NORMALMAPPING &&
                         modelo->getTexturaMapaNormal()) {
                    //Usar normal mapping
                    if (LuzActual == indiceLuzPuntual) {
                        indiceIluminacionModelo = indiceLuzPuntualNormalMapping;
                    } else if (LuzActual == indiceLuzDireccional) {
                        indiceIluminacionModelo = indiceLuzDireccionalNormalMapping;
                    } else if (LuzActual == indiceLuzFoco) {
                        indiceIluminacionModelo = indiceLuzFocoNormalMapping;
                    } else {
                        indiceIluminacionModelo = indiceLuzPuntual;
                    }
                }
                else {
                    indiceIluminacionModelo = LuzActual;
                }

                //activar ambas subrutinas JUNTAS
                GLuint indices[2] = {indiceColorModelo, indiceIluminacionModelo};
                glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, indices);

                //activar textura si existe
                const Textura* textura = modelo->getTextura();
                if (textura && (modelo->getMRenderizado() == MetodoRenderizado::TEXTURA || modelo->getMRenderizado() == MetodoRenderizado::NORMALMAPPING)) {
                    textura->enlazar(0);
                    glUniform1i(glGetUniformLocation(programIDActivo, "muestreador"), 0);
                }

                //activar TexturaMapaNormales si existe
                const Textura* texturaNormalMap = modelo->getTexturaMapaNormal();

                if(texturaNormalMap && modelo->getMRenderizado() == MetodoRenderizado::NORMALMAPPING){
                    texturaNormalMap->enlazar(1);
                    glUniform1i(glGetUniformLocation(programIDActivo, "muestreadorNormal"), 1);
                }

                if (modelo->getMRenderizado() != MetodoRenderizado::ALAMBRE &&modelo->getMRenderizado() != MetodoRenderizado::TEXTURA) {
                    //enviamos las propiedades del material
                    Material *mat = &materiales.find(modelo->getNombreMaterial())->second;

                    glUniform3f(glGetUniformLocation(programIDActivo, "uColorAmbiente"),
                                mat->getColorAmbiente().x, mat->getColorAmbiente().y, mat->getColorAmbiente().z);
                    glUniform3f(glGetUniformLocation(programIDActivo, "uColorDifuso"),
                                mat->getColorDifuso().x, mat->getColorDifuso().y, mat->getColorDifuso().z);
                    glUniform3f(glGetUniformLocation(programIDActivo, "uColorEspecular"),
                                mat->getColorEspecular().x, mat->getColorEspecular().y, mat->getColorEspecular().z);
                    glUniform1f(glGetUniformLocation(programIDActivo, "uExponenteEspecular"), mat->getExponente());

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

    const std::vector<std::unique_ptr<Luz>> &Renderer::getLuces() const {
        return luces;
    }

    void Renderer::addLuz(std::string nombre, std::unique_ptr<Luz> nuevaLuz){

        nuevaLuz->setNombreLuz(nombre);
        luces.push_back(std::move(nuevaLuz));
        ControllerMensajes::getInstancia().anadirMensaje("Nueva luz creada: " + nombre);
    }

    void Renderer::borrarLuz(std::string nombre ){
        for(size_t i=0;i< luces.size();i++){
            if(luces[i]->getNombreLuz()==nombre){
                ControllerMensajes::getInstancia().anadirMensaje("Luz eliminada: "+nombre);
                luces.erase(luces.begin()+i);
                return;
            }
        }
        ControllerMensajes::getInstancia().anadirMensaje("Luz no encontrada:  "+nombre);
    }

    void Renderer::cargarIndicesSubrutinas() {
        if (!programIDActivo) return;

        glUseProgram(programIDActivo);

        //Obtenemos las ubicaciones de los uniforms de las subrutinas
        locFuenteColorBase = glGetSubroutineUniformLocation(programIDActivo, GL_FRAGMENT_SHADER, "uFuenteColorBase");
        locMetodoLuzElegido = glGetSubroutineUniformLocation(programIDActivo, GL_FRAGMENT_SHADER, "uMetodoLuzElegido");

        if (locFuenteColorBase == -1 || locMetodoLuzElegido == -1) {
            ControllerMensajes::getInstancia().anadirMensaje("No hay uniforms de subrutinas");
        }

        //Cargamos indices de la subrutina fObtenerColorBase
        indiceColorDesdeVertice = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "colorDesdeVertice");
        indiceColorDesdeTextura = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "colorDesdeTextura");
        indiceColorDesdeMaterial = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "colorDesdeMaterial");

        //Cargamos los indices de la subrutine fProcesaLuz
        indiceColorRGB = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "colorRGB");
        indiceLuzAmbiente = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "luzAmbiente");
        indiceLuzPuntual = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "luzPuntual");
        indiceLuzDireccional = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "luzDireccional");
        indiceLuzFoco = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "luzFoco");
        indiceLuzPuntualNormalMapping = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "luzPuntualNormalMapping");
        indiceLuzDireccionalNormalMapping = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "luzDireccionalNormalMapping");
        indiceLuzFocoNormalMapping = glGetSubroutineIndex(programIDActivo, GL_FRAGMENT_SHADER, "luzFocoNormalMapping");

        //Verifica que todos los índices son válidos
        if (indiceColorDesdeVertice == GL_INVALID_INDEX ||
                indiceColorDesdeTextura == GL_INVALID_INDEX ||
                indiceColorDesdeMaterial == GL_INVALID_INDEX ||
                indiceColorRGB == GL_INVALID_INDEX ||
                indiceLuzAmbiente == GL_INVALID_INDEX ||
                indiceLuzPuntual == GL_INVALID_INDEX ||
                indiceLuzDireccional == GL_INVALID_INDEX ||
                indiceLuzFoco == GL_INVALID_INDEX ||
                indiceLuzPuntualNormalMapping == GL_INVALID_INDEX ||
                indiceLuzDireccionalNormalMapping == GL_INVALID_INDEX ||
                indiceLuzFocoNormalMapping == GL_INVALID_INDEX) {
            ControllerMensajes::getInstancia().anadirMensaje("Los putos indices de subrutina han petado :,(");
        }

        ControllerMensajes::getInstancia().anadirMensaje("Indices de subrutinas cargados correctamente.");

    }

    const std::vector<std::unique_ptr<Textura>> &Renderer::getTexturas() const {
        return texturas;
    }

    void Renderer::addTextura(std::unique_ptr<Textura> textura) {
            texturas.push_back(std::move(textura));
            ControllerMensajes::getInstancia().anadirMensaje("Textura añadida.");
    }

    void Renderer::borrarTextura(size_t index) {
        if (index < texturas.size()) {
            std::string nombre = texturas[index]->getNombre();
            texturas.erase(texturas.begin() + index);
            ControllerMensajes::getInstancia().anadirMensaje("Textura eliminada: " + nombre);
        }
    }

    Textura* Renderer::getTexturaPorNombre(const std::string& nombre) {
        for (const auto& textura : texturas) {
            if (textura && textura->getNombre() == nombre) {
                return textura.get();
            }
        }
        return nullptr;
    }

}