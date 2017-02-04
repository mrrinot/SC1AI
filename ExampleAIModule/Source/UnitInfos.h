#pragma once

class UnitInfos
{
public:
  UnitInfos(int ecoRange, float ecoPower, int milRange, float milPower);
  ~UnitInfos();

  int   getEcoRange() const;
  int   getMilRange() const;
  float getEcoPower() const;
  float getMilPower() const;

  void  setEcoRange(int range);
  void  setMilRange(int range);
  void  setMilPower(float pow);
  void  setEcoPower(float pox);

private:
  int _ecoRange;
  int _milRange;
  float _ecoPower;
  float _milPower;
};