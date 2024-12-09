#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 128  // Size of the buffer (adjust as needed)

typedef struct {
    uint8_t buffer[BUFFER_SIZE];  // The circular buffer storage
    volatile uint16_t head;       // Points to the next read position
    volatile uint16_t tail;       // Points to the next write position
} CircularBuffer;

// Function prototypes
void buffer_init(CircularBuffer *cb);
bool buffer_is_empty(CircularBuffer *cb);
bool buffer_is_full(CircularBuffer *cb);
void buffer_write(CircularBuffer *cb, uint8_t data);
uint8_t buffer_read(CircularBuffer *cb);


void buffer_init(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
}



bool buffer_is_empty(CircularBuffer *cb) {
    return cb->head == cb->tail;
}


bool buffer_is_full(CircularBuffer *cb) {
    return ((cb->tail + 1) % BUFFER_SIZE) == cb->head;
}


void buffer_write(CircularBuffer *cb, uint8_t data) {
    if (!buffer_is_full(cb)) {
        cb->buffer[cb->tail] = data;
        cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    } else {
        // Buffer is full, handle the overflow (optional)
        printf("Buffer overflow\n");
    }
}


uint8_t buffer_read(CircularBuffer *cb) {
    uint8_t data = 0;
    if (!buffer_is_empty(cb)) {
        data = cb->buffer[cb->head];
        cb->head = (cb->head + 1) % BUFFER_SIZE;
    } else {
        // Buffer is empty, handle underflow (optional)
        printf("Buffer underflow\n");
    }
    return data;
}


// Simulate UART receive (e.g., an interrupt or polling mechanism)
void uart_receive(CircularBuffer *rxBuffer) {
    uint8_t received_data = 0;  // Read from UART hardware register here
    // Simulate data reception
    received_data = 'A';  // For example, simulate receiving 'A'
    
    // Write the received data into the buffer
    buffer_write(rxBuffer, received_data);
}


// Simulate UART send (e.g., interrupt-driven)
void uart_send(CircularBuffer *txBuffer) {
    if (!buffer_is_empty(txBuffer)) {
        uint8_t data_to_send = buffer_read(txBuffer);
        // Send the byte using UART hardware register
        printf("Sending data: %c\n", data_to_send);  // Simulate sending data
    }
}



int main(void) {
    CircularBuffer rxBuffer, txBuffer;
    
    // Initialize the buffers
    buffer_init(&rxBuffer);
    buffer_init(&txBuffer);

    // Simulate receiving data over UART (this would be called by an interrupt in real systems)
    uart_receive(&rxBuffer);
    
    // Simulate transferring data from rxBuffer to txBuffer
    while (!buffer_is_empty(&rxBuffer)) {
        uint8_t received_byte = buffer_read(&rxBuffer);
        buffer_write(&txBuffer, received_byte);
    }

    // Simulate sending data over UART (this would be called by an interrupt or polling in real systems)
    uart_send(&txBuffer);

    return 0;
}
