#include "Procedimento.h"

Procedimento::Procedimento() : tipo(INDEFINIDO), latencia(0), numUnidades(0), unidades(nullptr) {}

Procedimento::Procedimento(TipoProcedimento Tipo, float Duracao, int NumUnidades) {
    tipo = Tipo;
    latencia = Duracao;
    numUnidades = NumUnidades;
    unidades = new Unidade*[numUnidades];
    for(int i = 0; i < numUnidades; i++) {
        unidades[i] = new Unidade();
    }
}

Procedimento::Procedimento(const Procedimento& outro) {
    tipo = outro.tipo;
    latencia = outro.latencia;
    numUnidades = outro.numUnidades;

    unidades = new Unidade*[numUnidades];
    for (int i = 0; i < numUnidades; i++) {
        unidades[i] = new Unidade(*outro.unidades[i]);
    }
}

Procedimento& Procedimento::operator=(const Procedimento& outro) {
    if (this != &outro) {
        // Cria novos arrays primeiro
        Unidade** newUnidades = new Unidade*[outro.numUnidades];
        for (int i = 0; i < outro.numUnidades; i++) {
            newUnidades[i] = new Unidade(*outro.unidades[i]);
        }
        
        // Deleta arrays antigos apenas após alocar os novos
        if (unidades) {
            for (int i = 0; i < numUnidades; i++) {
                delete unidades[i];
            }
            delete[] unidades;
        }

        tipo = outro.tipo;
        latencia = outro.latencia;
        numUnidades = outro.numUnidades;
        unidades = newUnidades;
    }
    return *this;
}

Procedimento::Procedimento(Procedimento&& other) : 
    tipo(other.tipo),
    latencia(other.latencia),
    numUnidades(other.numUnidades),
    unidades(other.unidades) {
    other.unidades = nullptr;
    other.numUnidades = 0;
}

Procedimento& Procedimento::operator=(Procedimento&& other) {
    if (this != &other) {
        // Limpa recursos existentes
        if (unidades) {
            for (int i = 0; i < numUnidades; i++) {
                delete unidades[i];
            }
            delete[] unidades;
        }

        // Transfere propriedade
        tipo = other.tipo;
        latencia = other.latencia;
        numUnidades = other.numUnidades;
        unidades = other.unidades;

        // Reseta objeto fonte
        other.unidades = nullptr;
        other.numUnidades = 0;
    }
    return *this;
}


Procedimento::~Procedimento() {
    if (unidades) {
        limparUnidades();
    }
}

void Procedimento::limparUnidades() {
    for (int i = 0; i < numUnidades; i++) {
        delete unidades[i];
    }
    delete[] unidades;
    unidades = nullptr;
    numUnidades = 0;
}

void Procedimento::getUtilizacao() {
    double utilizacaoTotal = 0;
    for (int i = 0; i < numUnidades; i++) {
        utilizacaoTotal += unidades[i]->utilizacao;
        std::cout << "Utilizacao da unidade " << i + 1 << ": " << unidades[i]->utilizacao;
    }
    utilizacaoMedia = utilizacaoTotal / numUnidades;
    std::cout << "Utilizacao media: " << utilizacaoMedia << std::endl;
}