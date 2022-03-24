using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_FollowTarget : BTNManager.Precondition
	{
		public Act_FollowTarget()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_FollowTarget;
			ExtraInfo.Instance.register(id, this);
		}

		public override BTNManager.eRunState update(InputParameter _input, OutputParameter _output)
		{
			if(_input.mSelf.followerActor != null
			   && _input.mSelf.curState == AIState.AI_State_Follow) {
				Vector3 myPos = _input.mSelf.getActorPosition();
				Vector3 targetPos = _input.mSelf.followerActor.getActorPosition();
				
				float distance = Vector3.Distance(myPos, targetPos);
				if(distance < 1.0f) {
					return BTNManager.eRunState.State_Complete;
				} else {
					return BTNManager.eRunState.State_Executing;
				}
			}
			
			return BTNManager.eRunState.State_Unknow;
		}

		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			if(_input.mSelf.followerActor != null) {
				_input.mSelf.curState = AIState.AI_State_Follow;
				_input.mSelf.move(_input.mSelf.followerActor);

				return true;
			}

			return false;
		}
	}
}
