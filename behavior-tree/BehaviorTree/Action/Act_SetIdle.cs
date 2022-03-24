using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_SetIdle : BTNManager.Precondition
	{
		public Act_SetIdle()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_SetIdle;
			ExtraInfo.Instance.register(id, this);
		}
		
		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			_input.mSelf.curState = AIState.AI_State_Idle;
			
			return true;
		}
	}
}
