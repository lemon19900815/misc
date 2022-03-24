using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_ChangeFirstAttackTarget : BTNManager.Precondition
	{
		public Act_ChangeFirstAttackTarget()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_ChangeFirstAttackTarget;
			ExtraInfo.Instance.register(id, this);
		}

		public override BTNManager.eRunState update(InputParameter _input, OutputParameter _output)
		{
			return BTNManager.eRunState.State_Complete;
		}

		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			AIActor hatestEnemy = _input.mSelf.getHatestEnemy();
			if(hatestEnemy != null) {
				_input.mSelf.attackTarget = hatestEnemy;
				_input.mSelf.attackTargetTime = Time.time;

				return true;
			} else {
				// 没有敌人时，把target处理成null
				_input.mSelf.attackTarget = null;
				_input.mSelf.attackTargetTime = Time.time;
			}
			
			return false;
		}
	}
}
