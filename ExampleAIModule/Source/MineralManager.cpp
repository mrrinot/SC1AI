#include "MineralManager.h"
#include "ConstGameValues.h"
#include "Logger.hh"

MineralManager::MineralManager()
{

}

MineralManager::~MineralManager()
{
  for (auto it = _mineralFieldList.begin(); it != _mineralFieldList.end(); ++it)
    delete *it;
}

void MineralManager::unassignGatherer(Unit scv)
{
  for (auto it = _mineralFieldList.begin(); it != _mineralFieldList.end(); ++it)
  {
    if ((*it)->removeGatherer(scv) == true)
      break;
  }
}

void MineralManager::unassignWaiter(Unit scv)
{
  for (auto it = _mineralFieldList.begin(); it != _mineralFieldList.end(); ++it)
  {
    if ((*it)->removeWaiter(scv) == true)
      break;
  }
}

const std::vector<MineralField *> &MineralManager::getMineralFieldList() const
{
  return _mineralFieldList;
}

MineralField *MineralManager::getMineralFieldBySCV(Unit scv)
{
  for (auto it = _mineralFieldList.begin(); it != _mineralFieldList.end(); ++it)
  {
    if ((*it)->isGathering(scv) == true || (*it)->isWaiting(scv) == true)
      return *it;
  }
  return nullptr;
}

MineralField *MineralManager::getBestMineralField(Unit scv)
{
  MineralField *best = nullptr;
  float minFrames = 99999;
  for (auto minIt = _mineralFieldList.begin(); minIt != _mineralFieldList.end(); ++minIt)
  {
    if ((*minIt)->getGatherer() != nullptr && (*minIt)->getWaiter() != nullptr)
      continue;
    float frames = (*minIt)->getGatheringFrameLeft() + int((scv->getDistance((*minIt)->getUnit()) / SCV_SPEED * 0.50)) + int(((*minIt)->getDistanceToCC() / SCV_SPEED * 0.80));
    //Logger::getLogger().logInfo() << "MINERALFIELD ID = " << (*minIt)->getUnit()->getID();
    //Logger::getLogger().logInfo() << "DISTANCE TO CC = " << (*minIt)->getDistanceToCC();
    //Logger::getLogger().logInfo() << "DISTANCE TO SCV = " << scv->getDistance((*minIt)->getUnit());
    //Logger::getLogger().logInfo() << "FRAMES LEFT = " << (*minIt)->getGatheringFrameLeft() << " IS GATHERED = " << ((*minIt)->getGatherer() == nullptr);
    if ((*minIt)->getGatherer() != nullptr && (*minIt)->getGatheringFrameLeft() == 0)
      frames += MINERAL_GATHERING_FRAME_DURATION * 3;
    //Logger::getLogger().logInfo() << "TOTAL = " << frames << "\n";
    if (frames < minFrames)
    {
      minFrames = frames;
      best = *minIt;
    }
  }
  //if (best != nullptr)
    //Logger::getLogger().logInfo() << "CHOSEN = " << best->getUnit()->getID();
  return best;
}

void  MineralManager::updateFields()
{
  for (auto minIt = _mineralFieldList.begin(); minIt != _mineralFieldList.end(); ++minIt)
  {
    (*minIt)->updateBehaviour();
    //LogInfo() << "MINID = " << (*minIt)->getUnit()->getID() << " remaining estimate : " << (*minIt)->getGatheringFrameLeft();
    if ((*minIt)->getGatherer() != nullptr)
    {
      //LogInfo() << "gatherer: " << (*minIt)->getGatherer()->getID();
      Broodwar->drawTextMap((*minIt)->getUnit()->getPosition().x, (*minIt)->getUnit()->getPosition().y - 20, "%d", (*minIt)->getGatherer()->getID());
    }
    if ((*minIt)->getWaiter() != nullptr)
    {
      //LogInfo() << "waiter: " << (*minIt)->getWaiter()->getID();
      Broodwar->drawTextMap((*minIt)->getUnit()->getPosition().x, (*minIt)->getUnit()->getPosition().y - 10, "%d", (*minIt)->getWaiter()->getID());
    }
  }
}

void   MineralManager::addMineralField(MineralField *field)
{
  _mineralFieldList.push_back(field);
}

void  MineralManager::removeMineralField(MineralField *field)
{
  for (auto it = _mineralFieldList.begin(); it != _mineralFieldList.end(); ++it)
  {
    if ((*it) == field)
    {
      delete *it;
      _mineralFieldList.erase(it);
      return ;
    }
  }
}

void  MineralManager::removeMineralField(Unit field)
{
  for (auto it = _mineralFieldList.begin(); it != _mineralFieldList.end(); ++it)
  {
    if ((**it) == field)
    {
      delete *it;
      _mineralFieldList.erase(it);
      return;
    }
  }
}

void  MineralManager::setDistanceToCC(Unit cc)
{
  for (auto it = _mineralFieldList.begin(); it != _mineralFieldList.end(); ++it)
  {
    (*it)->setDistanceToCC((*it)->getUnit()->getDistance(cc));
    //Logger::getLogger().logInfo() << "DISTANCE TO CC FOR ID " << (*it)->getUnit()->getID() << " = " << (*it)->getDistanceToCC();
  }
}