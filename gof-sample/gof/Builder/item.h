#ifndef __Builder_Item_Inc_H__
#define __Builder_Item_Inc_H__

#include "../stdafx.h"

namespace Builder
{
	class Packing;

	class Item
	{
	public:
		virtual ~Item() {

		}

		virtual const std::string	name() = 0;

		virtual Packing*			packing() = 0;

		virtual float				price() = 0;
	};
}

#endif // __Builder_Item_Inc_H__
