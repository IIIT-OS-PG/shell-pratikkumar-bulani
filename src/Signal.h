/*
 * Signal.h
 *
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <unistd.h>

void alarmHandler(int signalNo){
	char buffer[] = "Reminder Alert! ";

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

//	int status;
//	waitpid(getpid(), &status, WNOHANG);
//	write(STDOUT_FILENO, to_string(status).c_str(), to_string(status).size());
//	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, buffer, 16);
	write(STDOUT_FILENO, asctime(timeinfo), 25);
}
void sigintHandler(int signalNo){
	char buffer[] = "Press Ctrl+Z to exit!\n";
	write(STDOUT_FILENO, buffer, 22);
}
void sigchldHandler(int signalNo){
	//if(tcsetpgrp(0, getpid()) == -1) throw "Error: Fore ground";
}

#endif /* SIGNAL_H_ */
