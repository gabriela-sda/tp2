#include "Data.h"

Data::Data() : ano(0), mes(0), dia(0), hora(0.0), horasTotais(0.0) {}

Data::Data(int ano, int mes, int dia, double hora) : ano(ano), mes(mes), dia(dia), hora(hora) {
    horasTotais = conversorParaHoras();
}

Data::Data(double horasTotais) : horasTotais(horasTotais) {
    *this = conversorParaData(horasTotais);
}

double Data::conversorParaHoras() const {
    time_t t = time(0);
    tm* timeInfo = localtime(&t);
    timeInfo->tm_year = ano - 1900;
    timeInfo->tm_mon = mes - 1;
    timeInfo->tm_mday = dia;
    timeInfo->tm_hour = static_cast<int>(hora);
    timeInfo->tm_min = static_cast<int>((hora - static_cast<int>(hora)) * 60);
    timeInfo->tm_sec = 0;
    return mktime(timeInfo) / 3600.0;
}

Data Data::conversorParaData(double horas) {
    time_t t = static_cast<time_t>(horas * 3600.0);
    tm* timeInfo = localtime(&t);
    Data data;
    data.ano = timeInfo->tm_year + 1900;
    data.mes = timeInfo->tm_mon + 1;
    data.dia = timeInfo->tm_mday;
    data.hora = timeInfo->tm_hour + (timeInfo->tm_min / 60.0);
    data.horasTotais = horas;
    return data;
}


int Data::getDiaDaSemana() const {
    int m = mes;
    int y = ano;
    if (m <= 2) {
        m += 12;
        y--;
    }
    int h = (dia + (13 * (m + 1)) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
    return (h + 6) % 7;
}

std::string Data::getDiaSemanaStr() const {
    static const char* diasSemana[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    return diasSemana[getDiaDaSemana()];
}

std::string Data::getMesStr() const {
    static const char* meses[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    return meses[mes - 1];
}

std::string Data::formatarData() const {
    char buffer[50];
    int horas = static_cast<int>(hora);
    int minutos = static_cast<int>(round((hora - horas) * 60));
    
    if (minutos == 60) {
        horas += 1;
        minutos = 0;
    }
    
    sprintf(buffer, "%s %s %02d %02d:%02d:%02d %d", 
            getDiaSemanaStr().c_str(), 
            getMesStr().c_str(), 
            dia,
            horas,
            minutos, 
            0, 
            ano);
    return std::string(buffer);
}