#include <LPS.h>

class PS
{
private:
    LPS lps;

public:
    void init();
    struct Reading
    {
        float pres;
        float alt;
        float temp;
    };
    Reading read();
};
