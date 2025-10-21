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
/**
 * Mover la camara y el punto en el eje x(horizontal, eje x) y en el eje y (profundidad, eje z)
 * @param posX
 * @param posY
 */
void Camara::dolly(float posX, float posY) {
    //obtenemos el vecto de la  camara al punto y quitamos su componente vertical
    glm::vec3 forward = glm::normalize(punto - posicion);
    forward.y = 0.0;

    //obtenemos el vector lateral para el movimiento en el eje z
    glm::vec3 right = glm::cross(y,forward);

    //normalizamos los dos vectores por si acaso
    forward = glm::normalize(forward);
    right = glm::normalize(right);

    //calculamos y aplicamos el desplazamiento
    glm::vec3 desplazamiento= right * posX  - forward * posY ;

    posicion += desplazamiento;
    punto += desplazamiento;

}

/**
 * Mover la camara y el punto a donde mira en el eje y
 * @param posY
 */
void Camara::crane(float posY) {

    //calculamos y aplicamos el desplazamiento
    glm::vec3 desplazamiento= y * posY;

    posicion += desplazamiento;
    punto += desplazamiento;
}

/**
 * cambiar el fov
 * @param nuevofov
 */
void Camara::zoom(float nuevofov) {
    fov += nuevofov;
}

void Camara::reset() {
    posicion=glm::vec3 (0.0f,0.0f,3.0f);
    punto=glm::vec3(0.0f,0.0f,0.0f);
    y=glm::vec3(0.0f,1.0f,0.0f);
    fov=(45.0f);
    zNear=(1.0f);
    zFar=(100.0f);
    width=(16.0f);
    height=(9.0f);
}
