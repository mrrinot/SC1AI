#include "EnemyManager.h"
#include "HeuristicManager.h"
#include "Logger.hh"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
  for (auto &&type : _inSightUnitMap)
  {
    for (auto &&unit : type.second)
    {
      delete unit;
    }
  }
  for (auto &&type : _seenUnitMap)
  {
    for (auto &&unit : type.second)
    {
      delete unit;
    }
  }
}

void  EnemyManager::hideUnit(Unit u)
{
  LogInfo() << "HIDE ENEMY UNIT: " << u->getID() << " type = " << u->getType();
  for (auto &&type : _inSightBuildingMap)
  {
    for (auto unit = type.second.begin(); unit != type.second.end(); ++unit)
    {
      if ((*unit)->getUnit()->getID() == u->getID())
      {
        LogInfo() << "TWAS A BUILDING " << type.first;
        _seenBuildingMap[type.first].push_back((*unit));
        type.second.erase(unit);
        LogInfo() << "END OF FUNC";
        return;
      }
    }
  }
  for (auto &&type : _inSightUnitMap)
  {
    for (auto unit = type.second.begin(); unit != type.second.end(); ++unit)
    {
      if ((*unit)->getUnit()->getID() == u->getID())
      {
        LogInfo() << "TWAS AN UNIT " << type.first;
        _seenUnitMap[type.first].push_back((*unit));
        type.second.erase(unit);
        LogInfo() << "END OF FUNC";
        return;
      }
    }
  }
}

bool  EnemyManager::showUnit(Unit u)
{
  bool found = false;
  LogInfo() << "SHOW ENEMY UNIT: " << u->getID();

  if (u->getType().isBuilding())
  {
    for (auto type = _seenBuildingMap.begin(); type != _seenBuildingMap.end() && found == false; ++type)
    {
      for (auto unit = (*type).second.begin(); unit != (*type).second.end(); ++unit)
      {
        if ((*unit)->getUnit()->getID() == u->getID())
        {
          _inSightBuildingMap[(*type).first].push_back((*unit));
          (*type).second.erase(unit);
          found = true;
          break;
        }
      }
    }
    if (!found) // addUnit
    {
      LogInfo() << "ADD NEW ENEMY UNIT: " << u->getID();
      _inSightBuildingMap[u->getType()].push_back(new BuildingHandle(u->getTilePosition(), nullptr));
      _inSightBuildingMap[u->getType()].back()->setUnit(u);
    }
  }
  else
  {
    for (auto type = _seenUnitMap.begin(); type != _seenUnitMap.end() && found == false; ++type)
    {
      for (auto unit = (*type).second.begin(); unit != (*type).second.end(); ++unit)
      {
        if ((*unit)->getUnit()->getID() == u->getID())
        {
          _inSightUnitMap[(*type).first].push_back((*unit));
          (*type).second.erase(unit);
          found = true;
          break;
        }
      }
    }
    if (!found) // addUnit
    {
      LogInfo() << "ADD NEW ENEMY UNIT: " << u->getID();
      _inSightUnitMap[u->getType()].push_back(new UnitHandle(u));
    }
  }
  return !found;
}

void  EnemyManager::destroyUnit(Unit u)
{
  LogInfo() << "DESTROY ENEMY UNIT: " << u->getID();
  for (auto type = _inSightUnitMap.begin(); type != _inSightUnitMap.end(); ++type)
  {
    for (auto unit = (*type).second.begin(); unit != (*type).second.end(); ++unit)
    {
      if ((*unit)->getUnit()->getID() == u->getID())
      {
        delete *unit;
        (*type).second.erase(unit);
        return;
      }
    }
  }
}

const std::map<UnitType, std::vector<UnitHandle *>> &EnemyManager::getEnemyInSightUnitList() const
{
  return _inSightUnitMap;
}

const std::map<UnitType, std::vector<UnitHandle *>> &EnemyManager::getEnemySeenUnitList() const
{
  return _seenUnitMap;
}

const std::map<UnitType, std::vector<BuildingHandle *>> &EnemyManager::getEnemyInSightBuildingList() const
{
  return _inSightBuildingMap;
}

const std::map<UnitType, std::vector<BuildingHandle *>> &EnemyManager::getEnemySeenBuildingList() const
{
  return _seenBuildingMap;
}

BuildingHandle *EnemyManager::getEnemyBuilding(UnitType type, TilePosition pos)
{
  for (auto &&building : _inSightBuildingMap[type])
  {
    if (building->getPosition() == pos)
      return building;
  }
  for (auto &&building : _seenBuildingMap[type])
  {
    if (building->getPosition() == pos)
      return building;
  }
  return nullptr;
}

UnitHandle *EnemyManager::getEnemyUnit(UnitType type, TilePosition pos)
{
  for (auto &&unit : _inSightUnitMap[type])
  {
    if (unit->getUnit()->getTilePosition() == pos)
      return unit;
  }
  for (auto &&unit : _seenUnitMap[type])
  {
    if (unit->getUnit()->getTilePosition() == pos)
      return unit;
  }
  return nullptr;
}


void  EnemyManager::updatePreviousStates()
{
  for (auto &&type : _inSightUnitMap)
  {
    for (auto &&unit : type.second)
    {
      unit->updateState();
    }
  }
}