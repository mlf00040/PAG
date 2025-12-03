//
// Created by Suspr on 23/11/2025.
//

#ifndef PAG_LUZ_H
#define PAG_LUZ_H

#include <glm/glm.hpp>
#include <string>

enum class tipoLuz {
    AMBIENTE,
    PUNTUAL,
    DIRECCIONAL,
    FOCO
};

class Luz {
private:
    tipoLuz tLuz = tipoLuz::AMBIENTE;

    //segun el tipo de luz que sea sera la intensidad ambiente, la difusa o la especualr
    glm::vec3 IAmbiente = {0.0f, 0.0f, 0.0f};
    glm::vec3 IDifusa = {0.0f, 0.0f, 0.0f};
    glm::vec3 IEspecular = {0.0f, 0.0f, 0.0f};

    //Para puntual y foco
    glm::vec3 pos = {0.0f,0.0f,0.0f};

    //para direccional
    glm::vec3 direccion = {1.0f,0.0f,0.0f};

    //para foco
    float angulo=45;
    float expoSuavizado=2;

    //identificador de la luz
    std::string nombreLuz="";
public:
    Luz();

    Luz(tipoLuz tLuz);

    tipoLuz getTLuz() const;

    void setTLuz(tipoLuz tLuz);

    const glm::vec3 &getIAmbiente() const;

    void setIAmbiente(const glm::vec3 &IAmbiente);

    const glm::vec3 &getIDifusa() const;

    void setIDifusa(const glm::vec3 &iDifusa);

    const glm::vec3 &getIEspecular() const;

    void setIEspecular(const glm::vec3 &iEspecular);

    const glm::vec3 &getPos() const;

    void setPos(const glm::vec3 &pos);

    const glm::vec3 &getDireccion() const;

    void setDireccion(const glm::vec3 &direccion);

    float getAngulo() const;

    void setAngulo(float angulo);

    float getExpoSuavizado() const;

    void setExpoSuavizado(float expoSuavizado);

    const std::string &getNombreLuz() const;

    void setNombreLuz(const std::string &nombreLuz);

};


#endif //PAG_LUZ_H
