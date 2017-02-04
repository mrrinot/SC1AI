#pragma once
#include <BWAPI.h>
#include <time.h>
#include <vector>

using namespace BWAPI;
class MineralField
{
public:
  MineralField(Unit unit);
  ~MineralField();

  void                    setDistanceToCC(float distance);
  void                    setGatherer(Unit SCV);
  void                    setWaiter(Unit scv);

  float                   getDistanceToCC() const;
  int                     getGatheringFrameLeft() const;
  const Unit              &getUnit() const;
  const Unit              &getGatherer() const;
  const Unit              &getWaiter() const;

  bool                    isGathering(Unit SCV) const;
  bool                    isWaiting(Unit SCV) const;

  bool                    removeGatherer(Unit SCV);
  bool                    removeWaiter(Unit SCV);

  void                    updateBehaviour();
  void                    resetMineralField();
  
  bool                    operator==(const MineralField &other);
  bool                    operator==(const Unit &unit);

private:
  void                    startGathering();

private:
  Unit              _unit;
  bool              _gathered;
  int               _frameBeforeAvailable;
  int               _framesCount;
  float             _distanceToCC;
  Unit              _gathererUnit;
  Unit              _scheduledUnit;
};

