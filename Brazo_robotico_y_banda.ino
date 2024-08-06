// Se incluye la librería para controlar los servomotores
#include <Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
int gradosServo1 = 90;
int gradosServo2 = 90;
int gradosServo3 = 80;
int gradosServo4 = 45;
int gradosServo5 = 130;

// Pines
const int releMotorreductor = 8;
const int sensorInductivo = 7;
const int sensorParoBanda = 10;
const int sensorArranqueBanda = 9;
const int arranque = 12;
const int paro = 13;
const int pinServo1 = 2;
const int pinServo2 = 3;
const int pinServo3 = 4;
const int pinServo4 = 5;
const int pinServo5 = 6;

// Variables de estado
boolean esMetal = false;
boolean banderaInductivo = false;
boolean banderaSensorArranque = false;
boolean banderaSensorParo = false;
boolean finCiclo = false;
boolean encendido = false;
int proceso = 0;
int canidadAluminio = 0;
int cantidadPlastico = 0;

int cuenta = 0;
int estadoParo;
int estadoParoAnterior;
int estadoArranque;
int estadoArranqueAnterior;
const int tiempoAntirebote = 10;

void setup(){
    Serial.begin(9600);
    servo1.attach(pinServo1);
    servo2.attach(pinServo2);
    servo3.attach(pinServo3);
    servo4.attach(pinServo4);
    servo5.attach(pinServo5);

    // Se setean las posiciones por defecto de los servomotores
    servo1.write(gradosServo1);
    servo2.write(gradosServo2);
    servo3.write(gradosServo3);
    servo4.write(gradosServo4);
    servo5.write(gradosServo5);

    // Declaramos entras y salidas
    pinMode(releMotorreductor, OUTPUT);
    pinMode(sensorParoBanda, INPUT);
    pinMode(sensorArranqueBanda, INPUT);
    pinMode(sensorInductivo, INPUT);
    pinMode(arranque, INPUT);
    pinMode(paro, INPUT);
    digitalWrite(releMotorreductor, HIGH);
    Serial.println("Arduino listo!");
}

boolean antirebote(int pin){
    int contador = 0;
    boolean estado;
    boolean estadoAnterior;
    do{
        estado = digitalRead(pin);
        if(estado != estadoAnterior){
            contador = 0;
            estadoAnterior = estado;
        }else{
            contador = contador + 1;
        }
        delay(1);
    }while(contador < tiempoAntirebote);
    return estado;
}

void arranqueParo(int pin, int estado, int estadoAnterior){
    estado = digitalRead(pin);
    if(estado != estadoAnterior){
        if(antirebote(pin)){
            if(pin == 13){
                proceso = 0;
                esMetal = false;                
                banderaInductivo = false;
                banderaSensorArranque = false;
                banderaSensorParo = false;
                finCiclo = false;
                encendido = false;
                encendido = false;
                digitalWrite(releMotorreductor, HIGH);
                Serial.println("Apagado");
            }else{
                encendido = true;
                Serial.println("Encendido");
            }            
        }
    }
    estadoAnterior = estado;
}

void rutinaInduccion(){
    int i;
    for(i=gradosServo2; i>30; i--){
        delay(15);
        servo2.write(i);
    }
    gradosServo2 = i;
    delay(500);
    for(i=gradosServo5; i>80; i--){
        delay(15);
        servo5.write(i);
    }
    gradosServo5 = i;
    delay(500);
    for(i=gradosServo2; i<90; i++){
        delay(15);
        servo2.write(i);
    }
    gradosServo2 = i;
    delay(500);
    for(i=gradosServo1; i>2; i--){
        delay(15);
        servo1.write(i);
    }
    gradosServo1 = i;
    delay(500);
    for(i=gradosServo2; i>40; i--){
        delay(15);
        servo2.write(i);
    }
    gradosServo2 = i;
    delay(500);
    for(i=gradosServo5; i<130; i++){
        delay(15);
        servo5.write(i);
    }
    gradosServo5 = i;
    delay(1000);

    // Volver de la rutina de inducción a home
    for(i=gradosServo2; i<90; i++){
        delay(15);
        servo2.write(i);
    }
    gradosServo2 = i;
    delay(500);
    for(i=gradosServo1; i<100; i++){
        delay(15);
        servo1.write(i);
    }
    gradosServo1 = i;
}

void rutinaPlastico(){
    int i;
    for(i=gradosServo3; i>50; i--){
        delay(15);
        servo3.write(i);
    }
    gradosServo3 = i;
    for(i=gradosServo2; i>60; i--){
        delay(15);
        servo2.write(i);
    }
    gradosServo2 = i;
    delay(200);
    for(i=gradosServo5; i>60; i--){
        delay(15);
        servo5.write(i);
    }
    gradosServo5 = i;
    delay(200);
    for(i=gradosServo2; i<100; i++){
        delay(15);
        servo2.write(i);
    }
    gradosServo2 = i;
    delay(200);
    for(i=gradosServo1; i<170; i++){
        delay(15);
        servo1.write(i);
    }
    gradosServo1 = i;
    delay(200);
    for(i=gradosServo2; i>40; i--){
        delay(15);
        servo2.write(i);
    }
    gradosServo2 = i;
    delay(200);
    for(i=gradosServo5; i<130; i++){
        delay(15);
        servo5.write(i);
    }
    gradosServo5 = i;
    delay(500);

    // Volvemos de la rutina de plastico al home
    for(i=gradosServo2; i<90; i++){
        delay(15);
        servo2.write(i);
    }
    gradosServo2 = i;
    delay(200);
    for(i=gradosServo1; i>75; i--){
        delay(15);
        servo1.write(i);
    }
    gradosServo1 = i;
    delay(200);
     for(i=gradosServo3; i<80; i++){
        delay(15);
        servo3.write(i);
    }
    gradosServo3 = i;    
}

void loop(){
    // Mandamos a llamar la funcion para arranque y paro
    arranqueParo(paro, estadoParo, estadoParoAnterior);
    arranqueParo(arranque, estadoArranque, estadoArranqueAnterior);

    // Si la maquina esta encendida comienza con el proceso
    if(encendido){
        if(!digitalRead(sensorArranqueBanda) && proceso == 0){
            Serial.println("Banda funcionando");
            banderaSensorArranque = true;
            proceso = 1;
            delay(3000);
            digitalWrite(releMotorreductor, LOW);
        }
        if(banderaSensorArranque){
            Serial.println(digitalRead(sensorInductivo));
            if(!digitalRead(sensorInductivo) && proceso == 1){
                esMetal = true;
                proceso = 2;
                Serial.println("Es metal");
            }
        }
        if(!digitalRead(sensorParoBanda) && (proceso == 2 || proceso == 1)){
            if(esMetal){
                digitalWrite(releMotorreductor, HIGH);
                Serial.println("Ciclo aluminio");
                canidadAluminio++;
                rutinaInduccion();
            }else{
                delay(450);
                digitalWrite(releMotorreductor, HIGH);
                Serial.println("Ciclo plastico");                
                cantidadPlastico++;
                rutinaPlastico();
            }
            finCiclo = true;
        }
        if(finCiclo){
            Serial.println("Termino proceso");
            esMetal = false;
            banderaInductivo = false;
            banderaSensorArranque = false;
            banderaSensorParo = false;
            finCiclo = false;
            proceso = 0;
        }
    }
}
