#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;
int main() {
    srand(time(NULL));
    ofstream archivo;
    ofstream archivo2;
    archivo2.open("conexiones.csv");
    archivo.open("servidores.csv");
    archivo<<"id,nombre,tipo"<<endl;
    archivo2<<"idCliente,idServidor,velocidad,distancia"<<endl;
    vector<string> cliente;
    vector<string> servidor;
    for (int i = 0; i < 300; i++) {
        int num = i;
        string nombre = "Servidor" + to_string(i);
        int random = rand()%5+1;
        cout<<random<<endl;
        string tipo;
        if(random == 5 || i == 299){
            tipo = "router";
            servidor.push_back(to_string(i));
            for(string client:cliente){
                int random2 = rand()%10+1;
                int velocidadrand = (rand()%3+1)*300;
                archivo2<<client<<","<<i<<","<<velocidadrand<<","<<random2<<endl;
            }
            cliente.clear();
        }else{
            tipo="cliente";
            cliente.push_back(to_string(i));
        }
        archivo << num << "," << nombre << "," << tipo << endl;
    }
    archivo.close();
    for(int i = 0; i< servidor.size(); i++){
        for(int j = i+1; j< servidor.size(); j++){
            if(i!=j){
                int moneda = rand()%4+1;
                if(moneda == 1){
                    int random = rand()%1000+1;
                    int velocidad = (rand()%3+1)*1000;
                    archivo2<<servidor[i]<<","<<servidor[j]<<","<<velocidad<<","<<random<<endl;
                }
            }
        }
        
    }
    archivo2.close();
    return 0;
}
