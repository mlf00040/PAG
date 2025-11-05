//
// Created by Suspr on 01/11/2025.
//


#include "Modelo.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <unordered_map>

//Como no se porque cojones el compliador no me los coje en el mapa, los tengo que definir nuevamente
struct IndexHash {
    std::size_t operator()(const tinyobj::index_t& idx) const noexcept {
        return (std::hash<int>{}(idx.vertex_index) << 2) ^
               (std::hash<int>{}(idx.normal_index) << 1) ^
               (std::hash<int>{}(idx.texcoord_index));
    }
};

struct IndexEqual {
    bool operator()(const tinyobj::index_t& a, const tinyobj::index_t& b) const noexcept {
        return a.vertex_index == b.vertex_index &&
               a.normal_index == b.normal_index &&
               a.texcoord_index == b.texcoord_index;
    }
};

Modelo::Modelo()=default;

Modelo::~Modelo() {
    if (idVAO) glDeleteVertexArrays(1, &idVAO);
    if (idVBO) glDeleteBuffers(1, &idVBO);
    if (idIBO) glDeleteBuffers(1, &idIBO);
}

void Modelo::cargarModelo(const std::string &ruta) {
    //es una prueba con el triangulo por ahora, en este caso las normales actuarian como los colores al no modificar el shader program todavia
/*
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
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)offsetof(Vertice, posicion) );
    glEnableVertexAttribArray ( 0 );

    //Normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)offsetof(Vertice, normal));
    glEnableVertexAttribArray(1);

    //craer el ibo
    glGenBuffers ( 1, &idIBO );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW );

    ControllerMensajes::getInstancia().anadirMensaje(("    Modelo cargado correctamente, con vertices: "+vertices.size()));
*/
    tinyobj::ObjReaderConfig config;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(ruta, config)) {
        if (!reader.Error().empty()) {
            ControllerMensajes::getInstancia().anadirMensaje("Error al cargar el OBJ");
        }

    }

    auto& atributos = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    std::unordered_map<tinyobj::index_t, unsigned int, IndexHash, IndexEqual> mapaVertices;;

    for(auto& shape : shapes){
        size_t contadorIndices = shape.mesh.indices.size();

        for(size_t i=0; i< contadorIndices;i++){
            tinyobj::index_t indice = shape.mesh.indices[i];

            //buscamos si existe ya el vertice para reutilizarlo.
            auto it = mapaVertices.find(indice);
            if (it != mapaVertices.end()) {
                indices.push_back(it->second);
            } else {
                //en caso de que no exista, lo creamos
                Vertice v;

                //le asignamos la posicion y en caso que no la tenga lo dejamos a 0
                if(indice.vertex_index>=0){
                    v.posicion = glm::vec3(atributos.vertices[3 * indice.vertex_index + 0],atributos.vertices[3 * indice.vertex_index + 1],atributos.vertices[3 * indice.vertex_index + 2]);
                }else{
                    v.posicion=glm::vec3(0.0f);
                }

                //le asignamos la normal y si no tiene a 0
                if(indice.normal_index>=0){
                    v.normal = glm::vec3(atributos.normals[3 * indice.normal_index + 0],atributos.normals[3 * indice.normal_index + 1],atributos.normals[3 * indice.normal_index + 2]);
                }else{
                    v.normal=glm::vec3(0.0f);
                }

                //añadimos el vertice al vector y lo guardamos en el mapa
                unsigned int vertexIndex = static_cast<unsigned int>(vertices.size());
                vertices.push_back(v);
                indices.push_back(vertexIndex);
                mapaVertices[indice] = vertexIndex;
            }
        }
    }

    //crear el vao
    glGenVertexArrays ( 1, &idVAO );
    glBindVertexArray ( idVAO );

    //crear el vbo
    glGenBuffers ( 1, &idVBO );
    glBindBuffer ( GL_ARRAY_BUFFER, idVBO );
    glBufferData ( GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertice), vertices.data(), GL_STATIC_DRAW );

    //Posicion
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)offsetof(Vertice, posicion) );
    glEnableVertexAttribArray ( 0 );

    //Normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)offsetof(Vertice, normal));
    glEnableVertexAttribArray(1);

    //craer el ibo
    glGenBuffers ( 1, &idIBO );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW );

    glBindVertexArray(0);
    ControllerMensajes::getInstancia().anadirMensaje("Modelo cargado correctamente");
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

void Modelo::trasladar(glm::vec3 &pos) {
    matrizModelado=glm::translate(matrizModelado,pos);
}

void Modelo::rotar(float angulo, glm::vec3 &eje) {
    matrizModelado=glm::rotate(matrizModelado,glm::radians(angulo),glm::normalize(eje));
}

void Modelo::escalar(glm::vec3 &escala) {
    matrizModelado=glm::scale(matrizModelado,escala);
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
