#include "SCVManager.h"
#include "Logger.hh"

SCVManager::SCVManager()
{

}

SCVManager::~SCVManager()
{

}

bool  SCVManager::addSCVToState(SCVManager::e_SCV_State state, Unit scv)
{
  //Logger::getLogger().logInfo() << "ADD TO " << state << " ID = " << scv->getID();
  _SCVMap[state].push_back(scv);
  //Logger::getLogger().logInfo() << "ADDED";
  return true;
}

bool  SCVManager::removeSCV(Unit scv)
{
  for (auto stateIt = _SCVMap.begin(); stateIt != _SCVMap.end(); ++stateIt)
  {
    for (auto it = _SCVMap[stateIt->first].begin(); it != _SCVMap[stateIt->first].end(); ++it)
    {
      if ((*it) == scv)
      {
        //Logger::getLogger().logInfo() << "FOUND IN LIST " << stateIt->first;
        _SCVMap[stateIt->first].erase(it);
        return true;
      }
    }
  }
  return false;
}

bool SCVManager::moveSCVToState(SCVManager::e_SCV_State state, Unit scv)
{
  if (removeSCV(scv) == false)
    return false;
  //Logger::getLogger().logInfo() << "REMOVE FOR " << state << " SUCCEEDED";
  return addSCVToState(state, scv);
}

SCVManager::e_SCV_State SCVManager::getSCVState(Unit scv) const
{
  for (auto stateIt = _SCVMap.begin(); stateIt != _SCVMap.end(); ++stateIt)
  {
    for (auto it = _SCVMap.at(stateIt->first).begin(); it != _SCVMap.at(stateIt->first).end(); ++it)
    {
      if ((*it) == scv)
        return stateIt->first;
    }
  }
  return ERROR;
}

Unit  SCVManager::getSCVFromState(SCVManager::e_SCV_State state)
{
  if (_SCVMap.find(state) != _SCVMap.end() && _SCVMap[state].size() > 0)
    return _SCVMap[state].front();
  return nullptr;
}

size_t  SCVManager::getTotalSCVByState(SCVManager::e_SCV_State state) const
{
  if (_SCVMap.find(state) != _SCVMap.end())
    return _SCVMap.at(state).size();
  return -1;
}

std::vector<Unit> &SCVManager::getSCVListByState(SCVManager::e_SCV_State state)
{
  return _SCVMap[state];
}

void   SCVManager::checkForIdlers()
{
  for (auto stateIt = _SCVMap.begin(); stateIt != _SCVMap.end(); ++stateIt)
  {
    if (stateIt->first == SCVManager::IDLE)
      continue;
    auto it = _SCVMap.at(stateIt->first).begin();
    while (it != _SCVMap.at(stateIt->first).end())
    {
      bool isIdle = false;
      Broodwar->drawTextMap((*it)->getPosition(), "%d", (*it)->getID());
      if (stateIt->first == SCVManager::MINERAL_CARRY && (*it)->isCarryingMinerals() == false)
        isIdle = true;
      if ((*it)->isIdle() == true && stateIt->first != SCVManager::WAIT && (*it)->isCompleted() == true)
        isIdle = true;
      if (isIdle == true)
      {
        //Logger::getLogger().logInfo() << "CONVERT TO IDLE FROM " << stateIt->first;
        moveSCVToState(SCVManager::IDLE, *it);
        it = _SCVMap.at(stateIt->first).begin();
      }
      else
        ++it;
    }
  }
}