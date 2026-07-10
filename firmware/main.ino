#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// =====================================================
// CONFIGURACIÓN WIFI
// =====================================================

const char* ssid = "TU_NOMBRE_WIFI";
const char* password = "TU_CLAVE_WIFI";

// =====================================================
// URL DEL DASHBOARD EN RAILWAY
// =====================================================

const char* serverUrl =
  "sistema-de-humedad-iot-production.up.railway.app;

// =====================================================
// CONFIGURACIÓN DEL SENSOR
// =====================================================

// Pin analógico conectado desde AO del LM393
const int SENSOR_PIN = 34;

// Valores iniciales de calibración.
// Más adelante puedes cambiarlos con tus lecturas reales.
//
// Tierra seca / aire = 0%
// Tierra muy mojada = 100%
const int valorSeco = 4095;
const int valorMojado = 1200;

// Enviar una medición cada 10 segundos
const unsigned long INTERVALO_ENVIO = 10000;

unsigned long ultimoEnvio = 0;

// =====================================================
// OBTENER ESTADO SEGÚN PORCENTAJE
// =====================================================

String obtenerEstado(int humedad) {
  if (humedad >= 80) {
    return "Muy mojado";
  }

  if (humedad >= 60) {
    return "No necesita riego";
  }

  if (humedad >= 40) {
    return "Humedad media";
  }

  if (humedad >= 20) {
    return "Necesita riego";
  }

  return "Muy seco";
}

// =====================================================
// DETERMINAR SI NECESITA RIEGO
// =====================================================

bool obtenerRiegoRecomendado(int humedad) {
  return humedad < 40;
}

// =====================================================
// CONECTAR A WIFI
// =====================================================

void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Conectando al WiFi");

  unsigned long tiempoInicio = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (millis() - tiempoInicio >= 20000) {
      Serial.println();
      Serial.println("No se pudo conectar. Reintentando...");

      WiFi.disconnect();
      delay(1000);
      WiFi.begin(ssid, password);

      tiempoInicio = millis();
    }
  }

  Serial.println();
  Serial.println("WiFi conectado correctamente");

  Serial.print("Direccion IP del ESP32: ");
  Serial.println(WiFi.localIP());
}

// =====================================================
// LEER SENSOR Y CALCULAR PORCENTAJE
// =====================================================

int obtenerPorcentajeHumedad(int lecturaCruda) {
  int porcentaje = map(
    lecturaCruda,
    valorSeco,
    valorMojado,
    0,
    100
  );

  porcentaje = constrain(porcentaje, 0, 100);

  return porcentaje;
}

// =====================================================
// ENVIAR MEDICIÓN AL DASHBOARD
// =====================================================

void enviarMedicion(
  int lecturaCruda,
  int humedadPorcentaje,
  String estado,
  bool riegoRecomendado
) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado.");
    Serial.println("Intentando reconectar...");

    conectarWiFi();
  }

  WiFiClientSecure clienteSeguro;

  // Permite conexión HTTPS con Railway
  clienteSeguro.setInsecure();

  HTTPClient http;

  bool conexionIniciada = http.begin(clienteSeguro, serverUrl);

  if (!conexionIniciada) {
    Serial.println("No se pudo iniciar la conexion con el servidor.");
    return;
  }

  http.addHeader("Content-Type", "application/json");

  String json = "{";
  json += "\"lectura\":" + String(lecturaCruda) + ",";
  json += "\"humedad\":" + String(humedadPorcentaje) + ",";
  json += "\"estado\":\"" + estado + "\",";
  json += "\"riegoRecomendado\":";
  json += riegoRecomendado ? "true" : "false";
  json += ",";
  json += "\"caso\":\"Medicion real ESP32\",";
  json += "\"ubicacion\":\"Caja prototipo\",";
  json += "\"dispositivo\":\"ESP32 sensor humedad\"";
  json += "}";

  Serial.println();
  Serial.println("JSON enviado:");
  Serial.println(json);

  int codigoHTTP = http.POST(json);

  Serial.print("Codigo HTTP: ");
  Serial.println(codigoHTTP);

  if (codigoHTTP > 0) {
    String respuesta = http.getString();

    Serial.print("Respuesta del servidor: ");
    Serial.println(respuesta);

    if (codigoHTTP == 200 || codigoHTTP == 201) {
      Serial.println("Medicion enviada correctamente al dashboard.");
    } else if (codigoHTTP == 404) {
      Serial.println("Error 404: no existe /api/medicion en el servidor.");
    } else if (codigoHTTP == 500) {
      Serial.println("Error 500: problema interno del servidor.");
    } else {
      Serial.println("El servidor respondio, pero hubo un problema.");
    }
  } else {
    Serial.print("Error de conexion HTTP: ");
    Serial.println(http.errorToString(codigoHTTP));
  }

  http.end();
}

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(SENSOR_PIN, INPUT);

  Serial.println();
  Serial.println("==================================");
  Serial.println("SISTEMA SENSOR DE HUMEDAD");
  Serial.println("==================================");

  conectarWiFi();

  // Hace la primera medición inmediatamente
  ultimoEnvio = millis() - INTERVALO_ENVIO;
}

// =====================================================
// LOOP
// =====================================================

void loop() {
  if (millis() - ultimoEnvio >= INTERVALO_ENVIO) {
    ultimoEnvio = millis();

    int lecturaCruda = analogRead(SENSOR_PIN);

    int humedadPorcentaje =
      obtenerPorcentajeHumedad(lecturaCruda);

    String estado =
      obtenerEstado(humedadPorcentaje);

    bool riegoRecomendado =
      obtenerRiegoRecomendado(humedadPorcentaje);

    Serial.println();
    Serial.println("----------------------------------");

    Serial.print("Lectura cruda: ");
    Serial.println(lecturaCruda);

    Serial.print("Humedad: ");
    Serial.print(humedadPorcentaje);
    Serial.println("%");

    Serial.print("Estado: ");
    Serial.println(estado);

    Serial.print("Riego recomendado: ");
    Serial.println(riegoRecomendado ? "SI" : "NO");

    enviarMedicion(
      lecturaCruda,
      humedadPorcentaje,
      estado,
      riegoRecomendado
    );

    Serial.println("----------------------------------");
    Serial.println("Proxima medicion en 10 segundos.");
  }
}

//El codigo comienza con las directivas de preprocesamiento que son:

//SENSOR_PIN 34
//LED_ROJO 25
//LED_VERDE 26
//LED_AZUL 27
//Los cuales tienen don nombres asignados para por los mismo componentes pines físicos que esta conectados a la placa y tambien es fácil de recordar esto permite que si se quiere cambiar un cable de sitio,
//solo se tienenque modificar el numero en una linea, justo despues se declara la variable global int valorHumedad = 0;, que funciona como un contenedor donde guardaremos el número que el sensor nos entregue en cada lectura.



//Dentro de la función setup(), que se ejecuta solo una vez al encender el dispositivo, la línea Serial.begin(115200) activa la comunicación con el computador para que se puedan ver los datos en pantalla a una velocidad de 115200 baudios.

//Después están las tres lineas, pinMode(LED_..., OUTPUT) preparan los pines de los LEDs para enviar electricidad (salida), y la llamada a apagarLED() asegura que el sistema comience con todas las luces apagadas para evitar lecturas visuales erróneas al arranque.

//En el loop principal loop(), que se repite infinitamente, la ejecución empieza con valorHumedad = analogRead(SENSOR_PIN);, donde el microcontrolador mide el voltaje en el pin 34 y lo convierte en un número entre 0 y 4095. 
//Las líneas Serial.print y Serial.println muestran este valor en tu monitor serie para que puedas monitorearlo. 
//Posteriormente, entra la lógica condicional: primero, if (valorHumedad >= 3000 && valorHumedad <= 4095) verifica si la tierra está muy seca (valores altos), en cuyo caso ejecuta la función rojo() para encender esa luz y escribe un aviso de riego en el monitor.
//Si eso no se cumple, el else if (valorHumedad >= 2000 && valorHumedad < 3000) revisa si la humedad es intermedia, activando la función amarillo(). Finalmente, el else if (valorHumedad < 2000) detecta si la tierra está bien hidratada, activando la función verde().
//Al terminar estas comparaciones, se imprime una línea divisoria y el programa se detiene durante 10 segundos con delay(10000); antes de volver a empezar.

//Al final del archivo se encuentran las funciones auxiliares que definen los colores. En rojo(), amarillo() y verde(), se utilizan comandos digitalWrite(PIN, HIGH/LOW) para enviar o cortar la energía a cada pin. 
//Por ejemplo, en la función amarillo(), se activan simultáneamente el LED_ROJO y el LED_VERDE porque, en la mezcla de colores luz, la unión de rojo y verde produce amarillo. 
//La función apagarLED() simplemente pone todos los pines en LOW para cortar el paso de corriente y apagar el dispositivo visual.
