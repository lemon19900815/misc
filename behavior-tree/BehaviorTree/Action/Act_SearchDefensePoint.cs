using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_SearchDefensePoint : BTNManager.Precondition
	{
		public Act_SearchDefensePoint()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_SearchDefensePoint;
			ExtraInfo.Instance.register(id, this);
		}
		
		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			Vector3 pt = new Vector3();
			if(_input.mSelf.getDefensePoint(out pt)) {
				_input.mSelf.movetoTargetPosition = pt;
				_input.mSelf.curState = AIState.AI_State_Defense;

				return true;
			}
			
			return false;
		}
	}
}
