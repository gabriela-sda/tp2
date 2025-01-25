#include "Escalonador.h"

Escalonador::Escalonador(): capacidade(CAPACIDADE_INICIAL), tamanho(0) {
    heap = new Evento*[capacidade];
    for (int i = 0; i < capacidade; i++) {
        heap[i] = nullptr;
    }
}

Escalonador::Escalonador(const Escalonador& outro) 
    : capacidade(outro.capacidade), tamanho(outro.tamanho) {
    heap = new Evento*[capacidade];
    for (int i = 0; i < tamanho; i++) {
        heap[i] = new Evento(*outro.heap[i]);
    }
    for (int i = tamanho; i < capacidade; i++) {
        heap[i] = nullptr;
    }
}

Escalonador& Escalonador::operator=(const Escalonador& outro) {
    if (this != &outro) {
        // Delete existing heap
        for (int i = 0; i < tamanho; i++) {
            delete heap[i];
        }
        delete[] heap;

        // Copy from other
        capacidade = outro.capacidade;
        tamanho = outro.tamanho;
        heap = new Evento*[capacidade];
        for (int i = 0; i < tamanho; i++) {
            heap[i] = new Evento(*outro.heap[i]);
        }
        for (int i = tamanho; i < capacidade; i++) {
            heap[i] = nullptr;
        }
    }
    return *this;
}


Escalonador::~Escalonador() {
// Libera a memória de todos os eventos
    for (int i = 0; i < tamanho; i++) {
        delete heap[i];
    }
    delete[] heap;
}

void Escalonador::inserirEvento(Evento* evento) {
    if (!evento) return;  // Proteção contra ponteiro nulo

    // Verifica se precisa redimensionar
    if (tamanho == capacidade) {
        redimensionar();
    }
    // Insere o novo evento no final
    heap[tamanho] = evento;

    // Mantém a propriedade de heap
    subirHeap(tamanho);
    tamanho++;
}

Evento* Escalonador::retirarProximoEvento() {
    if (tamanho == 0) {
        return nullptr;
    }
    // Remove o evento com menor tempo (raiz)
    Evento* eventoRemovido = heap[0];

    // Move o último elemento para a raiz e reduz o tamanho
    heap[0] = heap[tamanho - 1];
    tamanho--;

    // Mantém a propriedade de heap
    if (tamanho > 0) {
        descerHeap(0);
    }
    return eventoRemovido;
}

bool Escalonador::atualizarHorario(Evento* evento, double novoHorario) {
    int indice = encontrarIndice(evento);
    if (indice == -1) {
        return false;  // Evento não encontrado
    }

    double horarioAntigo = evento->horarioEvento;
    evento->horarioEvento = novoHorario;

    // Se o novo horário é menor, sobe na heap
    if (novoHorario < horarioAntigo) {
        subirHeap(indice);
    }
    // Se o novo horário é maior, desce na heap
    else if (novoHorario > horarioAntigo) {
        descerHeap(indice);
    }

    return true;
}

bool Escalonador::removerEvento(Evento* evento) {
    int indice = encontrarIndice(evento);
    if (indice == -1) {
        return false;  // Evento não encontrado
    }

    // Substitui pelo último elemento
    heap[indice] = heap[tamanho - 1];
    tamanho--;

    // Se não era o último elemento, reorganiza a heap
    if (indice < tamanho) {
        // Tenta subir e descer o elemento que substituiu (apenas uma das operações tem efeito)
        subirHeap(indice);
        descerHeap(indice);
    }
    return true;
}

bool Escalonador::temEventos() const {
    return tamanho > 0;
}

double Escalonador::getProximoHorario() const {
    if (tamanho == 0) {
        return -1;
    }
    return heap[0]->horarioEvento;
}

int Escalonador::getTamanho() const {
    return tamanho;
}

// Mantém a propriedade de heap subindo um elemento
void Escalonador::subirHeap(int indice) {
    while (indice > 0 && heap[pai(indice)]->horarioEvento > heap[indice]->horarioEvento) {
        // Troca o evento atual com seu pai
        Evento* temp = heap[indice];
        heap[indice] = heap[pai(indice)];
        heap[pai(indice)] = temp;
            
        indice = pai(indice);
    }
}

// Mantém a propriedade de heap descendo um elemento
void Escalonador::descerHeap(int indice) {
    int menorIndice = indice;
    int esquerdo = filhoEsquerdo(indice);
    int direito = filhoDireito(indice);

    // Compara com filho esquerdo
    if (esquerdo < tamanho && heap[esquerdo]->horarioEvento < heap[menorIndice]->horarioEvento) {
        menorIndice = esquerdo;
    }

    // Compara com filho direito
    if (direito < tamanho && heap[direito]->horarioEvento < heap[menorIndice]->horarioEvento) {
        menorIndice = direito;
    }

    // Se necessário, troca e continua descendo
    if (menorIndice != indice) {
        Evento* temp = heap[indice];
        heap[indice] = heap[menorIndice];
        heap[menorIndice] = temp;
            
        descerHeap(menorIndice);
    }
}

// Encontra o índice de um evento específico
int Escalonador::encontrarIndice(Evento* evento) {
    for (int i = 0; i < tamanho; i++) {
        if (heap[i] == evento) {
            return i;
        }
    }
    return -1;
}

void Escalonador::redimensionar() {
    int novaCapacidade = capacidade * 2;
    Evento** novoHeap = new Evento*[novaCapacidade];
        
    for (int i = 0; i < novaCapacidade; i++) {
        novoHeap[i] = (i < capacidade) ? heap[i] : nullptr;
    }
    delete[] heap;
    heap = novoHeap;
    capacidade = novaCapacidade;
}