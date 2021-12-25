/*
 * BOMBASTIK.cpp
 * Data Transporter
 *
 *  Created on: Dec 18, 2021
 *
 * Author: Oguzhan "Oz" KANDAKOGLU
 */

// TODO: Add Double Speed Operation
// TODO: I used Timeout between each request.

/*
 * Packaging:
 * ---: Transmission Control characters:
 *     - Y: Start Transmission
 *     - Q: reQuest for requesting note.
 *     - W: ansWer  for sending requested note.
 *     - X: End Transmission
 *
 * ---: Package control characters:
 *     - #: Data Separator
 */

#include <Arduino.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <musico.h> // for the buzzer

#define F_CPU 16000000UL // 16 MHz Already Defined at .vscode/c_cpp_properties.json/configurations/defines/F_CPU
#define BAUD 9600        // Try to increase baud
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

// Transmission Codes
const char START_TRANSMISSION = 'Y';
const char REQUEST_NOTE = 'Q';
const char ANSWER_NOTE = 'W';
const char END_TRANSMISSION = 'X';
char *START = "Y", *REQUEST = "Q", *ANSWER = "W", *END = "X";

// Package Control Characters
const char DATA_SEPARATOR = '#';
const char PACKAGE_SEPARATOR = '\n';

// musico
// req
void request_note();                // returns the next note to be played and duration of the node
void parse_data();                  // returns true if there is data to be read
void parse_note();                  // Recv note from serial to note and duration
bool is_requested = false;          // true if a note is already requested
unsigned long request_timeout = 60; // timeout for the request
unsigned long request_time = 0;     // time of the last request
int max_request_count = 3;          // max request count
int request_count = 0;              // request count

// play
void tone_note();     // Play note via buzzer
void send_note();     //  Send note via serial
int note_pointer = 0; //  Pointer to the next note to be send
int buzzer_pin = 11;
unsigned long buzzer_timer = 0;
int note = 0, duration = 0;

const int DURATION_MULTIPLIER = 2;
unsigned long spacer = 0;

bool ready_to_tone; // New note is available to be tonned

// controls start or stop playing musico
bool is_playing;

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

  pinMode(buzzer_pin, OUTPUT); // set buzzer pin to output

  // TODO: SEND: START TRANSMISSION AS MASTER
}

bool done = false;
void loop()
{
  // BASIC CODE FLOW
  request_note();
  parse_data();
  tone_note();
  delay(5);
}

// musico
void tone_note()
{
  if (is_playing)
  {
    if (ready_to_tone)
    {
      tone(buzzer_pin, note, duration);
      spacer = duration * DURATION_MULTIPLIER;
      send_buffer((char *)spacer);
      delay(1000);
      ready_to_tone = false;
      buzzer_timer = millis();
    }
    else if (millis() - buzzer_timer >= spacer)
    {
      noTone(buzzer_pin);
      is_requested = false;
    }
  }
}

// req
void request_note()
{
  if (!is_requested && is_playing)
  {
    is_requested = true;
    send_buffer(REQUEST);
    request_time = millis();
  }
  // else if (is_requested && millis() - request_time >= request_timeout)
  // {
  //   if (request_count >= max_request_count)
  //   {
  //     is_playing = false;
  //     request_count = 0;
  //     tone(11, 523, 62);
  //     return;
  //   }
  //   is_requested = false;
  //   request_count++;
  // }
}

void parse_data()
{
  char command = read_from_rx_buffer();
  switch (command)
  {
  case REQUEST_NOTE:
    send_note();
    break;

  case ANSWER_NOTE:
    parse_note();
    break;

  case START_TRANSMISSION:
    is_playing = true;
    break;

  case END_TRANSMISSION:
    is_playing = false;
    break;

  default:
    break;
  }
}

void send_note()
{
  if (note_pointer < rows)
  {
    String note_to_send; // Example: W325#2@ => C4(325), 4
    note_to_send += ANSWER_NOTE;
    note_to_send += String(melody[note_pointer][0]);
    note_to_send += DATA_SEPARATOR;
    note_to_send += String(melody[note_pointer][1]);
    note_to_send += PACKAGE_SEPARATOR;

    // convert to char array
    char note_to_send_char[note_to_send.length()];
    note_to_send.toCharArray(note_to_send_char, note_to_send.length());

    // send

    send_buffer(note_to_send_char);
    note_pointer++;
  }
  else
  {
    send_buffer(END);
  }
}

void parse_note()
{
  char reader = read_from_rx_buffer();
  byte i = 0;
  String raw_tone;
  while (reader != PACKAGE_SEPARATOR)
  {
    if (reader == DATA_SEPARATOR)
    {
      if (i == 0)
        note = raw_tone.toInt();
      else
      {
        duration = 1000 / (1 << raw_tone.toInt());
        break;
      }
      i++;
      raw_tone = "";
    }
    else
      raw_tone += reader;
    // delay(500); // TODO: REMOVE
    reader = read_from_rx_buffer();
  }
  ready_to_tone = true;
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

/*
 DEAD LOCK TEMPLATE
  char temp = read_from_rx_buffer();
  // if ((temp != 'a' || temp != 'b') && !done)
  // {
  //   send_buffer("1");
  // }

  if (temp == '1')
  {
    digitalWrite(13, HIGH);
    send_buffer("a");
    done = true;
  }
  else if (temp == '0')
  {
    digitalWrite(13, LOW);
    send_buffer("b");
    done = true;
  }
  delay(100);
*/

/*
  lcd
  gloabal:
    #include <SPI.h>
    #include <Adafruit_PCD8544.h>
    Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
    void println(String str);

  setup:
    display.begin();
    display.clearDisplay();

    // text display tests
    display.setTextSize(1);
    display.setCursor(0, 0);

    display.println("Hello, world!");
    display.display();
    display.clearDisplay();

  bottom - global:
    // LCD
    byte line_counter = 0;
    void println(String str)
    {
      if (line_counter < 6)
      {
        display.println(str);
      }
      else
      {
        display.clearDisplay();
        line_counter = 0;
        display.setCursor(0, 0);
        display.println(str);
      }
      line_counter++;
      display.display();
    }
*/