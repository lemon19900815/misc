using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class AIModule : MonoBehaviour
{
	public class AIPoint3
	{
		public float mX;
		public float mY;
		public float mZ;
	}

	public class HpInfo
	{
		public int cur;
		public int max;
	};
	
	public delegate void PDoSkill(int actorIdx, int skillIndex, int targetIdx);
	public delegate void PDoMove(int actorIdx, int targetIndex);
	public delegate void PDoMoveTo(int actorIdx, Vector3 to);
	public delegate void PDoMoveDir(int actorIdx, Vector3 dir, float speed);
	public delegate float PGetMoveSpeed(int actorIdx);
	public delegate bool PCanMove(int actorIdx);
	public delegate bool PCanDoAttack(int actorIdx, int skillIndex);
	public delegate float PGetSkillCastTime(int actorIdx, int skillIndex);
	public delegate int PGetSkillType(int actorIdx, int skillIndex);
	public delegate bool PGetHpInfo(int actorIdx);
	public delegate List<BTEngine.BevActor> PGetTargetsInRange(int actorIdx, float radius, float angle, bool sameCamp);
	public delegate bool PGetActorPosition(int actorIdx);
	public delegate bool PGetTraceStartPoint(int actorIdx);
	public delegate bool PGetNextDungeonPoint(int actorIdx);
	public delegate bool PGetOtherPatrolPoint(int actorIdx);
	public delegate bool PGetDefensePoint(int actorIdx);
	public delegate bool PGetBirthPoint(int actorIdx);
	public delegate bool PGetRandomMovePoint(int actorIdx);

	public static PDoSkill _doSkill;
	public static PDoMove _doMove;
	public static PDoMoveTo _doMoveTo;
	public static PDoMoveDir _doMoveDir;
	public static PGetMoveSpeed _getMoveSpeed;
	public static PCanMove _canMove;
	public static PCanDoAttack _canDoAttack;
	public static PGetSkillCastTime _getSkillCastTime;
	public static PGetSkillType _getSkillType;
	public static PGetHpInfo _getHpInfo;
	public static PGetTargetsInRange _getTargetsInRange;
	public static PGetActorPosition _getActorPosition;
	public static PGetTraceStartPoint _getTraceStartPoint;
	public static PGetNextDungeonPoint _getNextDungeonPoint;
	public static PGetOtherPatrolPoint _getOtherPatrolPoint;
	public static PGetDefensePoint _getDefensePoint;
	public static PGetBirthPoint _getBirthPoint;
	public static PGetRandomMovePoint _getRandomMovePoint;

	public static HpInfo _hpInfo = new HpInfo();
	public static AIPoint3 _aiPoint3 = new AIPoint3();

	// show ai debug info or not
	public bool mDebug;

	private static bool _debug = false;
	public static bool isDebug
	{
		get { return _debug; }
	}

	public static bool _dataOk;

	private static bool _occurError;

	public static AIDataLoader _loader;

	private int _downloadResNum;
	private Dictionary<string, int> _aiXmlData;

	void Start()
	{
		_debug = mDebug;

		_dataOk = false;
		_occurError = false;

		_downloadResNum = 0;
		_loader = new AIDataLoader();
		_aiXmlData = new Dictionary<string, int>();

		_loader._onLoadFinished = onLoadFinished;

		preload();
	}

	void Update()
	{
		if(_downloadResNum == 0) {
			if(!_occurError && !_dataOk) {
				_dataOk = true;
				Debug.Log("load ai data success");

				// test
				//AITest.doTest();
			}

			return;
		}

		if(_loader != null) {
			_loader.Update();
		}
	}

	void FixedUpdate()
	{
		onUpdateAI();
	}

	void preload()
	{
		Debug.Log("preload ai_define.txt...");

		string aifile = "/Settings/ai/pvp/ai_define.txt";
		_loader.addDataItemToWaitLoad("ai_define", aifile);
		_downloadResNum += 1;
	}

	void onLoadFinished(WWW www, string file)
	{
		if(file == "ai_define") {
			onParseAIConfig(www.text);
		} else {
			if(_aiXmlData.ContainsKey(file)) {
				BTEngine.BTNManager.BTNode bevTreeRoot = BTEngine.BehaviorTree.Instance.buildBTree(_aiXmlData[file], www.bytes);
				if(bevTreeRoot == null) {
					_occurError = true;

					Debug.LogError("failed to load aihandle:" + file);
				}

				_downloadResNum -= 1;
			}
		}
	}

	void onParseAIConfig(string txt)
	{
		TableFile tabFile = new TableFile("ai_define");
		List<string[]> lines = tabFile.AnalyzeString(txt);
		
		for(int i = 1; i < lines.Count; ++i)
		{
			string[] line = lines[i];
			
			int aiHandle = int.Parse(line[1]);
			string path = line[2];
			
			_aiXmlData[aiHandle.ToString()] = aiHandle;
			
			_downloadResNum += 1;
			
			_loader.addDataItemToWaitLoad(aiHandle.ToString(), path);
		}
		
		_downloadResNum -= 1;
	}

	/// <summary>
	/// Dos the skill.
	/// </summary>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="skillIndex">Skill index.</param>
	/// <param name="targetIdx">TargetIdx.</param>
	public static void doSkill(int actorIdx, int skillIndex, int targetIdx)
	{
		if(_doSkill != null) {
			_doSkill(actorIdx, skillIndex, targetIdx);
		}
	}

	/// <summary>
	/// Dos the move.
	/// </summary>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="targetIndex">Target index.</param>
	public static void doMove(int actorIdx, int targetIndex)
	{
		if(_doMove != null) {
			_doMove(actorIdx, targetIndex);
		}
	}

	/// <summary>
	/// Dos the move to target position
	/// </summary>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="to">To.</param>
	public static void doMoveTo(int actorIdx, Vector3 to)
	{
		if(_doMoveTo != null) {
			_doMoveTo(actorIdx, to);
		}
	}

	/// <summary>
	/// Dos the move dir.
	/// </summary>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="dir">Dir.</param>
	/// <param name="speed">Speed.</param>
	public static void doMoveDir(int actorIdx, Vector3 dir, float speed)
	{
		if(_doMoveDir != null) {
			_doMoveDir(actorIdx, dir, speed);
		}
	}

	/// <summary>
	/// Gets the move speed.
	/// </summary>
	/// <returns>The move speed.</returns>
	/// <param name="actorIdx">Actor index.</param>
	public static float getMoveSpeed(int actorIdx)
	{
		float speed = 0.0f;

		if(_getMoveSpeed != null) {
			speed = _getMoveSpeed(actorIdx); 
		}

		return speed;
	}

	/// <summary>
	/// Cans the move.
	/// </summary>
	/// <returns><c>true</c>, if move was caned, <c>false</c> otherwise.</returns>
	/// <param name="actorIdx">Actor index.</param>
	public static bool canMove(int actorIdx)
	{
		bool can = false;

		if(_canMove != null) {
			can = _canMove(actorIdx);
		}

		return can;
	}

	/// <summary>
	/// Cans the do attack.
	/// </summary>
	/// <returns><c>true</c>, if do attack was caned, <c>false</c> otherwise.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="skillIdx">Skill index.</param>
	public static bool canDoAttack(int actorIdx, int skillIdx)
	{
		bool can = false;

		if(_canDoAttack != null) {
			can = _canDoAttack(actorIdx, skillIdx);
		}

		return can;
	}

	/// <summary>
	/// Gets the cast time.
	/// </summary>
	/// <returns>The cast time.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="skillIdx">Skill index.</param>
	public static float getCastTime(int actorIdx, int skillIdx)
	{
		float casttime = 0.0f;

		if(_getSkillCastTime != null) {
			casttime = _getSkillCastTime(actorIdx, skillIdx);
		}

		return casttime;
	}

	/// <summary>
	/// Gets the type of the skill.
	/// </summary>
	/// <returns>The skill type.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="skillIdx">Skill index.</param>
	public static int getSkillType(int actorIdx, int skillIdx)
	{
		int skilltype = 0;

		if(_getSkillType != null) {
			skilltype = _getSkillType(actorIdx, skillIdx);
		}

		return skilltype;
	}

	/// <summary>
	/// Gets the hp info.
	/// </summary>
	/// <returns><c>true</c>, if hp info was gotten, <c>false</c> otherwise.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="cur">Current.</param>
	/// <param name="max">Max.</param>
	public static bool getHpInfo(int actorIdx, out int cur, out int max)
	{
		bool retval = false;

		_hpInfo.cur = 0;
		_hpInfo.max = 0;

		if(_getHpInfo != null
		   && _getHpInfo(actorIdx)) {
			retval = true;
		}

		cur = _hpInfo.cur;
		max = _hpInfo.max;

		return retval;
	}

	/// <summary>
	/// Gets the targets in range.
	/// </summary>
	/// <returns>The targets in range.</returns>
	/// <param name="radius">Radius.</param>
	/// <param name="angle">Angle.</param>
	/// <param name="sameCamp">If set to <c>true</c> same camp.</param>
	public static List<BTEngine.BevActor> getTargetsInRange(int actorIdx, float radius, float angle, bool sameCamp)
	{
		List<BTEngine.BevActor> retval = null;

		if(_getTargetsInRange != null) {
			retval = _getTargetsInRange(actorIdx, radius, angle, sameCamp);
		}

		return retval;
	}

	/// <summary>
	/// Gets the actor position.
	/// </summary>
	/// <returns>The actor position.</returns>
	/// <param name="actorIdx">Actor index.</param>
	public static Vector3 getActorPosition(int actorIdx)
	{
		Vector3 retval = new Vector3();

		_aiPoint3.mX = 0.0f;
		_aiPoint3.mY = 0.0f;
		_aiPoint3.mZ = 0.0f;

		if(_getActorPosition != null
		   && _getActorPosition(actorIdx)) {

		}

		retval.x = _aiPoint3.mX;
		retval.y = _aiPoint3.mY;
		retval.z = _aiPoint3.mZ;

		return retval;
	}

	/// <summary>
	/// Gets the trace start point.
	/// </summary>
	/// <returns><c>true</c>, if trace start point was gotten, <c>false</c> otherwise.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="pos">Position.</param>
	public static bool getTraceStartPoint(int actorIdx, out Vector3 pos)
	{
		bool retval = false;

		_aiPoint3.mX = 0.0f;
		_aiPoint3.mY = 0.0f;
		_aiPoint3.mZ = 0.0f;
		
		if(_getTraceStartPoint != null
		   && _getTraceStartPoint(actorIdx)) {
			retval = true;
		}
		
		pos.x = _aiPoint3.mX;
		pos.y = _aiPoint3.mY;
		pos.z = _aiPoint3.mZ;

		return true;
	}

	/// <summary>
	/// Gets the next dungeon point.
	/// </summary>
	/// <returns><c>true</c>, if next dungeon point was gotten, <c>false</c> otherwise.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="pos">Position.</param>
	public static bool getNextDungeonPoint(int actorIdx, out Vector3 pos)
	{
		bool retval = false;
		
		_aiPoint3.mX = 0.0f;
		_aiPoint3.mY = 0.0f;
		_aiPoint3.mZ = 0.0f;
		
		if(_getNextDungeonPoint != null
		   && _getNextDungeonPoint(actorIdx)) {
			retval = true;
		}
		
		pos.x = _aiPoint3.mX;
		pos.y = _aiPoint3.mY;
		pos.z = _aiPoint3.mZ;
		
		return true;
	}

	/// <summary>
	/// Gets the other patrol point.
	/// </summary>
	/// <returns><c>true</c>, if other patrol point was gotten, <c>false</c> otherwise.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="pos">Position.</param>
	public static bool getOtherPatrolPoint(int actorIdx, out Vector3 pos)
	{
		bool retval = false;
		
		_aiPoint3.mX = 0.0f;
		_aiPoint3.mY = 0.0f;
		_aiPoint3.mZ = 0.0f;
		
		if(_getOtherPatrolPoint != null
		   && _getOtherPatrolPoint(actorIdx)) {
			retval = true;
		}
		
		pos.x = _aiPoint3.mX;
		pos.y = _aiPoint3.mY;
		pos.z = _aiPoint3.mZ;
		
		return true;
	}

	/// <summary>
	/// Gets the defense point.
	/// </summary>
	/// <returns><c>true</c>, if defense point was gotten, <c>false</c> otherwise.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="pos">Position.</param>
	public static bool getDefensePoint(int actorIdx, out Vector3 pos)
	{
		bool retval = false;
		
		_aiPoint3.mX = 0.0f;
		_aiPoint3.mY = 0.0f;
		_aiPoint3.mZ = 0.0f;
		
		if(_getDefensePoint != null
		   && _getDefensePoint(actorIdx)) {
			retval = true;
		}
		
		pos.x = _aiPoint3.mX;
		pos.y = _aiPoint3.mY;
		pos.z = _aiPoint3.mZ;
		
		return true;
	}

	/// <summary>
	/// Gets the birth point.
	/// </summary>
	/// <returns><c>true</c>, if birth point was gotten, <c>false</c> otherwise.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="pos">Position.</param>
	public static bool getBirthPoint(int actorIdx, out Vector3 pos)
	{
		bool retval = false;
		
		_aiPoint3.mX = 0.0f;
		_aiPoint3.mY = 0.0f;
		_aiPoint3.mZ = 0.0f;
		
		if(_getBirthPoint != null
		   && _getBirthPoint(actorIdx)) {
			retval = true;
		}
		
		pos.x = _aiPoint3.mX;
		pos.y = _aiPoint3.mY;
		pos.z = _aiPoint3.mZ;
		
		return true;
	}

	/// <summary>
	/// Gets the random move point.
	/// </summary>
	/// <returns><c>true</c>, if random move point was gotten, <c>false</c> otherwise.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="pos">Position.</param>
	public static bool getRandomMovePoint(int actorIdx, out Vector3 pos)
	{
		bool retval = false;
		
		_aiPoint3.mX = 0.0f;
		_aiPoint3.mY = 0.0f;
		_aiPoint3.mZ = 0.0f;
		
		if(_getRandomMovePoint != null
		   && _getRandomMovePoint(actorIdx)) {
			retval = true;
		}
		
		pos.x = _aiPoint3.mX;
		pos.y = _aiPoint3.mY;
		pos.z = _aiPoint3.mZ;
		
		return true;
	}
	
	private static BTEngine.AIWorld _aiWorld;

	public static BTEngine.AIWorld curAIWorld
	{
		set { _aiWorld = value; }
		get { return _aiWorld; }
	}

	/// <summary>
	/// Creates the AI world.
	/// </summary>
	/// <returns>The AI world.</returns>
	/// <param name="pvp">If set to <c>true</c> pvp.</param>
	public static bool createAIWorld(bool pvp)
	{
		_aiWorld = new BTEngine.AIWorld(pvp);
		return true;
	}

	/// <summary>
	/// Destroies the AI world.
	/// </summary>
	/// <returns><c>true</c>, if AI world was destroyed, <c>false</c> otherwise.</returns>
	public static bool destroyAIWorld()
	{
		if(_aiWorld != null) {
			_aiWorld = null;
			return true;
		}

		return false;
	}

	/// <summary>
	/// Starts AI
	/// </summary>
	/// <returns><c>true</c>, if A was started, <c>false</c> otherwise.</returns>
	public static bool startAI()
	{
		bool retval = false;

		if(_aiWorld != null) {
			_aiWorld.startAI();
			retval = true;
		}

		return retval;
	}

	/// <summary>
	/// Stops AI
	/// </summary>
	/// <returns><c>true</c>, if A was stoped, <c>false</c> otherwise.</returns>
	public static bool stopAI()
	{
		bool retval = false;
		
		if(_aiWorld != null) {
			_aiWorld.stopAI();
			retval = true;
		}
		
		return retval;
	}

	/// <summary>
	/// Adds the AI object to AIWorld
	/// </summary>
	/// <returns>The AI object.</returns>
	/// <param name="brainId">Brain identifier.</param>
	/// <param name="bevData">Bev data.</param>
	/// <param name="follower">Follower.</param>
	public static BTEngine.BevActor addAIObj(int brainId, BTEngine.BevActorData bevData, BTEngine.BevActor follower)
	{
		BTEngine.BevActor actor = null;
		if(_aiWorld != null) {
			actor = _aiWorld.addBevActor(brainId, bevData, follower);
		}

		return actor;
	}

	/// <summary>
	/// Ons the actor do attack.
	/// </summary>
	/// <param name="attacker">Attacker.</param>
	/// <param name="target">Target.</param>
	/// <param name="damage">Damage.</param>
	public static void onActorDoAttack(BTEngine.BevActor attacker, BTEngine.BevActor target, int damage)
	{
		if(_aiWorld != null) {
			_aiWorld.onBevActorAttack(attacker, target, damage);
		}
	}

	/// <summary>
	/// Ons the cast skill end.
	/// </summary>
	/// <param name="caster">Caster.</param>
	/// <param name="skillIndex">Skill index.</param>
	/// <param name="isBroken">If set to <c>true</c> is broken.</param>
	public static void onCastSkillEnd(BTEngine.BevActor caster, int skillIndex, bool isBroken)
	{
		if(_aiWorld != null) {
			_aiWorld.onCastEnd(caster, skillIndex, isBroken);
		}
	}

	/// <summary>
	/// Ons the actor die.
	/// </summary>
	/// <param name="attacker">Attacker.</param>
	/// <param name="target">Target.</param>
	public static void onActorDie(BTEngine.BevActor attacker, BTEngine.BevActor diedActor)
	{
		if(_aiWorld != null) {
			_aiWorld.onBevActorDie(attacker, diedActor);
		}
	}

	/// <summary>
	/// Ises the pvp.
	/// </summary>
	/// <returns><c>true</c>, if pvp was ised, <c>false</c> otherwise.</returns>
	public static bool isPvp()
	{
		if(_aiWorld != null) {
			return _aiWorld.isPvp;
		}

		return false;
	}

	/// <summary>
	/// Gets the bev actor.
	/// </summary>
	/// <returns>The bev actor.</returns>
	/// <param name="actorIdx">Actor index.</param>
	/// <param name="includeDiedActor">If set to <c>true</c> include died actor.</param>
	public static BTEngine.BevActor getBevActor(int actorIdx, bool includeDiedActor)
	{
		if(_aiWorld != null) {
			return _aiWorld.getBevActor(actorIdx, includeDiedActor);
		}

		return null;
	}

	/// <summary>
	/// Ons the update AI
	/// </summary>
	private static void onUpdateAI()
	{
		if(_aiWorld != null) {
			_aiWorld.onUpdatePerFrame();
		}
	}
}
