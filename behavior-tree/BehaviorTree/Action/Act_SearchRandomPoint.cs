using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_SearchRandomPoint : BTNManager.Precondition
	{
		public Act_SearchRandomPoint()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_SearchRandomPoint;
			ExtraInfo.Instance.register(id, this);
		}
		
		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			Vector3 randomPt;
			if(_input.mSelf.getRandomMovePoint(out randomPt)) {
				_input.mSelf.movetoTargetPosition = randomPt;
				//_input.mSelf.curState = AIState.AI_State_MoveTo;

				return true;
			}

			return false;
		}
	}
}
