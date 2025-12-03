//
// Created by Suspr on 06/10/2025.
//

#include <iostream>
#include "ControllerShaders.h"
#include "ControllerMensajes.h"

ControllerShaders *ControllerShaders::instancia = nullptr;

/**
* Constructor por defecto
*/
ControllerShaders::ControllerShaders() {}

/**
 * Destructor
 */
ControllerShaders::~ControllerShaders() {
    limpiar();
}
/**
    * Consulta del objeto único de la clase
    * @return La dirección de memoria del objeto
    */
ControllerShaders &ControllerShaders::getInstancia() {
    if (!instancia)  // Lazy initialization: si aún no existe, lo crea
    {
        instancia = new ControllerShaders();
    }
    return *instancia;
}
/**
 * Carga un shader ya sea vertex o fragment y te devuelve su id.
 * @param ruta
 * @param tiposhader
 * @return
 */
GLuint ControllerShaders::cargaShader(const std::string &ruta, GLenum tiposhader) {

    GLuint id = glCreateShader ( tiposhader );
    std::string tipoShaderString ;
    std::string rutafinal;
    if(tiposhader==GL_VERTEX_SHADER){
        tipoShaderString="vertex";
        rutafinal=ruta+"-vs.glsl";
    }else if(tiposhader==GL_FRAGMENT_SHADER){
        tipoShaderString="fragment";
        rutafinal=ruta+"-fs.glsl";
    }

    //por si falla al crear el shader
    if (id == 0) {
        throw std::runtime_error("Error: No se pudo crear el "+tipoShaderString+" shader (glCreateShader(Vertex) devolvió 0)  :(  ");
    }

    //Cargamos el archivo de lectura del VertexShader

    std::ifstream archivoShader;
    archivoShader.open ( rutafinal );

    if ( !archivoShader.is_open () )
    {
        throw std::runtime_error("Error: No se pudo abrir el archivo del "+tipoShaderString+" shader  :(  ");
    }

    std::stringstream streamShader;
    streamShader << archivoShader.rdbuf ();
    std::string miShader = streamShader.str ();

    archivoShader.close ();


    const GLchar* fuenteS = miShader.c_str ();
    glShaderSource ( id, 1, &fuenteS, nullptr );
    glCompileShader ( id );

    //comprobamos si el vertex shader se ha compilado bien:
    GLint resultadoCompilacion;
    glGetShaderiv ( id, GL_COMPILE_STATUS, &resultadoCompilacion );

    if ( resultadoCompilacion == GL_FALSE )
    {  // Ha habido un error en la compilación.
        // Para saber qué ha pasado, tenemos que recuperar el mensaje de error de OpenGL
        GLint tamMsj = 0;
        std::string mensaje = "";
        glGetShaderiv ( id, GL_INFO_LOG_LENGTH, &tamMsj );
        if ( tamMsj > 0 )
        {  GLchar* mensajeFormatoC = new GLchar[tamMsj];
            GLint datosEscritos = 0;
            glGetShaderInfoLog ( id, tamMsj, &datosEscritos, mensajeFormatoC );
            mensaje.assign ( mensajeFormatoC );
            delete[] mensajeFormatoC;
            mensajeFormatoC = nullptr;

            // En "mensaje" tenemos la información del error.
            throw std::runtime_error(mensaje);
        }else{
            ControllerMensajes::getInstancia().anadirMensaje("Error: A petado la compilacion del "+tipoShaderString+" shader y OPENGL no sabe porque ha sido.  :(  ");
            throw std::runtime_error("Error: A petado la compilacion del "+tipoShaderString+" shader y OPENGL no sabe porque ha sido.  :(  ");
        }

    }
    //devuelve el id del shader
    return id;
}

std::string ControllerShaders::crearPrograma(const std::string &nombre, const std::string rutaV,
                                             const std::string rutaF) {
    try{
        GLuint idVS = cargaShader(rutaV,GL_VERTEX_SHADER);
        GLuint idFS = cargaShader(rutaF,GL_FRAGMENT_SHADER);

        GLuint idSP = glCreateProgram ();
        //comprobamos que se haya creado el programa de shaders.
        if (idSP == 0) {
            ControllerMensajes::getInstancia().anadirMensaje("Error: No se pudo crear el programa de shaders (glCreateProgram devolvió 0)  :(  ");
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
                ControllerMensajes::getInstancia().anadirMensaje("Error: A petado el enlazado de los shaders y OPENGL no sabe porque ha sido.  :(  ");
                throw std::runtime_error("Error: A petado el enlazado de los shaders y OPENGL no sabe porque ha sido.  :(  ");
            }
        }else{
        }

        //creamos el shader para añadirlo al vector

        shaderProgram SP;
        SP.id=idSP;
        SP.nombre=nombre;
        SP.idVs=idVS;
        SP.idFs=idFS;

        Programas.push_back(SP);

        //limpiamos las variables intermedias idsp y idvf
        glDeleteShader(idVS);
        glDeleteShader(idFS);

        return "Programa creado con exito";
    }catch (const std::exception& e){
        return e.what();
    }
}
/**
 * Teniendo el nombre del programa de shader, decirle a opengl que vamos a usarlo
 * @param nombre
 * @return
 */
std::string ControllerShaders::usarPrograma(const std::string &nombre) {
    std::vector<shaderProgram>::iterator it = Programas.begin();
    while(it != Programas.end()){
        if (it->nombre == nombre) {
            glUseProgram(it->id);
            return ("Usando shaderProgram:  " + nombre);
        }
        ++it;
    }
    return ("No se ha encontrado ese shaderProgram ");
}

/**
 * Elimina un shader dado el nombre.
 * @param nombre
 * @return
 */
std::string ControllerShaders::eliminarPrograma(const std::string &nombre) {
    std::vector<shaderProgram>::iterator it = Programas.begin();
    while(it != Programas.end()){
        if (it->nombre == nombre) {
            glDeleteProgram(it->id);
            Programas.erase(it);
            return ("Borrado shaderProgram:  " + nombre);
        }
        ++it;
    }
    return ("No se ha encontrado ese shaderProgram ");
}

GLuint ControllerShaders::getProgramId(const std::string& nombre) {
    std::vector<shaderProgram>::iterator it = Programas.begin();
    while(it != Programas.end()){
        if (it->nombre == nombre) {
            return it->id;
        }
        ++it;
    }
    throw std::runtime_error("Programa no encontrado: " + nombre);
}

/**
 * Devuelve una lista con todos los shaderPrograms
 * @return
 */
std::vector<std::string > ControllerShaders::listaShadersPrograms() {
    std::vector<std::string> nombres;
    std::vector<shaderProgram>::iterator it = Programas.begin();
    while(it != Programas.end()){
        nombres.push_back(it->nombre);
        ++it;
    }
    return nombres;
}

void ControllerShaders::limpiar() {
    std::vector<shaderProgram>::iterator it = Programas.begin();
    while (it != Programas.end()) {
        glDeleteProgram(it->id);
        ++it;
    }
}