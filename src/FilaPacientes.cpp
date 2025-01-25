#include "FilaPacientes.h"

FilaPacientes::FilaPacientes() {
    inicio = nullptr;
    fim = nullptr;
    tamanhoFila = 0;
}

FilaPacientes::~FilaPacientes() {
    limparFila();
}

void FilaPacientes::enfileirar(Paciente* paciente) {
    No* novoNo = new No(paciente);
    if (estaVazia()) {
        inicio = novoNo;
        fim = novoNo;
    } else {
        fim->proximo = novoNo;
        fim = novoNo;
    }  
    tamanhoFila++;
}

Paciente* FilaPacientes::desenfileirar() {
    if (estaVazia()) {
        return nullptr;
    }
    
    No* noRemovido = inicio;
    Paciente* paciente = noRemovido->paciente;
    inicio = inicio->proximo;
    if (inicio == nullptr) {
        fim = nullptr;
    }
    
    delete noRemovido;
    tamanhoFila--;
    return paciente;
}

bool FilaPacientes::estaVazia() const {
    return inicio == nullptr;
}

void FilaPacientes::limparFila() {
    while (!estaVazia()) {
        No* temp = inicio;
        inicio = inicio->proximo;
        delete temp;
    }
    inicio = nullptr;
    fim = nullptr;
    tamanhoFila = 0;
}

int FilaPacientes::getTamanho() const {
    return tamanhoFila;
}