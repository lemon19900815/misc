using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_CloseTarget : BTNManager.Precondition
	{
		public Act_CloseTarget()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_CloseTarget;
			ExtraInfo.Instance.register(id, this);
		}

		public override BTNManager.eRunState update(InputParameter _input, OutputParameter _output)
		{
			if(_input.mSelf.attackTarget != null
			   && _input.mSelf.curState == AIState.AI_State_Move) {
				Vector3 myPos = _input.mSelf.getActorPosition();
				Vector3 targetPos = _input.mSelf.attackTarget.getActorPosition();

				float distance = Vector3.Distance(myPos, targetPos);
				if(distance < _input.mSelf.attackRadius) {
					return BTNManager.eRunState.State_Complete;
				} else {
					return BTNManager.eRunState.State_Executing;
				}
			}

			return BTNManager.eRunState.State_Unknow;
		}

		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			if(_input.mSelf.attackTarget != null
			   && _input.mSelf.canMove()) {
				_input.mSelf.curState = AIState.AI_State_Move;
				_input.mSelf.move(_input.mSelf.attackTarget);

				return true;
			}
		
			return false;
		}
	}
}
