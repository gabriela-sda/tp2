#ifndef FILA_PACIENTES_H
#define FILA_PACIENTES_H

#include "Paciente.h"

struct No {
    No(Paciente* p): paciente(p), proximo(nullptr) {}
    Paciente* paciente;
    No* proximo;
};

class FilaPacientes {
    public:
    FilaPacientes();
    ~FilaPacientes();
    void enfileirar(Paciente* paciente);
    Paciente* desenfileirar();
    bool estaVazia() const;
    void limparFila();
    int getTamanho() const;

    //private:
    No* inicio;
    No* fim;
    int tamanhoFila;
};

#endif