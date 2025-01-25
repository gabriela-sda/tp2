#ifndef PROCEDIMENTO_H
#define PROCEDIMENTO_H

#include <iostream>


enum TipoProcedimento {
    INDEFINIDO,
    TRIAGEM,
    ATENDIMENTO,
    MEDIDAS_HOSPITALARES,
    TESTES, 
    EXAMES,
    MEDICACAO
};

class Unidade {
    public:
    Unidade(): estaOcupada(false), tempoOcupada(0), tempoLivre(0), inicioUltimoProcedimento(0), fimUltimoProcedimento(0) {}
    void ocuparUnidade(double horarioAtual) {estaOcupada = true; inicioUltimoProcedimento = horarioAtual;}
    void liberarUnidade(double horarioAtual) {estaOcupada = false; fimUltimoProcedimento = horarioAtual;
    tempoOcupada += fimUltimoProcedimento - inicioUltimoProcedimento;}
    void registrarUtilizacao(double horarioFinal) {utilizacao = tempoOcupada/horarioFinal;}
    //private:
    bool estaOcupada;
    double tempoOcupada;
    double tempoLivre;
    double utilizacao;
    double inicioUltimoProcedimento;
    double fimUltimoProcedimento;

};

class Procedimento {
    public:
    Procedimento();
    Procedimento(TipoProcedimento Tipo, float Duracao, int NumUnidades);
    Procedimento(const Procedimento& other);
    Procedimento& operator=(const Procedimento& other);
    Procedimento(Procedimento&& other);
    Procedimento& operator=(Procedimento&& other);
    ~Procedimento();
    void limparUnidades();
    void getUtilizacao();
    //getTipoProcedimento()
    //getLatencia()
    //getNumUnidades()

    //private:
    TipoProcedimento tipo;
    float latencia;
    int numUnidades;
    Unidade** unidades;
    double utilizacaoMedia;
 
};


#endif