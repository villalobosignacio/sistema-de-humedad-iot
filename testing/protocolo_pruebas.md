Protocolo de Pruebas Sistema de Monitoreo de Humedad de Suelo IoT

Proyecto: Sistema de Monitoreo de Humedad de Suelo IoT  
Curso: TEI201 UAI 2026  
Integrantes: Nicolas Oliden, Benjamin Pizarro, Ignacio Villalobos, Emmanuel Poulangeon  
Fecha de pruebas: Julio 2026

Condiciones generales
* Dispositivo alimentado por USB.
* Monitor Serie abierto a 115200 baudios para verificar peticiones HTTP.
* Conexión a red Wi-Fi local establecida de forma estable.
* Sensor insertado verticalmente en el sustrato de prueba.

Prueba 1 — Detección de tierra seca y envío a API
Objetivo: Verificar que el sistema detecta correctamente suelo seco, mapea el porcentaje y envía la alerta al servidor mediante HTTPS.

Procedimiento:
1. Conectar el ESP32 a la energía y verificar conexión Wi-Fi.
2. Insertar el sensor en tierra completamente seca (o sostenerlo en el aire).
3. Esperar a que se ejecute la lectura y se arme el JSON.
4. Verificar la respuesta del servidor en el Monitor Serie o en el dashboard de Railway.

Criterio de éxito: Humedad calculada < 40% | JSON incluye "riegoRecomendado": true | Servidor responde con Código HTTP 200 o 201.

Resultado obtenido:
| Intento | Lectura Cruda | Humedad (%) | Estado en JSON | Código HTTP | ¿Correcto? |
| 1 | 4095 | 0% | Muy seco | 200 | Sí |

Prueba 2 — Detección de humedad media
Objetivo: Verificar que el sistema detecta suelo con humedad intermedia y actualiza el estado correctamente en la nube.

Procedimiento:
1. Tomar tierra seca y agregar agua en cantidad moderada.
2. Mezclar hasta obtener tierra húmeda pero no encharcada.
3. Insertar el sensor y esperar el intervalo de 10 segundos.

Criterio de éxito: Humedad calculada entre 40% y 79% | JSON incluye "Humedad media" o "No necesita riego" | Código HTTP 200.

Resultado obtenido:
| Intento | Lectura Cruda | Humedad (%) | Estado en JSON | Código HTTP | ¿Correcto? |
| 1 | 2650 | 50% | Humedad media | 200 | Sí |

Prueba 3 — Detección de tierra saturada
Objetivo: Verificar que el sistema detecta suelo bien irrigado y registra que no se requiere intervención.

Procedimiento:
1. Agregar agua abundante a la tierra hasta saturarla.
2. Insertar el sensor completamente en el sustrato.
3. Esperar lectura y registrar el comportamiento de la petición POST.

Criterio de éxito: Humedad calculada ≥ 80% | JSON incluye "Muy mojado" | Código HTTP 200.

Resultado obtenido:
| Intento | Lectura Cruda | Humedad (%) | Estado en JSON | Código HTTP | ¿Correcto? |
| 1 | 1200 | 100% | Muy mojado | 200 | Sí |

<img width="373" height="266" alt="image (1)" src="https://github.com/user-attachments/assets/4a17b8e2-bb13-4ad6-b4fe-d95a2ec88737" />


Prueba 4 — Estabilidad de transmisión IoT
Objetivo: Verificar que el sistema realiza lecturas y peticiones HTTP cada 10 segundos de forma continua sin caídas del microcontrolador.

Procedimiento:
1. Dejar el sensor en una condición fija.
2. Observar el Monitor Serie o los logs de Railway durante 2 minutos.
3. Contar las peticiones exitosas recibidas por la API.

Criterio de éxito: Nueva petición POST enviada y confirmada cada 10 segundos sin reinicios del ESP32.

Resultado obtenido:
* Lecturas enviadas en 2 minutos: 12
* Intervalo promedio de transmisión: 10 segundos
* ¿Hubo interrupciones de Wi-Fi o reinicios? No.


Falla detectada y solución

Falla 1 (Física): Durante las pruebas, el sensor entregaba valores inestables al estar en tierra con piedras.
Causa: El sensor requiere contacto completo de las dos puntas. Materiales aislantes interfieren la resistencia.
Solución: Se definió que el sensor debe insertarse en tierra suelta.

Falla 2 (Software): Caídas eventuales de la red Wi-Fi bloqueaban el envío de datos permanentemente.
Causa: El código original no contemplaba reconexión si el router perdía la señal un instante.
Solución: Se implementó una rutina de reconexión automática en la función `enviarMedicion()`, donde el ESP32 verifica `WiFi.status() != WL_CONNECTED` y reinicia la conexión antes de intentar enviar el JSON.

Validación contra el problema original (Avance 1)

El problema identificado en Avance 1 fue: En hogares urbanos de Chile, el riego de plantas se realiza sin información objetiva sobre el estado real del suelo, generando desperdicio de agua por riego excesivo o muerte de plantas por falta de riego oportuno.

Nuestras pruebas demuestran que el sistema evolucionó de una simple alerta local a una solución de monitoreo continuo (IoT). Al mapear los valores de resistencia a un porcentaje exacto (0-100%) y enviarlos mediante peticiones HTTPS a nuestra propia API en Railway, el usuario ya no depende de estar físicamente frente a la maceta para ver una luz LED. 

Esto responde al problema con un estándar tecnológico superior: elimina la incertidumbre del riego entregando datos centralizados y categorizados ("Necesita riego", "Humedad media", etc.). El usuario puede tomar decisiones objetivas desde cualquier dispositivo conectado, optimizando el uso del recurso hídrico con precisión.
