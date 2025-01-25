#ifndef SIMULADOR_HOSPITAL_H
#define SIMULADOR_HOSPITAL_H

#include "Escalonador.h"
#include "Procedimento.h"
#include "FilaPacientes.h"
#include "ListaPacientes.h"

#include <string>
#include <fstream>

class SimuladorHospital {
    public:
    SimuladorHospital(const std::string& arquivoConfiguracoes);
    ~SimuladorHospital();
    void iniciarSimulacao();
    void gerarRelatorio();
    void gerarEstatisticas();
    void finalizarSimulacao();

    private:
    Escalonador escalonador;
    double horarioInicial;
    double horarioFinal;
    double horarioAtual;
    double horarioAnterior;
    int numPacientes;
    int numPacientesAtual;
    FilaPacientes filaTriagem, filaAtendimento[3], filaMedidas[3], filaTestes[3], filaExames[3], filaMedicacao[3];
    ListaPacientes listaGlobal;
    Procedimento triagem, atendimento, medidasHospitalares, testes, exames, medicacao;
    bool simulacaoFinalizada;

    // Funções principais para processamento de eventos
    void processarFaseA(); //Avança o relógio para o instante do próximo evento B agendado
    void processarFaseB(); //Processa eventos do tipo B
    void processarFaseC(); //Processa eventos do tipo C cujas condições foram avaliadas como verdadeiras

    // Funções auxiliares para processamento de eventos
    void iniciarProcedimento(Paciente* paciente, TipoProcedimento procedimentoIniciado);
    void encaminharParaProximoProcedimento(Paciente* paciente, TipoProcedimento proximoProcedimento);
    void finalizarProcedimento(Paciente* paciente, TipoProcedimento procedimentoFinalizado);

    // Funções para gerar estatisticas
    
    void calcularTempoTotalMedio();
    void calcularTempoConsultaMedio();
    void calcularTempoEsperaMedio();
    void calcularTempoEsperaMedioPorPrioridade();
    void calcularUtilizacaoMediaPorProcedimento(); //copiar de procedimento

};

#endif