#include "Battle.h"

// ������ ���۵� ��, callback �Լ��� ���Ͽ� �ҷ���.
void Battle::onStart()
{
	//Broodwar->enableFlag(Flag::UserInput);
	// Step B
	// ���� �迭 �ʱ�ȭ 
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
		m_pSCVs[i] = nullptr;

	// Step C
	// �̳׶� �迭 �ʱ�ȭ
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
		m_pMineral[i] = nullptr;
	// ���� �迭 �ʱ�ȭ
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
		m_pGas[i] = nullptr;


	// Step D
	// ���� �迭 �ʱ�ȭ
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
		m_pBarracks[i] = nullptr;

	// �跰 ������ TilePosition �ʱ�ȭ
	mBarrackCount = 0;
	mMarineCount = 0;
	mTileBarrack = TilePosition(0, 0);

	mSCVCount = 0;

	// Step F
	// ���� �迭 �ʱ�ȭ
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
	{
		m_pUnitMarine[i] = nullptr;
		m_pUnitSupply[i] = nullptr;
	}

	// Step F
	m_pUnitCommand = nullptr;

	//����º��� �ʱ�ȭ
	initInfluenceMap();
}

// ������ ����� ��, callback �Լ��� ���Ͽ� �ҷ���.
void Battle::onEnd(bool isWinner)
{
	//
}

// ������ ������ ��, �ݹ� ���·� �ҷ�����.
void Battle::onUnitCreate(Unit u)
{
	// Step C
	// ��� ��ü�� �������� Unit ������.
	// u �� �̳׶� �ʵ����� ���� �ʵ����� Ȯ����. && ä����ϰ� �ִ��� && �÷��̾� �þ߿� ���̴���
	if (IsMineralField(u) && !u->isBeingGathered() && u->isVisible())
		addUnit(m_pMineral, u);
	// Unit�� Ÿ���� Gas�� ���.
	if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
		addUnit(m_pGas, u);
	// Step B
	// ������ ������ SCV�� ��� ������ �迭�� �߰���.
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
		// 'Ŀ�ǵ� ����'�� TilePosition' ������ �޾ƿ�.
		// 'Ŀ�ǵ� ����'�� ��ǥ -= 5
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
	// ������ ������ ������ ��� ������ �迭�� �߰���.
	if (u->getType() == UnitTypes::Terran_Marine)
	{
		addUnit(m_pUnitMarine, u);
		mMarineCount++;
	}
}

// ������ �ı��� ��, �ݹ� ���·� �ҷ�����.
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
	// ������ �ı��Ǹ� ������ �迭�� ����ִ� ���������� �ʱ�ȭ��.
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

// ������ �� ������ ���� callback �Լ��� ���Ͽ� �ҷ���.
void Battle::onFrame()
{
	time(&endTime);

	//3�ʸ��� ����º��� ������Ʈ
	if (endTime - startTime > INFLUENCE_MAP_UPDATE_INTERVAL)
	{
		updateInfluenceMap(0.5f, 0.25f);
		time(&startTime);
	}
	//����� ������ �׸���
	drawInfluenceMap();

	//SCV�� �̳׶� ķ
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
			// ������ Ȯ�ε� TilePosition ��ǥ�� '���ö��� ����' �� ������.
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
		// ��, 1 �ε����� SCV�� Ÿ�ٿ� �Ǽ��� ������ ���.
		if (m_pSCVs[2]->canBuild(UnitTypes::Terran_Barracks, mTileBarrack))
			// �跰�� ���� 1���� ���� ��,
			if (mBarrackCount < 1)
				// ������ Ȯ�ε� TilePosition ��ǥ�� '�跰' �� ������.
				if (m_pSCVs[2]->canBuild(UnitTypes::Terran_Barracks, mTileBarrack))
					m_pSCVs[2]->build(UnitTypes::Terran_Barracks, mTileBarrack);

	if (m_pSCVs[3] != nullptr)
		if (m_pSCVs[3]->canBuild(UnitTypes::Terran_Barracks, mTileBarrack))
			// ������ Ȯ�ε� TilePosition ��ǥ�� '�跰' �� ������.
			if (m_pSCVs[3]->canBuild(UnitTypes::Terran_Barracks, TilePosition(mTileBarrack.x, mTileBarrack.y - 6)))
				m_pSCVs[3]->build(UnitTypes::Terran_Barracks, TilePosition(mTileBarrack.x, mTileBarrack.y - 6));

	//���� ����
	if (m_pBarracks[0] != nullptr)
		if (m_pBarracks[0]->canTrain(UnitTypes::Terran_Marine))
			m_pBarracks[0]->train(UnitTypes::Terran_Marine);

	if (m_pBarracks[1] != nullptr)
		if (m_pBarracks[1]->canTrain(UnitTypes::Terran_Marine))
			m_pBarracks[1]->train(UnitTypes::Terran_Marine);

	//����� �м��� ���� �ൿ ����
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

//����º��� ���� �ʱ�ȭ
void Battle::initInfluenceMap()
{
	for (int i = MAP_STARTING_X; i < MAP_WIDTH; i++)
		for (int j = MAP_STARTING_Y; j < MAP_HEIGHT; j++)
			for (int k = 0; k < INFLUENCE_MAP_CNT; k++)
				mapInfluenceInfo[i][j][k] = 0;
}

//����º��� ���� ������Ʈ
void Battle::updateInfluenceMap(float besideDescentrate, float edgeDescentrate)
{
	initInfluenceMap();
	int nUnitInfunce = 1;
	int enenUnitInfunce = 1;

	//�� ������ ã�� �ǹ��� SCV�� �����ϰ� ����º����� �ݿ�
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

	//������ ����º��� �ݿ�
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

//����º��� ���� �׸���
void Battle::drawInfluenceMap()
{
	for (int i = MAP_STARTING_X; i < MAP_WIDTH; i++)
	{
		for (int j = MAP_STARTING_Y; j < MAP_HEIGHT; j++)
		{
			//Ÿ�ϰ��� +�� �ʷ� -�� ���� �߸��� ȸ��
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

			//����º��� ���� ���
			Broodwar->drawTextMap(Position((i * 3)*TILE_SIZE + TILE_SIZE, (j * 3)* TILE_SIZE + TILE_SIZE), "%.2f", mapInfluenceInfo[i][j][0]);
			Broodwar->drawTextMap(Position((i * 3)*TILE_SIZE + TILE_SIZE * 2, (j * 3)*TILE_SIZE + TILE_SIZE), "%.0f", mapInfluenceInfo[i][j][1]);
			Broodwar->drawTextMap(Position((i * 3)*TILE_SIZE + TILE_SIZE, (j * 3)*TILE_SIZE + TILE_SIZE * 2), "%.0f", mapInfluenceInfo[i][j][2]);
			Broodwar->drawTextMap(Position((i * 3)*TILE_SIZE + TILE_SIZE * 2, (j * 3)*TILE_SIZE + TILE_SIZE * 2), "%.0f", mapInfluenceInfo[i][j][3]);
			
		}
	}
}
//����º��� �м�
bool Battle::analyseInfluenceMap(Unit unit)
{
	int i = unit->getTilePosition().x / 3;
	int j = unit->getTilePosition().y / 3;

	//��ó�� ������� -�� false +�� true�� ��ȯ.
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