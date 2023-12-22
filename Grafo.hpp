#ifndef GRAFO_HPP
#define GRAFO_HPP 

#include "Arista.hpp"
#include "Nodo.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <stack>

using namespace std;

/**
 * @class Grafo
 * Clase que representa un grafo
 * con aristas y nodos
 * 
*/
class Grafo 
{
    public:

        inline static const string CLIENTE = "cliente";

        /**
         * Lista de todos los nodos en el grafo
        */
        vector<Nodo*>* listaNodos;
        /**
         * Lista de todas las aristas en el grafo
        */
        vector<Arista*>* listaArista;
        /**
         * lista de todos los ID de los clientes en el grafo
        */
        vector<int>* listaIDClientes;
        
        /**
         * Constructor de Grafo
        */
        Grafo()
        {
            listaNodos = new vector<Nodo*>(0);
            listaArista = new vector<Arista*>(0);
            listaIDClientes = new vector<int>(0);
        };

        ~Grafo()
        {
            listaArista->~vector();
            listaNodos->~vector();
            listaIDClientes->~vector();
        }

        /**
         * retorna el indice por id, -1 si no se encuentra
         * @param id id a pregunta
         * @return indice de id, -1 si no se encontro
        */
        int getIndexPorId(int id)
        {
            for(int i=0; i<listaNodos->size();i++)
            {
                if(listaNodos->at(i)->getId() == id) return i;
            }
            return -1;
        }

        /**
         * Retorna un puntero a Nodo con un indice
         * @param index indice de la lista 
         * @return puntero al nodo de la lista de indice
        */
        Nodo* getNodo(int index)
        {
            return listaNodos->at(index);
        }

        /**
         * Crea y agrega un nodo en la lista nodos
         * @param id id del servidor
         * @param nombre nombre del servidor
         * @param tipo tipo del servidor, cliente o router
         * @return false si ya existe el nodo en el grafo, true si se agrego correctamente
        */
        bool agregarNodo(int id, string nombre, string tipo)
        {
            if(getIndexPorId(id) != -1) return false; // si se encontro, retorne false
            
            listaNodos->push_back(new Nodo(id, nombre));
            if(tipo == CLIENTE)           
            {
                listaIDClientes->push_back(id);
            }
            return true;
        };


        /**
         * verifica si existe una conexion entre dos servidores
         * @param id1 id servidor
         * @param id2 id servidor
         * @return true si existe la arista, false lo contrario
         * 
         * 
        */
        bool aristaExiste(int id1, int id2)
        {
            for(Arista* arista : *listaArista)
            {
                Nodo* origen = arista->getOrigen();
                Nodo* destino = arista->getDestino();
                if(origen->getId() == id1 && destino->getId() == id2) return true;
            }
            return false;
        }

        
        /**
         * Agrega una arista al grafo, como es  bidireccional entonces crea dos , o->d, d->o 
         * @param id1 id del servidor
         * @param id2 id del segundo servidor
         * @param distancia, transferencia parametros para crear la Arista
         * @return true si se pudo crear las aristas, false lo contrario
        */
        bool agregarArista(int id1, int id2, int distancia, int transferencia)
        {
            int indexO = getIndexPorId(id1);
            int indexD = getIndexPorId(id2);
            if(indexD == -1 || indexO == -1 || aristaExiste(id1, id2)) false;

            listaArista->push_back(new Arista(listaNodos->at(indexO), listaNodos->at(indexD), distancia, transferencia));
            listaArista->push_back(new Arista(listaNodos->at(indexD), listaNodos->at(indexO), distancia, transferencia));
            return true;
        }


        /**
         * Algoritmo de bellman Ford para buscar el camino mas corto para cada nodo entre un origen
         * @param idO idO origen
         * @param sizeArchivo atributo para ponderar el peso entre caminos
         * @return retorna un vector de pares donde pair<Nodo*, int>> donde: Nodo* -> el nodo donde conviene llegar al nodo del indice
         * int -> la ponderacion
        */
        vector<pair<Nodo*,int>> bellmanFord(int idO, int sizeArchivo)
        {
            int indexO = getIndexPorId(idO);
            if(indexO == -1) return vector<pair<Nodo*,int>>(0);


            vector<pair<Nodo*,int>> distancias(listaNodos->size());

            for(int i=0; i<listaNodos->size();i++)
            {
                distancias[i] = make_pair(nullptr, INT_MAX);
            }
            distancias[indexO].first = listaNodos->at(indexO);
            distancias[indexO].second = 0;

            for(int i=0; i<listaNodos->size() - 1;i++) // Se busca para N-1 nodos
            {
                for(Arista* arista : *listaArista) // Se pregunta para todas las aristas del grafo
                {
                    int indexOrigen = getIndexPorId(arista->getOrigen()->getId());
                    int indexDestino = getIndexPorId(arista->getDestino()->getId());
                    int peso = arista->getPeso(sizeArchivo);

                    if(distancias[indexOrigen].second != INT_MAX && distancias[indexOrigen].second + peso < distancias[indexDestino].second)
                    {
                        distancias[indexDestino].first = arista->getOrigen();
                        distancias[indexDestino].second = distancias[indexOrigen].second + peso;  
                    }
                }
            }

            return distancias;
        };

        /**
         * Método que imprime mas corto entre idOrigen e idDestino, con el siguiente formato: Origen: idOrigen -> costo(costo) -> idDestino
         * @param distancias las distancias que se retornaron con bellmanFord 
         * @param idDestino id del servidor destino
         * @param idOrigen id del servidor de origen
         * @return string con el formato mencionado
         * 
        */
        string encontrarCamino(vector<pair<Nodo*,int>>& distancias, int idOrigen, int idDestino)
        {
            stack<pair<Nodo*,int>> camino;
            int idCurrent = idDestino;
            int indexCurrent = getIndexPorId(idCurrent);
            do 
            {
                pair<Nodo*,int>& par = distancias[indexCurrent];
                camino.push(par);// Se guarda en el stack
                idCurrent = par.first->getId(); // id del nodo para llegar a idCurrent, se reemplaza
                indexCurrent = getIndexPorId(idCurrent); // se encuentra el indice del idCurrent
            }while(idCurrent != idOrigen); // Se va guardando en una pila los caminos, como las distancias estan determinadas para el idOrigen, se considera que todos los nodos
            // tienen la distancia respecto a idOrigen, por lo tanto el llegara en algun momento a idOrigen, en ese momento se encontro el recorrido a idDestino

            
            string caminoDestino = "Origen: ";
            int costoAnterior = 0;
            while(!camino.empty()) // Al ser pila el primer objeto en salir es el origen y el ultimo es el idDestino
            {
                caminoDestino += to_string(camino.top().first->getId()) + " -> Costo (" + to_string(camino.top().second - costoAnterior) + ") -> ";
                costoAnterior = camino.top().second;
                camino.pop();
            }
            int indexDestino = getIndexPorId(idDestino);
            caminoDestino += to_string(getNodo(indexDestino)->getId()) + "\n";
            caminoDestino += "Total: " +  to_string(costoAnterior) + " \n";
            return caminoDestino;
        };

        /**
         * Método que retorna un bool si existe o no un idCliente por parametro
         * @param idC id a verificar
         * @return true, si existe, false lo contrario
        */
        bool existeIDCliente(int idC)
        {
            for(int id : *listaIDClientes)
            {
                if(id == idC) return true;
            }
            return false;
        }
        /**
         * Verifica si la lista IDClientes esta vacia o no
         * @return true si esta vacia, false lo contrario
        */
        bool IDClienteEmpty()
        {
            return listaIDClientes->size() == 0;
        }
};
#endif 