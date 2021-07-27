#include "funcoes.h"
#include "bits.h"
#include "lcd_max.h"
#include "lcd.h"
#include "ssd.h"
#include "atraso.h"
#include "bits.h"
#include "io.h"
#include <stdlib.h>
#include <pic18f4520.h>

void lcdPosition(unsigned char linha, unsigned char coluna) {
    if (linha == 0) {
        lcdCommand(0x80 + (coluna % 16)); // primeira linha, coluna 0 + numero da coluna
    }
    if (linha == 1) {
        lcdCommand(0xC0 + (coluna % 16)); // segunda linha, coluna 0 + numero da coluna
    }
    if (linha == 2) {
        lcdCommand(0x90 + (coluna % 16));
    }
    if (linha == 3) {
        lcdCommand(0xD0 + (coluna % 16));
    }
}

void configuraLed(void) {
    TRISD = 0x00;
    PORTD = 0x00;
}

void ligarLed(char num) {
    unsigned char aux = PORTD;
    if (num == 0) {
        PORTD = aux | 0x01;
    }
    if (num == 1) {
        PORTD = aux | 0x02;
    }
    if (num == 2) {
        PORTD = aux | 0x04;
    }
    if (num == 3) {
        PORTD = aux | 0x08;
    }
    if (num == 4) {
        PORTD = aux | 0x10;
    }
    if (num == 5) {
        PORTD = aux | 0x20;
    }
    if (num == 6) {
        PORTD = aux | 0x40;
    }
    if (num == 7) {
        PORTD = aux | 0x80;
    }
}

void desligarLed(char num) {
    unsigned char aux = PORTD;
    if (num == 0) {
        PORTD = aux ^ 0x01;
    }
    if (num == 1) {
        PORTD = aux ^ 0x02;
    }
    if (num == 2) {
        PORTD = aux ^ 0x04;
    }
    if (num == 3) {
        PORTD = aux ^ 0x08;
    }
    if (num == 4) {
        PORTD = aux ^ 0x10;
    }
    if (num == 5) {
        PORTD = aux ^ 0x20;
    }
    if (num == 6) {
        PORTD = aux ^ 0x40;
    }
    if (num == 7) {
        PORTD = aux ^ 0x80;
    }
}

char retornaTeclaPressionada(int tecla) {
    if (bitTst(tecla, 4))
        return 0;
    if (bitTst(tecla, 3))
        return 1;
    if (bitTst(tecla, 7))
        return 2;
    if (bitTst(tecla, 11))
        return 3;
    if (bitTst(tecla, 2))
        return 4;
    if (bitTst(tecla, 6))
        return 5;
    if (bitTst(tecla, 10))
        return 6;
    if (bitTst(tecla, 1))
        return 7;
    if (bitTst(tecla, 5))
        return 8;
    if (bitTst(tecla, 9))
        return 9;

    return 'e';
}

void contaPartes(unsigned char parte) {
    int tempo;
    
    ssdInit();
    
    ssdDigit(parte, 0);
    ssdDigit(0x0D, 1);
    ssdDigit(0x0E, 2);
    ssdDigit(0x05, 3);
    ssdUpdate();
    
    for(tempo = 0; tempo < 1000; tempo++);
}

void desligarDisplays(void) {
    digitalWrite(PIN_A2, LOW);
    digitalWrite(PIN_A3, LOW);
    digitalWrite(PIN_A4, LOW);
    digitalWrite(PIN_A5, LOW);
}

unsigned char autorizacaoSenha(char senhaPassada[4]) {
    unsigned char i;
    char senhaCorreta[4] = {'1', '4', '2', '3'};

    for (i = 0; i < 4; i++) {
        if (senhaPassada[i] != senhaCorreta[i])
            return 0;
    }

    return 1;
}

unsigned char temSaldoParaRealizarACompra(char *valorDaCompra, char opcaoDeCompra) {
    int valor;
    unsigned int limite = 1600, saldo = 150;

    valor = atoi(valorDaCompra);

    switch (opcaoDeCompra) {
        case 1:
            if (valor < limite)
                return 1;
            break;

        case 2:
            if (valor < saldo)
                return 1;
            break;
    }

    return 0;
}

void imprimeVia(char *valorCompra, char opcaoCartao) {
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Imprimindo");
    atraso_ms(400);
    lcd_str(".");
    atraso_ms(400);
    lcd_str(".");
    atraso_ms(400);
    lcd_str(".");
    atraso_ms(400);
    bitSet(PORTC, 2); //liga o cooler
    atraso_ms(5000);
    bitClr(PORTC, 2); // desliga o cooler
    
    lcd_cmd(L_CLR);
    lcd_cmd(0x85);
    lcd_str("COMPRA");
    lcd_cmd(0xC0);
    lcd_str("----------------");
    lcd_cmd(0x91);
    lcd_str("Valor: R$");
    lcd_str(valorCompra);
    lcd_cmd(0xD1);
    lcd_str("Opcao: ");
    switch(opcaoCartao) {
        case 1:
            lcd_str("Credito");
            break;
        case 2:
            lcd_str("Debito");
            break;
    }
    atraso_ms(6000);
    lcd_cmd(L_CLR);
}

void conclude(void) {
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Concluido!");
    lcd_cmd(L_L2);
    atraso_ms(5000);
}