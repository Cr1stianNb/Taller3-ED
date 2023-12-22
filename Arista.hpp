#ifndef ARISTA_HPP 
#define ARISTA_HPP

#include <cmath>
#include "Nodo.hpp"
using namespace std;

/**
 * @class Arista
 * Clase que representa las aristas de un  grafo
 * con ponderacion
*/
class Arista
{
    private: 
        /**
         * Nodo origen 
        */
        Nodo* origen;
        /**
         * Nodo destino
        */
        Nodo* destino;
        /**
         * distancia entre ambos nodos
         * transferencia: Velocidad entre servidores
         * 
        */
        int distancia, transferencia;

    public:
        /**
         * Constructor de la clase
        */
        Arista(Nodo* origen, Nodo* destino, int distancia, int transferencia) 
        : origen(origen), destino(destino), distancia(distancia), transferencia(transferencia) {};
        /**
         * Destructor de la clase
        */
        ~Arista()
        {
            origen->~Nodo();
            destino->~Nodo();
        }

        /**
         * Getter
        */
        Nodo* getOrigen(){return origen;};
        /**
         * Getter
        */
        Nodo* getDestino(){return destino;};

        /**
         * retorna la ponderacion con un peso especificado
         * @param size tamaño del archivo
        */
        int getPeso(int size)
        {
            double sizeD = static_cast<double>(size);
            int partes = ceil((sizeD/transferencia));
            
            return partes*distancia;
        };
};

#endif 