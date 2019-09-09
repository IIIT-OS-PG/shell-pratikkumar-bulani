/*
 * DestroyTerminal.h
 *
 *  Created on: 08-Sep-2019
 *      Author: bullu
 */

#ifndef DESTROYTERMINAL_H_
#define DESTROYTERMINAL_H_

#include "BinaryExecution.h"

void destroy(){
	string command = "stty icanon";
	commandExecution(command, 0, false);
	command = "stty echo";
	commandExecution(command, 0, false);
}

#endif /* DESTROYTERMINAL_H_ */
