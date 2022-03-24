using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class AITest
{
	public static void doTest()
	{
		if(!AIModule.createAIWorld(false)) {
			Debug.LogError("create ai world failed");
			return;
		}

		BTEngine.BevActorData bevData = new BTEngine.BevActorData();
		bevData.mActorId = 0;
		bevData.mActorIdx = 0;
		bevData.mActorType = 0;
		bevData.mBaseHateVal = 10;
		bevData.mAttackRadius = 3.0f;
		bevData.mViewRadius = 5.0f;
		bevData.mTraceRadius = 10.0f;
		bevData.mViewDegree = 360.0f;
		bevData.mActorCampID = BTEngine.CampID.Camp_ID_A;
		bevData.mAttackType = BTEngine.AttackType.AT_Remote;

		BTEngine.BevActor bevActor = AIModule.addAIObj(3, bevData, null); // 搜索怪物
		if(bevActor == null) {
			Debug.LogError("1->add ai object to ai world failed");
			return;
		}

		BTEngine.BevActorData bevData2 = new BTEngine.BevActorData();
		bevData2.mActorId = 1;
		bevData2.mActorIdx = 1;
		bevData2.mActorType = 0;
		bevData2.mBaseHateVal = 10;
		bevData2.mAttackRadius = 2.0f;
		bevData2.mViewRadius = 5.0f;
		bevData2.mTraceRadius = 10.0f;
		bevData2.mViewDegree = 360.0f;
		bevData2.mActorCampID = BTEngine.CampID.Camp_ID_B;
		bevData2.mAttackType = BTEngine.AttackType.AT_ShortRange;
		
		BTEngine.BevActor bevActor2 = AIModule.addAIObj(4, bevData2, null); // 搜索怪物
		if(bevActor2 == null) {
			Debug.LogError("2->add ai object to ai world failed");
			return;
		}

		AIModule.startAI();
	}
}
