#include <iostream>
#include <fstream>
#include <sstream>
#include "SimuladorHospital.h"

int main(int argc, char* argv[]) {
    if (argc != 2 || argv[1] == nullptr) {
        std::cout << "Insira um arquivo para iniciar o simulador, no formato: " << argv[0] << " <arquivo.csv>" << std::endl;
        return 1;
    }
    
    SimuladorHospital simulador(argv[1]);
    simulador.iniciarSimulacao();
    simulador.gerarRelatorio();
    //simulador.gerarEstatisticas();
    simulador.finalizarSimulacao();
    return 0;
}
