using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Act_SearchEnemyInAttackRange : BTNManager.Precondition
	{
		public Act_SearchEnemyInAttackRange()
		{
			int id = (int)ExtraInfo.eActionID.Act_ID_SearchEnemyInAttackRange;
			ExtraInfo.Instance.register(id, this);
		}
		
		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			List<AIActor> enemy = _input.mSelf.getEnemyInAttackRange();
			if(enemy != null && enemy.Count > 0) {
				_input.mSelf.targetList = null;
				_input.mSelf.targetList = enemy;

				return true;
			}

			return false;
		}
	}
}
