FUENTES.md 

Proyecto: Sistema de Monitoreo de Humedad de Suelo con Indicador LED  
Curso: TEI201 — Taller de Diseño en Ingeniería
Integrantes:Nicolas Oliden, Benjamin Pizarro, Ignacio Villalobos, Emmanuel Poulangeon


1. Librerías utilizadas

Este proyecto no utiliza librerías externas. Todo el código se basa en las funciones nativas del núcleo Arduino para ESP32:

| Función / Módulo | Origen | Uso en el proyecto |

| `analogRead()` | Arduino ESP32 Core (built-in) | Lectura del sensor de humedad de suelo en pin 34, retorna valores 0–4095 |
| `digitalWrite()` | Arduino ESP32 Core (built-in) | Control de encendido/apagado de los LEDs rojo, verde y azul |
| `pinMode()` | Arduino ESP32 Core (built-in) | Configuración de pines 25, 26 y 27 como salidas digitales |
| `Serial` | Arduino ESP32 Core (built-in) | Comunicación con monitor serie a 115200 baudios para monitoreo de valores |
| `delay()` | Arduino ESP32 Core (built-in) | Pausa de 10 segundos entre lecturas del sensor |

Fuente del núcleo Arduino para ESP32: https://github.com/espressif/arduino-esp32

2. Código externo adaptado

No se utilizaron fragmentos de código de fuentes externas (GitHub, Stack Overflow, tutoriales, etc.). El código fue escrito íntegramente por el equipo y ayuda de IA.

#3. Uso de Inteligencia Artificial

Comentarios explicativos del código (`main.ino`, bloque de comentarios finales)
Herramienta: Claude 
Uso:Generación de comentarios que explican la lógica del código línea a línea, para documentación del proyecto.
Adaptación: Los comentarios fueron revisados y ajustados por el equipo para reflejar con precisión el funcionamiento real del sistema, incluyendo la aclaración sobre la mezcla de colores luz (rojo + verde = amarillo).
Comprensión del equipo: El equipo comprende completamente la lógica del código: el sensor entrega un valor analógico entre 0 y 4095 proporcional a la resistencia del suelo (suelo seco → alta resistencia → valor alto), y los umbrales 2000/3000 fueron definidos experimentalmente mediante pruebas con tierra seca, húmeda y muy mojada.

README.md
Herramienta: Claude (junio 2026)
Uso: Generación del template base del README.md; el contenido técnico específico fue completado por el equipo.

4. Recursos de referencia

| Recurso | URL | Uso |
| Documentación ESP32 Arduino Core | https://docs.espressif.com/projects/arduino-esp32/en/latest/ | Referencia para funciones analogRead() y pinout del ESP32 |

*Declaración preparada en cumplimiento de los requisitos de integridad académica del curso TEI201 — UAI 2026.*
