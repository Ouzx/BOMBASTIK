/*
 * BOMBASTIK.cpp
 * Data Transporter
 *
 *  Created on: Dec 18, 2021
 *
 * Author: Oguzhan "Oz" KANDAKOGLU
 */

// TODO: Add Double Speed Operation

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define BRC ((F_CPU / 16 / BAUD) - 1)

#define TX_BUFFER_SIZE 128
char tx_buffer[TX_BUFFER_SIZE];
uint8_t tx_buffer_read_pos = 0;  // Index of the next byte to be sent
uint8_t tx_buffer_write_pos = 0; // Index of the next free space in the buffer

void append_to_tx_buffer(char c);
void send_buffer(char[]);

#define RX_BUFFER_SIZE 128
char rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_buffer_read_pos = 0;  // Index of the next byte to be read
uint8_t rx_buffer_write_pos = 0; // Index of the next free space in the buffer

char read_from_rx_buffer();
char peek_from_rx_buffer();

// void uart_init(void);

void setup()
{
  // initialize serial communication at 9600 bps:
  UBRR0H = (BRC >> 8);
  UBRR0L = BRC;

  // Enable transmitter
  // Enable Reciever
  UCSR0B = (1 << TXEN0) | (1 << TXCIE0) | (1 << RXEN0) | (1 << RXCIE0);

  // UCSRnC => Mode 0: Asynchronous USART default

  // TODO: ADD PARITY MODE

  // USBSn => 1 Stop bit default

  // Characther Size 8 MOD
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  pinMode(13, OUTPUT);

  sei(); // turn on external interrupts
}
bool done = false;
void loop()
{
  // char *cx = "Hello World!\n\r";
  // send_buffer(cx);
  // _delay_ms(1000);

  char temp = read_from_rx_buffer();
  if ((temp != 'a' || temp != 'b') && !done)
  {
    send_buffer("1");
  }

  if (temp == 'b')
  {
    digitalWrite(13, HIGH);
    send_buffer("1");
    done = true;
  }
  else if (temp == 'a')
  {
    digitalWrite(13, LOW);
    send_buffer("0");
    done = true;
  }
  delay(100);
}

// TX
void append_to_tx_buffer(char c)
{
  tx_buffer[tx_buffer_write_pos] = c;
  tx_buffer_write_pos++;

  if (tx_buffer_write_pos >= TX_BUFFER_SIZE)
    tx_buffer_write_pos = 0;
}

void send_buffer(char c[])
{
  for (uint8_t i = 0; i < strlen(c); i++)
    append_to_tx_buffer(c[i]);

  if (UCSR0A & (1 << UDRE0))

    UDR0 = 0;
}

ISR(USART_TX_vect)
{
  if (tx_buffer_read_pos != tx_buffer_write_pos)
  {
    UDR0 = tx_buffer[tx_buffer_read_pos];
    tx_buffer_read_pos++;

    if (tx_buffer_read_pos >= TX_BUFFER_SIZE)
      tx_buffer_read_pos = 0;
  }
}

// RX

char peek_from_rx_buffer()
{
  char c = '\0';

  if (rx_buffer_read_pos != rx_buffer_write_pos)
    c = rx_buffer[rx_buffer_read_pos];

  return c;
}

char read_from_rx_buffer()
{
  char c = '\0';

  if (rx_buffer_read_pos != rx_buffer_write_pos)
  {
    c = rx_buffer[rx_buffer_read_pos];
    rx_buffer_read_pos++;

    if (rx_buffer_read_pos >= RX_BUFFER_SIZE)
      rx_buffer_read_pos = 0;
  }
  return c;
}

ISR(USART_RX_vect)
{
  rx_buffer[rx_buffer_write_pos] = UDR0;
  rx_buffer_write_pos++;

  if (rx_buffer_write_pos >= RX_BUFFER_SIZE)
    rx_buffer_write_pos = 0;
}