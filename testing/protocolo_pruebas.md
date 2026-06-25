Protocolo de Pruebas Sistema de Monitoreo de Humedad de Suelo

Proyecto: Sistema de Monitoreo de Humedad de Suelo  
Curso: TEI201 UAI 2026  
Integrantes: Nicolas Oliden, Benjamin Pizarro, Ignacio Villalobos, Emmanuel Poulangeon  
Fecha de pruebas: Junio 2026

Condiciones generales
Dispositivo alimentado por USB desde computador
Monitor Serie abierto a 115200 baudios en Arduino IDE
Temperatura ambiente: ~20°C
Sensor insertado verticalmente en el sustrato de prueba

Prueba 1 — Detección de tierra seca

Objetivo: Verificar que el sistema detecta correctamente suelo seco y activa la alerta roja.

Procedimiento:
1. Conectar el ESP32 al computador via USB
2. Abrir Monitor Serie (115200 baudios)
3. Insertar el sensor en tierra completamente seca (o sostenerlo en el aire)
4. Esperar 10 segundos para la primera lectura
5. Registrar valor del sensor y color del LED

Criterio de éxito: Valor ADC ≥ 3000 | LED rojo encendido | Serial muestra "TIERRA SECA, NECESITA RIEGO"

Resultado obtenido:
| Intento | Valor ADC | LED activado | Estado Serial | ¿Correcto? |

| 1 | [4095] | [rojo] | [TIERRA SECA - NECESITA RIEGO] | [Sí] |


Prueba 2  Detección de humedad media

Objetivo: Verificar que el sistema detecta suelo con humedad intermedia y activa el indicador amarillo.

Procedimiento:
Tomar tierra seca y agregar agua en cantidad moderada
Mezclar hasta obtener tierra húmeda pero no encharcada
Insertar el sensor y esperar lectura
Registrar valor y color

Criterio de éxito: Valor ADC entre 2000 y 2999 | LED amarillo encendido | Serial muestra "HUMEDAD MEDIA"

Resultado obtenido:
| Intento | Valor ADC | LED activado | Estado Serial | ¿Correcto? |
| 1 | [2077] | [amarillo] | [HUMEDAD MEDIA] | [Sí] |

#Prueba 3  Detección de tierra húmeda

Objetivo: Verificar que el sistema detecta suelo bien irrigado y activa el indicador verde.

Procedimiento:
1. Agregar agua abundante a la tierra hasta saturarla
2. Insertar el sensor completamente
3. Esperar lectura y registrar

Criterio de éxito: Valor ADC < 2000 | LED verde encendido | Serial muestra "TIERRA HÚMEDA / MUY MOJADA"

Resultado obtenido:
| Intento | Valor ADC | LED activado | Estado Serial | ¿Correcto? |
| 1 | [1279] | [verde] | [TIERRA HÚMEDA / MUY MOJADA] | [Sí] |

Prueba 4  Frecuencia de muestreo

Objetivo: Verificar que el sistema realiza lecturas cada 10 segundos de forma continua y estable.

Procedimiento:
1. Dejar el sensor en una condición fija (ej. tierra seca)
2. Observar el Monitor Serie durante 2 minutos
3. Contar las lecturas y verificar intervalo

Criterio de éxito: Nueva lectura aparece cada 10 segundos (±1 seg) sin interrupciones.

Resultado obtenido:
- Lecturas registradas en 2 minutos: 12
- Intervalo promedio entre lecturas: 10 segundos
- ¿Hubo interrupciones o reinicios? No 

Falla detectada y solución

Falla: Durante las pruebas iniciales, el sensor entregaba valores inestables al estar en tierra con piedras, ya que el contacto eléctrico no era uniforme.

Causa: El sensor resistivo requiere contacto completo de las dos puntas con el sustrato. Materiales no conductores entre las puntas interfieren la lectura.

Solución aplicada:Se definió que el sensor debe insertarse en tierra suelta y uniforme, evitando zonas con piedras o raíces. Además se aumentó el delay entre lecturas de 5 a 10 segundos para permitir que la señal se estabilice.

Resultado post-corrección: Lecturas estables con variación < 50 unidades ADC en condición constante.

Validación contra el problema original (Avance 1)

El problema identificado en Avance 1 fue: En hogares urbanos de Chile, el riego de plantas se realiza sin información objetiva sobre el estado real del suelo, generando desperdicio de agua por riego excesivo o muerte de plantas por falta de riego oportuno.

Nuestras pruebas demuestran que el sistema detecta correctamente los tres estados de humedad del suelo mediante valores ADC calibrados experimentalmente. Los umbrales 2000 y 3000 fueron determinados midiendo el sensor en tierra seca, tierra con riego moderado y tierra saturada, permitiendo al usuario identificar visualmente cuándo una planta necesita riego (LED rojo), está en condición óptima (LED verde) o en estado intermedio (LED amarillo).

Esto responde directamente al problema planteado, ya que el sistema elimina la incertidumbre del riego entregando una señal visual inmediata: el usuario sabe en todo momento si debe regar (LED rojo), esperar (LED amarillo) o no intervenir (LED verde), sin necesidad de medir manualmente ni adivinar el estado del suelo.
