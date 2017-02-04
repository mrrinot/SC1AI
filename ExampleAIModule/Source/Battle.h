#pragma once
#include <BWAPI.h>
#include <time.h>

// Step B
// Unit을 담고 있는 배열의 최대 크기
#define MAX_UNIT_COUNT 1000

#define MAP_STARTING_X 0
#define MAP_STARTING_Y 0
#define MAP_WIDTH MAP_STARTING_X+21
#define MAP_HEIGHT MAP_STARTING_Y+21
#define INFLUENCE_MAP_CNT 4
#define INFLUENCE_MAP_UPDATE_INTERVAL 3

using namespace BWAPI;
using namespace Filter;

class Battle : public BWAPI::AIModule
{
public:
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onUnitCreate(Unit unit);
	virtual void onUnitDestroy(Unit unit);

	static void addUnit(Unit unitSet[], Unit u)
	{
		for (int i = 0; i < MAX_UNIT_COUNT; i++)
			// 배열의 값이 null 일 때, 들어온 유닛의 포인터를 저장함.
			if (unitSet[i] == nullptr)
			{
				unitSet[i] = u;
				break;
			}
	}

	// 유닛 배열 관리 함수, onUnitDestroy()콜백 함수에서 불러짐. 무조건 불러짐.
	static void deleteUnit(Unit unitSet[], Unit u)
	{
		// 배열의 값과 유닛의 타입이 같을 때, 초기화 함. 들어오는 Unit 포인터는 파괴된 유닛의 포인터임.
		for (int i = 0; i < MAX_UNIT_COUNT; i++)
			if (unitSet[i] == u)
			{
				unitSet[i] = nullptr;
				break;
			}
	}

private:
	Unit m_pSCVs[MAX_UNIT_COUNT];// 유닛 SCV를 담고있는 배열임.   
	Unit m_pMineral[MAX_UNIT_COUNT];// 미네랄과 가스를 담고있는 배열임.
	Unit m_pGas[MAX_UNIT_COUNT];
	Unit m_pUnitCommand;// 아군 커맨드 센터 포인터 저장.
	Unit m_pUnitSupply[MAX_UNIT_COUNT];
	Unit m_pUnitMarine[MAX_UNIT_COUNT];// 유닛 메딕을 담고있는 배열임.
	Unit m_pBarracks[MAX_UNIT_COUNT];// 베럭을 담고있는 배열임.
	TilePosition mTileBarrack;// '배럭' 건설 예정 TilePosition 

	int mBarrackCount;// '배럭' 건물이 몇 개 있는지 확인함.
	int mVultureCount;
	int mMarineCount;// 마린, 메딕, 파이어뱃의 숫자를 세기 위한 변수임.
	int mSCVCount;
	int mSupplyDepotCount;

public:
	void initInfluenceMap();//영향력분포의 정보 초기화 및 업데이트
	void updateInfluenceMap(float besideDescentrate, float edgeDescentrate);
	bool analyseInfluenceMap(Unit unit);
	void drawInfluenceMap();//영향력분포를 맵에 그리는 함수

private:
	time_t startTime;//영향력분포의 업데이트 시간
	time_t endTime;
	float mapInfluenceInfo[MAP_WIDTH][MAP_HEIGHT][INFLUENCE_MAP_CNT];//영향력분포의 정보배열
};
