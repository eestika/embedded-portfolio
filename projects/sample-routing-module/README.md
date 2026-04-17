# Sample Routing Module

## English

### Project Overview

Sample Routing Module (SRM) is a modular embedded communication project designed to simulate a small industrial or medical machine architecture.

The current system is based on:

* Raspberry Pi 4 Model B as master node
* STM32 NUCLEO-F207ZG as slave node
* RS485 physical layer
* Custom SRM binary protocol
* CRC16 frame validation
* RX ring buffer handling
* Modular parser / dispatcher architecture
* LCD1602 I2C display integration
* LED control and status monitoring
* Menu-driven Raspberry test application

The goal of the project is to create a scalable embedded framework that can later support:

* Multiple slave nodes
* LCD modules
* EEPROM memory
* Stepper motors
* Servo motors
* I2C sensors
* CAN bus
* WiFi or Ethernet gateway
* Distributed actuator and sensor control

---

### Current Milestone

The current implementation supports:

* Bidirectional RS485 communication
* Binary SRM frames
* CRC16 validation
* RX ring buffer
* Frame parser
* Command dispatcher
* Request / response protocol
* Source and destination addressing
* Sequence numbers
* Raspberry text menu
* STM32 LED ON / LED OFF control
* STM32 STATUS response
* LCD1602 I2C driver
* LCD clear command
* LCD line 1 write command
* LCD line 2 write command
* READY / PONG machine-style LCD feedback concept

Currently implemented commands:

* PING_REQ / PING_RSP
* LED_ON_REQ / LED_ON_RSP
* LED_OFF_REQ / LED_OFF_RSP
* STATUS_REQ / STATUS_RSP
* LCD_CLEAR_REQ / LCD_CLEAR_RSP
* LCD_WRITE_LINE1_REQ / LCD_WRITE_LINE1_RSP
* LCD_WRITE_LINE2_REQ / LCD_WRITE_LINE2_RSP

STATUS_RSP currently returns:

* LED state
* System uptime in milliseconds

---

### Why RS485 Was Chosen

RS485 was selected because it is widely used in industrial automation, embedded systems, machine control, and distributed devices.

Compared to a direct UART connection, RS485 offers:

* Better noise immunity
* Longer cable distances
* Differential signaling
* Support for multiple devices on the same bus
* Higher reliability in industrial environments
* Better scalability for future multi-node systems

This makes RS485 a realistic choice for a project inspired by industrial and medical equipment, where multiple slave boards may need to communicate with a central controller.

Typical RS485 systems can support up to 32 nodes on the same bus and cable lengths up to approximately 1200 meters at lower speeds. Differential signaling also helps tolerate electrical noise and ground offsets. The Waveshare RS485 converter used in the project includes ESD protection, surge protection, isolation, and optional 120 ohm termination. :contentReference[oaicite:0]{index=0} :contentReference[oaicite:1]{index=1}

---

### Why a Custom Binary Protocol Was Chosen

A custom binary protocol was chosen instead of plain text commands because it is:

* Faster
* More compact
* Easier to validate
* More scalable
* More realistic for industrial applications
* Easier to extend with new commands and payloads

Text protocols are simple to debug but become inefficient when many commands, sensors, or slave nodes are added.

The SRM frame format is:

```text
SOF | VER | FLAGS | SRC | DST | CMD | SEQ | LEN | PAYLOAD | CRC16

```
Field description:

SOF = Start of Frame
VER = Protocol version
FLAGS = Frame flags
SRC = Source address
DST = Destination address
CMD = Command identifier
SEQ = Sequence number
LEN = Payload length
PAYLOAD = Command data
CRC16 = Frame validation

This structure allows:

Future multi-slave addressing
Command expansion
Frame validation
Error detection
Reliable request / response behavior
Example Communication Flow
Raspberry sends LCD_WRITE_LINE1_REQ
STM32 validates frame and CRC
STM32 dispatches the command
STM32 updates LCD line 1
STM32 sends LCD_WRITE_LINE1_RSP
Raspberry validates response and CRC

The same mechanism is also used for:

PING
LED control
STATUS reporting
LCD clear
LCD line 2 update
Current Hardware
Raspberry Pi 4 Model B master board
STM32 NUCLEO-F207ZG slave board
RS485 transceiver
USB to RS485 converter
LCD1602 I2C display
On-board STM32 user LED

The Raspberry Pi 4 was selected because it offers GPIO, Linux support, serial interfaces, and enough performance to act as a small supervisory master node.

The STM32F207ZG was selected because it provides multiple UARTs, I2C, CAN, timers, DMA, and sufficient GPIO resources for future expansion. It also supports future CAN integration directly in hardware.

Current Firmware Modules
Raspberry Side
main.c

Entry point of the Raspberry application.

menu_test.c

Implements the text-based command menu and test functions.

serial_port.c

Handles Linux serial communication with the RS485 adapter.

srm_frame_builder.c

Builds outgoing SRM binary frames.

srm_crc.c

Calculates and validates CRC16 values.

srm_defs.h

Contains command IDs, protocol constants, frame sizes, and shared definitions.

config.h

Contains serial port and configuration parameters.

STM32 Side
main.c

Minimal STM32 startup file generated by CubeMX / CubeIDE.

app_main.c

Contains the main application logic.

rs485_if.c

Handles low-level RS485 communication.

uart_if.c

Provides UART abstraction for the application.

srm_rx_buffer.c

Implements the RX ring buffer.

srm_rx_parser.c

Parses incoming SRM frames.

srm_dispatcher.c

Routes validated commands to the proper module.

srm_crc.c

Calculates and validates CRC16 values.

srm_defs.h

Contains shared protocol definitions and command IDs.

lcd.c

Handles LCD1602 I2C display operations.

debug_console.c

Provides debug output through Virtual COM / USART3.

Planned Next Steps
Custom LCD text entry
LCD scrolling text
LCD custom symbols
EEPROM integration
Stepper motor integration
Servo motor integration
Multi-slave addressing
CAN bus support
WiFi gateway support
Ethernet gateway support
Integration with rover and future Nordic Node architecture
Italiano
Panoramica Progetto

Sample Routing Module (SRM) è un progetto embedded modulare progettato per simulare l'architettura di una piccola macchina industriale o medicale.

Il sistema attuale è basato su:

Raspberry Pi 4 Model B come nodo master
STM32 NUCLEO-F207ZG come nodo slave
Livello fisico RS485
Protocollo binario SRM personalizzato
Validazione frame con CRC16
Gestione RX tramite ring buffer
Architettura modulare parser / dispatcher
Integrazione display LCD1602 I2C
Controllo LED e monitoraggio stato
Applicazione Raspberry con menu testuale

L'obiettivo del progetto è creare un framework embedded scalabile che in futuro possa supportare:

Più nodi slave
Moduli LCD
Memorie EEPROM
Stepper motor
Servo motor
Sensori I2C
CAN bus
Gateway WiFi o Ethernet
Controllo distribuito di sensori e attuatori

Milestone Attuale

L'implementazione attuale supporta:

Comunicazione bidirezionale RS485
Frame binari SRM
Validazione CRC16
Ring buffer RX
Parser dei frame
Dispatcher dei comandi
Protocollo request / response
Indirizzamento source / destination
Sequence number
Menu testuale lato Raspberry
Controllo LED ON / LED OFF lato STM32
Risposta STATUS lato STM32
Driver LCD1602 I2C
Comando clear LCD
Scrittura linea 1 LCD
Scrittura linea 2 LCD
Concetto READY / PONG in stile macchina industriale

Comandi attualmente implementati:

PING_REQ / PING_RSP
LED_ON_REQ / LED_ON_RSP
LED_OFF_REQ / LED_OFF_RSP
STATUS_REQ / STATUS_RSP
LCD_CLEAR_REQ / LCD_CLEAR_RSP
LCD_WRITE_LINE1_REQ / LCD_WRITE_LINE1_RSP
LCD_WRITE_LINE2_REQ / LCD_WRITE_LINE2_RSP

STATUS_RSP restituisce attualmente:

Stato LED
Uptime del sistema in millisecondi


Perché è Stato Scelto RS485

RS485 è stato scelto perché è molto utilizzato nell'automazione industriale, nei sistemi embedded, nel controllo macchine e nei dispositivi distribuiti.

Rispetto ad un collegamento UART diretto, RS485 offre:

Maggiore immunità al rumore
Distanze di collegamento superiori
Segnalazione differenziale
Supporto per più dispositivi sullo stesso bus
Maggiore affidabilità in ambienti industriali
Migliore scalabilità per sistemi multi-nodo futuri

Questo rende RS485 una scelta realistica per un progetto ispirato a macchine industriali e medicali.

Perché è Stato Scelto un Protocollo Binario Personalizzato

È stato scelto un protocollo binario personalizzato invece di semplici comandi testuali perché è:

Più veloce
Più compatto
Più facile da validare
Più scalabile
Più realistico per applicazioni industriali
Più semplice da estendere con nuovi comandi

Formato frame SRM:

SOF | VER | FLAGS | SRC | DST | CMD | SEQ | LEN | PAYLOAD | CRC16

---

Moduli Firmware Attuali
Lato Raspberry
main.c -> entry point dell'applicazione
menu_test.c -> menu testuale e funzioni di test
serial_port.c -> comunicazione seriale Linux
srm_frame_builder.c -> costruzione frame SRM
srm_crc.c -> gestione CRC16
srm_defs.h -> definizioni protocollo
config.h -> parametri seriale
Lato STM32
main.c -> startup minimale CubeMX
app_main.c -> logica applicativa
rs485_if.c -> comunicazione RS485
uart_if.c -> astrazione UART
srm_rx_buffer.c -> ring buffer RX
srm_rx_parser.c -> parser frame
srm_dispatcher.c -> dispatcher comandi
srm_crc.c -> CRC16
srm_defs.h -> definizioni condivise
lcd.c -> driver LCD1602
debug_console.c -> console debug USART3
Prossimi Step Previsti
Inserimento testo LCD personalizzato
Testo LCD scorrevole
Simboli personalizzati LCD
Integrazione EEPROM
Integrazione stepper motor
Integrazione servo motor
Indirizzamento multi-slave
Supporto CAN bus
Supporto gateway WiFi
Supporto gateway Ethernet
Integrazione con rover e futura architettura Nordic Node
Eesti
Projekti Ülevaade

Sample Routing Module (SRM) on modulaarne embedded-projekt, mis simuleerib väikese tööstusliku või meditsiinilise masina arhitektuuri.

Raspberry Pi 4 Model B master-seade
STM32 NUCLEO-F207ZG slave-seade
RS485 füüsiline kiht
Kohandatud binaarne SRM protokoll
CRC16 kontroll
RX ring buffer
Parser / dispatcher arhitektuur
LCD1602 I2C integratsioon
Praegune Milestone
Kahepoolne RS485 side
SRM binaarkaadrid
CRC16 kontroll
Parser ja dispatcher
LED juhtimine
STATUS vastus
LCD clear
LCD line 1 kirjutamine
LCD line 2 kirjutamine
READY / PONG masina stiilis tagasiside
Miks RS485 Valiti
Parem mürakindlus
Pikem sidekaugus
Diferentsiaalne signaal
Sobib mitme seadme jaoks
Tööstuslikult realistlik lahendus
Järgmised Sammud
EEPROM
Stepper motor
Servo motor
Multi-slave aadressimine
CAN bus
WiFi gateway
Ethernet gateway
Русский
Обзор Проекта

Sample Routing Module (SRM) — это модульный embedded-проект, имитирующий архитектуру небольшой промышленной или медицинской машины.

Raspberry Pi 4 Model B как master-узел
STM32 NUCLEO-F207ZG как slave-узел
Физический уровень RS485
Пользовательский бинарный протокол SRM
CRC16 проверка
RX ring buffer
Архитектура parser / dispatcher
LCD1602 I2C интеграция
Текущая Milestone
Двунаправленная связь RS485
Бинарные SRM кадры
CRC16 проверка
Parser и dispatcher
Управление LED
STATUS ответ
LCD clear
Запись LCD line 1
Запись LCD line 2
READY / PONG логика для дисплея
Почему Был Выбран RS485
Более высокая помехоустойчивость
Большая длина кабеля
Дифференциальный сигнал
Поддержка нескольких устройств
Реалистичный промышленный подход
Следующие Шаги
EEPROM
Stepper motor
Servo motor
Multi-slave адресация
CAN bus
WiFi gateway
Ethernet gateway