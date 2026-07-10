Declaración de Fuentes y Herramientas

Proyecto: Sistema de Monitoreo de Humedad de Suelo IoT
Curso: TEI201 — Taller de Diseño en Ingeniería
Integrantes: Nicolas Oliden, Benjamin Pizarro, Ignacio Villalobos, Emmanuel Poulangeon

1. Librerías utilizadas

El proyecto utiliza las siguientes librerías del núcleo de ESP32 para habilitar la lectura analógica y la conectividad a internet:

| Función / Módulo | Origen | Uso en el proyecto |
| `WiFi.h` | Arduino ESP32 Core | Conexión del microcontrolador a la red Wi-Fi local. |
| `HTTPClient.h` | Arduino ESP32 Core | Creación y envío de peticiones HTTP POST al servidor. |
| `WiFiClientSecure.h` | Arduino ESP32 Core | Manejo de la conexión segura (HTTPS) para interactuar con la API en Railway. |
| `analogRead()` | Arduino ESP32 Core | Lectura del sensor de humedad de suelo en el pin 34. |

2. Código externo adaptado

No se utilizaron fragmentos de código de fuentes externas (GitHub, Stack Overflow, tutoriales, etc.). La estructura de conexión Wi-Fi y las peticiones HTTP fueron desarrolladas por el equipo con asistencia de Inteligencia Artificial.

3. Uso de Inteligencia Artificial

Desarrollo del Firmware (C++)
Herramientas: Claude y Gemini.
Uso:Asistencia en la reestructuración del código para evolucionar de un sistema local a un prototipo IoT conectado. Generación de la lógica para el armado del string JSON y el envío de la petición POST segura (`clienteSeguro.setInsecure()`) a la API alojada en Railway.
Comprensión del equipo: El equipo comprende completamente el flujo de datos: el sensor lee el valor analógico en el pin 34, la función `map()` lo convierte a un porcentaje (0-100%), se construye el payload en formato JSON y se envía mediante protocolo HTTPS a la ruta `/api/medicion`.

Documentación (README.md)
Herramientas: Claude.
Uso: Generación del template base del `README.md`; el contenido técnico específico fue completado por el equipo.

4. Recursos de referencia

| Recurso | URL | Uso |
| Documentación ESP32 Arduino Core | https://docs.espressif.com/projects/arduino-esp32/en/latest/ | Referencia para el pinout del ESP32 y funciones analógicas. |
| Railway Documentation | https://docs.railway.app/ | Referencia para el despliegue y recepción de datos en la API. |

*Declaración preparada en cumplimiento de los requisitos de integridad académica del curso TEI201 — UAI.*
