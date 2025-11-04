//
// Created by Suspr on 01/11/2025.
//

#include "Modelo.h"

Modelo::Modelo()=default;

Modelo::~Modelo() {
    if (idVAO) glDeleteVertexArrays(1, &idVAO);
    if (idVBO) glDeleteBuffers(1, &idVBO);
    if (idIBO) glDeleteBuffers(1, &idIBO);
}

void Modelo::cargarModelo(const std::string &ruta) {
    //es una prueba con el triangulo por ahora, en este caso las normales actuarian como los colores al no modificar el shader program todavia

    vertices={
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };
    indices = { 0, 1, 2 };

    //crear el vao
    glGenVertexArrays ( 1, &idVAO );
    glBindVertexArray ( idVAO );

    //crear el vbo
    glGenBuffers ( 1, &idVBO );
    glBindBuffer ( GL_ARRAY_BUFFER, idVBO );
    glBufferData ( GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertice), vertices.data(), GL_STATIC_DRAW );

    //Posicion
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)offsetof(Vertice, position) );
    glEnableVertexAttribArray ( 0 );

    //Normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)offsetof(Vertice, normal));
    glEnableVertexAttribArray(1);

    //craer el ibo
    glGenBuffers ( 1, &idIBO );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW );

    ControllerMensajes::getInstancia().anadirMensaje(("    Modelo cargado correctamente, con vertices: "+vertices.size()));
}

void Modelo::dibuja() {
    if (!idVAO) return;
    glBindVertexArray(idVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

const glm::mat4 &Modelo::getMatrizModelado() {
    return matrizModelado;
}

void Modelo::setMatrizModelado(glm::mat4 &mModelado) {
    matrizModelado=mModelado;
}

GLuint Modelo::getIdVao() const {
    return idVAO;
}

void Modelo::setIdVao(GLuint idVao) {
    idVAO = idVao;
}

GLuint Modelo::getIdVbo() const {
    return idVBO;
}

void Modelo::setIdVbo(GLuint idVbo) {
    idVBO = idVbo;
}

GLuint Modelo::getIdIbo() const {
    return idIBO;
}

void Modelo::setIdIbo(GLuint idIbo) {
    idIBO = idIbo;
}

const std::vector<unsigned int> &Modelo::getIndices() const {
    return indices;
}

void Modelo::setIndices(const std::vector<unsigned int> &indices) {
    Modelo::indices = indices;
}
