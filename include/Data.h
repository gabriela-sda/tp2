#ifndef DATA_H
#define DATA_H

#include <string>
#include <iostream>
#include <ctime>
#include <cmath>

class Data {
    public:
    Data();
    Data(double horasTotais);
    Data(int ano, int mes, int dia, double hora);
    int ano;
    int mes;
    int dia;
    double hora;
    double horasTotais;
    double conversorParaHoras() const;
    Data conversorParaData(double horas);
    int getDiaDaSemana() const;
    std::string getDiaSemanaStr() const;
    std::string getMesStr() const;
    std::string formatarData() const;
};

#endif