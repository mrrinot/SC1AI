#include "Battle.h"

// 게임이 시작될 때, callback 함수를 통하여 불러짐.
void Battle::onStart()
{
	//Broodwar->enableFlag(Flag::UserInput);
	// Step B
	// 유닛 배열 초기화 
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
		m_pSCVs[i] = nullptr;

	// Step C
	// 미네랄 배열 초기화
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
		m_pMineral[i] = nullptr;
	// 가스 배열 초기화
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
		m_pGas[i] = nullptr;


	// Step D
	// 베럭 배열 초기화
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
		m_pBarracks[i] = nullptr;

	// 배럭 수량과 TilePosition 초기화
	mBarrackCount = 0;
	mMarineCount = 0;
	mTileBarrack = TilePosition(0, 0);

	mSCVCount = 0;

	// Step F
	// 마린 배열 초기화
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
	{
		m_pUnitMarine[i] = nullptr;
		m_pUnitSupply[i] = nullptr;
	}

	// Step F
	m_pUnitCommand = nullptr;

	//영향력분포 초기화
	initInfluenceMap();
}

// 게임이 종료될 떄, callback 함수를 통하여 불러짐.
void Battle::onEnd(bool isWinner)
{
	//
}

// 유닛이 생성될 때, 콜백 형태로 불러와짐.
void Battle::onUnitCreate(Unit u)
{
	// Step C
	// 모든 객체의 변수명은 Unit 형태임.
	// u 가 미네랄 필드인지 가스 필드인지 확인함. && 채취당하고 있는지 && 플레이어 시야에 보이는지
	if (IsMineralField(u) && !u->isBeingGathered() && u->isVisible())
		addUnit(m_pMineral, u);
	// Unit의 타입이 Gas일 경우.
	if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
		addUnit(m_pGas, u);
	// Step B
	// 생성된 유닛이 SCV인 경우 포인터 배열에 추가함.
	if (u->getType() == UnitTypes::Terran_SCV)
	{
		addUnit(m_pSCVs, u);
		mSCVCount++;
	}


	if (u->getType() == UnitTypes::Terran_Supply_Depot)
	{
		addUnit(m_pUnitSupply, u);
		mSupplyDepotCount++;
	}
	// Step D
	//
	if (u->getType() == UnitTypes::Terran_Command_Center)
	{
		// '커맨드 센터'의 TilePosition' 정보를 받아옴.
		// '커맨드 센터'의 좌표 -= 5
		mTileBarrack = u->getTilePosition();

		mTileBarrack.x -= 5;
		mTileBarrack.y -= 5;

		m_pUnitCommand = u;
	}
	if (u->getType() == UnitTypes::Terran_Barracks)
	{
		addUnit(m_pBarracks, u);

		mBarrackCount++;
	}
	// 생성된 유닛이 마린일 경우 포인터 배열에 추가함.
	if (u->getType() == UnitTypes::Terran_Marine)
	{
		addUnit(m_pUnitMarine, u);
		mMarineCount++;
	}
}

// 유닛이 파괴될 때, 콜백 형태로 불러와짐.
void Battle::onUnitDestroy(Unit u)
{
	if (u->getType() == UnitTypes::Resource_Mineral_Field)
	{
		deleteUnit(m_pMineral, u);
	}
	if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
	{
		deleteUnit(m_pGas, u);
	}
	// Step B
	// 유닛이 파괴되면 포인터 배열에 들어있는 유닛정보를 초기화함.
	if (u->getType() == UnitTypes::Terran_SCV)
	{
		deleteUnit(m_pSCVs, u);
		mSCVCount--;
	}
	if (u->getType() == UnitTypes::Terran_Supply_Depot)
	{
		deleteUnit(m_pUnitSupply, u);
		mSupplyDepotCount--;
	}
	if (u->getType() == UnitTypes::Terran_Barracks)
	{
		deleteUnit(m_pBarracks, u);

		mBarrackCount--;
	}

	// Step F
	if (u->getType() == UnitTypes::Terran_Marine)
	{
		deleteUnit(m_pUnitMarine, u);
		mMarineCount--;
	}
	// Step F
	if (u->getType() == UnitTypes::Terran_Command_Center)
		if (m_pUnitCommand == u)
			m_pUnitCommand = nullptr;
}

// 게임의 매 프레임 마다 callback 함수를 통하여 불러짐.
void Battle::onFrame()
{
	time(&endTime);

	//3초마다 영향력분포 업데이트
	if (endTime - startTime > INFLUENCE_MAP_UPDATE_INTERVAL)
	{
		updateInfluenceMap(0.5f, 0.25f);
		time(&startTime);
	}
	//영향력 분포도 그리기
	drawInfluenceMap();

	//SCV가 미네랄 캠
	for (int j = 0; j < MAX_UNIT_COUNT; j++)
	{
		if (m_pMineral[j] != nullptr)
		{
			if (m_pMineral[j]->isVisible())
			{
				for (int i = 0; i < mSCVCount; i++)
				{
					if (m_pSCVs[i] != nullptr)
						if (m_pSCVs[i]->isIdle())
							m_pSCVs[i]->gather(m_pMineral[j]);
				}
			}
		}
	}
	if (m_pSCVs[1] != nullptr)
	{

		if (m_pSCVs[1]->canBuild(UnitTypes::Terran_Supply_Depot, TilePosition(mTileBarrack.x + 10, mTileBarrack.y)))
			// 정보가 확인된 TilePosition 좌표에 '서플라이 디팟' 를 생성함.
			m_pSCVs[1]->build(UnitTypes::Terran_Supply_Depot, TilePosition(mTileBarrack.x + 10, mTileBarrack.y));

		if (m_pSCVs[1]->canBuild(UnitTypes::Terran_Supply_Depot, TilePosition(mTileBarrack.x + 10, mTileBarrack.y - 2)))
			m_pSCVs[1]->build(UnitTypes::Terran_Supply_Depot, TilePosition(mTileBarrack.x + 10, mTileBarrack.y - 2));
	}
	if (m_pUnitCommand != nullptr)
	{
		if (mSCVCount < 16)
		{
			if (m_pUnitCommand->canTrain(UnitTypes::Terran_SCV))
				m_pUnitCommand->train(UnitTypes::Terran_SCV);
		}
	}
	// Step D
	//
	if (m_pSCVs[2] != nullptr)
		// 단, 1 인덱스의 SCV가 타겟에 건설이 가능할 경우.
		if (m_pSCVs[2]->canBuild(UnitTypes::Terran_Barracks, mTileBarrack))
			// 배럭의 수가 1보다 작을 때,
			if (mBarrackCount < 1)
				// 정보가 확인된 TilePosition 좌표에 '배럭' 를 생성함.
				if (m_pSCVs[2]->canBuild(UnitTypes::Terran_Barracks, mTileBarrack))
					m_pSCVs[2]->build(UnitTypes::Terran_Barracks, mTileBarrack);

	if (m_pSCVs[3] != nullptr)
		if (m_pSCVs[3]->canBuild(UnitTypes::Terran_Barracks, mTileBarrack))
			// 정보가 확인된 TilePosition 좌표에 '배럭' 를 생성함.
			if (m_pSCVs[3]->canBuild(UnitTypes::Terran_Barracks, TilePosition(mTileBarrack.x, mTileBarrack.y - 6)))
				m_pSCVs[3]->build(UnitTypes::Terran_Barracks, TilePosition(mTileBarrack.x, mTileBarrack.y - 6));

	//마린 생산
	if (m_pBarracks[0] != nullptr)
		if (m_pBarracks[0]->canTrain(UnitTypes::Terran_Marine))
			m_pBarracks[0]->train(UnitTypes::Terran_Marine);

	if (m_pBarracks[1] != nullptr)
		if (m_pBarracks[1]->canTrain(UnitTypes::Terran_Marine))
			m_pBarracks[1]->train(UnitTypes::Terran_Marine);

	//영향력 분석후 마린 행동 결정
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
	{
		if (m_pUnitMarine[i] != nullptr)
		{
			if (analyseInfluenceMap(m_pUnitMarine[i]) == true)
			{
				if (m_pUnitMarine[i]->getOrder() != Orders::AttackMove)
					m_pUnitMarine[i]->attack(Position(100, 1900));
			}
			else
				if (m_pUnitMarine[i]->isMoving() || m_pUnitMarine[i]->isAttacking())
					m_pUnitMarine[i]->move(Position(Broodwar->self()->getStartLocation().x*TILE_SIZE, Broodwar->self()->getStartLocation().y*TILE_SIZE));
		}
	}
}

//영향력분포 정보 초기화
void Battle::initInfluenceMap()
{
	for (int i = MAP_STARTING_X; i < MAP_WIDTH; i++)
		for (int j = MAP_STARTING_Y; j < MAP_HEIGHT; j++)
			for (int k = 0; k < INFLUENCE_MAP_CNT; k++)
				mapInfluenceInfo[i][j][k] = 0;
}

//영향력분포 정보 업데이트
void Battle::updateInfluenceMap(float besideDescentrate, float edgeDescentrate)
{
	initInfluenceMap();
	int nUnitInfunce = 1;
	int enenUnitInfunce = 1;

	//내 유닛을 찾아 건물과 SCV를 제외하고 영향력분포에 반영
	for (auto u : Broodwar->self()->getUnits())
	{
		if (u->getType() == UnitTypes::Terran_Marine)
		{
			int i = u->getTilePosition().x / 3;
			int j = u->getTilePosition().y / 3;

			mapInfluenceInfo[i][j][0] += nUnitInfunce;

			if (i - 1 >= MAP_STARTING_X)
			{
				mapInfluenceInfo[i - 1][j][0] += (nUnitInfunce * besideDescentrate);
				if (j - 1 >= MAP_STARTING_Y)
					mapInfluenceInfo[i - 1][j - 1][0] += (nUnitInfunce * edgeDescentrate);
				if (j + 1 <= MAP_HEIGHT)
					mapInfluenceInfo[i - 1][j + 1][0] += (nUnitInfunce * edgeDescentrate);
			}

			if (i - 2 >= MAP_STARTING_X)
				mapInfluenceInfo[i - 2][j][0] += (nUnitInfunce * edgeDescentrate);

			if (i + 1 <= MAP_WIDTH)
			{
				mapInfluenceInfo[i + 1][j][0] += (nUnitInfunce * besideDescentrate);
				if (j - 1 >= MAP_STARTING_Y)
					mapInfluenceInfo[i + 1][j - 1][0] += (nUnitInfunce * edgeDescentrate);
				if (j + 1 <= MAP_HEIGHT)
					mapInfluenceInfo[i + 1][j + 1][0] += (nUnitInfunce * edgeDescentrate);
			}

			if (i + 2 <= MAP_WIDTH)
				mapInfluenceInfo[i + 2][j][0] += (nUnitInfunce * edgeDescentrate);

			if (j - 1 >= MAP_STARTING_Y)
				mapInfluenceInfo[i][j - 1][0] += (nUnitInfunce * besideDescentrate);

			if (j - 2 >= MAP_STARTING_Y)
				mapInfluenceInfo[i][j - 2][0] += (nUnitInfunce * edgeDescentrate);

			if (j + 1 <= MAP_HEIGHT)
				mapInfluenceInfo[i][j + 1][0] += (nUnitInfunce * besideDescentrate);

			if (j + 2 <= MAP_HEIGHT)
				mapInfluenceInfo[i][j + 2][0] += (nUnitInfunce * edgeDescentrate);
		}
	}

	//적유닛 영향력분포 반영
	for (auto e : Broodwar->enemy()->getUnits())
	{
		if (e->isVisible())
		{
			if (e->getType() == UnitTypes::Terran_Marine)
			{

				int i = e->getTilePosition().x / 3;
				int j = e->getTilePosition().y / 3;

				mapInfluenceInfo[i][j][0] -= enenUnitInfunce;
				if (i - 1 >= MAP_STARTING_X)
				{
					mapInfluenceInfo[i - 1][j][0] -= (enenUnitInfunce * besideDescentrate);
					if (j - 1 >= MAP_STARTING_Y)
						mapInfluenceInfo[i - 1][j - 1][0] -= (enenUnitInfunce * edgeDescentrate);
					if (j + 1 <= MAP_HEIGHT)
						mapInfluenceInfo[i - 1][j + 1][0] -= (enenUnitInfunce * edgeDescentrate);
				}
				if (i - 2 >= MAP_STARTING_X)
				{
					mapInfluenceInfo[i - 2][j][0] -= (enenUnitInfunce * edgeDescentrate);
				}
				if (i + 1 <= MAP_WIDTH)
				{
					mapInfluenceInfo[i + 1][j][0] -= (enenUnitInfunce * besideDescentrate);
					if (j - 1 >= MAP_STARTING_Y)
						mapInfluenceInfo[i + 1][j - 1][0] -= (enenUnitInfunce * edgeDescentrate);
					if (j + 1 <= MAP_HEIGHT)
						mapInfluenceInfo[i + 1][j + 1][0] -= (enenUnitInfunce * edgeDescentrate);
				}
				if (i + 2 <= MAP_WIDTH)
					mapInfluenceInfo[i + 2][j][0] -= (enenUnitInfunce * edgeDescentrate);
				if (j - 1 >= MAP_STARTING_Y)
					mapInfluenceInfo[i][j - 1][0] -= (enenUnitInfunce * besideDescentrate);
				if (j - 2 >= MAP_STARTING_Y)
					mapInfluenceInfo[i][j - 2][0] -= (enenUnitInfunce * edgeDescentrate);
				if (j + 1 <= MAP_HEIGHT)
					mapInfluenceInfo[i][j + 1][0] -= (enenUnitInfunce * besideDescentrate);
				if (j + 2 <= MAP_HEIGHT)
					mapInfluenceInfo[i][j + 2][0] -= (enenUnitInfunce * edgeDescentrate);
			}
		}
	}
}

//영향력분포 지도 그리기
void Battle::drawInfluenceMap()
{
	for (int i = MAP_STARTING_X; i < MAP_WIDTH; i++)
	{
		for (int j = MAP_STARTING_Y; j < MAP_HEIGHT; j++)
		{
			//타일값이 +면 초록 -면 빨강 중립은 회색
			Broodwar->drawBoxMap(Position((i * 3)*TILE_SIZE, (j * 3)*TILE_SIZE), 
					Position((i * 3)*TILE_SIZE + TILE_SIZE * 3, (j * 3)*TILE_SIZE + TILE_SIZE * 3), Colors::Grey);
			if (mapInfluenceInfo[i][j][0]>0)
				Broodwar->drawBoxMap(Position((i * 3)*TILE_SIZE, (j * 3)*TILE_SIZE), 
					Position((i * 3)*TILE_SIZE + TILE_SIZE * 3, (j * 3)*TILE_SIZE + TILE_SIZE * 3), Colors::Green);
			else if (mapInfluenceInfo[i][j][0] < 0)
				Broodwar->drawBoxMap(Position((i * 3)*TILE_SIZE, (j * 3)*TILE_SIZE), 
					Position((i * 3)*TILE_SIZE + TILE_SIZE * 3, (j * 3)*TILE_SIZE + TILE_SIZE * 3), Colors::Red);
			else
				Broodwar->drawBoxMap(Position((i * 3)*TILE_SIZE, (j * 3)*TILE_SIZE), 
					Position((i * 3)*TILE_SIZE + TILE_SIZE * 3, (j * 3)*TILE_SIZE + TILE_SIZE * 3), Colors::Grey);

			//영향력분포 정보 출력
			Broodwar->drawTextMap(Position((i * 3)*TILE_SIZE + TILE_SIZE, (j * 3)* TILE_SIZE + TILE_SIZE), "%.2f", mapInfluenceInfo[i][j][0]);
			Broodwar->drawTextMap(Position((i * 3)*TILE_SIZE + TILE_SIZE * 2, (j * 3)*TILE_SIZE + TILE_SIZE), "%.0f", mapInfluenceInfo[i][j][1]);
			Broodwar->drawTextMap(Position((i * 3)*TILE_SIZE + TILE_SIZE, (j * 3)*TILE_SIZE + TILE_SIZE * 2), "%.0f", mapInfluenceInfo[i][j][2]);
			Broodwar->drawTextMap(Position((i * 3)*TILE_SIZE + TILE_SIZE * 2, (j * 3)*TILE_SIZE + TILE_SIZE * 2), "%.0f", mapInfluenceInfo[i][j][3]);
			
		}
	}
}
//영향력분포 분석
bool Battle::analyseInfluenceMap(Unit unit)
{
	int i = unit->getTilePosition().x / 3;
	int j = unit->getTilePosition().y / 3;

	//근처의 영향력이 -면 false +면 true를 반환.
	if (mapInfluenceInfo[i][j][0] < 0)
		return false;

	if (i - 1 >= MAP_STARTING_X)
	{
		if (mapInfluenceInfo[i - 1][j][0] < 0)
			return false;
		if (j - 1 >= MAP_STARTING_Y)
			if (mapInfluenceInfo[i - 1][j - 1][0] < 0)
				return false;
		if (j + 1 <= MAP_HEIGHT)
			if (mapInfluenceInfo[i - 1][j + 1][0] < 0)
				return false;
	}

	if (i - 2 >= MAP_STARTING_X)
		if (mapInfluenceInfo[i - 2][j][0] < 0)
			return false;

	if (i + 1 <= MAP_WIDTH)
	{
		if (mapInfluenceInfo[i + 1][j][0] < 0)
			return false;
		if (j - 1 >= MAP_STARTING_Y)
			if (mapInfluenceInfo[i + 1][j - 1][0] < 0)
				return false;
		if (j + 1 <= MAP_HEIGHT)
			if (mapInfluenceInfo[i + 1][j + 1][0] < 0)
				return false;
	}

	if (i + 2 <= MAP_WIDTH)
		if (mapInfluenceInfo[i + 2][j][0] < 0)
			return false;

	if (j - 1 >= MAP_STARTING_Y)
		if (mapInfluenceInfo[i][j - 1][0] < 0)
			return false;

	if (j - 2 >= MAP_STARTING_Y)
		if (mapInfluenceInfo[i][j - 2][0] < 0)
			return false;

	if (j + 1 <= MAP_HEIGHT)
		if (mapInfluenceInfo[i][j + 1][0] < 0)
			return false;

	if (j + 2 <= MAP_HEIGHT)
		if (mapInfluenceInfo[i][j + 2][0] < 0)
			return false;
	return true;
}