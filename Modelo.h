//
// Created by Suspr on 01/11/2025.
//

#ifndef PAG_MODELO_H
#define PAG_MODELO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "ControllerMensajes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>



//Clase auxiliar con cada componente de un vertice.
struct Vertice {
    glm::vec3 posicion;
    glm::vec3 normal;
};

class Modelo {
private:
    GLuint idVAO = 0;   // Identificador del vertex array object
    GLuint idVBO = 0;   // Identificador del vertex buffer object
    GLuint idIBO = 0;   // Identificador del index buffer object

    std::vector<Vertice> vertices;
    std::vector<unsigned int> indices;

    glm::mat4 matrizModelado = glm::mat4(1.0f);

    std::string nombre="";
public:
    Modelo();

    ~Modelo();

    void cargarModelo(const std::string& ruta);

    const glm::mat4& getMatrizModelado();
    void setMatrizModelado(glm::mat4& mModelado);

    void trasladar(glm::vec3& pos);
    void rotar(float angulo, glm::vec3& eje);
    void escalar(glm::vec3& escala);

    GLuint getIdVao() const;

    void setIdVao(GLuint idVao);

    GLuint getIdVbo() const;

    void setIdVbo(GLuint idVbo);

    GLuint getIdIbo() const;

    void setIdIbo(GLuint idIbo);

    const std::vector<unsigned int> &getIndices() const;

    void setIndices(const std::vector<unsigned int> &indices);

    const std::string &getNombre() const;

    void setNombre(const std::string &nombre);
};


#endif //PAG_MODELO_H
