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

                //Coordenadas de textura y si no tiene las ponemos a 0
                if(indice.texcoord_index >= 0){
                    v.texturaCords = glm::vec2(atributos.texcoords[2 * indice.texcoord_index + 0], atributos.texcoords[2 * indice.texcoord_index + 1]);
                } else {
                    v.texturaCords = glm::vec2(0.0f);
                }

                //añadimos el vertice al vector y lo guardamos en el mapa
                unsigned int vertexIndex = static_cast<unsigned int>(vertices.size());
                vertices.push_back(v);
                indices.push_back(vertexIndex);
                mapaVertices[indice] = vertexIndex;
            }
        }
    }

    //una vez que ya tenemos indices, vertices y el mapa de textura, calculas la tangente y la bitangente
    for(size_t i = 0;i< indices.size();i+=3){
        uint32_t i1 = indices[i];
        uint32_t i2 = indices[i+1];
        uint32_t i3 = indices[i+2];

        Vertice &v1 = vertices[i1];
        Vertice &v2 = vertices[i2];
        Vertice &v3 = vertices[i3];

        glm::vec3 tang;
        glm::vec3 bitang;
        calcularTangBitang(v1.posicion, v2.posicion, v3.posicion, v1.texturaCords, v2.texturaCords, v3.texturaCords, tang, bitang);

        v1.tangente = tang;
        v2.tangente = tang;
        v3.tangente += tang;

        v1.bitangente += bitang;
        v2.bitangente += bitang;
        v3.bitangente += bitang;
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

    //Coordenadas Textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)offsetof(Vertice, texturaCords));
    glEnableVertexAttribArray(2);

    //Tangente
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void*)offsetof(Vertice, tangente));
    glEnableVertexAttribArray(3);

    //Bitangene
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)offsetof(Vertice, bitangente));
    glEnableVertexAttribArray(4);

    //craer el ibo
    glGenBuffers ( 1, &idIBO );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW );

    glBindVertexArray(0);

    ControllerMensajes::getInstancia().anadirMensaje("Modelo cargado correctamente");
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

const std::string &Modelo::getNombre() const {
    return nombre;
}

void Modelo::setNombre(const std::string &nombre) {
    Modelo::nombre = nombre;
}

const std::string &Modelo::getNombreMaterial() const {
    return nombreMaterial;
}

void Modelo::setNombreMaterial(const std::string &nombreMaterial) {
    Modelo::nombreMaterial = nombreMaterial;
}

MetodoRenderizado Modelo::getMRenderizado() const {
    return mRenderizado;
}

void Modelo::setMRenderizado(MetodoRenderizado mRenderizado) {
    Modelo::mRenderizado = mRenderizado;
}

void Modelo::setTextura(Textura *texturaNueva) {
    if (texturaNueva){
        this->textura = texturaNueva;
    } else {
        ControllerMensajes::getInstancia().anadirMensaje("Fallo al asignar la textura al modelo");
    }
}

const Textura* Modelo::getTextura() const {
    return textura;
}

// este metodo lo calcula por triangulo
void Modelo::calcularTangBitang(const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3,
                                const glm::vec2 &uv1, const glm::vec2 &uv2, const glm::vec2 &uv3,
                                glm::vec3 &tangente, glm::vec3 &bitangente){

    //vectores con los bordes del triangulo
    glm::vec3 borde1 = pos2 - pos1;
    glm::vec3 borde2 = pos3 - pos1;

    //vectores con diferencia de UV
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangente.x = f * (deltaUV2.y * borde1.x - deltaUV1.y * borde2.x);
    tangente.y = f * (deltaUV2.y * borde1.y - deltaUV1.y * borde2.y);
    tangente.z = f * (deltaUV2.y * borde1.z - deltaUV1.y * borde2.z);
    tangente = glm::normalize(tangente);

    bitangente.x = f * (-deltaUV2.x * borde1.x + deltaUV1.x * borde2.x);
    bitangente.y = f * (-deltaUV2.x * borde1.y + deltaUV1.x * borde2.y);
    bitangente.z = f * (-deltaUV2.x * borde1.z + deltaUV1.x * borde2.z);
    bitangente = glm::normalize(bitangente);

}

void Modelo::setTexturaMapaNormal(Textura *texturaNuevaMapaNormal) {
    if (texturaNuevaMapaNormal){
        this->texturaMapaNormal = texturaNuevaMapaNormal;
    } else {
        ControllerMensajes::getInstancia().anadirMensaje("Fallo al asignar la textura del mapa de normales al modelo");
    }
}

const Textura* Modelo::getTexturaMapaNormal() const {
    return texturaMapaNormal;
}


