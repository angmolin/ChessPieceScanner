#include "configuration.h"

Configuration::Configuration(const char *path)
{
    this->file = new std::ifstream(path);
}

Configuration::~Configuration()
{
    if (this->file != NULL)
    {
        if (this->file->is_open())
        {
            this->file->close();
        }

        delete this->file;
    }
}

bool Configuration::next()
{
    bool result = false;

    if (this->file != NULL && this->file->is_open() && !this->file->eof())
    {
        memset(line, '\0', CONFIGURATION_LINE_MAX_LENGHT);

        while (!this->file->eof() && (strlen(this->line) == 0 || this->line[0] == '#'))
        {
            this->file->getline(this->line, CONFIGURATION_LINE_MAX_LENGHT);
        }

        result = true;
    }

    return result;
}

char *Configuration::getline()
{
    return this->line;
}