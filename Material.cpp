//
// Created by Suspr on 17/11/2025.
//

#include "Material.h"

Material::Material()=default;

Material::Material(const glm::vec3 &ambiente, const glm::vec3 &difuso, const glm::vec3 &especular, float exponente): colorAmbiente(ambiente),
        colorDifuso(difuso),
        colorEspecular(especular),
        exponente(exponente){}

glm::vec3& Material::getColorAmbiente()  {
    return colorAmbiente;
}

void Material::setColorAmbiente(glm::vec3 &color){
    colorAmbiente=color;
};

glm::vec3& Material::getColorDifuso()  {
    return colorDifuso;
}

void Material::setColorDifuso(glm::vec3 &color){
    colorDifuso=color;
};

glm::vec3& Material::getColorEspecular()  {
    return colorEspecular;
}

void Material::setColorEspecular(glm::vec3 &color){
    colorEspecular=color;
};

float Material::getExponente() const {
    return exponente;
}

void Material::setExponente(float &expo){
    exponente=expo;
};