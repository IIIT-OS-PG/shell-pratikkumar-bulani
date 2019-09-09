/*
 * BinaryExecution.h
 *
 */

#ifndef BINARYEXECUTION_H_
#define BINARYEXECUTION_H_

#include <sys/wait.h>
#include <boost/algorithm/string.hpp>
#include <sys/file.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Alias.h"

//#define READ_END 0
//#define WRITE_END 1

using namespace std;

void binaryExecution(char * argv[], int pipeFD[], int countPipes, int processNo, bool background, bool lastProcess = false, int destFD = -1){
	char * new_argv[50];
	int j=0;
	while(argv[j]){
		new_argv[j] = argv[j];
		j++;
	}
	new_argv[j] = 0;
	int pid = fork();
	if(pid < 0) throw "Error: Fork failed";
	if(pid == 0){


		if(lastProcess == false){
			if(dup2(pipeFD[processNo*2+1], STDOUT_FILENO) == -1) throw "Error: Assigning write pipe";
		}

		if(processNo!=0){
			if(dup2(pipeFD[processNo*2-2], STDIN_FILENO) == -1) throw "Error: Assigning read pipe";
		}

		if(lastProcess && destFD!=-1){
			if(dup2(destFD, STDOUT_FILENO) == -1) throw "Error: Assigning destination write pipe";
			close(destFD);
		}

		for(int i=0;i<2*countPipes;i++) close(pipeFD[i]);

		if(lastProcess && background) setpgid(0, 0);

		int err = execvp(new_argv[0], new_argv);

		exit(err);
	}
}

void commandExecution(string& command, int countPipes, bool background){
	if(command.substr(0, 2) == "cd"){
		if(chdir(command.substr(3).c_str()) == -1) throw "Error: Changing directory";
		return;
	} else if(command.substr(0, 2) == "fg"){
		int childProcess = stoi(command.substr(3));
		if( tcsetpgrp(0, childProcess) == -1) throw "Error: Fore ground";
		kill(childProcess, SIGCONT);
		int status;
		wait(&status);
		ALIAS_MAP["$?"] = to_string(status);
		//if( tcsetpgrp(0, getpid()) == -1) throw "Error: Fore ground";
		return;
	} else if(command.substr(0, 5) == "alarm"){
		alarm(stoi(command.substr(6)));
		return;
	} else if(command.substr(0, 5) == "alias"){
		string subCommand = command.substr(6);
		boost::trim(subCommand);
		string key = subCommand.substr(0, subCommand.find("="));
		string value = subCommand.substr(subCommand.find("\'")+1);
		value.pop_back();
		ALIAS_MAP[key] = value;
		return;
	}
	vector<string> splitResult;
	boost::split(splitResult, command, boost::is_any_of(" "));
	int i, j, k;
	int pipeFD[2*countPipes];
	for(i=0;i<countPipes;i++){
		if(pipe(pipeFD+2*i)==-1) throw "Error: Creating pipe";
	}
	char * argv[50]; bool redirection = false;
	for (i = 0, j = 0, k = 0; i < splitResult.size(); i++, j++){
		if(splitResult[i] == "|"){
			argv[j] = 0;
			binaryExecution(argv, pipeFD, countPipes, k, background);
			k++;
			j=-1;
			continue;
		} else if(splitResult[i] == ">>" || splitResult[i] == ">") {
			redirection = true;
			break;
		}
		argv[j] = new char[splitResult[i].size()+1];
		splitResult[i].copy(argv[j], splitResult[i].size());
		argv[j][splitResult[i].size()] = '\0';
	}
	argv[j] = 0;
	int fd;
	if(redirection == false)
		binaryExecution(argv, pipeFD, countPipes, k, background, true);
	else{
		if(splitResult[i] == ">>"){
			fd = open(splitResult[i+1].c_str(), O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
		} else {
			fd = open(splitResult[i+1].c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
		}
		binaryExecution(argv, pipeFD, countPipes, k, background, true, fd);
	}

	for(i=0;i<2*countPipes;i++) close(pipeFD[i]);

	if(background == false){
		int status;
		for(i=0;i<=countPipes;i++){
			wait(&status);
			ALIAS_MAP["$?"] = to_string(status);
		}
	} else {
		for(i=0;i<countPipes;i++){
			int status;
			wait(&status);
			ALIAS_MAP["$?"] = to_string(status);
		}
	}

	if(redirection){
		close(fd);
	}
}

void modifyCommand(string &command){
	command.append(" ");
	command.insert(0, " ");
	for(map<string, string>::iterator i = ALIAS_MAP.begin(); i!=ALIAS_MAP.end(); i++){
		string valueToSearch = i->first;
		if(valueToSearch != "~")
			{valueToSearch.append(" "); valueToSearch.insert(0, " ");}
		string valueToReplace = i->second;
		if(valueToSearch != "~")
			{valueToReplace.append(" "); valueToReplace.insert(0, " ");}
		if(command.find(valueToSearch) != string::npos){
			command.replace(command.find(valueToSearch), valueToSearch.size(), valueToReplace);
		}
	}
	boost::trim(command);
}

#endif /* BINARYEXECUTION_H_ */
