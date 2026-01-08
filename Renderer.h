//
// Created by Suspr on 23/09/2025.
//

#ifndef PAG_RENDERER_H
#define PAG_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "ControllerMensajes.h"
#include "Camara.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "Modelo.h"

#include <map>
#include "Material.h"

#include "Luz.h"
#include "Textura.h"

#define GLM_ENABLE_EXPERIMENTAL

/**
 * Espacio de nombres para las prácticas de Programación de Aplicaciones
 * Gráficas
 */
namespace PAG {

    class Renderer {
    private:
        static  Renderer* instancia;
        Camara camara;
        float red=0;
        float green=0;
        float blue=0;
        Renderer();

        std::vector<std::unique_ptr<Modelo>> modelos;

        GLuint programIDActivo =0;

        //Indices de subrutina para la subrutina del color: fObtenerColorBase
        GLuint indiceColorDesdeVertice = 0;
        GLuint indiceColorDesdeTextura = 0;
        GLuint indiceColorDesdeMaterial = 0;

        //Indices de subrutina para la subrutina de la luz: fProcesaLuz
        GLuint indiceColorRGB = 0;
        GLuint indiceLuzAmbiente = 0;
        GLuint indiceLuzPuntual = 0;
        GLuint indiceLuzDireccional = 0;
        GLuint indiceLuzFoco = 0;

        GLuint indiceLuzPuntualNormalMapping = 0;
        GLuint indiceLuzDireccionalNormalMapping = 0;
        GLuint indiceLuzFocoNormalMapping = 0;

        //Posiciones de los uniforms de subrutina (para saber dónde poner cada indice)
        GLint locFuenteColorBase = -1;
        GLint locMetodoLuzElegido = -1;

        std::map<std::string, Material> materiales;

        std::vector<std::unique_ptr<Luz>> luces;

        std::vector<std::unique_ptr<Textura>> texturas;

        std::vector<std::unique_ptr<Textura>> texturasNormales;

    public:
        static Renderer& getInstancia ();
        virtual ~Renderer ();
        void refrescar ();
        void dibujaTodosModelos(GLuint &luzActual);
        void pintarColores();
        void inicializaOpenGL ();
        void resizeViewPort(int w,int h);

        void creaModelo(std::string& ruta);

        void borrarModelo(size_t indice);

        float getGreen() const;

        void setGreen(float green);

        float getRed() const;

        void setRed(float red);

        float getBlue() const;

        void setBlue(float blue);

        Camara &getCamara() ;

        void setCamara(const Camara &camara);

        GLuint getProgramIdActivo() const;

        void setProgramIdActivo(GLuint programIdActivo);

        const std::vector<std::unique_ptr<Modelo>> &getModelos() const;

        const std::map<std::string, Material> &getMateriales() const;

        void addMaterial(const std::string &nombre, Material &mat);

        void borrarMaterial(std::string& nombre );

        const std::vector<std::unique_ptr<Luz>> &getLuces() const;

        void addLuz(std::string nombre, std::unique_ptr<Luz> nuevaLuz);

        void borrarLuz(std::string nombre );

        void obtenerDatosContexto ();

        void cargarIndicesSubrutinas();

        const std::vector<std::unique_ptr<Textura>> &getTexturas() const;

        void addTextura(std::unique_ptr<Textura> textura);

        void borrarTextura(size_t index);

        Textura* getTexturaPorNombre(const std::string& nombre);

        const std::vector<std::unique_ptr<Textura>> &getTexturasNormal() const;

        void addTexturaNormal(std::unique_ptr<Textura> textura);

        void borrarTexturaNormal(size_t index);

        Textura* getTexturaPorNombreNormal(const std::string& nombre);

    };

} // PAG

#endif //PAG_RENDERER_H
