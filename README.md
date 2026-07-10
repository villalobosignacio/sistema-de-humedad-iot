Sistema de Monitoreo de Humedad de Suelo

Sistema IoT que monitorea la humedad del suelo en tiempo real mediante un sensor resistivo conectado a un ESP32. El dispositivo indica el estado del suelo a través de LEDs de colores y registra las lecturas en el monitor serie, permitiendo tomar decisiones de riego oportunas.

Integrantes
Nicolas Oliden 
Benjamin Pizarro 
Ignacio Villalobos 
Emmanuel Poulangeon 

Curso: TEI201 — Taller de Diseño en Ingeniería  
Universidad: Universidad Adolfo Ibáñez  
Año:2026

Problema que resuelve

En hogares y espacios urbanos de Chile, el riego de plantas se realiza de forma intuitiva y sin información objetiva sobre el estado real del suelo. Esto genera dos problemas frecuentes: el riego excesivo, que desperdicia agua y daña las raíces por saturación, y la falta de riego oportuno, que provoca la muerte de las plantas. Nuestro sistema elimina esta incertidumbre entregando una señal visual inmediata sobre el nivel de humedad del suelo, permitiendo al usuario regar solo cuando es necesario.  
El sistema permite a cualquier persona saber si una planta necesita riego sin necesidad de medir manualmente la humedad del suelo. Un LED rojo indica tierra seca (riego urgente), amarillo indica humedad media, y verde indica tierra bien regada.

Componentes de hardware necesarios

| Componente | Cantidad | Especificación |

Componentes de hardware necesarios

| Componente | Cantidad | Especificación |
| ESP32 NodeMCU | 1 | microcontrolador con Wi-Fi integrado, ADC 12-bit |
| Sensor humedad suelo | 1 | Módulo LM393 (Sonda resistiva) — Conectado a pin GPIO 34 |
| Protoboard 400 pts | 1 | Para el montaje y orden de conexiones internas |
| Cables jumper | 10 | Conexiones internas de señal y alimentación |
| Cable USB micro | 1 | Para suministro eléctrico continuo y programación |
| Filamento PLA | ~50g | Material para la manufactura de la carcasa protectora 3D |

Ver BOM completo con costos en `/hardware/BOM.md`

Instrucciones para cargar el firmware

Requisitos previos
Arduino IDE 2.x instalado
Soporte para ESP32 instalado en el Board Manager:
Ir a **Archivo → Preferencias**
En "URLs adicionales" agregar: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
Ir a Herramientas → Placa → Gestor de placas → buscar "esp32" → Instalar

Pasos
1. Clonar este repositorio:
   ```
   git clone https://github.com/villalobosignacio/sistema-de-humedad-iot.git
   ```
2. Abrir `firmware/main.ino` en Arduino IDE
3. Conectar el ESP32 por USB
4. Seleccionar la placa: Herramientas → Placa → ESP32 Arduino → ESP32 Dev Module
5. Seleccionar el puerto COM correspondiente en Herramientas → Puerto
6. Subir el código con el botón Upload(→)
7. Abrir Monitor Serie a 115200 baudios para ver las lecturas en tiempo real


Lógica de indicación LED

| Color LED | Valor ADC | Estado del suelo |

| Rojo | 3000 – 4095 | Tierra seca — necesita riego |
| Amarillo (R+G) | 2000 – 2999 | Humedad media |
| Verde | 0 – 1999 | Tierra húmeda / bien irrigada |

Dashboard y Visualización
Los datos capturados por el ESP32 se envían mediante peticiones HTTPS (POST) a nuestra propia API alojada en Railway. 

[Acceder a la API/Dashboard en Railway](https://sistema-de-humedad-iot-production.up.railway.app/api/medicion)

Estructura del repositorio

```
sistema-de-humedad-iot/
│
├── README.md
├── FUENTES.md
│
├── firmware/
│   └── main.ino
│
├── hardware/
│   ├── BOM.md
│   ├── esquematico.pdf
│   ├── foto_prototipo_1.jpg
│   ├── foto_prototipo_2.jpg
│   └── foto_prototipo_3.jpg
│
├── diseño-3d/
│   ├── encapsulado.f3d
│   ├── planos.pdf
│   └── renders/
│
├── testing/
│   └── protocolo_pruebas.md
│
└── docs/
    └── reporte_final.pdf
```
