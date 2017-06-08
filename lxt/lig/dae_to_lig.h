/*
 *  dae_to_lig.h
 *  convert_dae
 *
 *  Created by Justin on 8/07/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_DAE_TO_LIG_H__
#define __LXT_DAE_TO_LIG_H__

class daeElement;

namespace Lxt 
{
	struct LigScene;
	bool ConvertDAEtoLIG( daeElement* a_root, Lxt::LigScene& a_newScene );
}


#endif // __LXT_DAE_TO_LIG_H__
