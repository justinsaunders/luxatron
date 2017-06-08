/*
 *  dae_material_to_lig.cpp
 *  convert_dae
 *
 *  Created by Justin on 24/03/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "dae_material_to_lig.h"

#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domProfile_COMMON.h>

#include "math/vec4.h"
#include "lig_material.h"

using namespace Lxt;

bool GetColorFromDom( const domCommon_color_or_texture_typeRef& a_domColor,
					  Vec4& a_outColor)
{
	bool status = false; 
	
	if ( a_domColor.cast() )
	{
		const domCommon_color_or_texture_type_complexType::domColorRef colRef = 
														a_domColor->getColor();
		if ( colRef )
		{
			domFx_color_common& col = colRef->getValue();
			a_outColor = Vec4( col.get(0), col.get(1), col.get(2), col.get(3) );
			status = true;
		}
	}
	
	return status;
}

bool GetFloatFromDom( const domCommon_float_or_param_typeRef& a_domFloat,
					 float& a_outFloat )
{
	bool status = false;	
	
	if ( a_domFloat.cast() )
	{
		const domCommon_float_or_param_type_complexType::domFloatRef floatRef =
														a_domFloat->getFloat();
		if ( floatRef )
		{
			a_outFloat = floatRef->getValue();
			status = true;
		}
	}
	
	return status;
}

bool GetSamplerFromDom( const domCommon_color_or_texture_typeRef& a_domTexture,
					   std::string & a_outName )
{
	bool status = false;
	
	if ( a_domTexture.cast() )
	{
		const domCommon_color_or_texture_type_complexType::domTextureRef texRef= 
													a_domTexture->getTexture();
		
		if ( texRef )
		{
			a_outName = texRef->getTexture(); // ignore texture coord for now
			status = true;
		}
	}
	
	return status;
}

domCommon_newparam_typeRef GetNewParamBySID(domProfile_COMMONRef	a_profile,
											const std::string&		a_sid )
{
	daeSIDResolver resolver( a_profile, a_sid.c_str() );
	
	return daeSafeCast<domCommon_newparam_type>( resolver.getElement() );
}
	
Path GetFilePathForSampler(	domProfile_COMMONRef	a_profile,
							const std::string&		a_samplerName )
{
	Path path;
	
	// Get the newparam holding the sampler
	domCommon_newparam_typeRef newParamRef 
								= GetNewParamBySID( a_profile, a_samplerName );
	
	if ( newParamRef )
	{
		// Get the sampler
		domFx_sampler2D_commonRef samplerRef = newParamRef->getSampler2D();	
		
		if ( samplerRef )
		{
			// Now get newparam holding surface
			newParamRef = GetNewParamBySID( a_profile, samplerRef->getSource()->getValue() );
			
			if ( newParamRef )
			{
				//  Now get the surface
				domFx_surface_commonRef surfaceRef = newParamRef->getSurface();
				
				if ( surfaceRef )
				{
					// Now get the init_common
					domFx_surface_init_commonRef init_common = 
										surfaceRef->getFx_surface_init_common();
					
					// Now get the init_from array
					domFx_surface_init_from_common_Array init_from = 
												init_common->getInit_from_array();
					
					// Now get the reference of the first item in array
					domFx_surface_init_from_commonRef  ir = init_from.get(0);
					daeIDRef id = ir->getValue();
					
					// Now get the image referenced
					domImage* image = daeSafeCast<domImage>( id.getElement() );					
					LXT_ASSERT( image );
					
					// Now get the path of the URI of the Image.
					path = image->getInit_from()->getValue().str();
				}
			}
		}
	}
	
	return path;
}

void CreateLigMaterialFromDomEffect(const domEffectRef&	a_effect,
									LigMaterial&		a_outMaterial )
{
	// Enumerate profiles
	domFx_profile_abstract_Array&	profiles= a_effect->getFx_profile_abstract_array();
	domProfile_COMMON*				common  = NULL;
	
	// find common profile
	for ( size_t p = 0; p < profiles.getCount(); p++ )
	{
		domFx_profile_abstract* a = profiles[p].cast();
		
		if ( strcmp( a->getTypeName(), "profile_COMMON" ) == 0 )
		{
			common = daeSafeCast<domProfile_COMMON>( a );
		}
	}

	if ( !common ) 
	{
		LXT_LOG_SPAM("No common profile found for %s.", a_effect->getID() );
		return;
	}
	
	// Get name and blend type
	a_outMaterial.m_name		= a_effect->getID();
	a_outMaterial.m_blendType	= MBT_OPAQUE;		// TODO unhardcode
	
	// get technique
	domProfile_COMMON::domTechniqueRef technique = common->getTechnique();
	
	// get parameters - pretty darn ugly, but the lighting elements aren't
	// related so "polymorphism" won't work.
	domCommon_color_or_texture_typeRef	diff;
	domCommon_color_or_texture_typeRef	spec;
	domCommon_float_or_param_typeRef	shin;
	domCommon_float_or_param_typeRef	tran;
	
	domProfile_COMMON::domTechnique::domPhongRef	phong =		technique->getPhong();
	domProfile_COMMON::domTechnique::domBlinnRef	blinn =		technique->getBlinn();
	domProfile_COMMON::domTechnique::domLambertRef	lambert =	technique->getLambert();
	domProfile_COMMON::domTechnique::domConstantRef constant =	technique->getConstant();
	
	bool lit = true;
	
	if ( phong )
	{
		diff = phong->getDiffuse();
		spec = phong->getSpecular();
		shin = phong->getShininess();
		tran = phong->getTransparency();
	}
	else if ( blinn )
	{
		diff = blinn->getDiffuse();
		spec = blinn->getSpecular();
		shin = blinn->getShininess();
		tran = blinn->getTransparency();
	}
	else if ( lambert )
	{
		diff = lambert->getDiffuse();
		tran = lambert->getTransparency();
	}
	else if ( common )
	{
		lit	= false;
		
		diff = constant->getEmission();
		tran = constant->getTransparency();
	}
	
	// set values in material.
	std::string samplerName;
	
	if ( GetColorFromDom( diff, a_outMaterial.m_diffuseColour ) )
	{
		const Vec4& c = a_outMaterial.m_diffuseColour;
		LXT_LOG_SPAM("Got Diffuse = %f %f %f %f\n", 
					  c.m_v[0], c.m_v[1], c.m_v[2], c.m_v[3] );
	}
	else if ( GetSamplerFromDom( diff, samplerName ) )
	{
		a_outMaterial.m_texturePath = GetFilePathForSampler( common, samplerName );  
		
		LXT_LOG_SPAM("Got Diffuse Texture = %s\n", 
					 a_outMaterial.m_texturePath.c_str() );
	}
			 
	if ( GetColorFromDom( spec, a_outMaterial.m_specularColour ) )
	{
		const Vec4& c = a_outMaterial.m_specularColour;
		LXT_LOG_SPAM("Got Specular = %f %f %f %f\n", 
					 c.m_v[0], c.m_v[1], c.m_v[2], c.m_v[3] );
	}
	
	if ( GetFloatFromDom( shin, a_outMaterial.m_shininess ) )
	{
		// A quick hack to handle blinn materials - change exponent from 0-1 to
		// 0-128. This is completely wrong because the exponents are non-linear
		// and not proportional.
		if ( a_outMaterial.m_shininess > 0.0f && a_outMaterial.m_shininess < 1.0f )
		{
			a_outMaterial.m_shininess *= 128.0f;
		}
		
		LXT_LOG_SPAM("Got Shininess = %f\n", a_outMaterial.m_shininess );
	}
	
	a_outMaterial.m_lit = lit;
}

namespace Lxt 
{

bool ConvertDomMaterialToLigMaterial(domMaterial*	a_domMaterial,
									 LigMaterial&	a_ligMaterial )
{
	domInstance_effectRef	instance= a_domMaterial->getInstance_effect();
	
	// Look for effect referenced by material
	domEffect* effect = 
					daeSafeCast<domEffect>( instance->getUrl().getElement() );
	
	if ( effect )
	{
		LXT_LOG_SPAM("ConvertDomMaterialToLigMaterial(): found effect %s\n", 
					 effect->getId() );
		
		CreateLigMaterialFromDomEffect( domEffectRef(effect), 
										a_ligMaterial );
	}
	
	return true;
}
	
}