#include "UnitManager.h"

UnitManager::UnitManager()
{
}

UnitManager::~UnitManager()
{
  for (auto &&type : _allyUnitMap)
    for (auto &&unit : type.second)
      delete unit;
}

std::vector<UnitHandle *> &UnitManager::getAllAllyUnitsOfType(UnitType type)
{
  return _allyUnitMap[type];
}

const std::map<UnitType, std::vector<UnitHandle *>> &UnitManager::getAllAllyUnits() const
{
  return _allyUnitMap;
}

void  UnitManager::addUnit(UnitType type, Unit unit)
{
  _allyUnitMap[type].push_back(new UnitHandle(unit));
}

void  UnitManager::removeUnit(UnitType type, Unit unit)
{
  for (auto &&type : _allyUnitMap)
  {
    for (auto handle = type.second.begin(); handle != type.second.end(); ++handle)
    {
      if ((*handle)->getUnit() == unit)
      {
        delete *handle;
        type.second.erase(handle);
      }
    }
  }
}

void  UnitManager::updatePreviousStates()
{
  for (auto &&type : _allyUnitMap)
  for (auto &&unit : type.second)
    unit->updateState();
}