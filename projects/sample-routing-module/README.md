# Sample Routing Module

## English

### Project Overview

Sample Routing Module is a modular embedded communication project based on:

* Raspberry Pi 4 Model B acting as master node
* STM32 NUCLEO-F207 acting as slave node
* RS485 physical layer
* Custom SRM binary protocol
* CRC16 frame validation
* RX ring buffer handling
* Modular parser and dispatcher architecture
* LCD1602 I2C integration

The goal of the project is to build a scalable embedded communication framework that can later support:

* Multiple slave nodes
* LCD modules
* EEPROM storage
* I2C sensors
* CAN bus
* WiFi or Ethernet expansion
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
* Request/response protocol
* Source and destination addressing
* Sequence numbers
* Raspberry command menu
* STM32 LED ON / LED OFF control
* STM32 STATUS response
* LCD1602 I2C driver
* LCD clear command
* LCD line 1 write command
* LCD line 2 write command

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

### Example Communication Flow

1. Raspberry sends LCD_WRITE_LINE1_REQ
2. STM32 validates frame and CRC
3. STM32 dispatches the command
4. STM32 updates LCD line 1
5. STM32 sends LCD_WRITE_LINE1_RSP
6. Raspberry validates response and CRC

The same mechanism is also used for:

* PING
* LED control
* STATUS reporting
* LCD clear
* LCD line 2 update

---

### Current Firmware Modules

#### Raspberry Side

* srm_protocol
* srm_crc16
* srm_menu
* srm_test
* rs485_if

#### STM32 Side

* srm_parser
* srm_dispatcher
* rs485_if
* lcd
* app_main

---

### Planned Next Steps

* Custom LCD text entry
* LCD scrolling text
* Custom LCD symbols
* EEPROM integration
* Multi-slave addressing
* CAN bus support
* WiFi gateway support
* Integration with rover and future Nordic Node architecture

---

## Italiano

### Panoramica Progetto

Sample Routing Module è un progetto embedded modulare basato su:

* Raspberry Pi 4 Model B come nodo master
* STM32 NUCLEO-F207 come nodo slave
* Livello fisico RS485
* Protocollo binario SRM personalizzato
* Validazione frame con CRC16
* Gestione RX tramite ring buffer
* Architettura modulare con parser e dispatcher
* Integrazione LCD1602 I2C

L'obiettivo del progetto è costruire un framework embedded scalabile che in futuro possa supportare:

* Più nodi slave
* Moduli LCD
* Memorie EEPROM
* Sensori I2C
* CAN bus
* Espansione WiFi o Ethernet
* Controllo distribuito di sensori e attuatori

---

### Milestone Attuale

L'implementazione attuale supporta:

* Comunicazione bidirezionale RS485
* Frame binari SRM
* Validazione CRC16
* Ring buffer RX
* Parser dei frame
* Dispatcher dei comandi
* Protocollo request/response
* Indirizzamento source/destination
* Sequence number
* Menu comandi lato Raspberry
* Controllo LED ON / LED OFF lato STM32
* Risposta STATUS lato STM32
* Driver LCD1602 I2C
* Comando clear LCD
* Scrittura linea 1 LCD
* Scrittura linea 2 LCD

Comandi attualmente implementati:

* PING_REQ / PING_RSP
* LED_ON_REQ / LED_ON_RSP
* LED_OFF_REQ / LED_OFF_RSP
* STATUS_REQ / STATUS_RSP
* LCD_CLEAR_REQ / LCD_CLEAR_RSP
* LCD_WRITE_LINE1_REQ / LCD_WRITE_LINE1_RSP
* LCD_WRITE_LINE2_REQ / LCD_WRITE_LINE2_RSP

STATUS_RSP restituisce attualmente:

* Stato LED
* Uptime del sistema in millisecondi

---

### Esempio di Flusso Comunicazione

1. Raspberry invia LCD_WRITE_LINE1_REQ
2. STM32 valida frame e CRC
3. STM32 esegue il dispatch del comando
4. STM32 aggiorna la linea 1 del display
5. STM32 invia LCD_WRITE_LINE1_RSP
6. Raspberry valida la risposta e il CRC

Lo stesso meccanismo viene utilizzato anche per:

* PING
* Controllo LED
* Report STATUS
* Clear LCD
* Aggiornamento linea 2 LCD

---

### Moduli Firmware Attuali

#### Lato Raspberry

* srm_protocol
* srm_crc16
* srm_menu
* srm_test
* rs485_if

#### Lato STM32

* srm_parser
* srm_dispatcher
* rs485_if
* lcd
* app_main

---

### Prossimi Step Previsti

* Inserimento testo LCD personalizzato
* Testo LCD scorrevole
* Simboli personalizzati LCD
* Integrazione EEPROM
* Indirizzamento multi-slave
* Supporto CAN bus
* Supporto gateway WiFi
* Integrazione con rover e futura architettura Nordic Node

---

## Eesti

### Projekti Ülevaade

Sample Routing Module on modulaarne embedded-projekt, mis põhineb:

* Raspberry Pi 4 Model B kui master-seade
* STM32 NUCLEO-F207 kui slave-seade
* RS485 füüsiline kiht
* Kohandatud SRM binaarprotokoll
* CRC16 kontroll
* RX ring buffer
* Modulaarne parseri ja dispatcher'i arhitektuur
* LCD1602 I2C integratsioon

---

### Praegune Milestone

Praegune versioon toetab:

* Kahepoolset RS485 suhtlust
* SRM binaarkaadreid
* CRC16 kontrolli
* RX ring bufferit
* Kaadrite parserit
* Käskude dispatcher'it
* Request/response protokolli
* Source/destination aadresse
* Sequence number süsteemi
* Raspberry käsumenüüd
* STM32 LED ON / LED OFF juhtimist
* STATUS vastuseid
* LCD1602 I2C draiverit
* LCD clear käsku
* LCD line 1 kirjutamist
* LCD line 2 kirjutamist

Praegu toetatud käsud:

* PING_REQ / PING_RSP
* LED_ON_REQ / LED_ON_RSP
* LED_OFF_REQ / LED_OFF_RSP
* STATUS_REQ / STATUS_RSP
* LCD_CLEAR_REQ / LCD_CLEAR_RSP
* LCD_WRITE_LINE1_REQ / LCD_WRITE_LINE1_RSP
* LCD_WRITE_LINE2_REQ / LCD_WRITE_LINE2_RSP

---

### Järgmised Sammud

* LCD kohandatud tekst
* LCD scrolling text
* LCD custom symbols
* EEPROM integratsioon
* Multi-slave aadressimine
* CAN bus tugi
* WiFi gateway tugi
* Integratsioon roveri ja Nordic Node arhitektuuriga
