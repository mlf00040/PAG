//
// Created by Suspr on 17/11/2025.
//

#ifndef PAG_MATERIAL_H
#define PAG_MATERIAL_H

#include <glm/glm.hpp>

class Material {
private:
    glm::vec3 colorAmbiente = {0.2f, 0.2f, 0.2f};
    glm::vec3 colorDifuso = {0.8f, 0.8f, 0.8f};
    glm::vec3 colorEspecular = {1.0f, 1.0f, 1.0f};
    float exponente = 32.0f;

public:
    Material();

    Material(const glm::vec3 &ambiente, const glm::vec3 &difuso, const glm::vec3 &especular, float exponente);

    glm::vec3& getColorAmbiente();

    void setColorAmbiente(glm::vec3 &color);

    glm::vec3& getColorDifuso();

    void setColorDifuso(glm::vec3 &color);

    glm::vec3& getColorEspecular();

    void setColorEspecular(glm::vec3 &color);

    float getExponente() const;

    void setExponente(float &expo);


};


#endif //PAG_MATERIAL_H
