using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class Cond_HpPercentLessThan : BTNManager.Precondition
	{
		public Cond_HpPercentLessThan()
		{
			int id = (int)ExtraInfo.eConditionID.Cond_ID_Self_HpPercentLessThan;
			ExtraInfo.Instance.register(-id, this);
		}

		public override bool isTrue(InputParameter _input, OutputParameter _output)
		{
			int per;
			if(!int.TryParse(_input.mExtraParamer, out per))
				return false;

			int cur, max;
			if(_input.mSelf.getHpInfo(out cur,out max)) {
				if(max > 0) {
					return ((cur / max) < per);
				}
			}

			return false;
		}
	}
}
