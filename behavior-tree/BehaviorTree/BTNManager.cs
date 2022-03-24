using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BTEngine
{
	public class InputParameter
	{
		public AIActor mSelf;			// 自己
		//public List<AIActor> mTargets;	// 攻击、移动的目标列表

		//public Vector3 mTargetPt;		// 移动的目标点

		// 行为树节点执行需要填充的参数
		//public bool mIsRunAi;
		public string mExtraParamer;	// 节点附带参数--用于行为树执行条件检测时使用
	}

	public class OutputParameter
	{
		public BTNManager.BTNode _curNode;	// 当前正在执行的节点
		//public List<AIActor> mTargets;		// 选择列表--执行某些条件后返回的对象列表--如果列表为空，则使用下面的坐标位置

		//public Vector3 mTargetPos;			// 移动的目标点
	}

	// 行为树节点管理器
	public class BTNManager
	{
		private static BTNManager _inst;
		public static BTNManager Instance
		{
			get
			{
				if (_inst == null) {
					_inst = new BTNManager();
				}

				return _inst;
			}
		}

		public enum eRunState
		{
			State_Unknow,		// 未知状态
			State_Executing,	// 正在执行中
			State_Complete,		// 完成
		}

		// 前提条件
		public class Precondition
		{
			public virtual eRunState update(InputParameter _input, OutputParameter _output)
			{
				return eRunState.State_Unknow;
			}
			public virtual bool isTrue(InputParameter _input, OutputParameter _output)
			{
				return false;
			}
		}

		// 节点类型
		public enum eBTNodeType
		{
			BT_Node_Type_None,
			BT_Node_Type_Condition,
			BT_Node_Type_Action,
			BT_Node_Type_Sequence,
			BT_Node_Type_Selector,
		}

		public class BTNode
		{
			private string _name;					// 节点名
			private string _msg;					// 节点描述
			private int _type;						// 节点类型
			private BTNode _parent;					// 父节点
			protected List<BTNode> _children;		// 子节点
			protected Precondition _precondition;	// 前提条件
			protected string _parameter;			// 决定外部条件的参数

			public BTNode(BTNode nodeParent, int nodeType, string nodeName, string msgInfo)
			{
				_name = nodeName;
				_msg = msgInfo;
				_type = nodeType;
				_parent = nodeParent;

				_children = new List<BTNode>();
			}

			public string name
			{
				//set { _name = value; }
				get { return _name; }
			}

			public string msg
			{
				//set { _msg = value; }
				get { return _msg; }
			}

			public int type
			{
				//set { _type = value; }
				get { return _type; }
			}

			public BTNode parent
			{
				//set { _parent = value; }
				get { return _parent; }
			}

			// 跟节点
			public BTNode root()
			{
				if(parent == null)
					return this;

				return parent.root();
			}

			// 根节点下的第一个子节点
			// root
			//		-- subRoot
			//					--child
			public BTNode subRoot()
			{
				if(parent == null)
					return null;

				if(parent.parent == null)
					return this;

				return parent.subRoot();
			}

			// 添加子节点--单一节点不能添加子节点
			public BTNode append(BTNode _child)
			{
				if(type == (int)eBTNodeType.BT_Node_Type_Condition
				   || type == (int)eBTNodeType.BT_Node_Type_Action) {
					Debug.LogError("can't append child btnode to[" + ((eBTNodeType)type).ToString() + "]node");
					return this;
				}

				_children.Add(_child);

				return this;
			}

			// 绑定外部条件--复合节点不能绑定外部条件和行为
			public BTNode bind(Precondition _extra, string _param)
			{
				if(type == (int)eBTNodeType.BT_Node_Type_Selector
				   || type == (int)eBTNodeType.BT_Node_Type_Sequence) {
					Debug.LogError("can't append child btnode to[" + ((eBTNodeType)type).ToString() + "]node");
					return this;
				}

				_precondition = _extra;
				_parameter = _param;

				return this;
			}

			// 更新检测--当前的节点是否还是有效的-如果有效则直接执行，就不再对整棵行为树进行检测,提高AI执行效率
			public virtual eRunState upadte(InputParameter _input, OutputParameter _output)
			{
				return eRunState.State_Unknow;
			}

			// 执行行为节点
			public virtual bool run(InputParameter _input, OutputParameter _output)
			{
				return false;
			}
		}

		// 条件节点--单一节点
		public class CondBTNode : BTNode
		{
			public CondBTNode(BTNode nodeParent, int nodeType, string nodeName, string msgInfo)
				: base(nodeParent, nodeType, nodeName, msgInfo)
			{
			}

			// 更新检测--当前的节点是否还是有效的-如果有效则直接执行，就不再对整棵行为树进行检测,提高AI执行效率
			public override eRunState upadte(InputParameter _input, OutputParameter _output)
			{
				return eRunState.State_Unknow;
			}
			
			// 执行行为节点
			public override bool run(InputParameter _input, OutputParameter _output)
			{
				bool retval = false;

				_input.mExtraParamer = _parameter;
				_output._curNode = this;

				if(_precondition != null) {
					retval = _precondition.isTrue(_input, _output);
				}

				return retval;
			}
		}

		// 行为节点--单一节点
		public class ActionBTNode : BTNode
		{
			public ActionBTNode(BTNode nodeParent, int nodeType, string nodeName, string msgInfo)
				: base(nodeParent, nodeType, nodeName, msgInfo)
			{
			}

			// 更新检测--当前的节点是否还是有效的-如果有效则直接执行，就不再对整棵行为树进行检测,提高AI执行效率
			public override eRunState upadte(InputParameter _input, OutputParameter _output)
			{
				if(_precondition != null)
					return _precondition.update(_input, _output);

				return eRunState.State_Unknow;
			}
			
			// 执行行为节点
			public override bool run(InputParameter _input, OutputParameter _output)
			{
				bool retval = false;

				_input.mExtraParamer = _parameter;
				_output._curNode = this;

				if(_precondition != null) {
					retval = _precondition.isTrue(_input, _output);
				}
				
				return retval;
			}
		}

		// 顺序节点--全部满足
		public class SequenceBTNode : BTNode
		{
			public SequenceBTNode(BTNode nodeParent, int nodeType, string nodeName, string msgInfo)
				: base(nodeParent, nodeType, nodeName, msgInfo)
			{
			}

			// 更新检测--当前的节点是否还是有效的-如果有效则直接执行，就不再对整棵行为树进行检测,提高AI执行效率
			public override eRunState upadte(InputParameter _input, OutputParameter _output)
			{
				return eRunState.State_Unknow;
			}
			
			// 执行行为节点
			public override bool run(InputParameter _input, OutputParameter _output)
			{
				bool retval = false;

				_input.mExtraParamer = _parameter;
				_output._curNode = this;

				if(_children != null) {
					retval = true;

					int count = _children.Count;
					for(int i = 0; i < count; ++i)
					{
						BTNode _current = _children[i];
						if(!_current.run(_input, _output)) {
							retval = false;
							break;
						}
					}
				}
				
				return retval;
			}
		}

		// 选择节点--满足单一节点就可以
		public class SelectorBTNode : BTNode
		{
			public SelectorBTNode(BTNode nodeParent, int nodeType, string nodeName, string msgInfo)
				: base(nodeParent, nodeType, nodeName, msgInfo)
			{
			}

			// 更新检测--当前的节点是否还是有效的-如果有效则直接执行，就不再对整棵行为树进行检测,提高AI执行效率
			// 这里可能只对特定的条件进行检测
			public override eRunState upadte(InputParameter _input, OutputParameter _output)
			{
				return eRunState.State_Unknow;
			}
			
			// 执行行为节点
			public override bool run(InputParameter _input, OutputParameter _output)
			{
				bool retval = false;

				_input.mExtraParamer = _parameter;
				_output._curNode = this;

				if(_children != null) {
					int count = _children.Count;
					for(int i = 0; i < count; ++i)
					{
						BTNode _current = _children[i];
						if(_current.run(_input, _output)) {
							retval = true;
							break;
						}
					}
				}

				return retval;
			}
		}

		public BTNode createNode(BTNode parent, string name, string msg)
		{
			BTNode retval = null;

			if(name == "condition")
				retval = new CondBTNode(parent, (int)eBTNodeType.BT_Node_Type_Condition, name, msg);
			else if(name == "action")
				retval = new ActionBTNode(parent, (int)eBTNodeType.BT_Node_Type_Action, name, msg);
			else if(name == "selector")
				retval = new SelectorBTNode(parent, (int)eBTNodeType.BT_Node_Type_Selector, name, msg);
			else if(name == "sequence")
				retval = new SequenceBTNode(parent, (int)eBTNodeType.BT_Node_Type_Sequence, name, msg);
			else
				retval = new BTNode(parent, (int)eBTNodeType.BT_Node_Type_None, name, msg);

			return retval;
		}
	}
}
