#include "TileInfos.h"
#include "Logger.hh"

TileInfos::TileInfos(TilePosition pos)
: _pos(pos), _biggestBuildingPossible(TilePosition(0, 0)), _isBuildable(true), _isMineralPath(false), _isPath(false), _economicPower(0), _economicPowerEnemy(0), _militaryPower(0), _militaryPowerEnemy(0), _building(nullptr)
{
}

TileInfos::~TileInfos()
{
}

bool   TileInfos::isBuildable() const
{
  return _isBuildable;
}

bool   TileInfos::isPath() const
{
  return _isPath;
}

bool   TileInfos::isMineralPath() const
{
  return _isMineralPath;
}

float TileInfos::getEconomicPower() const
{
  return _economicPower;
}

float TileInfos::getMilitaryPower() const
{
  return _militaryPower;
}

float TileInfos::getEnemyEconomicPower() const
{
  return _economicPowerEnemy;
}

float TileInfos::getEnemyMilitaryPower() const
{
  return _militaryPowerEnemy;
}

BuildingHandle *TileInfos::getBuilding() const
{
  return _building;
}

TilePosition TileInfos::getPos() const
{
  return _pos;
}

TilePosition  TileInfos::getBiggestBuildingPossible() const
{
  return _biggestBuildingPossible;
}

TileInfos  *TileInfos::setPath(bool path)
{
  _isPath = path;
  return this;
}

TileInfos  *TileInfos::setMineralPath(bool path)
{
  _isMineralPath = path;
  return this;
}

TileInfos  *TileInfos::setBuildable(bool build)
{
  _isBuildable = build;
  return this;
}

TileInfos  *TileInfos::addEconomicPower(float added)
{
  //LogInfo() << "Pos = " << _pos.x << "/" << _pos.y;
  //LogInfo() << "added ally eco power: " << added << " after = " << _economicPower + added;
  _economicPower += added;
  return this;
}

TileInfos  *TileInfos::addMilitaryPower(float added)
{
  //LogInfo() << "Pos = " << _pos.x << "/" << _pos.y;
  //LogInfo() << "added ally mil power: " << added << " after = " << _militaryPower + added;
  _militaryPower += added;
  return this;
}

TileInfos  *TileInfos::addEnemyEconomicPower(float added)
{
  //LogInfo() << "Pos = " << _pos.x << "/" << _pos.y;
  //LogInfo() << "added enemy eco power: " << added << " after = " << _economicPowerEnemy + added;
  _economicPowerEnemy += added;
  return this;
}

TileInfos  *TileInfos::addEnemyMilitaryPower(float added)
{
  //LogInfo() << "Pos = " << _pos.x << "/" << _pos.y;
  //LogInfo() << "added enemy mil power: " << added << " after = " << _militaryPowerEnemy + added;
  _militaryPowerEnemy += added;
  return this;
}


TileInfos  *TileInfos::setBiggestBuildingPossible(TilePosition value)
{
  _biggestBuildingPossible = value;
  return this;
}

TileInfos  *TileInfos::setBiggestBuildingPossible(int x, int y)
{
  _biggestBuildingPossible = TilePosition(x, y);
  return this;
}

TileInfos *TileInfos::setBuilding(BuildingHandle *building)
{
  _building = building;
  return this;
}