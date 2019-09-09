#include "BinaryExecution.h"
#include "InitialData.h"
#include "History.h"
#include "DestroyTerminal.h"
#include "Trie.h"

using namespace std;

int main() {
	initialize();
	string command, prefixCommand; char commandBuffer[100]; int indexBuffer;
	char scannedCharacter;
	int countPipes; bool background;
	while(true){
		countPipes = 0; string prompt = createPS1(); background = false; indexBuffer = 0; command = "", prefixCommand = "";
		for(int i=0;i<100;i++) commandBuffer[i] = '\0';

		write(STDOUT_FILENO, prompt.c_str(), prompt.size());

		while(true){
			read(STDIN_FILENO, &scannedCharacter, 1);
			if(scannedCharacter == '\n'){
				write(STDOUT_FILENO, &scannedCharacter, 1);
				command = string(commandBuffer);
				if(command.find("&") != string::npos){
					command.replace(command.find("&"), 1, "");
				}
				boost::trim(command);
				break;
			} else if(scannedCharacter == 127){
				if(indexBuffer > 0){
					if(commandBuffer[indexBuffer] == '|') countPipes--;
					else if(commandBuffer[indexBuffer] == '&') background = false;
					write(STDOUT_FILENO, "\b \b", 3);
					commandBuffer[indexBuffer--] = '\0';
				}
			} else if(scannedCharacter == 9){
				write(STDOUT_FILENO, "\n", 1);
				prefixCommand = string(commandBuffer);
				vector<string> prefixes = findPrefixStrings(prefixCommand);
				for(int i=0;i<prefixes.size();i++){
					write(STDOUT_FILENO, prefixes[i].c_str(), prefixes[i].size());
					write(STDOUT_FILENO, "\n", 1);
				}
				write(STDOUT_FILENO, prompt.c_str(), prompt.size());
				write(STDOUT_FILENO, commandBuffer, indexBuffer);
			} else {
				commandBuffer[indexBuffer++] = scannedCharacter;
				commandBuffer[indexBuffer] = '\0';
				write(STDOUT_FILENO, &scannedCharacter, 1);
				if(scannedCharacter == '|') countPipes++;
				else if(scannedCharacter == '&') background = true;
			}
		}
		if(command == "exit") break;
		addToHistory(command);
		modifyCommand(command);
		commandExecution(command, countPipes, background);
	}
	destroy();
	return 0;
}
