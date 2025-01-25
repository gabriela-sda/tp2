#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "Evento.h"

class Escalonador {
    public:
    Escalonador();
    Escalonador(const Escalonador& outro) ;
    ~Escalonador();
    Escalonador& operator=(const Escalonador& outro);

    void inserirEvento(Evento* evento);
    Evento* retirarProximoEvento();
    bool atualizarHorario(Evento* evento, double novoHorario);
    bool removerEvento(Evento* evento);
    bool temEventos() const;
    double getProximoHorario() const;
    int getTamanho() const;

    //private:
    static const int CAPACIDADE_INICIAL = 100;
    Evento** heap;
    int capacidade;
    int tamanho;

    // Índices para navegação na heap
    int pai(int i) { return (i - 1) / 2; }
    int filhoEsquerdo(int i) { return 2 * i + 1; }
    int filhoDireito(int i) { return 2 * i + 2; }
    void subirHeap(int indice);
    void descerHeap(int indice);
    int encontrarIndice(Evento* evento);
    void redimensionar(); // Redimensiona o array quando necessário

};

#endif