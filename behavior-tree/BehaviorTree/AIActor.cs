using System;
using System.Collections;
using System.Collections.Generic;

using UnityEngine;

namespace BTEngine
{
	public enum AIState
	{
		AI_State_Invalid	= 0,	// 无效状态
		AI_State_Idle		= 1,	// 空闲状态
		AI_State_Patrol		= 2,	// 巡逻
		AI_State_Trace		= 3,	// 追击
		AI_State_Attack		= 4,	// 攻击
		AI_State_Move		= 5,	// 移动
		AI_State_MoveTo		= 6,	// 移动到目标点
		AI_State_MoveDir	= 7,	// 朝指定方向移动
		AI_State_Defense	= 8,	// 防守状态--防守副本相关设定
		AI_State_Follow		= 9,	// 跟随
	}

	/// <summary>
	/// AI actor.
	/// behavior tree handle object
	/// </summary>
	public class AIActor
	{
		// 仇恨列表
		public Dictionary<AIActor, int> _hateEnemy;

		private BTEngine.BTNManager.BTNode _btRoot;	// 行为树根节点
		
		private BTEngine.InputParameter _input = new BTEngine.InputParameter();
		private BTEngine.OutputParameter _output = new BTEngine.OutputParameter();
		
		private BTEngine.BTNManager.BTNode _curNode;

		// 延迟帧数--ai执行的延迟帧数
		private int _curDelay = 0;

		// 一般帧数延迟数
		private const int _delay = 10;

		// 转身默认时间，主要用于执行攻击时，delay的设定
		private const int _turnBodyDelay = 5;

		// 中间层对象
		private BevActor _bevActor;

		// 中间层--应用层和ai模块交互的对象
		public BevActor bevActor
		{
			set { _bevActor = value; }
			get { return _bevActor; }
		}

		// 跟随对象
		private AIActor _follwerActor = null;

		public AIActor followerActor
		{
			set { _follwerActor = value; }
			get { return _follwerActor; }
		}

		// 攻击半径
		public float attackRadius
		{
			get { return bevActor.bevActorData.mAttackRadius; }
		}

		// 视野半径
		public float viewRadius
		{
			get { return bevActor.bevActorData.mViewRadius; }
		}

		// 追击半径
		public float traceRadius
		{
			get { return bevActor.bevActorData.mTraceRadius; }
		}

		// 视角度数
		public float viewDegree
		{
			get { return bevActor.bevActorData.mViewDegree; }
		}

		// 上次被攻击的时间
		private float _beAttackedTime = 0f;

		public float beAttackedTime
		{
			set { _beAttackedTime = value; }
			get { return _beAttackedTime; }
		}

		// 对象列表--移动、攻击，治疗对象列表,一般作为技能目标对象,向应用层下达攻击指令后，需要把这个列表清空
		private List<AIActor> _targetList;

		public List<AIActor> targetList
		{
			set { _targetList = value; }
			get { return _targetList; }
		}

		// 锁定的常用攻击对象--只有切换目标时，才会对它进行改变,在指定帧数下执行ai才会切换目标
		private AIActor _attackTarget;

		public AIActor attackTarget
		{
			set { _attackTarget = value; }
			get { return _attackTarget; }
		}

		// 攻击对象改变时间
		private float _attackTargetTime;

		public float attackTargetTime
		{
			set { _attackTargetTime = value; }
			get { return _attackTargetTime; }
		}

		// use attack skill index
		private int _attackSkillIdx;

		public int attackSkillIdx
		{
			set { _attackSkillIdx = value; }
			get { return _attackSkillIdx; }
		}

		// 上次成功释放的技能，如果被中断，则不记录
		private int _lastSucessCastSkillIndex;

		public int lastSucessCastSkillIndex
		{
			set { _lastSucessCastSkillIndex = value; }
			get { return _lastSucessCastSkillIndex; }
		}

		// 移动位置坐标
		private Vector3 _movetoTargetPos;

		public Vector3 movetoTargetPosition
		{
			set { _movetoTargetPos = value; }
			get { return _movetoTargetPos; }
		}

		// 当前ai执行的状态
		private AIState _curState;

		// 当前ai执行的状态
		public AIState curState
		{
			set { _curState = value; }
			get { return _curState; }
		}

		public bool init(int brainId)
		{
			_btRoot = BehaviorTree.Instance.getBTree(brainId);
			if(_btRoot != null) {
				_hateEnemy = new Dictionary<AIActor, int>();
				_input.mSelf = this;
				//_input.mTargets = new List<AIActor>();
				//_input.mTargetPt = Vector2.zero;
				_input.mExtraParamer = string.Empty;

				_output._curNode = null;

				_curDelay = 0;
				curState = AIState.AI_State_Idle;

				attackSkillIdx = -1;
				lastSucessCastSkillIndex = -1;

				beAttackedTime = 0.0f;

				attackTarget = null;
				attackTargetTime = 0.0f;

				followerActor = null;

				return true;
			}

			return false;
		}

		public void addHateEnemy(AIActor _ai)
		{
			if(!_hateEnemy.ContainsKey(_ai)) {
				_hateEnemy[_ai] = _ai.bevActor.bevActorData.mBaseHateVal;
			}
		}

		public void removeHateEnemy(AIActor _ai)
		{
			_hateEnemy.Remove(_ai);
		}

		// 仇恨值衰减
		private void decreaseHate()
		{
			if(_hateEnemy.Count > 0) {
				List<AIActor> keys = new List<AIActor>(_hateEnemy.Keys);

				foreach(AIActor key in keys)
				{
					int hateval = _hateEnemy[key] - 10;
					if(hateval < key.bevActor.bevActorData.mBaseHateVal)
						hateval = key.bevActor.bevActorData.mBaseHateVal;

					_hateEnemy[key] = hateval;
				}
			}
		}

		private void readyToRunBevTree()
		{
			attackSkillIdx = -1;
			curState = AIState.AI_State_Idle;
		}

		// 每帧调用--由调用层触发
		public void update()
		{
			decreaseHate();

			if(_curDelay-- <= 0 || curState == AIState.AI_State_Idle) {
				readyToRunBevTree();

				if(_btRoot.run(_input, _output)) {
					_curNode = _output._curNode;
					//_curDelay = _delay;
				} else {
					if(AIModule.isDebug && _output._curNode != null) {
						Debug.Log(bevActor.bevActorData.mActorIdx + ", actor " + _output._curNode.name + "," +
							_output._curNode.msg + ": bt node exe failed");
					} 
				}
			} else {
				run();
			}
		}

		// behavior tree node update check precondition
		private void run()
		{
			if(_curNode != null) {
				if(_curNode.upadte(_input, _output) != BTEngine.BTNManager.eRunState.State_Executing) {
					_curDelay = 0;
				}
			}
		}

		// 重置--行为树需要重新搜索对象
		public void reset()
		{
			targetList = null;
			attackTarget = null;
			curState = AIState.AI_State_Idle;
			movetoTargetPosition = Vector3.zero;
		}

		public void onCastEnd(int skillIndex, bool isBroken)
		{
			_curDelay = 0;
			curState = AIState.AI_State_Idle;

			if(isBroken) { // 技能被中断，连招需要重新设定
				lastSucessCastSkillIndex = -1; // 从头开始
			} else {
				lastSucessCastSkillIndex = skillIndex;
			}
		}

		public void onDied()
		{
		}

		/// <summary>
		/// Gets the hatest enemy.
		/// </summary>
		/// <returns>The hatest enemy.</returns>
		public AIActor getHatestEnemy()
		{
			AIActor retval = null;
			int compareVal = 0;
			
			foreach(KeyValuePair<AIActor, int> kp in _hateEnemy)
			{
				if(kp.Value > compareVal) {
					compareVal = kp.Value;
					retval = kp.Key;
				}
			}
			
			return retval;
		}

		// 计算
		private int calcHateValue(int damage)
		{
			return damage;
		}

		// 被攻击--累计仇恨值，可能附带处理一些其他操作
		public void onAttacked(AIActor _attacker, int damage)
		{
			if(_hateEnemy.ContainsKey(_attacker)) {
				_hateEnemy[_attacker] += calcHateValue(damage);
			} else {
				_hateEnemy[_attacker] = calcHateValue(damage);
			}

			_beAttackedTime = Time.time;
		}

		// 攻击目标--skillIdx技能索引，_targets攻击列表,向中间层下达攻击指令
		public void attack(int skillIdx, AIActor _target)
		{
			if(bevActor != null) {
				bevActor.doAttack(skillIdx, _target);

				int frameDelay = getSkillCastFrameTime(skillIdx) + _turnBodyDelay;
//				if(frameDelay < _delay) {
//					frameDelay = _delay;
//				}

				_curDelay = frameDelay;
			}
		}

		// 向目标位置移动
		public void move(AIActor _target)
		{
			if(bevActor != null) {
				bevActor.doMove(_target);
				_curDelay = _delay;
			}
		}

		// 移动到指定位置
		public void moveTo(Vector3 _pos)
		{
			if(bevActor != null) {
				bevActor.doMoveTo(_pos);
				_curDelay = _delay;
			}
		}

		// 以speed速度向_dir方向移动
		public void moveDir(Vector3 _dir, float _speed, int delay)
		{
			if(bevActor != null) {
				bevActor.doMoveDir(_dir, _speed);
				_curDelay = Math.Min(_delay, delay);
			}
		}

		// 获取移动速度
		public float getMoveSpeed()
		{
			float speed = 1.0f; // default value

			if(bevActor != null) {
				speed = bevActor.getMoveSpeed();
			}

			return speed;
		}

		/// <summary>
		/// Gets the hp info.
		/// </summary>
		/// <returns><c>true</c>, if hp info was gotten, <c>false</c> otherwise.</returns>
		/// <param name="_cur">_cur.</param>
		/// <param name="_max">_max.</param>
		public bool getHpInfo(out int _cur, out int _max)
		{
			return bevActor.getHpInfo(out _cur, out _max);
		}

		/// <summary>
		/// ai actor is died or not.
		/// </summary>
		/// <returns><c>true</c>, if died was ised, <c>false</c> otherwise.</returns>
		public bool isDied()
		{
			return bevActor.isDied();
		}

		public List<AIActor> getEnemyInAttackRange()
		{
			return bevActor.getTargetsInRange(attackRadius, viewDegree, false);
		}

		public List<AIActor> getFriendInAttackRange()
		{
			return bevActor.getTargetsInRange(attackRadius, viewDegree, true);
		}

		public List<AIActor> getEnemyInViewRange()
		{
			return bevActor.getTargetsInRange(viewRadius, viewDegree, false);
		}

		public List<AIActor> getFriendInViewRange()
		{
			return bevActor.getTargetsInRange(viewRadius, viewDegree, true);
		}

		public List<AIActor> getEnemyInTraceRange()
		{
			return bevActor.getTargetsInRange(traceRadius, viewDegree, false);
		}
		
		public List<AIActor> getFriendInTraceRange()
		{
			return bevActor.getTargetsInRange(traceRadius, viewDegree, true);
		}

		public List<AIActor> getAllEnemyTargets()
		{
			// 全方位360度查找
			return bevActor.getTargetsInRange(1000000.0f, 360.0f, false);
		}

		public int getEnemyNumInAttackRange()
		{
			int count = 0;

			List<AIActor> enemy = getEnemyInAttackRange();
			if(enemy != null)
				count = enemy.Count;

			return count;
		}

		public int getFriendNumInAttackRange()
		{
			int count = 0;
			
			List<AIActor> enemy = getFriendInAttackRange();
			if(enemy != null)
				count = enemy.Count;
			
			return count;
		}

		public int getEnemyNumInViewRange()
		{
			int count = 0;
			
			List<AIActor> enemy = getEnemyInViewRange();
			if(enemy != null)
				count = enemy.Count;
			
			return count;
		}

		public int getFriendNumInViewRange()
		{
			int count = 0;
			
			List<AIActor> enemy = getFriendInViewRange();
			if(enemy != null)
				count = enemy.Count;
			
			return count;
		}

		public Vector3 getActorPosition()
		{
			return bevActor.getActorPosition();
		}

		public Vector3 getTraceStartPoint()
		{
			return bevActor.getTraceStartPoint();
		}

		public bool getNextDungeonPoint(out Vector3 pos)
		{
			return bevActor.getNextDungeonPoint(out pos);
		}

		public bool getOtherPatrolPoint(out Vector3 pos)
		{
			return bevActor.getOtherPatrolPoint(out pos);
		}

		public bool getDefensePoint(out Vector3 pos)
		{
			return bevActor.getDefensePoint(out pos);
		}

		public bool getBirthPoint(out Vector3 pos)
		{
			return bevActor.getBirthPoint(out pos);
		}

		public bool getRandomMovePoint(out Vector3 pos)
		{
			return bevActor.getRandomMovePoint(out pos);
		}

		public bool canMove()
		{
			return bevActor.canMove();
		}

		public bool canDoAttack(int skillIndex)
		{
			return bevActor.canAttack(skillIndex);
		}

		public int getSkillCastFrameTime(int skillIndex)
		{
			float secTime = bevActor.getSkillCastTime(skillIndex);
			return (int)(secTime / 20);
		}

		public bool isBuffSkill(int skillIndex)
		{
			return bevActor.getSkillType(skillIndex) == (int)SkillType.ST_Buff;
		}

		public bool isDebuffSkill(int skillIndex)
		{
			return bevActor.getSkillType(skillIndex) == (int)SkillType.ST_Debuff;
		}
	}
}


