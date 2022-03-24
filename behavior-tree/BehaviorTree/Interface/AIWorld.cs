using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class AIWorld
	{
		// current ai world is pvp battle or pve
		private bool _isPvp;

		public bool isPvp
		{
			set { _isPvp = value; }
			get { return _isPvp; }
		}

		// ai is runnning or not
		private bool _isRunningAi;

		public bool isRunningAi
		{
			set { _isRunningAi = value; }
			get { return _isRunningAi; }
		}

		private Dictionary<int, BevActor> _dictBevActors;
		private Dictionary<int, BevActor> _dictDiedActors;

		private List<BevActor> _actorsInCampA;
		private List<BevActor> _actorsInCampB;

		public AIWorld(bool pvp)
		{
			isRunningAi = false;
			isPvp = pvp;

			_dictBevActors = new Dictionary<int, BevActor>();
			_actorsInCampA = new List<BevActor>();
			_actorsInCampB = new List<BevActor>();
		}

		/// <summary>
		/// Start AI
		/// </summary>
		public void startAI()
		{
			isRunningAi = true;
		}

		/// <summary>
		/// Stop AI
		/// </summary>
		public void stopAI()
		{
			isRunningAi = false;
		}

		/// <summary>
		/// Adds the bev actor.
		/// </summary>
		/// <returns>The bev actor.</returns>
		/// <param name="brainId">Brain identifier.</param>
		/// <param name="data">Data.</param>
		/// <param name="follower">Follower.</param>
		public BevActor addBevActor(int brainId, BevActorData data, BevActor follower)
		{
			BevActor bevactor = null;

			if(!_dictBevActors.ContainsKey(data.mActorIdx)) {
				bevactor = new BevActor();

				if(bevactor.initalize(brainId, data)) {
					if(data.mActorCampID == CampID.Camp_ID_A) {
						_actorsInCampA.Add(bevactor);
						addHateEnemy(_actorsInCampB, bevactor);
					}
					else if(data.mActorCampID == CampID.Camp_ID_B) {
						_actorsInCampB.Add(bevactor);
						addHateEnemy(_actorsInCampA, bevactor);
					}

					bevactor.setFollower(follower);
					_dictBevActors[data.mActorIdx] = bevactor;
				} else {
					bevactor = null; // release
				}
			}

			return bevactor;
		}

		/// <summary>
		/// Gets the bev actor.
		/// </summary>
		/// <returns>The bev actor.</returns>
		/// <param name="actorIdx">Actor index.</param>
		/// <param name="inculeSearchDiedActors">If set to <c>true</c> incule search died actors.</param>
		public BevActor getBevActor(int actorIdx, bool inculeSearchDiedActors)
		{
			if(_dictBevActors.ContainsKey(actorIdx))
				return _dictBevActors[actorIdx];

			if(inculeSearchDiedActors && _dictDiedActors.ContainsKey(actorIdx))
				return _dictDiedActors[actorIdx];

			return null;
		}

		/// <summary>
		/// Adds the hate enemy.
		/// </summary>
		/// <param name="actors">Actors.</param>
		/// <param name="enemy">Enemy.</param>
		private void addHateEnemy(List<BevActor> actors, BevActor enemy)
		{
			foreach(BevActor actor in actors)
			{
				actor.addHateEnemy(enemy);
				enemy.addHateEnemy(actor);
			}
		}

		/// <summary>
		/// Ons the bev actor die.
		/// </summary>
		/// <param name="attacker">Attacker.</param>
		/// <param name="diedActor">Died actor.</param>
		public void onBevActorDie(BevActor attacker, BevActor diedActor)
		{
			if(_dictBevActors.ContainsKey(diedActor.bevActorData.mActorIdx)) {
				List<BevActor> infos = null;
				if(diedActor.bevActorData.mActorCampID == CampID.Camp_ID_A)
					infos = _actorsInCampB;
				else if(diedActor.bevActorData.mActorCampID == CampID.Camp_ID_B)
					infos = _actorsInCampA;

				if(infos != null) {
					foreach(BevActor actor in infos)
					{
						if(!actor.isDied()) {
							actor.removeHateEnemy(diedActor);
						}
					}

					attacker.reset(diedActor);

					_dictBevActors.Remove(diedActor.bevActorData.mActorIdx);
					_dictDiedActors[diedActor.bevActorData.mActorIdx] = diedActor;
				}
			}
		}

		/// <summary>
		/// Ons the bev actor attack.
		/// </summary>
		/// <param name="attacker">Attacker.</param>
		/// <param name="target">Target.</param>
		/// <param name="damage">Damage.</param>
		public void onBevActorAttack(BevActor attacker, BevActor target, int damage)
		{
			target.onAttacked(attacker, damage);
		}

		/// <summary>
		/// Ons the cast end.
		/// </summary>
		/// <param name="actor">Actor.</param>
		/// <param name="skillIndex">Skill index.</param>
		/// <param name="isBroken">If set to <c>true</c> is broken.</param>
		public void onCastEnd(BevActor actor, int skillIndex, bool isBroken)
		{
			actor.onCastEnd(skillIndex, isBroken);
		}

		/// <summary>
		/// Ons the updated.
		/// every frame need call it
		/// </summary>
		/// <param name="actor">Actor.</param>
		public void onUpdated(BevActor actor)
		{
			if(isRunningAi) {
				actor.update();
			}
		}

		/// <summary>
		/// Ons the update per frame.
		/// </summary>
		public void onUpdatePerFrame()
		{
			if(isRunningAi) {
				foreach(KeyValuePair<int, BevActor> kp in _dictBevActors)
				{
					kp.Value.update();
				}
			}
		}
	}
}
