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
        Renderer();
    public:
        static Renderer& getInstancia ();
        virtual ~Renderer ();
        void refrescar ();
    };

} // PAG

#endif //PAG_RENDERER_H
