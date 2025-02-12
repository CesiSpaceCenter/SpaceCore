#include <Arduino.h>
#include <AmebaFatFS.h>

class DataFile
{
private:
    AmebaFatFS fs;
    File file;

public:
    DataFile();
    void write(String data);
};