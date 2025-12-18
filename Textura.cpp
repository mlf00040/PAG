//
// Created by Suspr on 17/12/2025.
//

#include "Textura.h"

Textura::Textura() {}

Textura::~Textura() {
    if (idTextura != 0) {
        glDeleteTextures(1, &idTextura);
    }
}

/**
 * Metodo para cargar la textura despues de haber cargado una imagen.
 * @param ruta
 */
void Textura::cargar(std::string& ruta) {
    //empezamos con la carga de la textura
    std::vector<unsigned char> pixeles;
    unsigned ancho, alto;

    unsigned error= lodepng::decode(pixeles,ancho,alto,ruta);
    if(error){

        ControllerMensajes::getInstancia().anadirMensaje("Error de LodePNG al cargar la imagen");
        ControllerMensajes::getInstancia().anadirMensaje(lodepng_error_text(error));
    }else{
        //Darle la vuelta a la imagen, la idea de meter la imagen volteada directamente es muy chapuzera
        size_t filaBytes = ancho * 4;
        for (unsigned i = 0; i < alto / 2; ++i) {
            auto* top = &pixeles[i * filaBytes];
            auto* bottom = &pixeles[(alto - 1 - i) * filaBytes];
            for (size_t j = 0; j < filaBytes; ++j) {
                std::swap(top[j], bottom[j]);
            }
        }

        //una vez que ya esta la imagen cargada, asigano la textura con el opengl
        glGenTextures(1,&idTextura);

        glBindTexture(GL_TEXTURE_2D, idTextura);

        // Cómo resolver la minificación. En este caso, le decimos que utilice mipmaps, y que aplique interpolación lineal
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        // Cómo resolver la magnificación. En este caso, le decimos que utilice mipmaps, y que aplique interpolación lineal
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        // Cómo pasar de coordenadas de textura a coordenadas en el espacio de la textura en horizontal
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        // Cómo pasar de coordenadas de textura a coordenadas en el espacio de la textura en vertical
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        // Transferimos la información de la imagen. En este caso, la imagen está guardada en std::vector<unsigned char> _pixels;
        glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, ancho, alto, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) pixeles.data () );

        glGenerateTextureMipmap (idTextura );

    }
}

void Textura::enlazar(unsigned int id)const {

    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, idTextura);

}

GLuint Textura::getIdGl() const {
    return idTextura;
}

void Textura::setIdGl(GLuint idGl) {
    idTextura = idGl;
}

const std::string &Textura::getNombre() const {
    return nombre;
}

void Textura::setNombre(const std::string &nombre) {
    Textura::nombre = nombre;
}

Textura::Textura(const std::string &nombre) : nombre(nombre) {}
