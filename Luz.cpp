//
// Created by Suspr on 23/11/2025.
//

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Luz.h"

Luz::Luz()=default;


Luz::Luz(tipoLuz tLuz) : tLuz(tLuz) {

}

tipoLuz Luz::getTLuz() const {
    return tLuz;
}

void Luz::setTLuz(tipoLuz tLuz) {
    Luz::tLuz = tLuz;
}

const glm::vec3 &Luz::getIAmbiente() const {
    return IAmbiente;
}

void Luz::setIAmbiente(const glm::vec3 &IAmbiente) {
    Luz::IAmbiente = IAmbiente;
}

const glm::vec3 &Luz::getPos() const {
    return pos;
}

void Luz::setPos(const glm::vec3 &pos) {
    Luz::pos = pos;
}

const glm::vec3 &Luz::getDireccion() const {
    return direccion;
}

void Luz::setDireccion(const glm::vec3 &direccion) {
    Luz::direccion = direccion;
}

float Luz::getAngulo() const {
    return angulo;
}

void Luz::setAngulo(float angulo) {
    Luz::angulo = angulo;
}

float Luz::getExpoSuavizado() const {
    return expoSuavizado;
}

void Luz::setExpoSuavizado(float expoSuavizado) {
    Luz::expoSuavizado = expoSuavizado;
}

const std::string &Luz::getNombreLuz() const {
    return nombreLuz;
}

void Luz::setNombreLuz(const std::string &nombreLuz) {
    Luz::nombreLuz = nombreLuz;
}

const glm::vec3 &Luz::getIDifusa() const {
    return IDifusa;
}

void Luz::setIDifusa(const glm::vec3 &iDifusa) {
    IDifusa = iDifusa;
}

const glm::vec3 &Luz::getIEspecular() const {
    return IEspecular;
}

void Luz::setIEspecular(const glm::vec3 &iEspecular) {
    IEspecular = iEspecular;
}

glm::mat4 Luz::getMatrizMVLuz() {
    glm::mat4 mV;
    glm::mat4 mP;

    if(tLuz== tipoLuz::DIRECCIONAL){

        glm::vec3 centro = {0.0, 0.0, 0.0};
        glm::vec3 posicion = centro - direccion * 20.0f;

        mV = glm::lookAt(posicion, centro, glm::vec3(0, 1, 0));
        mP = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 150.0f);

    }else if(tLuz == tipoLuz::FOCO){

        //normalizar la direcciom
        glm::vec3 dirNorm = glm::normalize(direccion);

        //ajustar el up para evitar problemas con el lookAt
        glm::vec3 up = glm::vec3(0, 1, 0);
        if (glm::abs(glm::dot(dirNorm, up)) > 0.99f) {
            up = glm::vec3(0, 0, 1);
        }

        mV = glm::lookAt ( pos, pos + dirNorm, up );
        mP = glm::perspective( 2 * glm::radians(angulo),static_cast<float>(anchoMS) / altoMS,0.1f,100.0f);

    }else{

        return glm::mat4(1.0f);

    }

    return mP * mV;

}

void Luz::iniciaMapaSombras() {

    if(tLuz == tipoLuz::FOCO || tLuz == tipoLuz::DIRECCIONAL){

        //creamos el FBO para el mapa de sombras
        fboSombras = 0;
        glGenFramebuffers( 1, & fboSombras);

        //creamos la textura y la configuramos
        mapaSombrasTextura = 0;
        glGenTextures( 1, &mapaSombrasTextura);

        GLfloat borde[] = {1.0,1.0,1.0,1.0};

        glBindTexture(GL_TEXTURE_2D, mapaSombrasTextura);
        glTexImage2D ( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, anchoMS, altoMS, 0
                , GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borde);
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE
                , GL_COMPARE_REF_TO_TEXTURE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

        //activamos y adjuntamos el FBO
        glBindFramebuffer(GL_FRAMEBUFFER, fboSombras);
        glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D
                , mapaSombrasTextura, 0);

        //no se necesita info del color, solo la profundidad
        glReadBuffer (GL_NONE);
        glDrawBuffer (GL_NONE);

        GLenum estado = glCheckFramebufferStatus (GL_FRAMEBUFFER);
        if ( estado != GL_FRAMEBUFFER_COMPLETE)
        {
            ControllerMensajes::getInstancia().anadirMensaje("Error al hacer el inicio del mapa de sombras, el FBO no esta listo para generar el mapa de sombras");
        }

    }

}

bool Luz::compatibleSombra() {
    if(tLuz == tipoLuz::FOCO || tLuz == tipoLuz::DIRECCIONAL){
        return true;
    }
    return false;
}

bool Luz::mapaSombrasActivo() {
    if(fboSombras != 0 && mapaSombrasTextura != 0){
        return true;
    }
    return false;
}

GLuint Luz::getFboSombras() const {
    return fboSombras;
}

GLuint Luz::getMapaSombrasTextura() const {
    return mapaSombrasTextura;
}

int Luz::getAnchoMs() const {
    return anchoMS;
}

int Luz::getAltoMs() const {
    return altoMS;
}
