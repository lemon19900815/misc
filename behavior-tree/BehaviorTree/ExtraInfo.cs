using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class ExtraInfo
	{
		public enum eConditionID
		{
			Cond_ID_Invlid								= 0,		// 无效

			Cond_ID_Self_HpPercentLessThan				= 1,		// 自己血量超过百分比--extra 需要填写百分比
			Cond_ID_Self_HpPercentMoreThan				= 2,		// 血量高于某一百分比--extraParam-填写百分比数值
			Cond_ID_Self_HpValueLessThan				= 3,		// 自己血量低于百分比--extra 需要填写数值
			Cond_ID_Self_HpValueMoreThan				= 4,		// 血量高于某一数值--extraParam-填写具体数值

			Cond_ID_EnemyNumMoreThan_InAttackRange		= 5,		// 攻击目标内敌人数量多余N个--extraParam-填写具体个数
			Cond_ID_EnemyNumLessThan_InAttackRange		= 6,		// 攻击目标内敌人数量少余N个--extraParam-填写具体个数
			Cond_ID_FriendNumMoreThan_InAttackRange		= 7,		// 攻击目标内友军数量多余N个--extraParam-填写具体个数
			Cond_ID_FriendNumLessThan_InAttackRange		= 8,		// 攻击目标内友军数量少余N个--extraParam-填写具体个数

			Cond_ID_EnemyNumMoreThan_InViewRange		= 9,		// 视野范围内内敌人人数量多余N个--extraParam-填写具体个数
			Cond_ID_EnemyNumLessThan_InViewRange		= 10,		// 攻视野范围内敌人数量少余N个--extraParam-填写具体个数
			Cond_ID_FriendNumMoreThan_InViewRange		= 11,		// 视野范围内内友军数量多余N个--extraParam-填写具体个数
			Cond_ID_FriendNumLessThan_InViewRange		= 12,		// 视野范围内内友军数量少余N个--extraParam-填写具体个数

			Cond_ID_HaveFollower						= 13,		// 有跟随对象--并且未死亡
			Cond_ID_Follower_Outof_ViewRange			= 14,		// 跟随对象超出视野范围

			Cond_ID_Have_Target							= 15,		// 有攻击目标，并且攻击目标没有死亡
			Cond_ID_Not_Have_Target						= 16,		// 没有有攻击目标，或者攻击目标已经死亡

			Cond_ID_Target_IsInAttackRange				= 17,		// 目标在攻击范围内或追击目标进入到攻击范围内
			Cond_ID_Target_IsNotInAttackRange			= 18,		// 目标不在攻击范围内
			Cond_ID_Target_IsInViewRange				= 19,		// 目标在视野范围内
			Cond_ID_Target_IsNotInViewRange				= 20,		// 目标不在视野范围内
			Cond_ID_Target_IsInTraceRange				= 21,		// 目标在追击范围内
			Cond_ID_Target_IsNotInTraceRange			= 22,		// 目标不在追击范围内

			Cond_ID_OutOf_Trace_Range					= 23,		// 超出追击点范围--自己与基准点进行距离比较，只有怪物才会有基准点,玩家会一直追怪，直到目标发生改变为止

			Cond_ID_Arrived_TargetPosition				= 24,		// 到达目的地附近-以距离0.5f作为指标
			Cond_ID_NotArrivedTargetPosition			= 25,		// 没有到达目的地附近-以距离0.5f作为指标
			Cond_ID_Near_Target							= 26,		// 已经靠近目标-以距离0.5f作为指标
			Cond_ID_NotNear_Target						= 27,		// 还没有靠近目标
			Cond_ID_Near_Follower						= 28,		// 已经靠近跟随目标-以距离0.5f作为指标
			Cond_ID_NotNear_Follower					= 29,		// 还没有靠近跟随目标

			Cond_ID_NoEnemy_InCurScene					= 30,		// 当前场景中没有敌人

			Cond_ID_Outof_DefensePoint					= 31,		// 超出防守点范围--以视野范围进行判断

			Cond_ID_NotBeAttackedInSomeSecond			= 32,		// 多少秒内未被攻击--extraParam-填写秒
			Cond_ID_BeAttacked							= 33,		// 被攻击--被别人攻击（delta值小于0.1f）

			Cond_ID_IsBuffSkill							= 34,		// 是否是增益技能--作用于己方（前提条件都必须使用行为节点{Act_ID_SearchSkill: 搜索可以使用的技能}作为前提）
			Cond_ID_IsDebuffSkill						= 35,		// 减益技能--作用于敌人

			Cond_ID_IsTargetLockedTimeOut				= 36,		// 目标锁定是否超时----extraParam-填写时间，秒

			Cond_ID_Self_IsRemoteActor					= 37,		// 自身的攻击方式是远程
			Cond_ID_Self_IsShortRangeActor				= 38,		// 自身的攻击方式是近程

			Cond_ID_TargetIsTooNear						= 39,		// 目标太过靠近自己(主要用于放风筝怪物的相关设定)--以攻击距离的一半作为判断依据
			Cond_ID_TargetIsNotTooNear					= 40,		// 目标没有太过靠近自己(主要用于放风筝怪物的相关设定)--以攻击距离的一半作为判断依据

			Cond_ID_DistanceFromTargetLessThan			= 41,		// 与目标距离小于多少米--extraParam-填写米（浮点数）
			Cond_ID_DistanceFromTargetMoreThan			= 42,		// 与目标距离大于多少米--extraParam-填写米（浮点数）

			Cond_ID_StrategyPreSkillIsCasted			= 43,		// 前置技能是否释放----extraParam-填写前置技能索引(0、1、2、3)
			Cond_ID_CanCastSkill						= 44,		// 是否能够释放技能（由应用层检测冷却cd，状态相关条件）----extraParam-填写技能索引(0、1、2、3)

			Cond_ID_Max,
		}

		public enum eActionID
		{
			Act_ID_Invlid								= 0,		// 无效

			Act_ID_SearchEnemyInAttackRange				= 1,		// 搜索攻击范围内的敌人-技能辅助对象相关
			Act_ID_SearchFriendInAttackRange			= 2,		// 搜索攻击范围内的友军-技能辅助对象相关
			Act_ID_SearchEnemyInViewRange				= 3,		// 搜索视野范围内的敌人-技能辅助对象相关
			Act_ID_SearchFriendInViewRange				= 4,		// 搜索视野范围内的友军-技能辅助对象相关
			Act_ID_SearchEnemyInTraceRange				= 5,		// 搜索追击范围内的敌人-技能辅助对象相关
			Act_ID_SearchFriendInTraceRange				= 6,		// 搜索追击范围内的友军-技能辅助对象相关

			Act_ID_SearchNextDungeonPoint				= 7,		// 搜索下个副本节点
			Act_ID_SearchOtherPatrolPoint				= 8,		// 搜索其他巡逻点(到达巡逻点附近)
			Act_ID_SearchDefensePoint					= 9,		// 搜索防守副本节点
			Act_ID_SearchBirthPoint						= 10,		// 搜索出生点
			Act_ID_SearchRandomPoint					= 11,		// 搜索随机可移动点

			Act_ID_ChangeFirstAttackTarget				= 12,		// 转换第一攻击目标，直接对仇恨列表进行匹配
			Act_ID_CloseTarget							= 13,		// 靠近、追击目标
			Act_ID_FollowTarget							= 14,		// 跟随目标
			Act_ID_MovetoTargetPosition					= 15,		// 移动到目标点(下个副本节点，防守副本位置等)
			Act_ID_DoAttack								= 16,		// 执行攻击
			Act_ID_BackSomeStep							= 17,		// 后退几步试试--(主要用于放风筝怪物的相关设定)，后面如果有障碍，不能够后退，则

			Act_ID_SetIdle								= 18,		// 设置ai空闲状态，强制下一帧重新执行ai

			Act_ID_Max,
		}

		private static ExtraInfo _inst;
		public static ExtraInfo Instance
		{
			get
			{
				if(_inst == null) {
					_inst = new ExtraInfo();
					_inst.registerAll();
				}

				return _inst;
			}
		}

		private Dictionary<int, BTNManager.Precondition> _dictPreconditions = new Dictionary<int, BTNManager.Precondition>();

		public void register(int id, BTNManager.Precondition node)
		{
			_dictPreconditions[id] = node;
		}

		public BTNManager.Precondition getPrecondition(int id)
		{
			BTNManager.Precondition retval = null;

			if(_dictPreconditions.ContainsKey(id)) {
				retval = _dictPreconditions[id];
			}

			return retval;
		}

		private void registerAll()
		{
			new Cond_HpPercentLessThan();
			new Cond_HpPercentMoreThan();
			new Cond_HpValueLessThan();
			new Cond_HpValueMoreThan();
			new Cond_EnemyNumMoreThanInAttackRange();
			new Cond_EnemyNumLessThanInAttackRange();
			new Cond_FriendNumMoreThanInAttackRange();
			new Cond_FriendNumLessThanInAttackRange();
			new Cond_EnemyNumMoreThanInViewRange();
			new Cond_EnemyNumLessThanInViewRange();
			new Cond_FriendNumMoreThanInViewRange();
			new Cond_FriendNumLessThanInViewRange();
			new Cond_HaveFollower();
			new Cond_FollowerOutofViewRange();
			new Cond_HaveTarget();
			new Cond_NotHaveTarget();
			new Cond_TargetIsInAttackRange();
			new Cond_TargetIsNotInAttackRange();
			new Cond_TargetIsInViewRange();
			new Cond_TargetIsNotInViewRange();
			new Cond_TargetIsInTraceRange();
			new Cond_TargetIsNotInTraceRange();
			new Cond_OutOfTraceRange();
			new Cond_ArrivedTargetPosition();
			new Cond_NotArrivedTargetPosition();
			new Cond_NearTarget();
			new Cond_NotNearTarget();
			new Cond_NearFollower();
			new Cond_NotNearFollower();
			new Cond_NoEnemyInCurScene();
			new Cond_OutofDefensePoint();
			new Cond_NotBeAttackedInSomeSecond();
			new Cond_BeAttacked();
			new Cond_IsBuffSkill();
			new Cond_IsDebuffSkill();
			new Cond_IsTargetLockedTimeOut();
			new Cond_SelfIsRemoteActor();
			new Cond_SelfIsShortRangeActor();
			new Cond_TargetIsTooNear();
			new Cond_TargetIsNotTooNear();
			new Cond_DistanceFromTargetLessThan();
			new Cond_DistanceFromTargetMoreThan();
			new Cond_StrategyPreSkillIsCasted();
			new Cond_CanCastSkill();

			new Act_SearchEnemyInAttackRange();
			new Act_SearchFriendInAttackRange();
			new Act_SearchEnemyInViewRange();
			new Act_SearchFriendInViewRange();
			new Act_SearchEnemyInTraceRange();
			new Act_SearchFriendInTraceRange();
			new Act_SearchNextDungeonPoint();
			new Act_SearchOtherPatrolPoint();
			new Act_SearchDefensePoint();
			new Act_SearchBirthPoint();
			new Act_SearchRandomPoint();
			new Act_ChangeFirstAttackTarget();
			new Act_CloseTarget();
			new Act_FollowTarget();
			new Act_MovetoTargetPosition();
			new Act_DoAttack();
			new Act_BackSomeStep();
			new Act_SetIdle();
		}
	}
}
