#ifndef PACIENTE_H
#define PACIENTE_H

#include "Procedimento.h"
#include "Data.h"

#include <iostream>

enum EstadoPaciente {
    NAO_CHEGOU = 1,
    FILA_TRIAGEM,
    EM_TRIAGEM,
    FILA_ATENDIMENTO,
    EM_ATENDIMENTO,
    FILA_MEDIDAS,
    EM_MEDIDAS,
    FILA_TESTES,
    EM_TESTES,
    FILA_EXAMES,
    EM_EXAMES,
    FILA_MEDICACAO,
    EM_MEDICACAO,
    RECEBEU_ALTA
};

class Paciente {
    public:
    Paciente(long id, int alta, Data dataAdmissao, int urgencia, int numMedidas, int numTestes, int numExames,
    int numMedicacoes);
    ~Paciente();
    Paciente(const Paciente& outro);
    Paciente& operator=(const Paciente& outro);
    bool temProcedimentosPendentes() const;
    TipoProcedimento getProximoProcedimentoPendente() const;
    void setEstadoAtual(int novoEstado);
    void setHorarioAtual(double novoHorario);
    void registrarTempoHospital();
    void registrarDataAlta();

    long id;
    int alta;
    int urgencia; //0 = verde, 1 = amarelo, 2 = vermelho
    int medidasNecessarias, testesNecessarios, examesNecessarios, medicacoesNecessarias;
    bool triagemRealizada, atendimentoRealizado;
    int medidasRealizadas, testesRealizados, examesRealizados, medicacoesRealizadas;
    Data dataAdmissao;
    Data dataAlta;
    EstadoPaciente estadoAtual;
    Unidade* ultimaUnidade;
    double admissaoEmHorarioSimulacao; //horarioAdmissaoEmTempoSimulacao
    double horarioAtual;
    double tempoEspera; // tempo em fila
    double tempoConsulta; // tempo em procedimentos
    double tempoHospital; // Tp = tempoEspera + tempoConsulta

};

#endif