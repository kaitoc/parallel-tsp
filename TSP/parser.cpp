//
// Created by Jonathan Prieto on 27/11/22.
//

#include <iostream>
#include<vector>
#include<tuple>
#include "parser.h"

using namespace std;

void llenar_datos(ifstream& ff){
    vector<tuple<int, int, int>> coordenas;
    int numero_puntos;
    int coordenadaX;
    int coordenadaY;
    int idCiudad;
    ff >> numero_puntos;
    for(int i = 0; i<numero_puntos; i++){
        ff >> idCiudad;
        ff >> coordenadaX;
        ff >> coordenadaY;
        coordenas.emplace_back(idCiudad, coordenadaX, coordenadaY);
    }

    for (int i = 0; i<coordenas.size(); i++){
        cout << get<0>(coordenas[i]) << ", " << get<1>(coordenas[i]) << ", " << get<2>(coordenas[i]) << endl;
    }
}

