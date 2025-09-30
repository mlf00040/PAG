//
// Created by Suspr on 23/09/2025.
//

#ifndef PAG_RENDERER_H
#define PAG_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * Espacio de nombres para las prácticas de Programación de Aplicaciones
 * Gráficas
 */
namespace PAG {

    class Renderer {
    private:
        static  Renderer* instancia;
        float red=0;
        float green=0;
        float blue=0;
        Renderer();

        GLuint idVS = 0;    // Identificador del vertex shader
        GLuint idFS = 0;    // Identificador del fragment shader
        GLuint idSP = 0;    // Identificador del shader program
        GLuint idVAO = 0;   // Identificador del vertex array object
        GLuint idVBO = 0;   // Identificador del vertex buffer object
        GLuint idIBO = 0;   // Identificador del index buffer object

    public:
        static Renderer& getInstancia ();
        virtual ~Renderer ();
        void refrescar ();
        void pintarColores();
        void inicializaOpenGL ();
        void resizeViewPort(int w,int h);

        void creaShaderProgram();

        void creaModelo();

        float getGreen() const;

        void setGreen(float green);

        float getRed() const;

        void setRed(float red);

        float getBlue() const;

        void setBlue(float blue);
    };

} // PAG

#endif //PAG_RENDERER_H
