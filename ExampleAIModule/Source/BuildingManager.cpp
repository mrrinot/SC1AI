#include "BuildingManager.h"
#include "Logger.hh"

BuildingManager::BuildingManager()
{

}

BuildingManager::~BuildingManager()
{
  for (auto &&type : _buildingMap)
  {
    for (auto &&building : type.second)
      delete building;
  }
}

BuildingManager *BuildingManager::addBuildingFundation(UnitType type, Unit builder, TilePosition pos)
{
  BuildingHandle *info = new BuildingHandle(pos, builder);
  info->setAssignedBuilder(builder);
  _buildingMap[type].push_back(info);
  return this;
}

BuildingManager *BuildingManager::addBuildingFundation(UnitType type, Unit builder, int x, int y)
{
  BuildingHandle *info = new BuildingHandle(TilePosition(x, y), builder);
  info->setAssignedBuilder(builder);
  _buildingMap[type].push_back(info);
  return this;
}

BuildingManager *BuildingManager::buildAddon(UnitType type, Unit builder, TilePosition pos)
{
  builder->buildAddon(type);
  BuildingHandle *info = new BuildingHandle(pos, builder);
  info->setAssignedBuilder(builder);
  _buildingMap[type].push_back(info);
  return this;
}

BuildingManager *BuildingManager::buildAddon(UnitType type, Unit builder, int x, int y)
{
  builder->buildAddon(type);
  BuildingHandle *info = new BuildingHandle(TilePosition(x, y), builder);
  info->setAssignedBuilder(builder);
  _buildingMap[type].push_back(info);
  return this;
}

std::vector<BuildingHandle *> &BuildingManager::getAllBuildingOfType(UnitType type)
{
  return _buildingMap[type];
}


std::vector<BuildingHandle *> BuildingManager::getAllBuildingOfTypeWithState(UnitType type, BuildingHandle::e_Building_State state)
{
  std::vector<BuildingHandle *> ret;

  for (auto &&building : _buildingMap[type])
  {
    if (building->getState() == state)
      ret.push_back(building);
  }
  return ret;
}

std::vector<BuildingHandle *> BuildingManager::getAllBuildingWithState(BuildingHandle::e_Building_State state)
{
  std::vector<BuildingHandle *> ret;

  for (auto &&type : _buildingMap)
  {
    for (auto &&building : _buildingMap[type.first])
    {
      if (building->getState() == state)
        ret.push_back(building);
    }
  }
  return ret;
}


BuildingManager *BuildingManager::setBuilder(Unit builder, TilePosition pos)
{
  for (auto &&type : _buildingMap)
  {
    for (auto &&building : type.second)
    {
      if (building->getPosition() == pos)
      {
        building->setAssignedBuilder(builder);
        return this;
      }
    }
  }
}

BuildingManager *BuildingManager::setBuilding(Unit buildingUnit, TilePosition pos)
{
  for (auto &&type : _buildingMap)
  {
    for (auto &&building : type.second)
    {
      if (building->getPosition() == pos)
      {
        building->setUnit(buildingUnit)->setState(BuildingHandle::IDLE);
        return this;
      }
    }
  }
}

BuildingHandle  *BuildingManager::getBuilding(TilePosition pos)
{
  for (auto &&type : _buildingMap)
  {
    Logger::getLogger().logInfo() << "BROWSING TYPES : " << type.first;
      for (auto &&building : type.second)
      {
        Logger::getLogger().logInfo() << "LOOKING FOR " << pos << " compared to " << building->getPosition();
        if (building->getPosition() == pos)
          return building;
      }
  }
  return nullptr;
}

bool  BuildingManager::removeBuilding(BuildingHandle *building)
{
  for (auto &&type : _buildingMap)
  {
    for (auto buildIt = type.second.begin(); buildIt != type.second.end(); ++buildIt)
    {
      if ((*buildIt) == building)
      {
        type.second.erase(buildIt);
        return true;
      }
    }
  }
  return false;
}

bool  BuildingManager::eraseBuilding(BuildingHandle *building)
{
  for (auto &&type : _buildingMap)
  {
    for (auto buildIt = type.second.begin(); buildIt != type.second.end(); ++buildIt)
    {
      if ((*buildIt) == building)
      {
        delete building;
        type.second.erase(buildIt);
        return true;
      }
    }
  }
  return false;
}