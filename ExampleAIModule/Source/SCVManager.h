#pragma once
#include <BWAPI.h>
#include <map>
#include <string>

using namespace BWAPI;
class SCVManager
{
public:

  enum e_SCV_State
  {
    IDLE = 0,
    MINERAL_GATHER,
    MINERAL_CARRY,
    GAS_GATHER,
    BUILD,
    REPAIR,
    UNBORN,
    WAIT,
    SCOOT,
    ERROR
  };

public:
  SCVManager();
  ~SCVManager();

  bool addSCVToState(e_SCV_State state, Unit scv);
  bool removeSCV(Unit scv);
  bool moveSCVToState(e_SCV_State state, Unit scv);

  e_SCV_State getSCVState(Unit scv) const;
  size_t  getTotalSCVByState(e_SCV_State state) const;
  Unit    getSCVFromState(e_SCV_State state);
  std::vector<Unit> &getSCVListByState(e_SCV_State state);

  void  checkForIdlers();

private:
  std::map<e_SCV_State, std::vector<Unit>> _SCVMap;
};