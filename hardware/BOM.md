BOM — Lista de Materiales

Proyecto: Sistema de Monitoreo de Humedad de Suelo  
Curso: TEI201 — UAI 2026  
Integrantes: Nicolas Oliden, Benjamin Pizarro, Ignacio Villalobos, Emmanuel Poulangeon


| N° | Componente | Cantidad | Especificación técnica | Costo unitario (CLP) | Costo total (CLP) |

| 1 | ESP32 NodeMCU | 1 | ESP-32 38 pines, WiFi + Bluetooth, dual core 240MHz, ADC 12-bit | $8.500 | $8.500 |
| 2 | Sensor humedad suelo | 1 | Sensor resistivo, salida analógica 0–3.3V, rango 0–4095 ADC | $1.800 | $1.800 |
| 3 | LED Rojo 5mm | 1 | Voltaje directo 2.0V, corriente 20mA | $200 | $200 |
| 4 | LED Verde 5mm | 1 | Voltaje directo 2.1V, corriente 20mA | $200 | $200 |
| 5 | LED Azul 5mm | 1 | Voltaje directo 3.2V, corriente 20mA | $200 | $200 |
| 6 | Resistencias 220Ω | 3 | 1/4W, tolerancia 5%, para protección de LEDs | $50 | $150 |
| 7 | Protoboard | 1 | 400 puntos, sin soldadura | $2.500 | $2.500 |
| 8 | Cables jumper | 10 | Macho-macho, 20cm | $150 | $1.500 |
| 9 | Cable USB micro | 1 | Micro-USB, datos + alimentación, para programar ESP32 | $1.500 | $1.500 |
| TOTAL | $16.550 |

Justificación de componentes principales

ESP32: Seleccionado por su conversor analógico-digital (ADC) de 12 bits (resolución 0–4095), necesario para leer el sensor de humedad con precisión. Además incluye WiFi integrado para futura expansión del sistema.

Sensor resistivo de humedad: Seleccionado por disponibilidad, bajo costo y compatibilidad directa con el ADC del ESP32 (salida analógica 0–3.3V). La variación de resistencia del suelo se traduce directamente en variación de voltaje.

LEDs separados R/G/B: Permiten combinar colores (rojo + verde = amarillo) para representar 3 estados de humedad (seco, medio, húmedo) con solo 3 pines digitales del microcontrolador.
