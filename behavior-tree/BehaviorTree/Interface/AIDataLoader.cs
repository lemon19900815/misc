using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public delegate void POnAIDataLoadFinsihed(WWW www, string name);

public struct AIItem
{
	public string name;
	public string path;
}

public class AIDataLoader
{
	private WWW _www;
	private List<AIItem> _lstItems = new List<AIItem>();
	private string _curDownloadFileName = string.Empty;

	public POnAIDataLoadFinsihed _onLoadFinished;

	public void addDataItemToWaitLoad(string name, string path)
	{
		AIItem item = new AIItem();
		item.name = name;
		item.path = path;

		_lstItems.Add(item);
	}

	string getUrlPath(string path)
	{
		//#if UNITY_EDITOR_WIN
		#if UNITY_STANDALONE_WIN && !UNITY_WEBPLAYER
		return "file:///" + Application.streamingAssetsPath + path;
		#else
		return Application.streamingAssetsPath + path;
		#endif
	}

	void startLoad()
	{
		_www = null;

		if(_lstItems.Count > 0)
		{
			AIItem aiItem = _lstItems[0];
			_lstItems.RemoveAt(0);

			_curDownloadFileName = aiItem.name;
			_www = new WWW(getUrlPath(aiItem.path));
		}
	}

	public bool isLoadFinished
	{
		get
		{
			if(_lstItems.Count > 0)
				return false;

			if(_www == null)
				return true;

			return _www.isDone;
		}
	}

	public void Update()
	{
		if(_www == null) {
			startLoad();
			return;
		}

		if(_www.isDone) { // download finished
			if(_onLoadFinished != null) {
				_onLoadFinished(_www, _curDownloadFileName);
			}

			startLoad(); // download another one
		}
		else if(!string.IsNullOrEmpty(_www.error)) {
			Debug.LogError("load ai data failed: " + _www.error);
		}
	}
}
