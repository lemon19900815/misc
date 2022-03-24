using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_SearchBirthPoint : BTNManager.Precondition
	{
		public Act_SearchBirthPoint()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_SearchBirthPoint;
			ExtraInfo.Instance.register(id, this);
		}
		
		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			Vector3 pt = new Vector3();
			if(_input.mSelf.getBirthPoint(out pt)) {
				_input.mSelf.movetoTargetPosition = pt;
				//_input.mSelf.curState = AIState.AI_State_Defense;
				
				return true;
			}
			
			return false;
		}
	}
}
