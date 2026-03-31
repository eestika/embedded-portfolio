# Sample Routing Module

## English

### Project Overview

Sample Routing Module is a modular embedded communication project based on:

* Raspberry Pi 4 Model B acting as master node
* STM32 NUCLEO-F207 acting as slave node
* RS485 physical layer
* Custom SRM binary protocol
* CRC16 frame validation
* Ring buffer RX handling
* Modular parser and dispatcher architecture

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

Currently implemented commands:

* PING_REQ / PING_RSP
* LED_ON_REQ / LED_ON_RSP
* LED_OFF_REQ / LED_OFF_RSP

---

### Example Communication Flow

1. Raspberry sends PING_REQ
2. STM32 validates frame and CRC
3. STM32 dispatches the command
4. STM32 sends PING_RSP
5. Raspberry validates response and CRC

The same mechanism is now used for LED ON and LED OFF commands.

---

### Planned Next Steps

* STATUS_REQ / STATUS_RSP
* Uptime reporting
* LED state reporting
* EEPROM integration
* LCD integration
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

Comandi attualmente implementati:

* PING_REQ / PING_RSP
* LED_ON_REQ / LED_ON_RSP
* LED_OFF_REQ / LED_OFF_RSP

---

### Esempio di Flusso Comunicazione

1. Raspberry invia PING_REQ
2. STM32 valida frame e CRC
3. STM32 esegue il dispatch del comando
4. STM32 invia PING_RSP
5. Raspberry valida la risposta e il CRC

Lo stesso meccanismo viene ora utilizzato anche per i comandi LED ON e LED OFF.

---

### Prossimi Step Previsti

* STATUS_REQ / STATUS_RSP
* Report uptime
* Report stato LED
* Integrazione EEPROM
* Integrazione LCD
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

Projekti eesmärk on luua skaleeritav embedded-raamistik, mis toetab tulevikus:

* Mitut slave-seadet
* LCD mooduleid
* EEPROM mälu
* I2C sensoreid
* CAN bus süsteemi
* WiFi või Ethernet laiendust
* Hajutatud sensorite ja aktuaatorite juhtimist

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

Praegu toetatud käsud:

* PING_REQ / PING_RSP
* LED_ON_REQ / LED_ON_RSP
* LED_OFF_REQ / LED_OFF_RSP

---

### Näidis Sidevoog

1. Raspberry saadab PING_REQ käsu
2. STM32 kontrollib kaadrit ja CRC-d
3. STM32 töötleb käsu
4. STM32 saadab PING_RSP vastuse
5. Raspberry kontrollib vastust ja CRC-d

Sama mehhanismi kasutatakse nüüd ka LED ON ja LED OFF käskude jaoks.

---

### Järgmised Sammud

* STATUS_REQ / STATUS_RSP
* Uptime info
* LED oleku info
* EEPROM integratsioon
* LCD integratsioon
* Multi-slave aadressimine
* CAN bus tugi
* WiFi gateway tugi
* Integratsioon roveri ja tulevase Nordic Node arhitektuuriga
