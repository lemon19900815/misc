using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_SearchNextDungeonPoint : BTNManager.Precondition
	{
		public Act_SearchNextDungeonPoint()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_SearchNextDungeonPoint;
			ExtraInfo.Instance.register(id, this);
		}
		
		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			Vector3 nextDungeonPoint = new Vector3();
			if(_input.mSelf.getNextDungeonPoint(out nextDungeonPoint)) {
				_input.mSelf.curState = AIState.AI_State_MoveTo;
				//_input.mSelf.movetoTargetPosition = nextDungeonPoint;
				return true;
			}
			
			return false;
		}
	}
}
