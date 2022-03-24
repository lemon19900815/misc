using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_MovetoTargetPosition : BTNManager.Precondition
	{
		public Act_MovetoTargetPosition()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_MovetoTargetPosition;
			ExtraInfo.Instance.register(id, this);
		}

		public override BTNManager.eRunState update(InputParameter _input, OutputParameter _output)
		{
			if(_input.mSelf.curState == AIState.AI_State_MoveTo) {
				Vector3 myPos = _input.mSelf.getActorPosition();
				
				float distance = Vector3.Distance(myPos, _input.mSelf.movetoTargetPosition);
				if(distance < 0.5f) {
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
				_input.mSelf.curState = AIState.AI_State_MoveTo;
				_input.mSelf.moveTo(_input.mSelf.movetoTargetPosition);
			}
			
			return false;
		}
	}
}
