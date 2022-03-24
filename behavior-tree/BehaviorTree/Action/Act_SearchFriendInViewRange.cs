using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_SearchFriendInViewRange : BTNManager.Precondition
	{
		public Act_SearchFriendInViewRange()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_SearchFriendInViewRange;
			ExtraInfo.Instance.register(id, this);
		}
		
		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			List<AIActor> enemy = _input.mSelf.getFriendInViewRange();
			if(enemy != null && enemy.Count > 0) {
				_input.mSelf.targetList = null;
				_input.mSelf.targetList = enemy;
				
				return true;
			}

			return false;
		}
	}
}
