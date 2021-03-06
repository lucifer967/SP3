#include "Enemy.h"

Enemy::Enemy() :prevX(0.f), RL(false), m_timer(0.f), m_timer2(0.f)
{
	//this->type = typeValue;
	this->scale = Vector3(1, 1, 1);
	this->mass = 1.f;
	this->momentOfInertia = 1.f;
	this->angularVelocity = 0.f;
}

Enemy::~Enemy()
{

}

void Enemy::Update(double dt, CPlayer *PlayerRef, std::vector<Enemy*> m_enemies, std::vector<CCollider*> m_Colliders)
{
	switch (this->type)
	{
	case(GameObject::GO_ENEMY_MELEE):
		this->EnemyType = MELEE;
		break;
	case(GameObject::GO_ENEMY_MELEE_2):
		this->EnemyType = MELEE;
		break;
	case(GameObject::GO_ENEMY_RANGED):
		this->EnemyType = RANGED;
		break;
	case(GameObject::GO_ENEMY_RANGED_2):
		this->EnemyType = RANGED;
		break;
	case(GameObject::GO_BOSS_1):
		this->EnemyType = BOSS_1;
		break;
	case(GameObject::GO_BOSS_2):
		this->EnemyType = BOSS_2;
		break;
	}

	switch (this->EnemyType)
	{
	case(MELEE):
		//Behaviour
	{
		// Variable Update
		attackBT--;

		m_timer += dt;

		//Left Collider Update
		LeftSight->SetMinAABB(Vector3(this->pos.x - 50, this->pos.y - 50, this->pos.z));
		LeftSight->SetMaxAABB(Vector3(this->pos.x, this->pos.y + 50, this->pos.z));
		//Right Collider Update
		RightSight->SetMinAABB(Vector3(this->pos.x, this->pos.y - 50, this->pos.z));
		RightSight->SetMaxAABB(Vector3(this->pos.x + 50, this->pos.y + 50, this->pos.z));

		// Continous changing/checking of which Enemy is the closest ally
		for (std::vector<Enemy *>::iterator it = m_enemies.begin(); it != m_enemies.end(); ++it)
		{
			Enemy *enemy = (Enemy *)*it;
			if (!(enemy->active))
				continue;
			if (enemy->EnemyType == MELEE || enemy->EnemyType == RANGED)
			{
				if (this != enemy)
				{
					if (ClosestEnemy == NULL)
					{
						std::cout << "Assign" << std::endl;
						ClosestEnemy = enemy;
					}
					else if ((this->pos - enemy->pos).Length() < (this->pos - ClosestEnemy->pos).Length())
					{
						std::cout << "Change!" << std::endl;
						ClosestEnemy = enemy;
					}
				}
			}
		}

		// Enemy to Player Detection
		if (LeftSight->CheckOverlap(LeftSight->GetMinAABB(), LeftSight->GetMaxAABB(), PlayerRef->GetMinAABB(), PlayerRef->GetMaxAABB()))
		{
			for (std::vector<CCollider *>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
			{
				CCollider *collider = (CCollider *)*it;

				Vector3 colliderPos = collider->GetMinAABB() + collider->GetMaxAABB();
				colliderPos.x *= 0.5f;
				colliderPos.y *= 0.5f;
				colliderPos.z *= 0.5f;

				if (it != m_Colliders.end() && (next(it) == m_Colliders.end()))
				{
					DetectedPlayer = true;
					break;
				}

				if (colliderPos.y < this->pos.y)
				{
					continue;
				}

				if (LeftSight->CheckOverlap(LeftSight->GetMinAABB(), LeftSight->GetMaxAABB(), collider->GetMinAABB(), collider->GetMaxAABB()))
				{
					if ((this->pos - colliderPos).Length() < (this->pos - PlayerRef->pos).Length())
					{
						DetectedPlayer = false;
						break;
					}
				}
			}
		}

		if (RightSight->CheckOverlap(RightSight->GetMinAABB(), RightSight->GetMaxAABB(), PlayerRef->GetMinAABB(), PlayerRef->GetMaxAABB()))
		{
			for (std::vector<CCollider *>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
			{
				CCollider *collider = (CCollider *)*it;

				Vector3 colliderPos = collider->GetMinAABB() + collider->GetMaxAABB();
				colliderPos.x *= 0.5f;
				colliderPos.y *= 0.5f;
				colliderPos.z *= 0.5f;

				if (it != m_Colliders.end() && (next(it) == m_Colliders.end()))
				{
					DetectedPlayer = true;
				}

				if (colliderPos == this->pos || colliderPos.y < this->pos.y)
				{
					continue;
				}

				if (RightSight->CheckOverlap(RightSight->GetMinAABB(), RightSight->GetMaxAABB(), collider->GetMinAABB(), collider->GetMaxAABB()))
				{
					if ((this->pos - colliderPos).Length() < (this->pos - PlayerRef->pos).Length())
					{
						std::cout << (this->pos - colliderPos).Length() << (this->pos - PlayerRef->pos).Length() << std::endl;
						DetectedPlayer = false;
						break;
					}
				}

				//if ((itr != Mine.end()) && (next(itr) == Mine.end()))
			}
		}

		// Enemy to Enemy Detection
		if (ClosestEnemy != NULL)
		{
			if (DetectedPlayer)
			{
				if ((this->pos - ClosestEnemy->pos).Length() >= 30)
				{
					if (this->pos.x < ClosestEnemy->pos.x)
					{
						this->pos.x += 12.5f * dt;
					}
					else
					{
						this->pos.x -= 12.5f * dt;
					}
				}
			}
		}

		// Enemy Action
		if (DetectedPlayer)
		{
			// Movement
			if ((this->pos - PlayerRef->pos).Length() > 12)
			{
				if (ClosestEnemy != NULL)
				{
					for (std::vector<CCollider *>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
					{
						CCollider *collider = (CCollider *)*it;

						Vector3 colliderPos = collider->GetMinAABB() + collider->GetMaxAABB();
						colliderPos.x *= 0.5f;
						colliderPos.y *= 0.5f;
						colliderPos.z *= 0.5f;

						if (((this->pos + 7.5f * dt) - ClosestEnemy->pos).Length() > 12 && ((this->pos + 7.5f * dt) - colliderPos).Length() > 12)
						{
							if (this->pos.x < PlayerRef->pos.x)
							{
								this->pos.x += 7.5f * dt;
								break;
							}
						}
						if (((this->pos - 7.5f * dt) - ClosestEnemy->pos).Length() > 12 && ((this->pos - 7.5f * dt) - colliderPos).Length() > 12)
						{
							if (this->pos.x > PlayerRef->pos.x)
							{
								this->pos.x -= 7.5f * dt;
								break;
							}
						}
					}
				}
				else
				{
					for (std::vector<CCollider *>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
					{
						CCollider *collider = (CCollider *)*it;

						Vector3 colliderPos = collider->GetMinAABB() + collider->GetMaxAABB();
						colliderPos.x *= 0.5f;
						colliderPos.y *= 0.5f;
						colliderPos.z *= 0.5f;

						if (it != m_Colliders.end() && (next(it) == m_Colliders.end()))
						{
							if (this->pos.x < PlayerRef->pos.x)
							{
								this->pos.x += 7.5f * dt;
							}
							else
							{
								this->pos.x -= 7.5f * dt;
							}
						}

						if (colliderPos.y < this->pos.y)
						{
							continue;
						}

						if (PlayerRef->pos.x < this->pos.x && colliderPos.x < this->pos.x)
						{
							if (((this->pos - 7.5f * dt) - colliderPos).Length() < 12)
							{
								break;
							}
						}
						if (PlayerRef->pos.x > this->pos.x && colliderPos.x > this->pos.x)
						{
							if (((this->pos + 7.5f * dt) - colliderPos).Length() < 12)
							{
								break;
							}
						}
					}
				}
			}
			// Attack
			else if ((this->pos - PlayerRef->pos).Length() <= 18)
			{
				if (attackBT <= 0)
				{
					attackBT = 30.f;

					std::cout << "Attack. (Melee)" << std::endl;
					PlayerRef->HP--;
				}
				SetAnimationStatus(RL, false, true, false, dt);
			}
		}

		if (ClosestEnemy != NULL && ClosestEnemy->active == false)
		{
			ClosestEnemy = NULL;
		}
	}


	//Animation
	if (this->HP <= 0)
	{

		SetAnimationStatus(RL, false, false, true, dt);
		if (m_timer >= 5.0f)
		{
			std::cout << "died" << std::endl;
			this->active = false;
			
			m_timer = 0.f;
		}
	}
	else
	{
		if (this->pos.x < prevX)
		{
			RL = true;
			SetAnimationStatus(RL, true, false, false, dt);
		}
		else if (this->pos.x > prevX)
		{
			RL = false;
			SetAnimationStatus(RL, true, false, false, dt);
		}
		else if (!DetectedPlayer)
		{
			SetAnimationStatus(RL, false, false, false, dt);
		}
	}

	prevX = this->pos.x;

	break;

	case(RANGED):
		//Behaviour
	{
		// Variable Update
		attackBT--;

		m_timer2 += dt;

		//Left Collider Update
		LeftSight->SetMinAABB(Vector3(this->pos.x - 50, this->pos.y - 50, this->pos.z));
		LeftSight->SetMaxAABB(Vector3(this->pos.x, this->pos.y + 50, this->pos.z));
		//Right Collider Update
		RightSight->SetMinAABB(Vector3(this->pos.x, this->pos.y - 50, this->pos.z));
		RightSight->SetMaxAABB(Vector3(this->pos.x + 50, this->pos.y + 50, this->pos.z));

		// Continous changing/checking of which Enemy is the closest ally
		for (std::vector<Enemy *>::iterator it = m_enemies.begin(); it != m_enemies.end(); ++it)
		{
			Enemy *enemy = (Enemy *)*it;
			if (!enemy->active)
				continue;
			if (enemy->EnemyType == MELEE || enemy->EnemyType == RANGED)
			{
				if (this != enemy)
				{
					if (ClosestEnemy == NULL)
					{
						std::cout << "Assign" << std::endl;
						ClosestEnemy = enemy;
					}
					else if ((this->pos - enemy->pos).Length() < (this->pos - ClosestEnemy->pos).Length())
					{
						std::cout << "Change!" << std::endl;
						ClosestEnemy = enemy;
					}
				}
			}
		}

		// Enemy to Player Detection
		if (LeftSight->CheckOverlap(LeftSight->GetMinAABB(), LeftSight->GetMaxAABB(), PlayerRef->GetMinAABB(), PlayerRef->GetMaxAABB()))
		{
			for (std::vector<CCollider *>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
			{
				CCollider *collider = (CCollider *)*it;

				Vector3 colliderPos = collider->GetMinAABB() + collider->GetMaxAABB();
				colliderPos.x *= 0.5f;
				colliderPos.y *= 0.5f;
				colliderPos.z *= 0.5f;

				if (it != m_Colliders.end() && (next(it) == m_Colliders.end()))
				{
					//std::cout << "Detected" << std::endl;
					DetectedPlayer = true;
				}

				if (colliderPos.y < this->pos.y)
				{
					continue;
				}

				if (LeftSight->CheckOverlap(LeftSight->GetMinAABB(), LeftSight->GetMaxAABB(), collider->GetMinAABB(), collider->GetMaxAABB()))
				{
					if ((this->pos - colliderPos).Length() < (this->pos - PlayerRef->pos).Length())
					{
						DetectedPlayer = false;
						break;
					}
				}
			}
		}

		if (RightSight->CheckOverlap(RightSight->GetMinAABB(), RightSight->GetMaxAABB(), PlayerRef->GetMinAABB(), PlayerRef->GetMaxAABB()))
		{
			for (std::vector<CCollider *>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
			{
				CCollider *collider = (CCollider *)*it;

				Vector3 colliderPos = collider->GetMinAABB() + collider->GetMaxAABB();
				colliderPos.x *= 0.5f;
				colliderPos.y *= 0.5f;
				colliderPos.z *= 0.5f;

				if (it != m_Colliders.end() && (next(it) == m_Colliders.end()))
				{
					//std::cout << "Detected" << std::endl;
					DetectedPlayer = true;
				}

				if (colliderPos == this->pos || colliderPos.y < this->pos.y)
				{
					continue;
				}

				if (RightSight->CheckOverlap(RightSight->GetMinAABB(), RightSight->GetMaxAABB(), collider->GetMinAABB(), collider->GetMaxAABB()))
				{
					if ((this->pos - colliderPos).Length() < (this->pos - PlayerRef->pos).Length())
					{
						std::cout << (this->pos - colliderPos).Length() << (this->pos - PlayerRef->pos).Length() << std::endl;
						DetectedPlayer = false;
						break;
					}
				}

				//if ((itr != Mine.end()) && (next(itr) == Mine.end()))
			}
		}

		//// Enemy to Enemy Detection
		//if (ClosestEnemy != NULL)
		//{
		//	if (DetectedPlayer)
		//	{
		//		if ((this->pos - ClosestEnemy->pos).Length() >= 30)
		//		{
		//			if (this->pos.x < ClosestEnemy->pos.x)
		//			{
		//				this->pos.x += 12.5f * dt;
		//			}
		//			else
		//			{
		//				this->pos.x -= 12.5f * dt;
		//			}
		//		}
		//	}
		//}

		// Enemy Action
		if (DetectedPlayer)
		{
			// Movement
			if ((this->pos - PlayerRef->pos).Length() > 40)
			{
				for (std::vector<CCollider *>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
				{
					CCollider *collider = (CCollider *)*it;

					Vector3 colliderPos = collider->GetMinAABB() + collider->GetMaxAABB();
					colliderPos.x *= 0.5f;
					colliderPos.y *= 0.5f;
					colliderPos.z *= 0.5f;

					if (it != m_Colliders.end() && (next(it) == m_Colliders.end()))
					{
						if (this->pos.x < PlayerRef->pos.x)
						{
							this->pos.x += 7.5f * dt;
						}
						else
						{
							this->pos.x -= 7.5f * dt;
						}
						CanAttack = true;
					}

					if (colliderPos.y < this->pos.y - 6)
					{
						continue;
					}

					if (PlayerRef->pos.x < this->pos.x && colliderPos.x < this->pos.x)
					{
						if (((this->pos - 7.5f * dt) - colliderPos).Length() < 12)
						{
							CanAttack = false;
							break;
						}
					}
					if (PlayerRef->pos.x > this->pos.x && colliderPos.x > this->pos.x)
					{
						if (((this->pos + 7.5f * dt) - colliderPos).Length() < 12)
						{
							CanAttack = false;
							break;
						}
					}
				}
			}
			// Attack
			else
			{
				if (ClosestEnemy != NULL)
				{
					if ((this->pos - ClosestEnemy->pos).Length() < 30)
					{
						if (ClosestEnemy->EnemyType == MELEE)
						{
							// Distance from this enemy to Player is less than the closest enemy to the player
							if ((this->pos - PlayerRef->pos).Length() < ((ClosestEnemy->pos - PlayerRef->pos).Length()) + 18)
							{
								if (ClosestEnemy->pos.x < PlayerRef->pos.x)
								{
									this->pos.x -= 7.5f * dt;
								}
								else
								{
									this->pos.x += 7.5f * dt;
								}
								CanAttack = false;
							}
							else
							{
								CanAttack = true;
							}
						}
					}
				}
				else
				{
					CanAttack = true;
				}

				if (attackBT <= 0 && CanAttack == true)
				{
					attackBT = 120.f;
					IsShooting = true;
					std::cout << "Attack. (Ranged)" << std::endl;
				}
				SetAnimationStatus(RL, false, true, false, dt);
			}
		}

		if (ClosestEnemy != NULL && ClosestEnemy->active == false)
		{
			ClosestEnemy = NULL;
		}

		if (DetectedPlayer)
		{
			if (PlayerRef->pos.x < this->pos.x)
			{
				RL = true;
			}
			else
			{
				RL = false;
			}
		}
	}

	//Animation
	if (this->HP <= 0)
	{

		SetAnimationStatus(RL, false, false, true, dt);
		if (m_timer2 >= 5.0f)
		{
			std::cout << m_timer << std::endl;
			this->active = false;
			m_timer2 = 0.f;
		}
	}
	else
	{
		if (this->pos.x < prevX)
		{
			RL = true;
			SetAnimationStatus(RL, true, false, false, dt);
		}
		else if (this->pos.x > prevX)
		{
			RL = false;
			SetAnimationStatus(RL, true, false, false, dt);
		}
		else if (!DetectedPlayer)
		{
			SetAnimationStatus(RL, false, false, false, dt);
		}
	}

	prevX = this->pos.x;

	break;

	case(BOSS_1):
		//Behaviour
	{
		attackBT--;

		if (attackBT <= 0)
		{

		}
	}
	break;

	case(BOSS_2):
	{

	}
	break;
	}
}
