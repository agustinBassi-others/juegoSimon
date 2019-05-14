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

#define SKILL_DELAY_MIN 	100
#define SKILL_DELAY_MAX 	1000
#define SKILL_DELAY_STEP 	100

typedef enum SoundType{
	SOUND_1,
	SOUND_2,
	SOUND_3,
	SOUND_4,
	SOUND_5,
	SOUND_6,
} SoundType_t;

/*==================[internal data declaration]==============================*/

static uint32_t SkillDelay = SKILL_DELAY_MAX;
static uint8_t GameType;
static uint8_t CurrentPlayElements;
static uint8_t CurrentPLay[NUMBER_OF_MAX_PLAYS];

/*==================[internal functions declaration]=========================*/

static void SendSound (SoundType_t soundType);
//static void InitSecuence 		(void);
static void InitSimonBoard (void);
static void ConfigSimonPin (SimonPin_t simonPin, SimontPinConfig_t pinConfig);
static void WriteSimonPin (uint8_t ledNumber, bool_t state);
static void ShowBestGame (void);
static void ChangeGame (void);
static void ChangeSkill (void);
static void SetAllLeds (bool_t state);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

extern gpioMap_t SimonDriver_GpioMap [];

/*==================[internal functions definition]==========================*/

/******************* CAPA HAL (depende de sAPI) ******************************/

/**
 * Configura un pin como entrada o salida. Junto con las de leer y escribir pin son las unicas que llaman a sAPI
 * @param simonPin pin del juego Simon
 * @param pinConfig configurado como entrada SIMON_PIN_INPUT o salida SIMON_PIN_OUTPUT
 */
static void ConfigSimonPin(SimonPin_t simonPin, SimontPinConfig_t pinConfig){
	gpioConfig( SimonDriver_GpioMap[simonPin], 	pinConfig );
}

/**
 * Unica funcion dependiente del hardware. Cambiar la llamada para otra plataforma que no sea EDU-CIAA.
 * Utilizado para escribir los LEDs
 * @param simonPin pin o LED a escribir.
 * @param pinState ON or OFF.
 */
static void WriteSimonPin(SimonPin_t ledPin, bool_t pinState){
	if (ledPin == SIMON_PIN_LED_AMARILLO || ledPin == SIMON_PIN_LED_ROJO || 
	    ledPin == SIMON_PIN_LED_AZUL || ledPin == SIMON_PIN_LED_VERDE){
		gpioWrite(SimonDriver_GpioMap[ledPin], pinState);
	}
}

/**
 * Unica funcion dependiente del hardware. Cambiar la llamada para otra plataforma que no sea EDU-CIAA.
 * Utilizado para leer las teclas.
 * @param simonPin pin a leer de la placa del juego
 * @return TRUE si el pin esta bajo FALSE si el pin esta alto.
 */
static bool_t ReadSimonPin (SimonPin_t simonPin){
	bool_t pinState = pinState;
	pinState = !gpioRead( SimonDriver_GpioMap[simonPin])? TRUE : FALSE;
	return pinState;
}


/******************* CAPA DRIVER DE LA APLICACION ******************************/

static void InitSimonBoard (void){
	/* Configurar pines del juego Simon. */
	ConfigSimonPin(SIMON_PIN_BUZZER, 		GPIO_OUTPUT );
	ConfigSimonPin(SIMON_PIN_LED_AMARILLO, 	GPIO_OUTPUT );
	ConfigSimonPin(SIMON_PIN_TEC_AMARILLO, 	GPIO_INPUT );
	ConfigSimonPin(SIMON_PIN_TEC_START, 	GPIO_INPUT );
	ConfigSimonPin(SIMON_PIN_TEC_GAME, 		GPIO_INPUT );
	ConfigSimonPin(SIMON_PIN_TEC_AZUL, 		GPIO_INPUT );
	ConfigSimonPin(SIMON_PIN_LED_AZUL, 		GPIO_OUTPUT );
	ConfigSimonPin(SIMON_PIN_LED_ROJO, 		GPIO_OUTPUT );
	ConfigSimonPin(SIMON_PIN_TEC_ROJO, 		GPIO_INPUT );
	ConfigSimonPin(SIMON_PIN_TEC_SKILL, 	GPIO_INPUT );
	ConfigSimonPin(SIMON_PIN_TEC_BEST, 		GPIO_INPUT );
	ConfigSimonPin(SIMON_PIN_TEC_VERDE, 	GPIO_INPUT );
	ConfigSimonPin(SIMON_PIN_LED_VERDE, 	GPIO_OUTPUT );
}

SimonEvent_t ReadButtons(){
	SimonEvent_t simonEvent = SIMON_EVENT_NONE;

	if(ReadSimonPin (SIMON_PIN_TEC_AMARILLO )){
		simonEvent = SIMON_EVENT_TEC_AMARILLO_PULSADA;
	}	
	else if(ReadSimonPin (SIMON_PIN_TEC_START )){	
		simonEvent = SIMON_EVENT_TEC_START_PULSADA;
	}
	else if(ReadSimonPin (SIMON_PIN_TEC_GAME )){
		simonEvent = SIMON_EVENT_TEC_GAME_PULSADA;
	}
	else if(ReadSimonPin (SIMON_PIN_TEC_AZUL )){
		simonEvent = SIMON_EVENT_TEC_AZUL_PULSADA;
	}
	else if(ReadSimonPin (SIMON_PIN_TEC_ROJO )){
		simonEvent = SIMON_EVENT_TEC_ROJO_PULSADA;
	}
	else if(ReadSimonPin (SIMON_PIN_TEC_SKILL )){
		simonEvent = SIMON_EVENT_TEC_SKILL_PULSADA;
	}
	else if(ReadSimonPin (SIMON_PIN_TEC_BEST )){
		simonEvent = SIMON_EVENT_TEC_BEST_PULSADA;
	}
	else if(ReadSimonPin (SIMON_PIN_TEC_VERDE )){
		simonEvent = SIMON_EVENT_TEC_VERDE_PULSADA;
	}
	delay(100);
	//todo; hacer un parche para detectar si es la tecla roja o skill.
	return simonEvent;
}

static void SetAllLeds(bool_t state){
	WriteSimonPin(SIMON_PIN_LED_AMARILLO, state);
	WriteSimonPin(SIMON_PIN_LED_ROJO, state);
	WriteSimonPin(SIMON_PIN_LED_AZUL, state);
	WriteSimonPin(SIMON_PIN_LED_VERDE, state);
}

static void SendSound(SoundType_t soundType){
	// todo sacar un PWM por software entre 200 Hz y 20 Khz

}

/******************* CAPA DEL JUEGO PROPIAMENTE DICHA ******************************/

static void InitSecuence(void){
	SetAllLeds(OFF);
	WriteSimonPin(SIMON_PIN_LED_AMARILLO, ON);
	delay(500);
	WriteSimonPin(SIMON_PIN_LED_AZUL, ON);
	delay(500);
	WriteSimonPin(SIMON_PIN_LED_ROJO, ON);
	delay(500);
	WriteSimonPin(SIMON_PIN_LED_VERDE, ON);
	delay(500);
	SetAllLeds(OFF);
}

static void ShowBestGame(void){
	//todo leer el arreglo bestArray e ir prendiendo leds y emitiendo sonido
}

static void	ChangeGame(void){
	//todo cambiar tipo de juego (no implementado por ahora)
}

static void ChangeSkill(void){
	delay(500);
	SkillDelay = SkillDelay - SKILL_DELAY_STEP;
	if (SkillDelay <= SKILL_DELAY_MIN){
		SkillDelay = SKILL_DELAY_MAX;
	}
	SetAllLeds(OFF);
	SetAllLeds(TRUE);
	delay(SkillDelay);
	SetAllLeds(OFF);
	//todo cambiar velocidad del juego (no implementado por ahora)
}

static bool_t GetCurrentPlay(){
	static uint8_t elementIndex = 0;
	SimonEvent_t eventOcurred = SIMON_EVENT_NONE;
	bool_t endOfGame = FALSE;

	for(elementIndex = 0; elementIndex < CurrentPlayElements && endOfGame == FALSE; elementIndex++){
		/** Se queda esperando que se apriete una tecla. */
		while ((eventOcurred = ReadButtons()) == SIMON_EVENT_NONE) ;
		/** Si la tecla recibida es distinta a la que se lanzo en la jugada deja de esperar mas teclas. */
		if (eventOcurred != CurrentPLay[elementIndex]){
			/** Reinicia el contador global de la jugada a cero. */
			CurrentPlayElements = 0;
			/** Haciendo eso fuerza la condicion de salida, ya que no es mas menor que playElements. */
			endOfGame = TRUE;
			bzero(CurrentPLay, NUMBER_OF_MAX_PLAYS);
		}
		delay(200);
	}
	return endOfGame;
}

void LaunchNextPlay(){
//	Se seteo el define __RAND_MAX dentro de config.h (en el direcorio gcc-.../arm-none-eabi/include/sys/config.h
static uint8_t randomValue, elementIndex;

	//randomValue = rand(); todo: hay que poder enviar resultados pseudoaleatorios.
	if (++randomValue == 4){
		randomValue = 0;
	}
	//Guarda el numero aleatorio que salio en el historial de la jugada
	if 		(randomValue == 0) CurrentPLay[CurrentPlayElements] = SIMON_EVENT_TEC_AMARILLO_PULSADA;
	else if (randomValue == 1) CurrentPLay[CurrentPlayElements] = SIMON_EVENT_TEC_AZUL_PULSADA;
	else if (randomValue == 2) CurrentPLay[CurrentPlayElements] = SIMON_EVENT_TEC_ROJO_PULSADA;
	else if (randomValue == 3) CurrentPLay[CurrentPlayElements] = SIMON_EVENT_TEC_VERDE_PULSADA;
	CurrentPlayElements++;
	/** Enciende los LEDs en funcion del numero random que salio. */
	for (elementIndex = 0; elementIndex < CurrentPlayElements; elementIndex++){
		SetAllLeds(OFF);
		if 		(CurrentPLay[elementIndex] == SIMON_EVENT_TEC_AMARILLO_PULSADA)		WriteSimonPin(SIMON_PIN_LED_AMARILLO, ON);
		else if (CurrentPLay[elementIndex] == SIMON_EVENT_TEC_AZUL_PULSADA)			WriteSimonPin(SIMON_PIN_LED_AZUL, ON);
		else if (CurrentPLay[elementIndex] == SIMON_EVENT_TEC_ROJO_PULSADA)			WriteSimonPin(SIMON_PIN_LED_ROJO, ON);
		else if (CurrentPLay[elementIndex] == SIMON_EVENT_TEC_VERDE_PULSADA)			WriteSimonPin(SIMON_PIN_LED_VERDE, ON);
		delay(SkillDelay);
	}
	SetAllLeds(OFF);
}

/*==================[external functions definition]==========================*/

void SimonDriver_MachineState(){

static SimonState_t simonState = SIMON_STATE_INIT;
SimonEvent_t eventOcurred = SIMON_EVENT_NONE;
bool_t endOfGame = TRUE;

	switch(simonState){
		case SIMON_STATE_INIT:
			InitSimonBoard();
			InitSecuence();
			simonState = SIMON_STATE_WAIT_BUTTON;
		break;
		case SIMON_STATE_WAIT_BUTTON:
			while (simonState == SIMON_STATE_WAIT_BUTTON){
				eventOcurred = ReadButtons();
				if 		(eventOcurred == SIMON_EVENT_TEC_START_PULSADA) 	simonState = SIMON_STATE_LAUNCH_GAME;
				else if (eventOcurred == SIMON_EVENT_TEC_SKILL_PULSADA) 	simonState = SIMON_STATE_CHANGE_SKILL;
				else if (eventOcurred == SIMON_EVENT_TEC_BEST_PULSADA) 		simonState = SIMON_STATE_SHOW_BEST;
				else if (eventOcurred == SIMON_EVENT_TEC_GAME_PULSADA) 		simonState = SIMON_STATE_CHANGE_SKILL;// OJO: PARCHE DE PRUEBA simonState = SIMON_STATE_CHANGE_GAME;
			}

		break;
		case SIMON_STATE_SHOW_BEST:
			ShowBestGame();
			simonState = SIMON_STATE_WAIT_BUTTON;
		break;
		case SIMON_STATE_CHANGE_GAME:
			ChangeGame();
			simonState = SIMON_STATE_WAIT_BUTTON;
		break;
		case SIMON_STATE_CHANGE_SKILL:
			ChangeSkill();
			simonState = SIMON_STATE_WAIT_BUTTON;
		break;
		case SIMON_STATE_LAUNCH_GAME:
			delay(500);
			/** Lanzar la siguiente jugada y guardar el historial */
			LaunchNextPlay();
			/** Compara la jugada lanzada con la jugada recibida */
			endOfGame = GetCurrentPlay();
			/** Espera hasta que se termine la jugada por error o cantidad maxima de juegos. */
			if (endOfGame)	simonState = SIMON_STATE_FINISH_GAME;
		break;
		case SIMON_STATE_FINISH_GAME:
			SetAllLeds(ON);
			delay(1000);
			SetAllLeds(OFF);
			delay(1000);
			simonState = SIMON_STATE_WAIT_BUTTON;
		break;
		default:
			simonState = SIMON_STATE_WAIT_BUTTON;
		break;
	}
}

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main (void){
	/* Inicializar la placa */
	boardConfig();
	/* Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook */
	tickConfig( 1, 0 );
	delay(1000);
	/* Inicializar GPIOs */
	gpioConfig( 0, GPIO_ENABLE );
	gpioConfig( 1, GPIO_ENABLE );
	gpioConfig( 2, GPIO_ENABLE );
	gpioConfig( 3, GPIO_ENABLE );
	gpioConfig( 4, GPIO_ENABLE );
	gpioConfig( 5, GPIO_ENABLE );

	while (1){
		SimonDriver_MachineState();
	}
	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	por ningun S.O. */
	return 0 ;
}

/*==================[end of file]============================================*/