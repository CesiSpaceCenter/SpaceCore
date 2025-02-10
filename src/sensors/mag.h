#include <LIS3MDL.h>

class MAG
{
private:
    LIS3MDL lis;
public:
    void init();
    typedef LIS3MDL::vector<int16_t> Reading;
    Reading read();
};
