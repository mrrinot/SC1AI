#include "UnitHandle.h"
#include "HeuristicManager.h"

UnitHandle::UnitHandle(Unit unit)
: _unit(unit), _previousHP(0), _previousPos(TilePosition(0, 0))
{
  _infos = HeuristicManager::getInstance().getInfos(unit->getType());
}

UnitHandle::~UnitHandle()
{
}

UnitInfos *UnitHandle::getUnitInfos() const
{
  return _infos;
}

Unit  UnitHandle::getUnit() const
{
  return _unit;
}

void   UnitHandle::updateState()
{
  _previousHP = _unit->getHitPoints();
  _previousPos = _unit->getTilePosition();
}

int   UnitHandle::getPreviousHP() const
{
  return _previousHP;
}

TilePosition  UnitHandle::getPreviousPos() const
{
  return _previousPos;
}