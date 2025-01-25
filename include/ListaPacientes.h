#ifndef LISTA_PACIENTES_H
#define LISTA_PACIENTES_H

#include "Paciente.h"
#include <iostream>

struct NoPaciente {
    NoPaciente(Paciente* p): paciente(p), proximo(nullptr) {}
    Paciente* paciente;
    NoPaciente* proximo;
};

// Iterador para percorrer a lista
class Iterator {
    public:
    NoPaciente* atual;
    Iterator(NoPaciente* no): atual(no) {}
    Iterator& operator++() { if(atual) atual = atual->proximo; return *this; }
    Paciente* operator*() { return atual ? atual->paciente : nullptr; }
    bool operator!=(const Iterator& other) { return atual != other.atual; }
};

class ListaPacientes {
    public:
    ListaPacientes();
    ~ListaPacientes();
    void inserirPaciente(Paciente* paciente);
    void limparLista();
    Iterator begin();
    Iterator end();
    
    //private:
    NoPaciente* inicio;
    NoPaciente* fim;
    int tamanho;
};

#endif