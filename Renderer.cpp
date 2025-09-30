//
// Created by Suspr on 23/09/2025.
//


#include "Renderer.h"
#include <string>
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
        if ( idVS != 0 )
        {  glDeleteShader ( idVS );
        }

        if ( idFS != 0 )
        {  glDeleteShader ( idFS );
        }

        if ( idSP != 0 )
        {  glDeleteProgram ( idSP );
        }

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
        glUseProgram ( idSP );
        glBindVertexArray ( idVAO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glDrawElements ( GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr );
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
     * Método para crear, compilar y enlazar el shader program
     * @note No se incluye ninguna comprobación de errores
     */
    void PAG::Renderer::creaShaderProgram(){


        std::string miVertexShader =
                "#version 410\n"
                "layout (location = 0) in vec3 posicion;\n"
                "void main ()\n"
                "{  gl_Position = vec4 ( posicion, 1 );\n"
                "}\n";

        std::string miFragmentShader =
                "#version 410\n"
                "out vec4 colorFragmento;\n"
                "void main ()\n"
                "{  colorFragmento = vec4 ( 1.0, .4, .2, 1.0 );\n"
                "}\n";

        idVS = glCreateShader ( GL_VERTEX_SHADER );

        //por si falla al crear el vertex shader
        if (idVS == 0) {
            throw std::runtime_error("Error: No se pudo crear el vertex shader (glCreateShader(Vertex) devolvió 0)  :(  ");
        }

        const GLchar* fuenteVS = miVertexShader.c_str ();
        glShaderSource ( idVS, 1, &fuenteVS, nullptr );
        glCompileShader ( idVS );

        //comprobamos si el vertex shader se ha compilado bien:
        GLint resultadoCompilacionV;
        glGetShaderiv ( idVS, GL_COMPILE_STATUS, &resultadoCompilacionV );

        if ( resultadoCompilacionV == GL_FALSE )
        {  // Ha habido un error en la compilación.
            // Para saber qué ha pasado, tenemos que recuperar el mensaje de error de OpenGL
            GLint tamMsj = 0;
            std::string mensaje = "";
            glGetShaderiv ( idVS, GL_INFO_LOG_LENGTH, &tamMsj );
            if ( tamMsj > 0 )
            {  GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetShaderInfoLog ( idVS, tamMsj, &datosEscritos, mensajeFormatoC );
                mensaje.assign ( mensajeFormatoC );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                // En "mensaje" tenemos la información del error.
                throw std::runtime_error(mensaje);
            }else{
                throw std::runtime_error("Error: A petado la compilacion del vertex shader y OPENGL no sabe porque ha sido.  :(  ");
            }

        }


        idFS = glCreateShader ( GL_FRAGMENT_SHADER );

        //por si falla al crear el fragment shader
        if (idFS == 0) {
            throw std::runtime_error("Error: No se pudo crear el programa de shaders (glCreateShader (Fragment) devolvió 0)  :(  ");
        }

        const GLchar* fuenteFS = miFragmentShader.c_str ();
        glShaderSource ( idFS, 1, &fuenteFS, nullptr );
        glCompileShader ( idFS );

        //comprobamos si el fragment shader se ha compilado bien:
        GLint resultadoCompilacionF;
        glGetShaderiv ( idFS, GL_COMPILE_STATUS, &resultadoCompilacionF );

        if ( resultadoCompilacionF == GL_FALSE )
        {  // Ha habido un error en la compilación.
            // Para saber qué ha pasado, tenemos que recuperar el mensaje de error de OpenGL
            GLint tamMsj = 0;
            std::string mensaje = "";
            glGetShaderiv ( idFS, GL_INFO_LOG_LENGTH, &tamMsj );
            if ( tamMsj > 0 )
            {  GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetShaderInfoLog ( idFS, tamMsj, &datosEscritos, mensajeFormatoC );
                mensaje.assign ( mensajeFormatoC );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                // En "mensaje" tenemos la información del error.
                throw std::runtime_error(mensaje);
            }else{
                throw std::runtime_error("Error: A petado la compilacion del fragment shader y OPENGL no sabe porque ha sido.  :(  ");
            }

        }

        idSP = glCreateProgram ();
        //comprobamos que se haya creado el programa de shaders.
        if (idSP == 0) {
            throw std::runtime_error("Error: No se pudo crear el programa de shaders (glCreateProgram devolvió 0)  :(  ");
        }

        glAttachShader ( idSP, idVS );
        glAttachShader ( idSP, idFS );
        glLinkProgram ( idSP );

        //Comprobamos que se hayan enlazado bien los shaders
        GLint resultadoEnlazado = 0;
        glGetProgramiv ( idSP, GL_LINK_STATUS, &resultadoEnlazado );
        if ( resultadoEnlazado == GL_FALSE )
        {  // Ha habido un error y hay que recuperar su descripción, para saber qué ha pasado
            GLint tamMsj = 0;
            std::string mensaje = "";
            glGetProgramiv ( idSP, GL_INFO_LOG_LENGTH, &tamMsj );
            if ( tamMsj > 0 )
            {  GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetProgramInfoLog ( idSP, tamMsj, &datosEscritos, mensajeFormatoC );
                mensaje.assign ( mensajeFormatoC );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;
                // En "mensaje" tenemos la información del error. Comunicarla de alguna forma
                throw std::runtime_error(mensaje);
            }else{
                throw std::runtime_error("Error: A petado el enlazado de los shaders y OPENGL no sabe porque ha sido.  :(  ");
            }
        }
    }

    /**
     * Método para crear el VAO para el modelo a renderizar
     * @note No se incluye ninguna comprobación de errores
     */
    void PAG::Renderer::creaModelo ()
    {  GLfloat vertices[] = { -.5, -.5, 0,
                              .5, -.5, 0,
                              .0,  .5, 0 };
        GLuint indices[] = { 0, 1, 2 };

        glGenVertexArrays ( 1, &idVAO );
        glBindVertexArray ( idVAO );
        glGenBuffers ( 1, &idVBO );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBO );
        glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW );
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr );
        glEnableVertexAttribArray ( 0 );
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


}