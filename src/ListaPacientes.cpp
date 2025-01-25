#include "ListaPacientes.h"

ListaPacientes::ListaPacientes() {
    inicio = nullptr;
    fim = nullptr;
    tamanho = 0;
}

ListaPacientes::~ListaPacientes() {
    limparLista();
}

void ListaPacientes::inserirPaciente(Paciente* paciente) {
    NoPaciente* novoNo = new NoPaciente(paciente);
    if (!inicio) {
        inicio = fim = novoNo;
    } else {
        fim->proximo = novoNo;
        fim = novoNo;
    }
    tamanho++;
}

void ListaPacientes::limparLista() {
    NoPaciente* atual = inicio;
    while (atual != nullptr) {
        NoPaciente* proximo = atual->proximo;
        delete atual->paciente; // Deleta o paciente
        delete atual; // Deleta o nó
        atual = proximo;
    }
    inicio = nullptr;
    fim = nullptr;
}

Iterator ListaPacientes::begin() {
    return Iterator(inicio);
}

Iterator ListaPacientes::end() {
    return Iterator(nullptr);
}