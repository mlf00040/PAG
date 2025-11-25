//
// Created by Suspr on 23/11/2025.
//

#ifndef PAG_LUZ_H
#define PAG_LUZ_H

#include <glm/glm.hpp>
enum class tipoLuz {
    AMBIENTE,
    PUNTUAL,
    DIRECCIONAL,
    FOCO
};

class Luz {
private:
    tipoLuz tLuz = tipoLuz::AMBIENTE;

    glm::vec3 cAmbiente = {0.0f,0.0f,0.0f};

    //Para puntual y foco
    glm::vec3 pos = {0.0f,0.0f,0.0f};

    //para direccional
    glm::vec3 direccion = {1.0f,0.0f,0.0f};

    //para foco
    float angulo=45;
    float expoSuavizado=2;
public:
    Luz();

    Luz(tipoLuz tLuz);

    tipoLuz getTLuz() const;

    void setTLuz(tipoLuz tLuz);

    const glm::vec3 &getCAmbiente() const;

    void setCAmbiente(const glm::vec3 &cAmbiente);

    const glm::vec3 &getPos() const;

    void setPos(const glm::vec3 &pos);

    const glm::vec3 &getDireccion() const;

    void setDireccion(const glm::vec3 &direccion);

    float getAngulo() const;

    void setAngulo(float angulo);

    float getExpoSuavizado() const;

    void setExpoSuavizado(float expoSuavizado);

};


#endif //PAG_LUZ_H
