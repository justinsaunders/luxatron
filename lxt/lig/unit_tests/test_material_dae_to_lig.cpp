/*
 *  test_material_dae_to_lig.cpp
 *  mac_test_runner
 *
 *  Created by Justin on 25/10/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */
#include <UnitTest++.h>
#include "dae_material_to_lig.h"

#include <dae.h>
#include <dom/domCOLLADA.h>
#include "../lig_material.h"

namespace Lxt 
{
	struct MaterialFixture
	{
		DAE				f_dae;
		domMaterial*	f_domMaterial;
		
		MaterialFixture( char* a_dae_path, char* a_material_id )
		{
			daeElement* root = f_dae.open( a_dae_path );
			LXT_ASSERT( root != NULL );
			
			// Get the first geometry node
			std::vector<daeElement*> elements= 
							f_dae.getDatabase()->idLookup( a_material_id );
			LXT_ASSERT( 1 == elements.size() );
			
			f_domMaterial =(domMaterial*) elements[0];
			LXT_ASSERT( NULL != f_domMaterial );
		}
	};

	struct DuckFixture : public MaterialFixture
	{
		DuckFixture() :
		MaterialFixture( "/Users/justin/Projects/msg/art/test/unit_tested/duck/duck.dae",
						"blinn3" )
		{
		}
	};
	
	TEST_FIXTURE( DuckFixture, DOM_textured_to_LIG )
	{
		LigMaterial ligMaterial;
		
		CHECK( ConvertDomMaterialToLigMaterial(f_domMaterial,ligMaterial ) );
		
		CHECK( ligMaterial.m_texturePath.find("duckCM.png") != std::string::npos );
		CHECK_EQUAL( 0.3f * 128.f, ligMaterial.m_shininess );
		CHECK( ligMaterial.m_diffuseColour.Equal( Vec4( 1.f, 1.f, 1.f, 1.f ) ) );
		CHECK( ligMaterial.m_specularColour.Equal( Vec4( 0.f, 0.f, 0.f, 1.f ) ) );
		CHECK_EQUAL( true, ligMaterial.m_lit );
		CHECK_EQUAL( MBT_OPAQUE, ligMaterial.m_blendType );
	}
}
