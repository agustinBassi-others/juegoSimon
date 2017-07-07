/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-04-26
 */

/*==================[inclusions]=============================================*/

#include "simon_driver.h"   /* <= own header */

#include "sapi.h"            /* <= sAPI header */

/*==================[macros and definitions]=================================*/

typedef enum SoundType{
	SOUND_1,
	SOUND_2,
	SOUND_3,
	SOUND_4,
	SOUND_5,
	SOUND_6,
} SoundType_t;

/*==================[internal data declaration]==============================*/

static uint8_t Skill;
static uint8_t Game;
static uint8_t MaxIndexReached;
static uint8_t BestArray[SECUENCIA_MAX_SIZE];

/*==================[internal functions declaration]=========================*/

static void EnviarSonido 			(SoundType_t soundType);
static void SecuenciaInicio 		(void);
static void ConfigurarPines			(void);
static void EscribirPin				(uint8_t ledNumber, bool_t state);
static void MostrarMejorSecuencia	(void);
static void CambiarJuego			(void);
static void CambiarHabilidad		(void);
static void SetAllLeds 			(bool_t state);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

extern gpioMap_t SimonDriver_GpioMap [];

/*==================[internal functions definition]==========================*/

static void EnviarSonido 			(SoundType_t soundType){
	// todo sacar un PWM por software entre 200 Hz y 20 Khz
}

static void SecuenciaInicio 		(void){
	SetAllLeds(OFF);
	EscribirPin(SIMON_PIN_LED_AMARILLO, ON);
	delay(500);
	EscribirPin(SIMON_PIN_LED_AMARILLO, ON);
	delay(500);
	EscribirPin(SIMON_PIN_LED_AMARILLO, ON);
	delay(500);
	EscribirPin(SIMON_PIN_LED_AMARILLO, ON);
	delay(500);
	SetAllLeds(OFF);
}

static void ConfigurarPines			(void){
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_BUZZER], 			GPIO_OUTPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_LED_AMARILLO], 	GPIO_OUTPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_TEC_AMARILLO], 	GPIO_INPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_TEC_START], 		GPIO_INPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_TEC_GAME], 		GPIO_INPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_TEC_AZUL], 		GPIO_INPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_LED_AZUL], 		GPIO_OUTPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_LED_ROJO], 		GPIO_OUTPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_TEC_ROJO], 		GPIO_INPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_TEC_SKILL], 		GPIO_INPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_TEC_BEST], 		GPIO_INPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_TEC_VERDE], 		GPIO_INPUT );
	gpioConfig( SimonDriver_GpioMap[SIMON_PIN_LED_VERDE], 		GPIO_OUTPUT );
}

/**
 * Unica funcion dependiente del hardware. Cambiar la llamada para otra plataforma que no sea EDU-CIAA.
 * Utilizado para escribir los LEDs
 * @param simonPin pin o LED a escribir.
 * @param pinState ON or OFF.
 */
static void EscribirPin (SimonPin_t ledPin, bool_t pinState){
	if (ledPin == SIMON_PIN_LED_AMARILLO || ledPin == SIMON_PIN_LED_ROJO || ledPin == SIMON_PIN_LED_AZUL || ledPin == SIMON_PIN_LED_VERDE){
		gpioWrite(SimonDriver_GpioMap[ledPin], pinState);
	}
}

/**
 * Unica funcion dependiente del hardware. Cambiar la llamada para otra plataforma que no sea EDU-CIAA.
 * Utilizado para leer las teclas.
 * @param simonPin pin a leer de la placa del juego
 * @return TRUE si el pin esta bajo FALSE si el pin esta alto. 
 */
static bool_t LeerPin (SimonPin_t simonPin){
	bool_t pinState = pinState;
	pinState = !gpioRead( SimonDriver_GpioMap[simonPin])? TRUE : FALSE;
	return pinState;
}

static void SetAllLeds (bool_t state){
	EscribirPin(SIMON_PIN_LED_AMARILLO, state);
	EscribirPin(SIMON_PIN_LED_ROJO, state);
	EscribirPin(SIMON_PIN_LED_AZUL, state);
	EscribirPin(SIMON_PIN_LED_VERDE, state);
}

static void MostrarMejorSecuencia	(void){
	//todo leer el arreglo bestArray e ir prendiendo leds y emitiendo sonido
}

static void CambiarJuego			(void){
	//todo cambiar tipo de juego (no implementado por ahora)
}

static void CambiarHabilidad		(void){
	//todo cambiar velocidad del juego (no implementado por ahora)
}

SimonEvent_t LeerTeclas (){
	SimonEvent_t simonEvent = SIMON_EVENT_NINGUNO;

	if 		(!gpioRead( SimonDriver_GpioMap[SIMON_PIN_TEC_AMARILLO] ))	simonEvent = SIMON_EVENT_TEC_AMARILLO_PULSADA;
	else if (!gpioRead( SimonDriver_GpioMap[SIMON_PIN_TEC_START] ))		simonEvent = SIMON_EVENT_TEC_START_PULSADA;
	else if (!gpioRead( SimonDriver_GpioMap[SIMON_PIN_TEC_GAME] ))		simonEvent = SIMON_EVENT_TEC_GAME_PULSADA;
	else if (!gpioRead( SimonDriver_GpioMap[SIMON_PIN_TEC_AZUL] ))		simonEvent = SIMON_EVENT_TEC_AZUL_PULSADA;
	else if (!gpioRead( SimonDriver_GpioMap[SIMON_PIN_TEC_ROJO] ))		simonEvent = SIMON_EVENT_TEC_ROJO_PULSADA;
	else if (!gpioRead( SimonDriver_GpioMap[SIMON_PIN_TEC_SKILL] ))		simonEvent = SIMON_EVENT_TEC_SKILL_PULSADA;
	else if (!gpioRead( SimonDriver_GpioMap[SIMON_PIN_TEC_BEST] ))		simonEvent = SIMON_EVENT_TEC_BEST_PULSADA;
	else if (!gpioRead( SimonDriver_GpioMap[SIMON_PIN_TEC_VERDE] ))		simonEvent = SIMON_EVENT_TEC_VERDE_PULSADA;

	delay(100);

	return simonEvent;
}

SimonEvent_t LanzarSiguiente (){
//	todo: lanzar un random entre 0 y 3
//	Se seteo el define __RAND_MAX dentro de config.h (en el direcorio gcc-.../arm-none-eabi/include/sys/config.h

SimonEvent_t simonEventToWait;
uint8_t randomValue;

	//randomValue = rand();
	if (++randomValue == 3){
		randomValue = 0;
	}

	SetAllLeds(OFF);

	if 		(randomValue == 0){
		EscribirPin(SIMON_PIN_LED_AMARILLO, ON);
		EnviarSonido(SOUND_1);
		simonEventToWait = SIMON_EVENT_TEC_AMARILLO_PULSADA;
	}
	else if (randomValue == 1){
		EscribirPin(SIMON_PIN_LED_AZUL, ON);
		EnviarSonido(SOUND_1);
		simonEventToWait = SIMON_EVENT_TEC_AZUL_PULSADA;
	}
	else if (randomValue == 2){
		EscribirPin(SIMON_PIN_LED_ROJO, ON);
		EnviarSonido(SOUND_1);
		simonEventToWait = SIMON_EVENT_TEC_ROJO_PULSADA;
	}
	else if (randomValue == 3){
		EscribirPin(SIMON_PIN_LED_VERDE, ON);
		EnviarSonido(SOUND_1);
		simonEventToWait = SIMON_EVENT_TEC_VERDE_PULSADA;
	}

	delay(500);

	SetAllLeds(OFF);

//	BestArray[MaxIndexReached] = randomValue; todo En realidad aca hay que guardar el pin

	return simonEventToWait;

}

/*==================[external functions definition]==========================*/

void SimonDriver_MachineState (SimonEvent_t simonEvent){

static simonState = SIMON_STATE_INICIANDO;
SimonEvent_t teclaLeida, colorEnviado;
bool_t finJuego, changeState;

	switch(simonState){
		case SIMON_STATE_INICIANDO:

			ConfigurarPines();
			SecuenciaInicio();

			simonState = SIMON_STATE_ESPERANDO_TECLAS;
		break;
		case SIMON_STATE_ESPERANDO_TECLAS:
			changeState = FALSE;
			while (!changeState){
				if ((teclaLeida = LeerTeclas()) != SIMON_EVENT_NINGUNO){
					if 		(teclaLeida == SIMON_EVENT_TEC_START_PULSADA) 	simonState = SIMON_STATE_LANZANDO_JUEGO;
					else if (teclaLeida == SIMON_EVENT_TEC_SKILL_PULSADA) 	simonState = SIMON_STATE_CAMBIANDO_SKILL;
					else if (teclaLeida == SIMON_EVENT_TEC_BEST_PULSADA) 	simonState = SIMON_STATE_MOSTRANDO_BEST;
					else if (teclaLeida == SIMON_EVENT_TEC_GAME_PULSADA) 	simonState = SIMON_STATE_CAMBIANDO_JUEGO;
					changeState = TRUE;
				}
			}
		break;
		case SIMON_STATE_MOSTRANDO_BEST:
			MostrarMejorSecuencia();
			simonState = SIMON_STATE_ESPERANDO_TECLAS;
		break;
		case SIMON_STATE_CAMBIANDO_JUEGO:
			CambiarJuego();
			simonState = SIMON_STATE_ESPERANDO_TECLAS;
		break;
		case SIMON_STATE_CAMBIANDO_SKILL:
			CambiarHabilidad();
			simonState = SIMON_STATE_ESPERANDO_TECLAS;
		break;
		case SIMON_STATE_LANZANDO_JUEGO:
			finJuego = FALSE;
			while (!finJuego){
				colorEnviado = LanzarSiguiente();
				while ((teclaLeida = LeerTeclas()) != SIMON_EVENT_NINGUNO) ;	//se queda esperando leer una tecla valida
				if (teclaLeida != colorEnviado){
					simonState = SIMON_STATE_FINALIZANDO_JUEGO;
					finJuego = TRUE;
				}
			}

		break;
		case SIMON_STATE_FINALIZANDO_JUEGO:

			SetAllLeds(ON);
			delay(1000);
			SetAllLeds(OFF);
			delay(1000);
			simonState = SIMON_STATE_ESPERANDO_TECLAS;
		break;
		default:
			simonState = SIMON_STATE_ESPERANDO_TECLAS;
		break;
	}
}

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook */
   tickConfig( 1, 0 );

   /* Inicializar GPIOs */
   gpioConfig( 0, GPIO_ENABLE );
   gpioConfig( 1, GPIO_ENABLE );
   gpioConfig( 2, GPIO_ENABLE );
   gpioConfig( 3, GPIO_ENABLE );
   gpioConfig( 4, GPIO_ENABLE );
   gpioConfig( 5, GPIO_ENABLE );



   while (1){
	   SimonDriver_MachineState(SIMON_STATE_INICIANDO);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/


//   /* Configuración de pines de entrada para Teclas de la CIAA-NXP */
//   gpioConfig( TEC1, GPIO_INPUT );
//   gpioConfig( TEC2, GPIO_INPUT );
//   gpioConfig( TEC3, GPIO_INPUT );
//   gpioConfig( TEC4, GPIO_INPUT );
//
//   /* Configuración de pines de salida para Leds de la CIAA-NXP */
//   gpioConfig( LEDR, GPIO_OUTPUT );
//   gpioConfig( LEDG, GPIO_OUTPUT );
//   gpioConfig( LEDB, GPIO_OUTPUT );
//   gpioConfig( LED1, GPIO_OUTPUT );
//   gpioConfig( LED2, GPIO_OUTPUT );
//   gpioConfig( LED3, GPIO_OUTPUT );

//   /* Variable de Retardo no bloqueante */
//   delay_t delay;//
//   /* Inicializar Retardo no bloqueante con tiempo en milisegundos
//      (500ms = 0,5s) */
//   delayConfig( &delay, 500 );
/* 	delayRead retorna TRUE cuando se cumple el tiempo de retardo */
//   if ( delayRead( &delay ) )
