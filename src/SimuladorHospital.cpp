#include "SimuladorHospital.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

SimuladorHospital::SimuladorHospital(const std::string& arquivoConfiguracoes): 
escalonador(), horarioFinal(0.0), horarioAtual(0.0), numPacientesAtual(0), filaTriagem(), filaAtendimento{},
filaMedidas{}, filaTestes{}, filaExames{}, filaMedicacao{}, listaGlobal(), simulacaoFinalizada(false) {

    try {
    std::ifstream arquivo(arquivoConfiguracoes.c_str());
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo de configurações.");
    }
    double tempo;
    int unidades;
    int id, alta, ano, mes, dia, hora, urgencia, mh, tl, ei, im;

    // Lê configurações do procedimento de triagem
    arquivo >> tempo >> unidades;
    triagem = Procedimento(TRIAGEM, tempo, unidades);

    // Lê configurações do procedimento de atendimento
    arquivo >> tempo >> unidades;
    atendimento = Procedimento(ATENDIMENTO, tempo, unidades);

    // Lê configurações do procedimento de medidas hospitalares
    arquivo >> tempo >> unidades;
    medidasHospitalares = Procedimento(MEDIDAS_HOSPITALARES, tempo, unidades);

    // Lê configurações do procedimento de testes
    arquivo >> tempo >> unidades;
    testes = Procedimento(TESTES, tempo, unidades);

    // Lê configurações do procedimento de exames
    arquivo >> tempo >> unidades;
    exames = Procedimento(EXAMES, tempo, unidades);

    // Lê configurações do procedimento de medicação
    arquivo >> tempo >> unidades;
    medicacao = Procedimento(MEDICACAO, tempo, unidades);

    // Lê número de pacientes
    arquivo >> numPacientes;

    // Lê e registra primeiro paciente
    arquivo >> id >> alta >> ano >> mes >> dia >> hora >> urgencia >> mh >> tl >> ei >> im;
    Data dataAdmissao{ano, mes, dia, static_cast<double>(hora)};
    Paciente* paciente = new Paciente(id, alta, dataAdmissao, urgencia, mh, tl, ei, im);
    paciente->admissaoEmHorarioSimulacao = 0.0;
    horarioInicial = paciente->dataAdmissao.horasTotais;
    listaGlobal.inserirPaciente(paciente);

    // Lê e registra demais pacientes
    for (int i = 0; i < numPacientes-1; i++) {
        arquivo >> id >> alta >> ano >> mes >> dia >> hora >> urgencia >> mh >> tl >> ei >> im;
        Data dataAdmissao{ano, mes, dia, static_cast<double>(hora)};
        Paciente* paciente = new Paciente(id, alta, dataAdmissao, urgencia, mh, tl, ei, im);
        paciente->admissaoEmHorarioSimulacao = dataAdmissao.horasTotais - horarioInicial; 
        paciente->horarioAtual = paciente->admissaoEmHorarioSimulacao;
        listaGlobal.inserirPaciente(paciente);

    }

    // Cria eventos de chegada
    for (auto it = listaGlobal.begin(); it != listaGlobal.end(); ++it) {
        Paciente* paciente = *it;
        Evento* evento = new Evento(CHEGADA, paciente->admissaoEmHorarioSimulacao, paciente);
        escalonador.inserirEvento(evento);
    }
    arquivo.close();
    } catch (...) {
        finalizarSimulacao(); // Se ocorrer qualquer exceção, libera recursos alocados
        throw;
    }
}

SimuladorHospital::~SimuladorHospital() {
    finalizarSimulacao();
}

void SimuladorHospital::iniciarSimulacao() {
    try {
        while (escalonador.temEventos()) { 
                processarFaseA();
                processarFaseB();
                processarFaseC();
        }
    } catch (...) {
        while (escalonador.temEventos()) {
            Evento* evento = escalonador.retirarProximoEvento();
            delete evento;
        }
        throw;
    }
    horarioFinal = horarioAtual;
    for (int i = 0; i < triagem.numUnidades; i++) {
        triagem.unidades[i]->registrarUtilizacao(horarioFinal);
    }
    for (int i = 0; i < atendimento.numUnidades; i++) {
        atendimento.unidades[i]->registrarUtilizacao(horarioFinal);
    }
    for (int i = 0; i < medidasHospitalares.numUnidades; i++) {
        medidasHospitalares.unidades[i]->registrarUtilizacao(horarioFinal);
    }
    for (int i = 0; i < testes.numUnidades; i++) {
        testes.unidades[i]->registrarUtilizacao(horarioFinal);
    }
    for (int i = 0; i < exames.numUnidades; i++) {
        exames.unidades[i]->registrarUtilizacao(horarioFinal);
    }
    for (int i = 0; i < medicacao.numUnidades; i++) {
        medicacao.unidades[i]->registrarUtilizacao(horarioFinal);
    }
}

void SimuladorHospital::processarFaseA() {
    if (escalonador.temEventos()) {
        double proximoHorario = escalonador.getProximoHorario();
        horarioAtual = proximoHorario;
    }
}

void SimuladorHospital::processarFaseB() {
    while (escalonador.temEventos() && escalonador.getProximoHorario() == horarioAtual) {
        Evento* evento = escalonador.retirarProximoEvento();
        if (!evento) continue;
        Paciente* paciente = evento->paciente;

        switch (evento->tipoEvento) {
            case CHEGADA:
                numPacientesAtual++;
                paciente->setEstadoAtual(FILA_TRIAGEM);
                paciente->setHorarioAtual(horarioAtual);
                filaTriagem.enfileirar(paciente);
                break;

            case INICIO_PROCEDIMENTO: break; // Não é processado na fase B (é condicional)

            case FIM_PROCEDIMENTO: { 
                switch (evento->tipoProcedimento) {
                    case INDEFINIDO: break;
                    case TRIAGEM: finalizarProcedimento(paciente, TRIAGEM); break;
                    case ATENDIMENTO: finalizarProcedimento(paciente, ATENDIMENTO); break;
                    case MEDIDAS_HOSPITALARES: finalizarProcedimento(paciente, MEDIDAS_HOSPITALARES); break;
                    case TESTES: finalizarProcedimento(paciente, TESTES); break;
                    case EXAMES: finalizarProcedimento(paciente, EXAMES); break;
                    case MEDICACAO: finalizarProcedimento(paciente, MEDICACAO); break;
                    }
                }
            }
            delete evento;
        }
    }

void SimuladorHospital::processarFaseC() {
    // Processa triagem
    for(int i = 0; i < triagem.numUnidades; i++) {
        if (!filaTriagem.estaVazia() && !triagem.unidades[i]->estaOcupada) {
            Paciente* paciente = filaTriagem.desenfileirar();
            paciente->ultimaUnidade = triagem.unidades[i];
            if (paciente)
            iniciarProcedimento(paciente, TRIAGEM);
            }
    }

    // Processa demais procedimentos por prioridade
    for (int urgencia = 2; urgencia >= 0; urgencia--) {
        // Atendimento
        for(int i = 0; i < atendimento.numUnidades; i++) {
            if (!filaAtendimento[urgencia].estaVazia() && !atendimento.unidades[i]->estaOcupada) {
                Paciente* paciente = filaAtendimento[urgencia].desenfileirar();
                paciente->ultimaUnidade = atendimento.unidades[i];
                if (paciente)
                iniciarProcedimento(paciente, ATENDIMENTO);
            }
        }

        // Medidas
        for(int i = 0; i < medidasHospitalares.numUnidades; i++) {
            if (!filaMedidas[urgencia].estaVazia() && !medidasHospitalares.unidades[i]->estaOcupada) {
                Paciente* paciente = filaMedidas[urgencia].desenfileirar();
                paciente->ultimaUnidade = medidasHospitalares.unidades[i];
                if (paciente)
                iniciarProcedimento(paciente, MEDIDAS_HOSPITALARES);
            }
        }

        // Testes
        for(int i = 0; i < testes.numUnidades; i++) {
            if (!filaTestes[urgencia].estaVazia() && !testes.unidades[i]->estaOcupada) {
                Paciente* paciente = filaTestes[urgencia].desenfileirar();
                paciente->ultimaUnidade = testes.unidades[i];
                if (paciente)
                iniciarProcedimento(paciente, TESTES);
            }
        }

        // Exames
        for(int i = 0; i < exames.numUnidades; i++) {
            if (!filaExames[urgencia].estaVazia() && !exames.unidades[i]->estaOcupada) {
                Paciente* paciente = filaExames[urgencia].desenfileirar();
                paciente->ultimaUnidade = exames.unidades[i];
                if (paciente)
                iniciarProcedimento(paciente, EXAMES);
            }
        }

        // Medicação
        for(int i = 0; i < medicacao.numUnidades; i++) {
            if (!filaMedicacao[urgencia].estaVazia() && !medicacao.unidades[i]->estaOcupada) {
                Paciente* paciente = filaMedicacao[urgencia].desenfileirar();
                paciente->ultimaUnidade = medicacao.unidades[i];
                if (paciente)
                iniciarProcedimento(paciente, MEDICACAO);
            }
        }
    }
}

void SimuladorHospital::finalizarSimulacao() {
    if (simulacaoFinalizada) {
        return;
    }
    // Eventos já são deletados ao fim da fase B.
    
    // Limpar todas as filas
    filaTriagem.limparFila();
    for(int i = 0; i < 3; i++) {
        filaAtendimento[i].limparFila();
        filaMedidas[i].limparFila();
        filaTestes[i].limparFila();
        filaExames[i].limparFila();
        filaMedicacao[i].limparFila();
    }

    // Limpar unidades de procedimentos
    triagem.limparUnidades();
    atendimento.limparUnidades();
    medidasHospitalares.limparUnidades();
    testes.limparUnidades();
    exames.limparUnidades();
    medicacao.limparUnidades();

    listaGlobal.limparLista();

    simulacaoFinalizada = true;
    horarioAtual = 0;
}

void SimuladorHospital::gerarRelatorio() {
    for (auto it = listaGlobal.begin(); it != listaGlobal.end(); ++it) {
        Paciente* paciente = *it;
        if (paciente->estadoAtual == RECEBEU_ALTA) {
            char buffer[200];
            sprintf(buffer, "%ld %s %s %.2f %.2f %.2f",
                    paciente->id,
                    paciente->dataAdmissao.formatarData().c_str(),
                    paciente->dataAlta.formatarData().c_str(),
                    paciente->tempoHospital,
                    paciente->tempoConsulta,
                    paciente->tempoEspera);
            std::cout << buffer << std::endl;
        }
    }
}

void SimuladorHospital::gerarEstatisticas() {
    calcularTempoTotalMedio();
    calcularTempoConsultaMedio();
    calcularTempoEsperaMedio();
    calcularTempoEsperaMedioPorPrioridade();
    calcularUtilizacaoMediaPorProcedimento();
}

void SimuladorHospital::iniciarProcedimento(Paciente* paciente, TipoProcedimento tipo) {
    paciente->ultimaUnidade->ocuparUnidade(horarioAtual);
    paciente->tempoEspera += horarioAtual - paciente->horarioAtual;
    paciente->horarioAtual = horarioAtual;
    Evento* fimProcedimento = nullptr;
    double latencia = 0;

    switch(tipo) {
        case INDEFINIDO: break;
        case TRIAGEM: paciente->setEstadoAtual(EM_TRIAGEM); latencia = triagem.latencia; break;
        case ATENDIMENTO: paciente->setEstadoAtual(EM_ATENDIMENTO); latencia = atendimento.latencia; break;
        case MEDIDAS_HOSPITALARES: paciente->setEstadoAtual(EM_MEDIDAS); latencia = medidasHospitalares.latencia; break;
        case TESTES: paciente->setEstadoAtual(EM_TESTES); latencia = testes.latencia; break;
        case EXAMES: paciente->setEstadoAtual(EM_EXAMES); latencia = exames.latencia; break;
        case MEDICACAO: paciente->setEstadoAtual(EM_MEDICACAO); latencia = medicacao.latencia; break;
    }

    fimProcedimento = new Evento(FIM_PROCEDIMENTO, tipo, horarioAtual + latencia, paciente);
    escalonador.inserirEvento(fimProcedimento);
}

void SimuladorHospital::encaminharParaProximoProcedimento(Paciente* paciente, TipoProcedimento proximoProcedimento) {
    if (proximoProcedimento == TRIAGEM) { filaTriagem.enfileirar(paciente); paciente->setEstadoAtual(FILA_TRIAGEM);}

    else if (proximoProcedimento == ATENDIMENTO) { filaAtendimento[paciente->urgencia].enfileirar(paciente);
    paciente->setEstadoAtual(FILA_ATENDIMENTO); }

    else if (proximoProcedimento == MEDIDAS_HOSPITALARES) { filaMedidas[paciente->urgencia].enfileirar(paciente);
    paciente->setEstadoAtual(FILA_MEDIDAS); }
    
    else if (proximoProcedimento == TESTES) { filaTestes[paciente->urgencia].enfileirar(paciente);
    paciente->setEstadoAtual(FILA_TESTES);}

    else if (proximoProcedimento == EXAMES) { filaExames[paciente->urgencia].enfileirar(paciente);
    paciente->setEstadoAtual(FILA_EXAMES); }
    
    else if (proximoProcedimento == MEDICACAO) { filaMedicacao[paciente->urgencia].enfileirar(paciente);
    paciente->setEstadoAtual(FILA_MEDICACAO); }
}

void SimuladorHospital::finalizarProcedimento(Paciente* paciente, TipoProcedimento tipo) {
    paciente->ultimaUnidade->liberarUnidade(horarioAtual);
    paciente->tempoConsulta += horarioAtual - paciente->ultimaUnidade->inicioUltimoProcedimento;
    paciente->horarioAtual = horarioAtual;

    switch(tipo) {
        case INDEFINIDO: break;
        case TRIAGEM: paciente->triagemRealizada = true; break;
        case ATENDIMENTO: paciente->atendimentoRealizado = true; break;
        case MEDIDAS_HOSPITALARES: paciente->medidasRealizadas++; break;
        case TESTES: paciente->testesRealizados++; break;
        case EXAMES: paciente->examesRealizados++; break;
        case MEDICACAO: paciente->medicacoesRealizadas++; break;
    }

    if ((paciente->alta == 0 && paciente->temProcedimentosPendentes()) || (paciente->alta == 1 && !paciente->atendimentoRealizado)) {
        encaminharParaProximoProcedimento(paciente, paciente->getProximoProcedimentoPendente());
    } else {
        numPacientesAtual--;
        paciente->setEstadoAtual(RECEBEU_ALTA);
    }
}

void SimuladorHospital::calcularTempoTotalMedio() {
    double tempoTotalMedio = 0;
    for (auto it = listaGlobal.begin(); it != listaGlobal.end(); ++it) {
        Paciente* paciente = *it;
        tempoTotalMedio += paciente->tempoHospital;
    }
    std::cout << "Tempo no hospital medio: " << tempoTotalMedio / numPacientes << std::endl;
}

void SimuladorHospital::calcularTempoConsultaMedio() {
    double tempoConsultaMedio = 0;
    for (auto it = listaGlobal.begin(); it != listaGlobal.end(); ++it) {
        Paciente* paciente = *it;
        tempoConsultaMedio += paciente->tempoConsulta;
    }
    std::cout << "Tempo consulta medio: " << tempoConsultaMedio / numPacientes << std::endl;
}

void SimuladorHospital::calcularTempoEsperaMedio() {
    double tempoEsperaMedio = 0;
    for (auto it = listaGlobal.begin(); it != listaGlobal.end(); ++it) {
        Paciente* paciente = *it;
        tempoEsperaMedio += paciente->tempoEspera;
    }
    std::cout << "Tempo espera medio: " << tempoEsperaMedio / numPacientes << std::endl;

}

void SimuladorHospital::calcularTempoEsperaMedioPorPrioridade() {
    int pacientesPrioridade0 = 0, pacientesPrioridade1 = 0, pacientesPrioridade2 = 0;
    double tempoEsperaPrioridade0 = 0, tempoEsperaPrioridade1 = 0, tempoEsperaPrioridade2 = 0;

    for (auto it = listaGlobal.begin(); it != listaGlobal.end(); ++it) {
        Paciente* paciente = *it;
        if (paciente->urgencia == 0) {
            pacientesPrioridade0++;
            tempoEsperaPrioridade0 += paciente->tempoEspera;

        } else if (paciente->urgencia == 1) {
            pacientesPrioridade1++;
            tempoEsperaPrioridade1 += paciente->tempoEspera;

        } else if (paciente->urgencia == 2) {
            pacientesPrioridade2++;
            tempoEsperaPrioridade2 += paciente->tempoEspera;
        }
    }
    if(pacientesPrioridade0 != 0) {
        double tempoEsperaMediaPrioridade0 = tempoEsperaPrioridade0 / pacientesPrioridade0;
        std::cout << "Tempo de espera medio por prioridade (0): " << tempoEsperaMediaPrioridade0 << std::endl;
    } else {
        double tempoEsperaMediaPrioridade0 = 0;
        std::cout << "Tempo de espera medio por prioridade (0): " << tempoEsperaMediaPrioridade0 << std::endl;
    }
    
    if(pacientesPrioridade1 != 0) {
        double tempoEsperaMediaPrioridade1 = tempoEsperaPrioridade1 / pacientesPrioridade1;
        std::cout << "Tempo de espera medio por prioridade (1): " << tempoEsperaMediaPrioridade1 << std::endl;
    } else {
        double tempoEsperaMediaPrioridade1 = 0;
        std::cout << "Tempo de espera medio por prioridade (1): " << tempoEsperaMediaPrioridade1 << std::endl;
    }

    if(pacientesPrioridade2 != 0) {
        double tempoEsperaMediaPrioridade2 = tempoEsperaPrioridade2 / pacientesPrioridade2;
        std::cout << "Tempo de espera medio por prioridade (2): " << tempoEsperaMediaPrioridade2 << std::endl;
    } else {
        double tempoEsperaMediaPrioridade2 = 0;
        std::cout << "Tempo de espera medio por prioridade (2): " << tempoEsperaMediaPrioridade2 << std::endl;
    }
}
void SimuladorHospital::calcularUtilizacaoMediaPorProcedimento() {
    // Para triagem
    double utilizacao = 0;
    std::cout << "Triagem:" << std::endl;
    for (int i = 0; i < triagem.numUnidades; i++) {
        utilizacao += triagem.unidades[i]->utilizacao;
        std::cout << "Utilizacao da unidade " << i + 1 << ": " << triagem.unidades[i]->utilizacao << std::endl;
    }
    double utilizacaoMedia = utilizacao / triagem.numUnidades;
    std::cout << "Utilizacao media: " << utilizacaoMedia << std::endl;

    // Para atendimento
    utilizacao = 0;
    std::cout << "Atendimento:" << std::endl;
    for (int i = 0; i < atendimento.numUnidades; i++) {
        utilizacao += atendimento.unidades[i]->utilizacao;
        std::cout << "Utilizacao da unidade " << i + 1 << ": " << atendimento.unidades[i]->utilizacao << std::endl;
    }
    utilizacaoMedia = utilizacao / atendimento.numUnidades;
    std::cout << "Utilizacao media: " << utilizacaoMedia << std::endl;

    // Para medidas hospitalares
    utilizacao = 0;
    std::cout << "Medidas hospitalares:" << std::endl;
    for (int i = 0; i < medidasHospitalares.numUnidades; i++) {
        utilizacao += medidasHospitalares.unidades[i]->utilizacao;
        std::cout << "Utilizacao da unidade " << i + 1 << ": " << medidasHospitalares.unidades[i]->utilizacao << std::endl;
    }
    utilizacaoMedia = utilizacao / medidasHospitalares.numUnidades;
    std::cout << "Utilizacao media: " << utilizacaoMedia << std::endl;

    // Para testes
    utilizacao = 0;
    std::cout << "Testes:" << std::endl;
    for (int i = 0; i < testes.numUnidades; i++) {
        utilizacao += testes.unidades[i]->utilizacao;
        std::cout << "Utilizacao da unidade " << i + 1 << ": " << testes.unidades[i]->utilizacao << std::endl;
    }
    utilizacaoMedia = utilizacao / testes.numUnidades;
    std::cout << "Utilizacao media: " << utilizacaoMedia << std::endl;

    // Para exames
    utilizacao = 0;
    std::cout << "Exames:" << std::endl;
    for (int i = 0; i < exames.numUnidades; i++) {
        utilizacao += exames.unidades[i]->utilizacao;
        std::cout << "Utilizacao da unidade " << i + 1 << ": " << exames.unidades[i]->utilizacao << std::endl;
    }
    utilizacaoMedia = utilizacao / exames.numUnidades;
    std::cout << "Utilizacao media: " << utilizacaoMedia << std::endl;

    // Para medicacao
    utilizacao = 0;
    std::cout << "Medicacao:" << std::endl;
    for (int i = 0; i < medicacao.numUnidades; i++) {
        utilizacao += medicacao.unidades[i]->utilizacao;
        std::cout << "Utilizacao da unidade " << i + 1 << ": " << medicacao.unidades[i]->utilizacao << std::endl;
    }
    utilizacaoMedia = utilizacao / medicacao.numUnidades;
    std::cout << "Utilizacao media: " << utilizacaoMedia << std::endl;
}
