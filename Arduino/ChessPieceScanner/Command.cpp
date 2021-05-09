#include "Command.h"

Command::Command() : number(0), argumentsCount(0), handler(NULL) { }

Command::Command(unsigned int number, unsigned int argumentsCount, void *handler) : number(number), argumentsCount(argumentsCount), handler(handler) {
  this->arguments = (argumentT *) malloc(sizeof(argumentT) * this->argumentsCount);
}

Command::~Command() {
  if (this->arguments != NULL)
    free(this->arguments);
}

bool Command::parse(String command) const {
  char commandCharArray[command.length() + 1];
  command.toCharArray(commandCharArray, command.length());

  unsigned int number, i;

  if (sscanf(commandCharArray, "cps%u%s", &number, commandCharArray) >= 1 && number == this->number) {
    for (i = 0; i < this->argumentsCount; i++) {
      this->arguments[i].letter = 0;
      this->arguments[i].number = 0;
      
      if (sscanf(commandCharArray, "%c%i%s", &this->arguments[i].letter, &this->arguments[i].number, commandCharArray) < 1)
        return false;
      else
        this->arguments[i].letter = this->arguments[i].letter - 'a' + 'A';
    }

    if (handler != NULL)
      (*handler)(this->argumentsCount, this->arguments);

    return true;
  }

  return false;
}

void Command::echoReturn(unsigned int value) {
  this->echoReturnEnabled = true;
  this->echoReturnValue = value;
}

void Command::echo() {
  String message;
  unsigned int i;

  if (this->number == 0) {
    message.concat("Chess Piece Scanner\n");
    message.concat("-------------------\n");
    message.concat("Scanner of chess pieces\n");
    message.concat("to train neural networks\n");
    message.concat("-------------------\n");
    message.concat("Copyright Angel Molina Nunez. All rights reserved.");
  }
  else if (this->echoReturnEnabled) {
    message.concat("Return: ");
    message.concat(this->echoReturnValue);
  }

  Serial.print(message);
  Serial.print('\0');
}
