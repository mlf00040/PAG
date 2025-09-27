//
// Created by Suspr on 23/09/2025.
//

#ifndef PAG_RENDERER_H
#define PAG_RENDERER_H

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
    public:
        static Renderer& getInstancia ();
        virtual ~Renderer ();
        void refrescar ();
        void pintarColores();
        void inicializaOpenGL ();
        void resizeViewPort(int w,int h);

        float getGreen() const;

        void setGreen(float green);

        float getRed() const;

        void setRed(float red);

        float getBlue() const;

        void setBlue(float blue);
    };

} // PAG

#endif //PAG_RENDERER_H
