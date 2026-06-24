#define SENSOR_PIN 34

#define LED_ROJO 25
#define LED_VERDE 26
#define LED_AZUL 27

int valorHumedad = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  apagarLED();
}

void loop() {
  valorHumedad = analogRead(SENSOR_PIN);

  Serial.print("Valor del sensor: ");
  Serial.println(valorHumedad);

  if (valorHumedad >= 3000 && valorHumedad <= 4095) {
    // TIERRA SECA → ROJO
    rojo();
    Serial.println("Estado: TIERRA SECA - NECESITA RIEGO");
  } 
  else if (valorHumedad >= 2000 && valorHumedad < 3000) {
    // HUMEDAD MEDIA → AMARILLO
    amarillo();
    Serial.println("Estado: HUMEDAD MEDIA");
  } 
  else if (valorHumedad < 2000) {
    // TIERRA HÚMEDA O MUY MOJADA → VERDE
    verde();
    Serial.println("Estado: TIERRA HÚMEDA / MUY MOJADA");
  }

  Serial.println("-------------------------");

  delay(10000); // vuelve a evaluar cada 15 segundos
}

void rojo() {
  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);
}

void amarillo() {
  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AZUL, LOW);
}

void verde() {
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AZUL, LOW);
}

void apagarLED() {
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);
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
