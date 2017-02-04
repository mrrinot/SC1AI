#include "MineralField.h"
#include "ConstGameValues.h"
#include <algorithm>
#include "Logger.hh"

MineralField::MineralField(Unit unit)
: _frameBeforeAvailable(0), _distanceToCC(0), _gathered(false), _unit(unit), _gathererUnit(nullptr), _scheduledUnit(nullptr), _framesCount(0)
{
}

MineralField::~MineralField()
{

}

void MineralField::setDistanceToCC(float distance)
{
  _distanceToCC = distance;
}

float MineralField::getDistanceToCC() const
{
  return _distanceToCC;
}

void MineralField::setGatherer(Unit scv)
{
  LogInfo() << "minid = " << _unit->getID() << " set gatherer " << scv->getID();
  _gathererUnit = scv;
}

bool MineralField::removeGatherer(Unit scv)
{
  if (_gathererUnit == scv)
  {
    _gathererUnit = nullptr;
    return true;
  }
  return false;
}

bool  MineralField::isGathering(Unit scv) const
{
  return scv == _gathererUnit;
}

void MineralField::setWaiter(Unit scv)
{
  _scheduledUnit = scv;
}

bool MineralField::removeWaiter(Unit scv)
{
  if (_scheduledUnit == scv)
  {
    _scheduledUnit = nullptr;
    return true;
  }
  return false;
}

bool  MineralField::isWaiting(Unit scv) const
{
  return scv == _scheduledUnit;
}

int MineralField::getGatheringFrameLeft() const
{
  return _frameBeforeAvailable;
}

void  MineralField::startGathering()
{
  _gathered = true;
  _framesCount = 0;
  _frameBeforeAvailable = MINERAL_GATHERING_FRAME_DURATION;
}

void  MineralField::resetMineralField()
{
  _frameBeforeAvailable = 0;
  _gathered = false;
}

void  MineralField::updateBehaviour()
{
  if (_unit->isBeingGathered() == true && _gathered == false)
    startGathering();
  if (--_frameBeforeAvailable <= 0)
  {
    _frameBeforeAvailable = 0;
    _gathered = false;
  }
  ++_framesCount;
  if (_unit->isBeingGathered() == false && _gathered == true)
  {
  }
}

const Unit  &MineralField::getUnit() const
{
  return _unit;
}

const Unit  &MineralField::getGatherer() const
{
  return _gathererUnit;
}

const Unit  &MineralField::getWaiter() const
{
  return _scheduledUnit;
}

bool  MineralField::operator==(const Unit &other)
{
  return other == _unit;
}

bool  MineralField::operator==(const MineralField &other)
{
  return other._unit == _unit;
}