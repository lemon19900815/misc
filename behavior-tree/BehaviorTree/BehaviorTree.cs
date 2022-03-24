using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;

namespace BTEngine
{
	public class BehaviorTree
	{
		private static BehaviorTree _inst;
		public static BehaviorTree Instance
		{
			get
			{
				if(_inst == null) {
					_inst = new BehaviorTree();
				}
				
				return _inst;
			}
		}

		private Dictionary<int, BTNManager.BTNode> _dictBTrees = new Dictionary<int, BTNManager.BTNode>();
		private XmlDocument _xmlDoc = new XmlDocument();

		byte[] bomBuffer = new byte[] { 0xef, 0xbb, 0xbf };

		private string _clearBOM(byte[] buffer)
		{
			int offset = 0;
			if (buffer[0] == bomBuffer[0]
			    && buffer[1] == bomBuffer[1]
			    && buffer[2] == bomBuffer[2])
			{
				offset = 3;
			}

			return new System.Text.UTF8Encoding(false).GetString(buffer, offset, buffer.Length - offset);
		}

		public BTNManager.BTNode getBTree(int brainId)
		{
			BTNManager.BTNode retval = null;

			if(_dictBTrees.ContainsKey(brainId)) {
				retval = _dictBTrees[brainId];
			}

			return retval;
		}

		public BTNManager.BTNode buildBTree(int brainId, string data)
		{
			byte[] _data = System.Text.Encoding.UTF8.GetBytes(data);
			return buildBTree(brainId, _data);
		}

		public BTNManager.BTNode buildBTree(int brainId, byte[] data)
		{
			try
			{
				string content = _clearBOM(data);
				_xmlDoc.LoadXml(content);

				BTNManager.BTNode root = createBTNode(_xmlDoc.DocumentElement, null);
				if(root != null) {
					if(!createBTree(_xmlDoc.DocumentElement, root)) {
						root = null;
					} else {
						_dictBTrees[brainId] = root;
					}
				}

				return root;
			}
			catch(XmlException e)
			{
				Debug.LogError("load xml docment failed with exception: " + e.Message);
				return null;
			}
		}

		BTNManager.BTNode createBTNode(XmlNode xn, BTNManager.BTNode _parent)
		{
			string msg = xn.Attributes["Info"].Value;
			string type = xn.Attributes["Type"].Value;
			int id = int.Parse(xn.Attributes["Id"].Value);
			string extra = xn.Attributes["Extra"].Value;

			BTNManager.BTNode node = BTNManager.Instance.createNode(_parent, type, msg);
			if(node != null && id != 0) {
				BTNManager.Precondition _Precondition = ExtraInfo.Instance.getPrecondition(id);
				if(_Precondition != null) { // 绑定外部条件
					node.bind(_Precondition, extra);
				} else {
					Debug.LogError("not register extra id: " + id);
					return null;
				}
			}

			return node;
		}

		bool createBTree(XmlNode _xn, BTNManager.BTNode _parent)
		{
			if(_xn.HasChildNodes) {
				foreach(XmlNode xn in _xn.ChildNodes)
				{
					BTNManager.BTNode node = createBTNode(xn, _parent);
					if(node != null) {
						if(!createBTree(xn, node)) {
							return false;
						}

						_parent.append(node);
					} else {
						Debug.LogError("create bt node error");
						return false;
					}
				}
			}

			return true;
		}
	}
}
