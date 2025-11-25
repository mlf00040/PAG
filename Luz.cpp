//
// Created by Suspr on 23/11/2025.
//

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

const glm::vec3 &Luz::getCAmbiente() const {
    return cAmbiente;
}

void Luz::setCAmbiente(const glm::vec3 &cAmbiente) {
    Luz::cAmbiente = cAmbiente;
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
