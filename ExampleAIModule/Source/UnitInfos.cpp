#include "UnitInfos.h"

UnitInfos::UnitInfos(int ecoRange, float ecoPow, int milRange, float milPow)
: _ecoRange(ecoRange), _ecoPower(ecoPow), _milRange(milRange), _milPower(milPow)
{
}

UnitInfos::~UnitInfos()
{
}

void  UnitInfos::setEcoPower(float pow)
{
  _ecoPower = pow;
}

void  UnitInfos::setMilPower(float pow)
{
  _milPower = pow;
}

void  UnitInfos::setEcoRange(int range)
{
  _ecoRange = range;
}

void  UnitInfos::setMilRange(int range)
{
  _milRange = range;
}


int UnitInfos::getEcoRange() const
{
  return _ecoRange;
}

int UnitInfos::getMilRange() const
{
  return _milRange;
}

float UnitInfos::getMilPower() const
{
  return _milPower;
}

float UnitInfos::getEcoPower() const
{
  return _ecoPower;
}
