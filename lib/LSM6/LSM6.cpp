#include "LSM6.h"
#include <Wire.h>
#include <math.h>

// Defines ////////////////////////////////////////////////////////////////

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define DS33_SA0_HIGH_ADDRESS 0b1101011
#define DS33_SA0_LOW_ADDRESS  0b1101010
#define DSO_SA0_HIGH_ADDRESS  0b1101011
#define DSO_SA0_LOW_ADDRESS   0b1101010

#define TEST_REG_ERROR -1

#define DS33_WHO_ID    0x69
#define DSO_WHO_ID    0x6C

// Constructors ////////////////////////////////////////////////////////////////

LSM6::LSM6()
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
  : bus(&Wire)
#else
  : bus(nullptr)
#endif
  , _device(device_auto)
{
}

// Public Methods //////////////////////////////////////////////////////////////

bool LSM6::init(deviceType device, sa0State sa0)
{
  // perform auto-detection unless device type and SA0 state were both specified
  if (device == device_auto || sa0 == sa0_auto)
  {
    // check for LSM6DS33 if device is unidentified or was specified to be this type
    if (device == device_auto || device == device_DS33)
    {
      // check SA0 high address unless SA0 was specified to be low
      if (sa0 != sa0_low && testReg(DS33_SA0_HIGH_ADDRESS, WHO_AM_I) == DS33_WHO_ID)
      {
        sa0 = sa0_high;
        device = device_DS33;
      }
      // check SA0 low address unless SA0 was specified to be high
      else if (sa0 != sa0_high && testReg(DS33_SA0_LOW_ADDRESS, WHO_AM_I) == DS33_WHO_ID)
      {
        sa0 = sa0_low;
        device = device_DS33;
      }
    }

    // check for LSM6DSO if device is unidentified or was specified to be this type
    if (device == device_auto || device == device_DSO)
    {
      // check SA0 high address unless SA0 was specified to be low
      if (sa0 != sa0_low && testReg(DSO_SA0_HIGH_ADDRESS, WHO_AM_I) == DSO_WHO_ID)
      {
        sa0 = sa0_high;
        device = device_DSO;
      }
      // check SA0 low address unless SA0 was specified to be high
      else if (sa0 != sa0_high && testReg(DSO_SA0_LOW_ADDRESS, WHO_AM_I) == DSO_WHO_ID)
      {
        sa0 = sa0_low;
        device = device_DSO;
      }
    }

    // make sure device and SA0 were successfully detected; otherwise, indicate failure
    if (device == device_auto || sa0 == sa0_auto)
    {
      return false;
    }
  }

  _device = device;

  switch (device)
  {
    case device_DS33:
      address = (sa0 == sa0_high) ? DS33_SA0_HIGH_ADDRESS : DS33_SA0_LOW_ADDRESS;
      break;

    case device_DSO:
      address = (sa0 == sa0_high) ? DSO_SA0_HIGH_ADDRESS : DSO_SA0_LOW_ADDRESS;
      break;

    default:
      return false; // this should not ever happen
  }

  return true;
}

/*
Enables the LSM6's accelerometer and gyro. Also:
- Sets sensor full scales (gain) to default power-on values, which are
  +/- 2 g for accelerometer and 245 dps for gyro
- Selects 1.66 kHz (high performance) ODR (output data rate) for accelerometer
  and 1.66 kHz (high performance) ODR for gyro. (These are the ODR settings for
  which the electrical characteristics are specified in the LSM6DS33 datasheet.)
- Enables automatic increment of register address during multiple byte access
Note that this function will also reset other settings controlled by
the registers it writes to.
*/
void LSM6::enableDefault()
{
  if (_device == device_DS33 || _device == device_DSO)
  {
    // Accelerometer

    // 0x80 = 0b10000000
    // ODR = 1000 (1.66 kHz (high performance)); FS_XL = 00 (+/-2 g full scale)
    writeReg(CTRL1_XL, 0x80);

    // Gyro

    // 0x80 = 0b010000000
    // ODR = 1000 (1.66 kHz (high performance)); FS_G = 00 (245 dps for DS33, 250 dps for DSO)
    writeReg(CTRL2_G, 0x80);

    // Common

    // 0x04 = 0b00000100
    // IF_INC = 1 (automatically increment register address)
    writeReg(CTRL3_C, 0x04);
  }
}

void LSM6::writeReg(uint8_t reg, uint8_t value)
{
  bus->beginTransmission(address);
  bus->write(reg);
  bus->write(value);
  last_status = bus->endTransmission();
}

uint8_t LSM6::readReg(uint8_t reg)
{
  uint8_t value;

  bus->beginTransmission(address);
  bus->write(reg);
  last_status = bus->endTransmission();

  bus->requestFrom(address, (uint8_t)1);
  delay(1);

  value = bus->read();

  return value;
}

// Reads the 3 accelerometer channels and stores them in vector a
void LSM6::readAcc()
{
  bus->beginTransmission(address);
  // automatic increment of register address is enabled by default (IF_INC in CTRL3_C)
  bus->write(OUTX_L_XL);
  bus->endTransmission();

  bus->requestFrom(address, (uint8_t)6);
  delay(1);

  uint8_t xla = bus->read();
  uint8_t xha = bus->read();
  uint8_t yla = bus->read();
  uint8_t yha = bus->read();
  uint8_t zla = bus->read();
  uint8_t zha = bus->read();

  // combine high and low bytes
  a.x = (int16_t)(xha << 8 | xla);
  a.y = (int16_t)(yha << 8 | yla);
  a.z = (int16_t)(zha << 8 | zla);
}

// Reads the 3 gyro channels and stores them in vector g
void LSM6::readGyro()
{
  bus->beginTransmission(address);
  // automatic increment of register address is enabled by default (IF_INC in CTRL3_C)
  bus->write(OUTX_L_G);
  bus->endTransmission();

  bus->requestFrom(address, (uint8_t)6);
  delay(1);

  uint8_t xlg = bus->read();
  uint8_t xhg = bus->read();
  uint8_t ylg = bus->read();
  uint8_t yhg = bus->read();
  uint8_t zlg = bus->read();
  uint8_t zhg = bus->read();

  // combine high and low bytes
  g.x = (int16_t)(xhg << 8 | xlg);
  g.y = (int16_t)(yhg << 8 | ylg);
  g.z = (int16_t)(zhg << 8 | zlg);
}

// Reads all 6 channels of the LSM6 and stores them in the object variables
void LSM6::read()
{
  readAcc();
  readGyro();
}

void LSM6::vector_normalize(vector<float> *a)
{
  float mag = sqrt(vector_dot(a, a));
  a->x /= mag;
  a->y /= mag;
  a->z /= mag;
}

// Private Methods //////////////////////////////////////////////////////////////

int16_t LSM6::testReg(uint8_t address, regAddr reg)
{
  bus->beginTransmission(address);
  bus->write((uint8_t)reg);
  if (bus->endTransmission() != 0)
  {
    return TEST_REG_ERROR;
  }

  bus->requestFrom(address, (uint8_t)1);
  delay(1);

  if (bus->available())
  {
    return bus->read();
  }
  else
  {
    return TEST_REG_ERROR;
  }
}
