/*
 * InitialData.h
 *
 */

#ifndef INITIALDATA_H_
#define INITIALDATA_H_

#include <boost/format.hpp>
#include "BinaryExecution.h"
#include "Signal.h"
#include "Alias.h"
#include "pwd.h"

#define MYRCPATH "/home/bullu/ShellBuiltins/.myrc"

using namespace std;

string USER, PS1_FORMAT = "%1%@%2%:%3%%4% ", HOSTNAME, PATH, PWD, HOME;

void sigcontHandler(int);

void initializePath(){
	PATH = "";
	int myrcFD = open(MYRCPATH, O_RDONLY);
	char buffer[4096]; int countChar;
	while((countChar = read(myrcFD, buffer, 4095)) != 0){
		buffer[countChar] = '\0';
		PATH.append(buffer);
	}
	char envPath[PATH.size()+1];
	PATH.copy(envPath, PATH.size());
	envPath[PATH.size()] = '\0';
	putenv(envPath);
	close(myrcFD);
}

void initializeUser(){
	USER = "";
	char buffer[100];
	if(getlogin_r(buffer, 100) != 0){
		throw "Error: Fetch user name";
	}
	USER.append(buffer);
}

void initializeHostname(){
	HOSTNAME = "";
	char buffer[100];
	if(gethostname(buffer, 100) != 0) throw "Error: Fetch host name";
	HOSTNAME.append(buffer);
}

void initializePwd(){
	PWD = "";
	char buffer[500];
	if(getcwd(buffer, 500) == NULL) throw "Error: Fetch pwd";
	PWD.append(buffer);
}

void initializeHome(){
	HOME = string(getpwuid(getuid())->pw_dir);
}

void initialize(){
	initializePath();
	initializeUser();
	initializeHostname();
	initializePwd();
	initializeHome();

	signal(SIGALRM, alarmHandler);
	signal(SIGINT, sigintHandler);
	signal(SIGCHLD, sigchldHandler);
	signal(SIGCONT, sigcontHandler);
	string command = "stty tostop";
	commandExecution(command, 0, false);
	command = "stty -icanon min 1 time 0";
	commandExecution(command, 0, false);
	command = "stty -echo";
	commandExecution(command, 0, false);
	ALIAS_MAP["$$"] = to_string(getpid());
	ALIAS_MAP["$PATH"] = PATH;
	ALIAS_MAP["$PWD"] = PWD;
	ALIAS_MAP["$USER"] = USER;
	ALIAS_MAP["$HOSTNAME"] = HOSTNAME;
	ALIAS_MAP["$HOME"] = HOME;
	ALIAS_MAP["$PS1"] = PS1_FORMAT;
	ALIAS_MAP["~"] = HOME;
}

string createPS1(){
	string PS1 = "";
	initializeUser(); initializePwd(); initializeHome();

	ALIAS_MAP["$USER"] = USER; ALIAS_MAP["$PWD"] = PWD; ALIAS_MAP["$HOME"] = HOME; ALIAS_MAP["~"] = HOME;

	PS1 = (boost::format(PS1_FORMAT) % USER % HOSTNAME % PWD % ((USER=="root")?"#":"$")).str();
	return PS1;
}

void sigcontHandler(int signalNo){
	initialize();
}

#endif /* INITIALDATA_H_ */
