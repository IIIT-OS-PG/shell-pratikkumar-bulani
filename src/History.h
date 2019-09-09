/*
 * History.h
 *
 */

#ifndef HISTORY_H_
#define HISTORY_H_

#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#define MYRCHISTORYPATH "/home/bullu/ShellBuiltins/.myrc_history"

using namespace std;

void addToHistory(string& command){
	int fd = open(MYRCHISTORYPATH, O_APPEND|O_WRONLY|O_CREAT, 0777);
	if(fd == -1) throw "Error: Unable to open history file";
	char n='\n';
	write(fd, command.c_str(), command.size());
	write(fd, &n, 1);
	close(fd);
}


#endif /* HISTORY_H_ */
