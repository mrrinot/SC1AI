#pragma once
#include <BWAPI.h>
#include <time.h>

// Step B
// Unit�� ��� �ִ� �迭�� �ִ� ũ��
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
			// �迭�� ���� null �� ��, ���� ������ �����͸� ������.
			if (unitSet[i] == nullptr)
			{
				unitSet[i] = u;
				break;
			}
	}

	// ���� �迭 ���� �Լ�, onUnitDestroy()�ݹ� �Լ����� �ҷ���. ������ �ҷ���.
	static void deleteUnit(Unit unitSet[], Unit u)
	{
		// �迭�� ���� ������ Ÿ���� ���� ��, �ʱ�ȭ ��. ������ Unit �����ʹ� �ı��� ������ ��������.
		for (int i = 0; i < MAX_UNIT_COUNT; i++)
			if (unitSet[i] == u)
			{
				unitSet[i] = nullptr;
				break;
			}
	}

private:
	Unit m_pSCVs[MAX_UNIT_COUNT];// ���� SCV�� ����ִ� �迭��.   
	Unit m_pMineral[MAX_UNIT_COUNT];// �̳׶��� ������ ����ִ� �迭��.
	Unit m_pGas[MAX_UNIT_COUNT];
	Unit m_pUnitCommand;// �Ʊ� Ŀ�ǵ� ���� ������ ����.
	Unit m_pUnitSupply[MAX_UNIT_COUNT];
	Unit m_pUnitMarine[MAX_UNIT_COUNT];// ���� �޵��� ����ִ� �迭��.
	Unit m_pBarracks[MAX_UNIT_COUNT];// ������ ����ִ� �迭��.
	TilePosition mTileBarrack;// '�跰' �Ǽ� ���� TilePosition 

	int mBarrackCount;// '�跰' �ǹ��� �� �� �ִ��� Ȯ����.
	int mVultureCount;
	int mMarineCount;// ����, �޵�, ���̾���� ���ڸ� ���� ���� ������.
	int mSCVCount;
	int mSupplyDepotCount;

public:
	void initInfluenceMap();//����º����� ���� �ʱ�ȭ �� ������Ʈ
	void updateInfluenceMap(float besideDescentrate, float edgeDescentrate);
	bool analyseInfluenceMap(Unit unit);
	void drawInfluenceMap();//����º����� �ʿ� �׸��� �Լ�

private:
	time_t startTime;//����º����� ������Ʈ �ð�
	time_t endTime;
	float mapInfluenceInfo[MAP_WIDTH][MAP_HEIGHT][INFLUENCE_MAP_CNT];//����º����� �����迭
};
