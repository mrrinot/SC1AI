#include "BuildingHandle.h"
#include "HeuristicManager.h"

BuildingHandle::BuildingHandle(TilePosition pos, Unit builder)
: _unit(nullptr), _assignedBuilder(builder), _position(pos), _infos(nullptr), _state(BuildingHandle::UNBORN)
{
}

BuildingHandle::~BuildingHandle()
{

}

BuildingHandle *BuildingHandle::setAssignedBuilder(Unit builder)
{
  _assignedBuilder = builder;
  return this;
}

BuildingHandle *BuildingHandle::setUnit(Unit building)
{
  _unit = building;
  _infos = HeuristicManager::getInstance().getInfos(building->getType());
  return this;
}

BuildingHandle  *BuildingHandle::setState(BuildingHandle::e_Building_State state)
{
  _state = state;
  return this;
}

BuildingHandle::e_Building_State  BuildingHandle::getState() const
{
  return _state;
}

Unit  BuildingHandle::getUnit() const
{
  return _unit;
}

Unit  BuildingHandle::getAssignedBuilder() const
{
  return _assignedBuilder;
}

TilePosition BuildingHandle::getPosition() const
{
  return _position;
}

UnitInfos *BuildingHandle::getInfos() const
{
  return _infos;
}

