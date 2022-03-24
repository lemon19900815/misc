using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public enum CampID
	{
		Camp_ID_A,
		Camp_ID_B,
	}

	public enum AttackType
	{
		AT_Remote,				// 远程攻击方式
		AT_ShortRange,			// 近程攻击方式
	}

	public enum SkillType
	{
		ST_Buff,				// 正面技能--作用于己方actor单位
		ST_Debuff,				// 负面技能--作用于敌方actor单位
	}

	// 基础不变的数据，通过注册函数加载的ai模块中
	public struct BevActorData
	{
		public int mActorIdx;			// actor的索引，唯一标示
		public int mActorId;			// actor的id
		public int mBaseHateVal;		// 基础仇恨值
		public int mActorType;			// actor类型
		public AttackType mAttackType;	// 攻击类型
		public CampID mActorCampID;		// 阵营id
		public float mAttackRadius;		// 攻击半径
		public float mViewRadius;		// 视野半径
		public float mTraceRadius;		// 追击半径
		public float mViewDegree;		// 视角度数
	}

	// ai模块向外提供的接口对象
	public class BevActor
	{
		private AIActor _aiObj;

		private BevActorData _bevActorData;

		public BevActorData bevActorData
		{
			set { _bevActorData = value; }
			get { return _bevActorData; }
		}

		// 初始化，需要ai的id作为初始化条件
		public bool initalize(int brainId, BevActorData data)
		{
			_aiObj = new AIActor();

			if(_aiObj.init(brainId)) {
				_aiObj.bevActor = this; // bind bev actor

				_bevActorData = new BevActorData();
				_bevActorData = data;

				return true;
			}

			_aiObj = null;
			return false;
		}

		/// <summary>
		/// Adds the hate enemy.
		/// </summary>
		/// <param name="_actor">_actor.</param>
		public void addHateEnemy(BevActor _actor)
		{
			_aiObj.addHateEnemy(_actor._aiObj);
		}

		/// <summary>
		/// Removes the hate enemy.
		/// </summary>
		/// <param name="_actor">_actor.</param>
		public void removeHateEnemy(BevActor _actor)
		{
			_aiObj.removeHateEnemy(_actor._aiObj);
		}

		/// <summary>
		/// when died bev actor is attacker's attackTarget, then Reset this instance.
		/// </summary>
		public void reset(BevActor dieBevActor)
		{
			if(dieBevActor._aiObj == _aiObj.attackTarget) {
				_aiObj.reset();
			}

			dieBevActor._aiObj.onDied();
		}

		public void setFollower(BevActor follower)
		{
			if(follower != null)
				_aiObj.followerActor = follower._aiObj;
			else
				_aiObj.followerActor = null;
		}

		/// <summary>
		/// Ons the cast end.
		/// skill cast end: broken or cast end
		/// </summary>
		/// <param name="skillIndex">Skill index.</param>
		public void onCastEnd(int skillIndex, bool isBroken)
		{
			_aiObj.onCastEnd(skillIndex, isBroken);
		}

		#region 应用层接口，actor做了某些事情需要通知到ai中去
		/// <summary>
		/// Attacks the actor.
		/// when attack a actor, tell damage to ai module
		/// </summary>
		/// <param name="_target">_target.</param>
		/// <param name="damage">Damage.</param>
		public void onAttacked(BevActor _attacker, int damage)
		{
			_aiObj.onAttacked(_attacker._aiObj, damage);
		}

		/// <summary>
		/// Update this instance.
		/// every frame need call
		/// </summary>
		public void update()
		{
			_aiObj.update();
		}

		#endregion

		#region 接入层接口，提供给ai模块调用
		/// <summary>
		/// Dos the attack.
		/// give a order to client: actor attack _target through skillIdx
		/// </summary>
		/// <param name="skillIdx">Skill index.</param>
		/// <param name="_target">_target.</param>
		public void doAttack(int skillIdx, AIActor _target)
		{
			if(_target != null) {
				AIModule.doSkill(bevActorData.mActorIdx, skillIdx, _target.bevActor.bevActorData.mActorIdx);
			}
			//_unit.doSkill(skillIdx, _targets);
		}

		/// <summary>
		/// Dos the move.
		/// give a order to client: actor move towards _target
		/// </summary>
		/// <param name="_target">_target.</param>
		public void doMove(AIActor _target)
		{
			AIModule.doMove(bevActorData.mActorIdx, _target.bevActor.bevActorData.mActorIdx);
			//_unit.doMove(_target);
		}

		/// <summary>
		/// Dos the move to.
		/// give a order to client: move to _pos
		/// </summary>
		/// <param name="_pos">_pos.</param>
		public void doMoveTo(Vector3 _pos)
		{
			AIModule.doMoveTo(bevActorData.mActorIdx, _pos);
			//_unit.doMoveTo(_pos);
		}

		/// <summary>
		/// Dos the move dir.
		/// move towards dir by given times
		/// </summary>
		/// <param name="_dir">_dir.</param>
		/// <param name="_t">_t.</param>
		public void doMoveDir(Vector3 _dir, float _speed)
		{
			AIModule.doMoveDir(bevActorData.mActorIdx, _dir, _speed);
			//_unit.doMoveDir(_dir, _speed);
		}

		/// <summary>
		/// Gets the move speed.
		/// </summary>
		/// <returns>The move speed.</returns>
		public float getMoveSpeed()
		{
			return AIModule.getMoveSpeed(bevActorData.mActorIdx);
			//return _unit.getMoveSpeed();
		}

		/// <summary>
		/// Cans the move.
		/// bev actor can move or not
		/// </summary>
		/// <returns><c>true</c>, if move was caned, <c>false</c> otherwise.</returns>
		public bool canMove()
		{
			return AIModule.canMove(bevActorData.mActorIdx);
			//return _unit.canMove();
		}

		/// <summary>
		/// Cans the attack.
		/// bev actor can attack or not by use skillIndex
		/// </summary>
		/// <returns><c>true</c>, if attack was caned, <c>false</c> otherwise.</returns>
		/// <param name="skillIndex">Skill index.</param>
		public bool canAttack(int skillIndex)
		{
			return AIModule.canDoAttack(bevActorData.mActorIdx, skillIndex);
			//return _unit.canAttack(skillIndex);
		}

		/// <summary>
		/// Gets the skill cast time. seconds
		/// </summary>
		/// <returns>The skill cast still time.</returns>
		/// <param name="skillIndex">Skill index.</param>
		public float getSkillCastTime(int skillIndex)
		{
			return AIModule.getCastTime(bevActorData.mActorIdx, skillIndex);
			//return _unit.getSkillCastTime(skillIndex);
		}

		/// <summary>
		/// Gets the type of the skill.
		/// </summary>
		/// <returns>The skill type.</returns>
		/// <param name="skillIndex">Skill index.</param>
		public int getSkillType(int skillIndex)
		{
			return AIModule.getSkillType(bevActorData.mActorIdx, skillIndex);
			//return _unit.getSkillType(skillIndex);
		}

		/// <summary>
		/// Gets the hp info.
		/// </summary>
		/// <returns><c>true</c>, if hp info was gotten, <c>false</c> otherwise.</returns>
		/// <param name="_cur">_cur.</param>
		/// <param name="_max">_max.</param>
		public bool getHpInfo(out int _cur, out int _max)
		{
			return AIModule.getHpInfo(bevActorData.mActorIdx, out _cur, out _max);
			//return _unit.getHpInfo(out _cur, out _max);
		}

		/// <summary>
		/// bev actor is died or not.
		/// </summary>
		/// <returns><c>true</c>, if died was ised, <c>false</c> otherwise.</returns>
		public bool isDied()
		{
			bool retval = true;
			
			int _cur, _max;
			if(getHpInfo(out _cur, out _max)) {
				retval = _cur <= 0;
			}
			
			return retval;
		}

		/// <summary>
		/// Gets the targets in range.
		/// </summary>
		/// <returns>The targets in range.</returns>
		/// <param name="radius">Radius.</param>
		/// <param name="angle">Angle.</param>
		/// <param name="sameCamp">If set to <c>true</c> same camp.</param>
		public List<AIActor> getTargetsInRange(float radius, float angle, bool sameCamp)
		{
			List<AIActor> retval = null;

			List<BevActor> targets = AIModule.getTargetsInRange(bevActorData.mActorIdx, radius, angle, sameCamp);
			if(targets != null && targets.Count > 0) {
				retval = new List<AIActor>();

				foreach(BevActor _bev in targets) {
					retval.Add(_bev._aiObj);
				}
			}

			return retval;
			//return _unit.getTargetsInRange(radius, angle, sameCamp);
		}

		/// <summary>
		/// Gets the actor position.
		/// </summary>
		/// <returns>The actor position.</returns>
		public Vector3 getActorPosition()
		{
			return AIModule.getActorPosition(bevActorData.mActorIdx);
			//return _unit.getActorPosition();
		}

		/// <summary>
		/// Gets the trace start point.
		/// if bev actor is monster, configure file should have this point, else return player's position
		/// </summary>
		/// <returns>The trace start point.</returns>
		public Vector3 getTraceStartPoint()
		{
			Vector3 tracePos = Vector3.zero;

			AIModule.getTraceStartPoint(bevActorData.mActorIdx, out tracePos);

			return tracePos;
			//return _unit.getTraceStartPoint();
		}

		/// <summary>
		/// Gets the next dungeon point.
		/// </summary>
		/// <returns>The next dungeon point.</returns>
		public bool getNextDungeonPoint(out Vector3 pos)
		{
			return AIModule.getNextDungeonPoint(bevActorData.mActorIdx, out pos);
			//return _unit.getNextDungeonPoint(out pos);
		}

		public bool getOtherPatrolPoint(out Vector3 pos)
		{
			return AIModule.getOtherPatrolPoint(bevActorData.mActorIdx, out pos);
			//return _unit.getOtherPatrolPoint(out pos);
		}

		/// <summary>
		/// Gets the defense point in defense dungeon, if not in defense dungeon, return false
		/// </summary>
		/// <returns><c>true</c>, if defense point was gotten, <c>false</c> otherwise.</returns>
		/// <param name="pos">Position.</param>
		public bool getDefensePoint(out Vector3 pos)
		{
			return AIModule.getDefensePoint(bevActorData.mActorIdx, out pos);
			//return _unit.getDefensePoint(out pos);
		}

		/// <summary>
		/// Gets the birth point.
		/// </summary>
		/// <returns><c>true</c>, if birth point was gotten, <c>false</c> otherwise.</returns>
		/// <param name="pos">Position.</param>
		public bool getBirthPoint(out Vector3 pos)
		{
			return AIModule.getBirthPoint(bevActorData.mActorIdx, out pos);
			//return _unit.getBirthPoint(out pos);
		}

		/// <summary>
		/// Gets the random move point.
		/// </summary>
		/// <returns><c>true</c>, if random move point was gotten, <c>false</c> otherwise.</returns>
		/// <param name="pos">Position.</param>
		public bool getRandomMovePoint(out Vector3 pos)
		{
			return AIModule.getRandomMovePoint(bevActorData.mActorIdx, out pos);
			//return _unit.getRandomMovePoint(out pos);
		}
		#endregion
	}
}
