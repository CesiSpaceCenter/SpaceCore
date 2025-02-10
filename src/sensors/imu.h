#include <LSM6.h>

class IMU
{
private:
    LSM6 lsm;
public:
    void init();
    struct Reading
    {
        LSM6::vector<int16_t> acc;
        LSM6::vector<int16_t> gyr;
    };
    Reading read();
};
