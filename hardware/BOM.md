BOM — Lista de Materiales

Proyecto: Sistema de Monitoreo de Humedad de Suelo IoT  
Curso: TEI201 — UAI 2026  
Integrantes: Nicolas Oliden, Benjamin Pizarro, Ignacio Villalobos, Emmanuel Poulangeon

| N° | Componente | Cantidad | Especificación técnica | Costo unitario (CLP) | Costo total (CLP) |
| 1 | ESP32 NodeMCU | 1 | ESP-32 38 pines, Wi-Fi + Bluetooth, dual core 240MHz, ADC 12-bit | $8.500 | $8.500 |
| 2 | Sensor humedad de suelo | 1 | Módulo LM393, salida analógica, rango 0–4095 ADC | $1.800 | $1.800 |
| 3 | Protoboard | 1 | 400 puntos, para montaje de pines internos | $2.500 | $2.500 |
| 4 | Cables jumper | 10 | Macho-hembra / Macho-macho, 20cm | $150 | $1.500 |
| 5 | Cable USB micro | 1 | Micro-USB, datos + alimentación (energía del sistema) | $1.500 | $1.500 |
| 6 | Filamento PLA (Impresión 3D) | ~50g | Material para la manufactura de la carcasa protectora | $20/gramo | $1.000 |
| TOTAL | | | | | $16.800 |

Justificación de componentes principales

ESP32 NodeMCU: Seleccionado estratégicamente por su módulo Wi-Fi integrado, el cual es el núcleo de nuestro sistema IoT. Permite capturar la lectura analógica de la tierra con precisión (ADC de 12 bits) y procesar el envío de datos en formato JSON de forma segura (HTTPS) hacia nuestra API en Railway.
Sensor resistivo de humedad: Seleccionado por disponibilidad, bajo costo y compatibilidad directa con el pin 34 del ESP32. La variación de la conductividad de la tierra se traduce en un nivel de voltaje que el microcontrolador mapea de 0 a 100%.
Carcasa impresa en 3D (PLA): Pieza diseñada a medida y parametrizada para encapsular de forma segura el microcontrolador y el cableado, protegiendo la electrónica y manteniendo expuesta únicamente la sonda que va insertada en el sustrato.
