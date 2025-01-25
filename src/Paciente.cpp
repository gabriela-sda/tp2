#include "Paciente.h"

Paciente::Paciente(long Id, int Alta, Data DataAdmissao, int Urgencia, int NumMedidas, int NumTestes, int NumExames,
    int NumMedicacoes) {
    id = Id;
    alta = Alta;
    dataAdmissao = DataAdmissao;
    urgencia = Urgencia;
    medidasNecessarias = NumMedidas;
    testesNecessarios = NumTestes;
    examesNecessarios = NumExames;
    medicacoesNecessarias = NumMedicacoes;
    triagemRealizada = false;
    atendimentoRealizado = false;
    medidasRealizadas = 0;
    testesRealizados = 0;
    examesRealizados = 0;
    medicacoesRealizadas = 0;
    estadoAtual = NAO_CHEGOU;
    horarioAtual = 0;
    tempoEspera = 0;
    tempoConsulta = 0;
    tempoHospital = 0;
    
    // Data de alta começa zerada
    dataAlta = {0, 0, 0, 0};
}

Paciente::~Paciente() {
    // We don't delete ultimaUnidade here because it's owned by Procedimento
    ultimaUnidade = nullptr;
}

Paciente::Paciente(const Paciente& outro) :
    id(outro.id),
    alta(outro.alta),
    urgencia(outro.urgencia),
    medidasNecessarias(outro.medidasNecessarias),
    testesNecessarios(outro.testesNecessarios),
    examesNecessarios(outro.examesNecessarios),
    medicacoesNecessarias(outro.medicacoesNecessarias),
    triagemRealizada(outro.triagemRealizada),
    atendimentoRealizado(outro.atendimentoRealizado),
    medidasRealizadas(outro.medidasRealizadas),
    testesRealizados(outro.testesRealizados),
    examesRealizados(outro.examesRealizados),
    medicacoesRealizadas(outro.medicacoesRealizadas),
    dataAdmissao(outro.dataAdmissao),
    dataAlta(outro.dataAlta),
    estadoAtual(outro.estadoAtual),
    ultimaUnidade(outro.ultimaUnidade),
    horarioAtual(outro.horarioAtual),
    tempoEspera(outro.tempoEspera),
    tempoConsulta(outro.tempoConsulta),
    tempoHospital(outro.tempoHospital) {}

Paciente& Paciente::operator=(const Paciente& outro) {
    if (this != &outro) {
        id = outro.id;
        alta = outro.alta;
        urgencia = outro.urgencia;
        medidasNecessarias = outro.medidasNecessarias;
        testesNecessarios = outro.testesNecessarios;
        examesNecessarios = outro.examesNecessarios;
        medicacoesNecessarias = outro.medicacoesNecessarias;
        triagemRealizada = outro.triagemRealizada;
        atendimentoRealizado = outro.atendimentoRealizado;
        medidasRealizadas = outro.medidasRealizadas;
        testesRealizados = outro.testesRealizados;
        examesRealizados = outro.examesRealizados;
        medicacoesRealizadas = outro.medicacoesRealizadas;
        dataAdmissao = outro.dataAdmissao;
        dataAlta = outro.dataAlta;
        estadoAtual = outro.estadoAtual;
        ultimaUnidade = outro.ultimaUnidade;
        horarioAtual = outro.horarioAtual;
        tempoEspera = outro.tempoEspera;
        tempoConsulta = outro.tempoConsulta;
        tempoHospital = outro.tempoHospital;
    }
    return *this;
}


bool Paciente::temProcedimentosPendentes() const {
    return (!triagemRealizada) || 
           (!atendimentoRealizado) ||
           (medidasRealizadas < medidasNecessarias) ||
           (testesRealizados < testesNecessarios) ||    
           (examesRealizados < examesNecessarios) ||
           (medicacoesRealizadas < medicacoesNecessarias);
}

TipoProcedimento Paciente::getProximoProcedimentoPendente() const {
    if (!triagemRealizada) return TRIAGEM;
    if (!atendimentoRealizado) return ATENDIMENTO;
    if (medidasRealizadas < medidasNecessarias) return MEDIDAS_HOSPITALARES;
    if (testesRealizados < testesNecessarios) return TESTES;
    if (examesRealizados < examesNecessarios) return EXAMES;
    if (medicacoesRealizadas < medicacoesNecessarias) return MEDICACAO;
    return INDEFINIDO;
}

void Paciente::setEstadoAtual(int novoEstado) {
    estadoAtual = static_cast<EstadoPaciente>(novoEstado);
    
    if (novoEstado == RECEBEU_ALTA) {
        registrarTempoHospital();
        registrarDataAlta();
    }
}

void Paciente::setHorarioAtual(double novoHorario) {
    if (novoHorario > horarioAtual) {
        horarioAtual = novoHorario;
    }
}

void Paciente::registrarTempoHospital() {
    tempoHospital = tempoEspera + tempoConsulta;
}

void Paciente::registrarDataAlta() {
    double horasAlta = dataAdmissao.horasTotais + tempoHospital;
    dataAlta = Data(horasAlta);
}
