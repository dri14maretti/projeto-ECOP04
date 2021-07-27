/* 
 * File:   funcoes.h
 * Author: drizi
 *
 * Created on 25 de Julho de 2021, 18:38
 */

#ifndef FUNCOES_H
#define	FUNCOES_H

void lcdPosition(unsigned char linha, unsigned char coluna);

void configuraLed(void);
void ligarLed(char num);
void desligarLed(char num); 

char retornaTeclaPressionada(int tecla); // retorna o numero da tecla que fora pressionada
void contaPartes(unsigned char parte);
void desligarDisplays(void);

unsigned char autorizacaoSenha(char senhaEscrita[4]); //verifica se a senha coincide
unsigned char temSaldoParaRealizarACompra(char *valorDaCompra, char opcaoDeCompra); //verifica se a compra pode ser realizada a partir do saldo do cliente

void imprimeVia(char *valorCompra, char opcaoCartao); //imprime a via do cartão para o cliente
void conclude(void); //conclui

#endif	/* FUNCOES_H */

