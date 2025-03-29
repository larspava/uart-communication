#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// para várias placas é possível criar um byte de identificação para cada
#define MY_BOARD 0x01
#define BOARD_2 0x02
#define BOARD_3 0x03
#define BOARD_4 0x04

// tamanho do buffer de recepção
#define BUFFER_SIZE 256

// Polinômio CRC-16-CCITT. Referência: https://srecord.sourceforge.net/crc16-ccitt.html
#define CRC16_POLYNOMIAL 0x1021
#define CRC16_INITIAL_VALUE 0xFFFF  // Valor inicial do CRC

uint8_t rxBuffer[BUFFER_SIZE]; // buffer de recepção
uint8_t rxIndex = 0; // índice do buffer
bool messageCompleted = false; // booleano de mensagem completa
bool receivingMessage = false; // flag para indicar se a mensagem está sendo recebida


/**
 * Calcula o CRC-16 de um conjunto de dados.
 * @param data Ponteiro para o array de bytes da mensagem.
 * @param length Número de bytes na mensagem.
 * @return O valor do CRC-16 calculado.
 */
uint16_t calculate_crc16(const uint8_t *data, size_t length) {
    uint16_t crc = CRC16_INITIAL_VALUE;
    
    for (size_t i = 0; i < length; i++) {
        crc ^= (data[i] << 8);  // Adiciona o byte ao CRC
        
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ CRC16_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

void send_message(uint8_t *msg, size_t msg_size) {
    // CRC da mensagem
    uint16_t msg_crc = calculate_crc16(msg, sizeof(msg));
    
    // Tamanho total da mensagem a ser enviada
    size_t whole_size = 1 + sizeof(msg_crc) + sizeof(uint8_t) + msg_size; // 1 byte para MY_BOARD, CRC, tamanho e mensagem

    uint8_t data[whole_size];

    // Índice para preencher o array de dados
    size_t index = 0;

    // Adiciona o byte de MY_BOARD (identificador da placa)
    data[index++] = MY_BOARD;  // 1 byte: identificador da placa
    
    // Adiciona o CRC (2 bytes)
    data[index++] = (msg_crc >> 8) & 0xFF; // Byte alto do CRC
    data[index++] = msg_crc & 0xFF;        // Byte baixo do CRC
    
    // Adiciona o tamanho da mensagem (1 byte)
    data[index++] = (uint8_t)msg_size;
    
    // Adiciona a própria mensagem
    for (size_t i = 0; i < msg_size; i++) {
        data[index++] = msg[i];
    }

    send_to_uart(data, whole_size); // Abstração comunicação com placa.
}

// Função que simula o recebimento dos dados via UART (Abstração de dados recebidos via UART)
void UART_handler(uint8_t receivedByte) {

    if (receivedByte == 0x01) {  // Início da mensagem (EXEMPLO: 0x01 pode ser o byte de início)
        receivingMessage = true;  // Inicia a recepção da mensagem
        rxIndex = 0;             // Reinicia o índice do buffer
    }

    if (receivingMessage) {
        if (rxIndex < BUFFER_SIZE - 1) { // Verifica se ainda há espaço no buffer
            rxBuffer[rxIndex++] = receivedByte;

            if (rxIndex >= BUFFER_SIZE - 1) {  // Buffer cheio, não recebe mais dados
                receivingMessage = false;
                messageCompleted = true;  // Marca a mensagem como completa
            }
        }
    }
}

// Função para analisar a mensagem recebida
bool parseMessage(uint8_t *message, int16_t *payload) {
    // Exemplo de estrutura de mensagem esperada: [ID] [CRC] [Tamanho] [Mensagem]
    size_t msg_size = message[3];  // O 4º byte contém o tamanho da mensagem
    uint16_t received_crc = (message[1] << 8) | message[2];
    
    uint16_t calculated_crc = calculate_crc16(&message[4], msg_size); // Calcular CRC da mensagem

    if (received_crc == calculated_crc) { // Verificar o CRC da mensagem recebida
        printf("CRC Verificado com Sucesso!\n");
        
        // A partir do byte 4, temos a mensagem em si
        *payload = 0;  // Exemplo de conversão para payload, dependendo do tipo de dados da mensagem
        
        // Exemplo simples: converte os primeiros 2 bytes para payload (como inteiro)
        if (msg_size >= 2) {
            *payload = (message[4] << 8) | message[5];
        }
        return true;
    } else {
        printf("Erro de CRC! Mensagem Corrompida.\n");
        return false;
    }
}

int main() {

    // Exemplo de envio de mensagem para placa 3
    uint8_t message[] = { 0x43, 0x61, 0x6C, 0x6C, 0x69, 0x6E, 0x67, 0x20, 
        0x66, 0x72, 0x6F, 0x6D, 0x20, 0x43, 0x75, 0x72, 
        0x69, 0x74, 0x69, 0x62, 0x61, 0x00 }; // Inclui terminador nulo

    send_message(message, size(message));

    // Recebimento de mensagens
    while (1) {
        if (messageCompleted) {
            // Se a mensagem foi recebida, tenta parseá-la
            int16_t payload;
            if (parseMessage(rxBuffer, &payload)) {
                printf("Payload: %d\n", payload);
            } else {
                printf("Erro ao ler a mensagem!\n");
            }
            messageCompleted = false; // Reseta a flag para receber uma nova mensagem
        }
        delay(100);
    }

    return 0;
}
