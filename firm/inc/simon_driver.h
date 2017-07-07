/* Copyright 2015-2016, Eric Pernia.
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

/* Date: 2015-09-23 */

#ifndef _SIMON_DRIVER_H_
#define _SIMON_DRIVER_H_

/*==================[inclusions]=============================================*/

#include "sapi.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

typedef enum SimonState {
	SIMON_STATE_INICIANDO,
	SIMON_STATE_ESPERANDO_TECLAS,
	SIMON_STATE_MOSTRANDO_BEST,
	SIMON_STATE_CAMBIANDO_SKILL,
	SIMON_STATE_CAMBIANDO_JUEGO,
	SIMON_STATE_LANZANDO_JUEGO,
	SIMON_STATE_FINALIZANDO_JUEGO
} SimonState_t;

typedef enum SimonEvent {
	SIMON_EVENT_NINGUNO,
	SIMON_EVENT_TEC_AMARILLO_PULSADA,
	SIMON_EVENT_TEC_VERDE_PULSADA,
	SIMON_EVENT_TEC_AZUL_PULSADA,
	SIMON_EVENT_TEC_ROJO_PULSADA,
	SIMON_EVENT_TEC_SKILL_PULSADA,
	SIMON_EVENT_TEC_START_PULSADA,
	SIMON_EVENT_TEC_BEST_PULSADA,
	SIMON_EVENT_TEC_GAME_PULSADA
} SimonEvent_t;

typedef enum SimonPin{
	SIMON_PIN_BUZZER,
	SIMON_PIN_LED_AMARILLO,
	SIMON_PIN_TEC_AMARILLO,
	SIMON_PIN_TEC_START,
	SIMON_PIN_TEC_GAME,
	SIMON_PIN_TEC_AZUL,
	SIMON_PIN_LED_AZUL,
	SIMON_PIN_LED_ROJO,
	SIMON_PIN_TEC_ROJO,
	SIMON_PIN_TEC_SKILL,
	SIMON_PIN_TEC_BEST,
	SIMON_PIN_TEC_VERDE,
	SIMON_PIN_LED_VERDE,
} SimonPin_t;

gpioMap_t SimonDriver_GpioMap [] = {
		ENET_RXD0,//SIMON_PIN_BUZZER
		SPI_MISO,//SIMON_PIN_LED_AMARILLO
		ENET_RXD1,//SIMON_PIN_TEC_AMARILLO //En realidad va SPI_SCK,
		LCD4,//SIMON_PIN_TEC_START
		LCDRS,//SIMON_PIN_TEC_GAME
		LCD3,//SIMON_PIN_TEC_AZUL
		LCD2,//SIMON_PIN_LED_AZUL
		GPIO3,//SIMON_PIN_LED_ROJO
		GPIO5,//SIMON_PIN_TEC_ROJO
		GPIO7,//SIMON_PIN_TEC_SKILL
		GPIO8,//SIMON_PIN_TEC_BEST
		GPIO6,//SIMON_PIN_TEC_VERDE
		GPIO4//SIMON_PIN_LED_VERDE
};

#define SECUENCIA_MAX_SIZE			20

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

void SimonDriver_MachineState (SimonEvent_t simonEvent);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _SIMON_DRIVER_H_ */
