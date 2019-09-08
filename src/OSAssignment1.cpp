#include "NonCanonicalModeTerminal.h"
#include "BinaryExecution.h"
#include "InitialData.h"

using namespace std;

int main() {
	//set_input_mode();
	initialize();
	string command = "";
	char c[2] = {' ', '\0'};
	int countPipes; bool background;
	while(true){
		countPipes = 0;
		string prompt = createPS1();
		write(STDOUT_FILENO, prompt.c_str(), prompt.size());
		background = false;
		read(STDIN_FILENO, c, 1);
		while(c[0]!='\n'){
			//cout<<int(c[0])<<endl;
			//if(c[0] == 9) cout<<"tabpressed\n";
			//else if(c[0] == 127) cout << '\b';
			//write(STDOUT_FILENO, c, 1);
			if(c[0] == '|') countPipes++;
			if(c[0] == '&') background = true;
			else command.append(c);
			read(STDIN_FILENO, c, 1);
		}
		//cout << "\n";
		boost::trim(command);
		if(command == "exit"){
			break;
		}
		modifyCommand(command);
		commandExecution(command, countPipes, background);
		command = "";
	}
	return 0;
}
