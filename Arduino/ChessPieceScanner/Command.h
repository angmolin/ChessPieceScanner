#ifndef COMMAND_H
#define COMMAND_H

#include <Arduino.h>

class Command {
  public:
    typedef struct {
      char letter;
      int number;
    } argumentT;
    
    Command();

    Command(unsigned int number, unsigned int argumentsCount, void *handler);

    ~Command();

    bool parse(String command) const;

    void echoReturn(unsigned int value);

    void echo();

  private:
    unsigned int number;

    argumentT *arguments = NULL;
    unsigned int argumentsCount;

    bool echoReturnEnabled = false;
    unsigned int echoReturnValue;

    void (*handler)(unsigned int argumentsCount, argumentT *arguments);
};

#endif
