using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_BackSomeStep : BTNManager.Precondition
	{
		public Act_BackSomeStep()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_BackSomeStep;
			ExtraInfo.Instance.register(id, this);
		}

		public override BTNManager.eRunState update(InputParameter _input, OutputParameter _output)
		{
			if(_input.mSelf.attackTarget != null
			   && _input.mSelf.curState == AIState.AI_State_MoveDir) {
				Vector3 myPos = _input.mSelf.getActorPosition();
				Vector3 targetPos = _input.mSelf.attackTarget.getActorPosition();
				
				float distance = Vector3.Distance(myPos, targetPos);
				if(_input.mSelf.attackRadius - distance < 1.0f) {
					return BTNManager.eRunState.State_Complete;
				} else {
					return BTNManager.eRunState.State_Executing;
				}
			}
			
			return BTNManager.eRunState.State_Unknow;
		}

		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			if(_input.mSelf.attackTarget != null) {
				Vector3 myPos = _input.mSelf.getActorPosition();
				Vector3 targetPos = _input.mSelf.attackTarget.getActorPosition();

				Vector3 delta = targetPos - myPos;	// 后退方向
				float distance = _input.mSelf.attackRadius - delta.magnitude - 0.5f;

				if(distance > 0) {
					float speed = _input.mSelf.getMoveSpeed();
					if(speed > 0) {
						float delay = distance / speed * 20;

						_input.mSelf.moveDir(delta.normalized, speed, (int)delay);
						_input.mSelf.curState = AIState.AI_State_MoveDir;

						return true;
					}
				}
			}

			return false;
		}
	}
}
