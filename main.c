#include <pic18f4520.h>
#include "config.h"
#include "atraso.h"
#include "lcd.h"
#include "bits.h"
#include "lcd.h"
#include "keypad.h"
#include "lcd_max.h"
#include "io.h"
#include "funcoes.h"

#define cooler PORTCbits.RC2

void main(void) {
    unsigned int tecla = 16, tamanho = 0;
    char valor, total[6] = {}, senha[4] = {};
    unsigned char i, opcaoDePagamento = 0, coluna, confirma = 0, parteAtual = 1;
    unsigned char logo[32] = {
        0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F,
        0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x1E, 0x07, 0x03, 0x03,
        0x03, 0x03, 0x07, 0x1E, 0x1C, 0x00, 0x00, 0x00
    };

    lcdInit();
    kpInit();
    lcdCommand(0x40);

    TRISC = 0x01;

    for (i = 0; i < 32; i++) {
        lcdData(logo[i]);
    }
    lcdCommand(0x80);
    lcdData(0);
    lcdData(2);
    lcdCommand(0xC0);
    lcdData(1);
    lcdData(3);

    lcdPosition(0, 3);
    lcd_str("Stone");
    lcdPosition(1, 3);
    lcd_str("Bem-vindo!");
    lcdPosition(3, 0);
    lcd_str("Pressione (*)");

    while (1) {
        contaPartes(parteAtual);
        kpDebounce();
        if (tecla != kpRead()) {
            tecla = kpRead();
            if (bitTst(tecla, 0)) {
                lcd_cmd(L_CLR);
                break;
            }
        }
    }
    lcd_cmd(L_L1);
    lcd_str("Selecione abaixo");
    lcd_cmd(0x91);
    lcd_str("1) Credito");
    lcd_cmd(0xD1);
    lcd_str("2) Debito");
    parteAtual++;

    while (1) {
        contaPartes(parteAtual);
        kpDebounce();

        if (tecla != kpRead()) {
            tecla = kpRead();

            if (bitTst(tecla, 3)) {
                opcaoDePagamento = 1;
                lcd_cmd(L_CLR);
                break;
            }

            if (bitTst(tecla, 7)) {
                opcaoDePagamento = 2;
                lcd_cmd(L_CLR);
                break;
            }

        }
    }

    atraso_ms(2);

    lcd_cmd(L_L1);
    lcd_str("Digite o valor");
    lcd_cmd(L_L2);
    lcd_str("da compra:");
    lcd_cmd(0x90);
    lcd_str("R$");
    lcd_cmd(0xD0);
    lcd_str("Confirma (*)");


    coluna = 0x92;
    parteAtual++;

    while (confirma == 0) {
        contaPartes(parteAtual);
        kpDebounce();

        if (kpRead() != tecla) {
            tecla = kpRead();

            if (bitTst(tecla, 0))
                confirma = 1;

            valor = retornaTeclaPressionada(tecla);
            if (valor != 'e') {
                lcd_cmd(coluna);
                coluna++;
                lcd_dat(valor + 48);
                total[tamanho] = valor + 48;
                tamanho++;
            }
        }
    }

    lcd_cmd(L_CLR);

    lcd_cmd(0x80);
    lcd_str("Valor: R$");
    lcd_str(total);
    lcd_cmd(0x90);
    lcd_str("Confirma (*)");
    lcd_cmd(0xD0);
    lcd_str("Cancela (#)");
    parteAtual++;

    while (1) {
        contaPartes(parteAtual);
        kpDebounce();

        if (kpRead() != tecla) {
            tecla = kpRead();

            if (bitTst(tecla, 8)) {
                lcd_cmd(L_CLR);
                lcd_cmd(0x85);
                lcd_str("COMPRA");
                lcd_cmd(0xC3);
                lcd_str("CANCELADA!");
                atraso_ms(4000);
                return;
            }

            if (bitTst(tecla, 0))
                break;
        }
    }

    unsigned char cont = 0, texto = 1;
    parteAtual++;

    while (1) {
        contaPartes(parteAtual);
        kpDebounce();

        if (texto == 1) {
            lcd_cmd(L_CLR);
            lcd_cmd(0x80);
            lcd_str("Digite a senha:");
            lcd_cmd(0x90);
            lcd_str("Confirmar (*)");
            lcd_cmd(0xD0);
            lcd_str("Apagar (#)");
            coluna = 0xC0;
            texto = 0;
        }

        if (kpRead() != tecla) {
            tecla = kpRead();

            if (bitTst(tecla, 8)) {
                cont--;
                coluna--;
                senha[cont] = 0;
                lcd_cmd(coluna);
                lcd_str(" ");
            }

            valor = retornaTeclaPressionada(tecla);

            if (valor != 'e' && cont < 4) {
                lcd_cmd(coluna);
                coluna++;
                lcd_str("*");
                senha[cont] = valor + 48;
                cont++;
            }

            if (bitTst(tecla, 0)) {
                if (autorizacaoSenha(senha)) {
                    desligarDisplays();
                    lcd_cmd(L_CLR);
                    lcd_cmd(0x82);
                    lcd_str("Processando");
                    lcd_cmd(0xC6);
                    atraso_ms(800);
                    lcd_str(".");
                    atraso_ms(800);
                    lcd_str(".");
                    atraso_ms(800);
                    lcd_str(".");
                    configuraLed();

                    for (i = 0; i < 8; i++) {
                        ligarLed(i);
                        atraso_ms(500);
                    }

                    if (temSaldoParaRealizarACompra(total, opcaoDePagamento)) {
                        lcd_cmd(L_CLR);
                        lcd_cmd(L_L1);
                        lcd_str("Compra realizada");
                        lcd_cmd(L_L2);
                        lcd_str("com sucesso!");
                        atraso_ms(4000);
                        break;
                    } else {
                        lcd_cmd(L_CLR);
                        lcd_cmd(L_L1);
                        if(opcaoDePagamento == 1)
                            lcd_str("LIMITE");
                        if(opcaoDePagamento == 2)
                            lcd_str("SALDO");                        
                        lcd_cmd(L_L2);
                        lcd_str("INSUFICIENTE");
                        atraso_ms(5000);
                        return;
                    }
                }
                else {
                    for (i = 0; i < 4; i++)
                        senha[i] = 0;
                    cont = 0;
                    lcd_cmd(L_CLR);
                    lcd_cmd(0x80);
                    lcd_str("SENHA INCORRETA!");
                    atraso_ms(5000);
                    lcd_cmd(L_CLR);
                    texto = 1;
                }
            }

        }
    }

    imprimeVia(total, opcaoDePagamento);
    
    lcd_cmd(L_L1);
    lcd_str("Imprimir via");
    lcd_cmd(L_L2);
    lcd_str("do cliente?");
    lcd_cmd(0x91);
    lcd_str("1)Sim");
    lcd_cmd(0xD1);
    lcd_str("2)Nao");

    while (1) {
        kpDebounce();

        if (kpRead() != tecla) {
            tecla = kpRead();

            if (bitTst(tecla, 3)) {
                imprimeVia(total, opcaoDePagamento);
                conclude();
                return;
            }

            if (bitTst(tecla, 7)) {
                conclude();
                return;
            }
        }
    }
}