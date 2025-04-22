# CESE - TP3 - Testing Unitario

Este repositorio contiene el Trabajo Práctico 3 de la materia Testing de Software Embebido de la carrera de Especialización en Sistemas Embebidos (CESE) de la Universidad de Buenos Aires.

# Desarollo 

En este trabajo se desarrollaron 9 tests unitarios para un módulo (pmic_interface.c) correspondiente a un MSP430, el cual se encarga de manejar la máquina de power de un sistema más general. En estos casos de prueba se evalúan las 9 transiciones principales entre los 4 estados posibles dentro de la máquina de estados.

A continuación se describen brevemente los cuatro estados del módulo:

- **INIT**: Es un estado transitorio antes de que el módulo pase al estado OFF.

- **OFF**: En este estado, el módulo verifica cada 10 ms las entradas en busca de una condición de encendido. Cuando se detecta alguna de estas condiciones, el módulo transiciona al estado ON.

- **ON**: En este estado, el módulo actualiza los temporizadores cada 10 ms y verifica si existe alguna condición de apagado. Si alguno de estos temporizadores se activa, se ejecuta la acción correspondiente. Dependiendo del temporizador, el módulo puede transicionar a IDLE o a INIT.

- **IDLE**: Es un estado intermedio entre ON e INIT, que se alcanza cuando una presión prolongada (LKP) en el botón de encendido genera un reinicio forzado. En este estado, el módulo espera que se libere el botón de encendido para transicionar a INIT. Este estado existe porque, si el LKP generara una transición inmediata a INIT, podría detectarse una condición de encendido no deseada.