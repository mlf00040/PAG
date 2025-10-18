//
// Created by Suspr on 13/10/2025.
//

#include "Camara.h"

Camara::Camara() : posicion(0.0f,0.0f,1.0f), punto(0.0f,0.0f,0.0f),y(0.0f,1.0f,0.0f), fov(45.0f), zNear(1.0f), zFar(100.0f), width(16.0f),
                               height(9.0f) {};

/**
 * Devuelve la matriz de vision de la camara.
 * @return
 */
glm::mat4 Camara::matrizVision() {
    return glm::lookAt(posicion,punto,y);
}

/**
 * Devuelve la matraiz de proyeccion de la camara.
 * @return
 */
glm::mat4 Camara::matrizProyeccion() {
    return glm::perspective(glm::radians(fov),(width/height),zNear,zFar);
}

/**
 * Mirar a los lados, que es una rotacion del punto de vision sobre el eje y local de la camara
 * @param angulo
 */
void Camara::pan(float angulo) {
    //creas la matriz de rotacion
    glm::mat4 rotacion = glm::rotate((glm::mat4(1.0f)),glm::radians(angulo),y);
    //aplica la matriz de rotacion al vector
    glm::vec4 aplicadaRotacion = rotacion * glm::vec4(punto - posicion, 0.0f);
    //aplicas la rotacion ya al punto.
    punto = posicion + glm::vec3(aplicadaRotacion);
}

/**
 * mirar arriba y abajo, que es una rotacion del punto de vision sobre el eje z de la camara.
 * @param angulo
 */
void Camara::tilt(float angulo) {
    //calculamos el eje de rotacion
    glm::vec3 eje = glm::cross(y,glm::normalize(punto-posicion));
    //creas la matriz de rotacion
    glm::mat4 rotacion = glm::rotate((glm::mat4(1.0f)),glm::radians(angulo),eje);
    //aplica la matriz de rotacion al vector
    glm::vec4 aplicadaRotacion = rotacion * glm::vec4(punto - posicion, 0.0f);
    //aplicas la rotacion ya al punto.
    punto = posicion + glm::vec3(aplicadaRotacion);
}

void Camara::reset() {
    posicion=glm::vec3 (0.0f,0.0f,1.0f);
    punto=glm::vec3(0.0f,1.0f,0.0f);
    y=glm::vec3(0.0f,1.0f,0.0f);
    fov=(45.0f);
    zNear=(1.0f);
    zFar=(100.0f);
    width=(16.0f);
    height=(9.0f);
}