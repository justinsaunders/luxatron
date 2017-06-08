/*
 *  lig_node.h
 *  convert_dae
 *
 *  Created by Justin on 8/07/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_LIG_NODE_H__
#define __LXT_LIG_NODE_H__

#include "math/mat4.h"

namespace Lxt 
{
	struct LigNode;
	struct LigMesh;
	struct LigNode
	{
		std::string				m_name;
		Mat4					m_transform;
		std::vector<LigMesh*>	m_meshes;
		std::vector<LigNode*>	m_children;
	};
}

#endif // __LXT_LIG_NODE_H__
