#include "Evento.h"

Evento::Evento(TipoEvento Tipo, TipoProcedimento TipoProcedimento, double Horario,  Paciente* Paciente) {
    tipoEvento = Tipo;
    tipoProcedimento = TipoProcedimento;
    horarioEvento = Horario;
    paciente = Paciente;
}

Evento::Evento(TipoEvento Tipo, double Horario, Paciente* Paciente) {
    tipoEvento = Tipo;
    tipoProcedimento = INDEFINIDO;
    horarioEvento = Horario;
    paciente = Paciente;
}

Evento::Evento(const Evento& outro) :
    tipoEvento(outro.tipoEvento),
    horarioEvento(outro.horarioEvento),
    paciente(outro.paciente) {}

Evento& Evento::operator=(const Evento& outro) {
    if (this != &outro) {
        tipoEvento = outro.tipoEvento;
        horarioEvento = outro.horarioEvento;
        paciente = outro.paciente;
    }
    return *this;
}
