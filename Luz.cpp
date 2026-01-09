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

        glm::vec3 centro = {0.0,0.0,0.0};
        glm::vec3 posicion = centro - direccion * 5.0f;
        mV = glm::lookAt ( posicion, centro, glm::vec3(0, 1, 0) );
        mP = glm::ortho ( -3.0, 3.0, -3.0, 3.0, 0.1, 10.0 );

    }else if(tLuz == tipoLuz::FOCO){

        mV = glm::lookAt ( pos, pos + direccion, glm::vec3(0, 1, 0) );
        mP =  glm::perspective ( 2 * glm::radians(angulo), static_cast<float>(anchoMS) / altoMS, 0.1f, 10.0f );

    }else{

        return glm::mat4(1.0f);

    }

    return mP * mV;

}
