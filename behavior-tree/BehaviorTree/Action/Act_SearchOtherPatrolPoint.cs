using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_SearchOtherPatrolPoint : BTNManager.Precondition
	{
		public Act_SearchOtherPatrolPoint()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_SearchOtherPatrolPoint;
			ExtraInfo.Instance.register(id, this);
		}
		
		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			Vector3 otherPatrolPoint = new Vector3();
			if(_input.mSelf.getOtherPatrolPoint(out otherPatrolPoint)) {
				_input.mSelf.movetoTargetPosition = otherPatrolPoint;
				//_input.mSelf.curState = AIState.AI_State_Patrol;
				return true;
			}
			
			return false;
		}
	}
}

