#include "HeuristicManager.h"
#include "Logger.hh"

HeuristicManager::HeuristicManager()
{
  _heuristicUnitMapInfos[UnitTypes::Terran_Command_Center] = new UnitInfos(6, 500, 0, 0);
  addAllyImpactValue(UnitTypes::Terran_Command_Center, UnitTypes::Terran_SCV, 10);
  addAllyImpactValue(UnitTypes::Terran_Command_Center, UnitTypes::Terran_Command_Center, -9999);

  _heuristicUnitMapInfos[UnitTypes::Terran_SCV] = new UnitInfos(3, 75, 3, 9);
  addEnemyImpactValue(UnitTypes::Terran_SCV, UnitTypes::Terran_Vulture, 10);

  _heuristicUnitMapInfos[UnitTypes::Special_Power_Generator] = new UnitInfos(4, 300, 0, 0);
  addEnemyImpactValue(UnitTypes::Special_Power_Generator, UnitTypes::Terran_Marine, 5);

  _heuristicUnitMapInfos[UnitTypes::Terran_Supply_Depot] = new UnitInfos(2, 80, 0, 0);
  addEnemyImpactValue(UnitTypes::Terran_Supply_Depot, UnitTypes::Terran_Marine, 5);
  
  _heuristicUnitMapInfos[UnitTypes::Resource_Mineral_Field] = new UnitInfos(3, 100, 0, 0);
  addAllyImpactValue(UnitTypes::Resource_Mineral_Field, UnitTypes::Terran_SCV, 5);

  _heuristicUnitMapInfos[UnitTypes::Terran_Barracks] = new UnitInfos(2, 50, 4, 120);
  addEnemyImpactValue(UnitTypes::Terran_Barracks, UnitTypes::Terran_Vulture, 10);
  addEnemyImpactValue(UnitTypes::Terran_Barracks, UnitTypes::Terran_Bunker, 15);

  _heuristicUnitMapInfos[UnitTypes::Terran_Marine] = new UnitInfos(2, 10, UnitTypes::Terran_Marine.groundWeapon().maxRange() / 32, 15);
  addEnemyImpactValue(UnitTypes::Terran_Marine, UnitTypes::Terran_Bunker, 10);
  addEnemyImpactValue(UnitTypes::Terran_Marine, UnitTypes::Terran_Vulture, 5);
  addEnemyImpactValue(UnitTypes::Terran_Marine, UnitTypes::Terran_Marine, 2);
  addAllyImpactValue(UnitTypes::Terran_Marine, UnitTypes::Terran_Marine, 5);
  addAllyImpactValue(UnitTypes::Terran_Marine, TechTypes::Stim_Packs, 10);

  _heuristicUnitMapInfos[UnitTypes::Terran_Academy] = new UnitInfos(3, 60, 4, 80);
}

HeuristicManager::~HeuristicManager()
{
  for (auto &&infos : _heuristicUnitMapInfos)
  {
    delete infos.second;
  }
}

void  HeuristicManager::updateHeuristic()
{
  LogInfo() << "GLOABL HEURISTICS LIST";
  for (auto &&heur : _globalheuristicMap)
    LogInfo() << heur.first.toString() << " value = " << heur.second;
  LogInfo() << "DONE LISTING";
}

UnitInfos *HeuristicManager::getInfos(UnitType type)
{
  return _heuristicUnitMapInfos[type];
}

HeuristicManager *HeuristicManager::addAllyImpactValue(AnyType created, AnyType affected, float value)
{
  _allyMap[created][affected] = value;
  return this;
}

HeuristicManager *HeuristicManager::addEnemyImpactValue(AnyType created, AnyType affected, float value)
{
  _enemyMap[created][affected] = value;
  return this;
}

void    HeuristicManager::OnResearchTech(AnyType tech)
{
  for (auto &&typeAffected : _allyMap[tech])
  {
    _globalheuristicMap[typeAffected.first] += typeAffected.second;
  }
}

void   HeuristicManager::OnAllyUnitBorn(AnyType type)
{
  for (auto &&typeAffected : _allyMap[type])
  {
    _globalheuristicMap[typeAffected.first] += typeAffected.second;
  }
}

void   HeuristicManager::OnAllyUnitDeath(AnyType type)
{
  for (auto &&typeAffected : _allyMap[type])
  {
    _globalheuristicMap[typeAffected.first] -= typeAffected.second;
  }
}

void  HeuristicManager::OnEnemyUnitDestroyed(AnyType type)
{
  for (auto &&typeAffected : _enemyMap[type])
  {
    _globalheuristicMap[typeAffected.first] -= typeAffected.second;
  }
}

void  HeuristicManager::OnEnemyUnitShow(AnyType type)
{
  for (auto &&typeAffected : _enemyMap[type])
  {
    _globalheuristicMap[typeAffected.first] += typeAffected.second;
  }
}

AnyType HeuristicManager::getBestHeuristic()
{
  float highest = -999999;
  AnyType chosen = _globalheuristicMap.begin()->first;

  for (auto type = _globalheuristicMap.begin(); type != _globalheuristicMap.end(); ++type)
  {
    if (type->second > highest)
    {
      chosen = type->first;
      highest = type->second;
    }
  }
  return chosen;
}