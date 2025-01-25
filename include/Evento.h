#ifndef EVENTO_H
#define EVENTO_H

#include "Paciente.h"
#include "Procedimento.h"

enum TipoEvento {
    CHEGADA, //evento B (agendado), processado na fase B.
    INICIO_PROCEDIMENTO, //evento C (condicional), processado na fase C. Não é agendado.
    FIM_PROCEDIMENTO, //evento B (agendado), processado na fase B.
};

class Evento {
    public:
    Evento(TipoEvento tipoEvento, TipoProcedimento tipoProcedimento, double horarioEvento, Paciente* paciente);
    Evento(TipoEvento tipoEvento, double horarioEvento, Paciente* paciente);
    Evento(const Evento& outro);
    Evento& operator=(const Evento& outro);
    ~Evento(){};
    bool operator>(const Evento& outro) const {
        return horarioEvento > outro.horarioEvento;
    }

    //private:
    TipoEvento tipoEvento;
    TipoProcedimento tipoProcedimento;
    double horarioEvento;
    Paciente* paciente;

};

#endif