//
// Created by Suspr on 13/10/2025.
//

#ifndef PAG_CAMARA_H
#define PAG_CAMARA_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>



enum class MovCamara {
    NO,
    PAN,
    TILT,
    DOLLY,
    CRANE,
    ORBIT,
    ZOOM
};

class Camara {
private:
    glm::vec3 posicion;
    glm::vec3 punto;
    glm::vec3 y;

    float fov;
    float zNear;
    float zFar;
    float width;
    float height;
public:
    Camara();

    glm::mat4 matrizVision();
    glm::mat4 matrizProyeccion();

    void pan(float angulo);
    void tilt(float angulo);
    void reset();


    void mover();

};


#endif //PAG_CAMARA_H
