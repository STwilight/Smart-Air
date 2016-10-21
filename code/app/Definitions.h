/*
 * Definitions.h
 *
 * Заголовочный файл, описывающий специфические определения.
 *
 */

#ifndef APP_DEFINITIONS_H_
#define APP_DEFINITIONS_H_

/* Определение названий выводов для ESP-01 */
#ifdef ESP01
	#define GPIO0	0
	#define GPIO1	1
	#define GPIO2	2
	#define GPIO3	3
#endif

/* Определение названий выводов для ESP-03 */
#ifdef ESP03
	#define GPIO0	0
	#define GPIO1	1
	#define GPIO2	2
	#define GPIO3	3
	#define GPIO12	12
	#define GPIO13	13
	#define GPIO14	14
	#define GPIO15	15
	#define GPIO16	16
#endif

/* Определение названий выводов для ESP-05 */
#ifdef ESP05
	#define GPIO1	1
	#define GPIO3	3
#endif

/* Определение названий выводов для ESP-07/ESP-12/ESP-201 */
#if defined ESP07 or defined ESP12 or defined ESP201
	#define GPIO0	0
	#define GPIO1	1
	#define GPIO2	2
	#define GPIO3	3
	#define GPIO4	4
	#define GPIO5	5
	#define GPIO12	12
	#define GPIO13	13
	#define GPIO14	14
	#define GPIO15	15
	#define GPIO16	16
#endif

/* Определение состояния вывода порта */
#define On		true
#define Off		false

#endif /* APP_DEFINITIONS_H_ */
