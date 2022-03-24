using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_DoAttack : BTNManager.Precondition
	{
		public Act_DoAttack()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_DoAttack;
			ExtraInfo.Instance.register(id, this);
		}

		public override BTNManager.eRunState update(InputParameter _input, OutputParameter _output)
		{
			return BTNManager.eRunState.State_Executing;
		}

		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			if(_input.mSelf.canDoAttack(_input.mSelf.attackSkillIdx)) {
				if(_input.mSelf.attackSkillIdx != -1) {
					AIActor target = null;

					if(_input.mSelf.isBuffSkill(_input.mSelf.attackSkillIdx)) { // 正面技能对自己释放
						target = _input.mSelf;
					} else if(_input.mSelf.isDebuffSkill(_input.mSelf.attackSkillIdx)) { // 负面技能对敌人释放
						target = _input.mSelf.attackTarget;
					}

					if(target != null) {
						_input.mSelf.attack(_input.mSelf.attackSkillIdx, target);
						_input.mSelf.curState = AIState.AI_State_Attack;

						return true;
					}
				}
			}
			
			return false;
		}
	}
}
