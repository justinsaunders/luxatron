/*
 *  dae_to_lig.cpp
 *  convert_dae
 *
 *  Created by Justin on 8/07/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "dae_to_lig.h"

#include <dae.h>
#include <dom/domCOLLADA.h>
#include "lig_node.h"
#include "lig_scene.h"
#include "lig_material.h"
#include "dae_material_to_lig.h"
#include "dae_geometry_to_lig.h"

using namespace Lxt;

// Convert an "instance_geometry" element into one or more meshes.
bool ConvertDAEInstanceGeometry(const domInstance_geometryRef	a_igNode,
								LigMaterialMap&					a_materials,
								std::vector<LigMesh*>&			a_outMeshes)
{
	// Create instance material map
	domBind_materialRef bindMaterial = a_igNode->getBind_material();
	
	if ( bindMaterial )
	{
		domBind_material::domTechnique_commonRef bindTech = 
											bindMaterial->getTechnique_common();
		LXT_ASSERT( bindTech );
		
		domInstance_material_Array& ima = 
										bindTech->getInstance_material_array();
		
		// generate an instance material map based on symbols in ima
		for ( size_t i = 0; i < ima.getCount(); i++ )
		{
			domInstance_materialRef imat = ima.get(i);
			LXT_LOG_SPAM( "Found instance material: %s -> %s\n", 
						 imat->getSymbol(), 
						 imat->getTarget().originalStr().c_str() );
		
			domMaterial* dmaterial = 
				daeSafeCast<domMaterial>(imat->getTarget().getElement() );
			
			if ( dmaterial )
			{
				Lxt::LigMaterial lmaterial;
				
				if(! Lxt::ConvertDomMaterialToLigMaterial( dmaterial, lmaterial ) )
					break;
				
				a_materials.insert( std::make_pair( imat->getSymbol(), lmaterial ) );
			}
		}
	}
	
	// Get geometry node
	daeURI			uri		= a_igNode->getUrl();
	domGeometry*	geometry= daeSafeCast<domGeometry>( uri.getElement() );
	
	LXT_LOG_SPAM("Converting Geometry Node: %s\n", 
				 uri.originalStr().c_str() );
	
	if ( geometry )
	{
		// A single geometry element may create multiple nodes which are 
		// siblings with the same transform
		return ConvertDomGeometryToLigMeshes( geometry, a_materials, 
												a_outMeshes );
	}
	else
	{
		LXT_LOG_SPAM("Warning, geometry node not found.\n");
	}
	
	return true;
}

// Generate a transform as specified by in a DAE node element, by
// post-multiplying in order specified in element.
void BuildTransformForNode( const domNodeRef	a_visualNode,
							Mat4&				a_outTransform )
{
	daeElementRefArray& contents= a_visualNode->getContents();
	Mat4				previous = Mat4::Identity;
	
	for ( size_t i = 0; i < contents.getCount(); i++ )
	{
		domTranslate*	translate	= daeSafeCast<domTranslate>( contents[i] );
		domRotate*		rotate		= daeSafeCast<domRotate>( contents[i] );
		Mat4			m			= Mat4::Identity;
		
		if ( translate )
		{
			domListOfFloats fv = translate->getValue();
			m.SetColumn( 3, Vec4( fv[0], fv[1], fv[2], 1.0f ) );
		}
		else if ( rotate )
		{
			domListOfFloats fv = rotate->getValue();
			MakeRotationMatrix( fv[3], fv[0], fv[1], fv[2], m );
		}
		
		Mat4 result;
		Mat4::Multiply( previous, m, result );
		previous = result;
	}
	
	// replace output
	a_outTransform = previous;
}

bool ConvertDAEVisualNode(	const domNodeRef		a_visualNode,
							LigMaterialMap&			a_materials,
							std::vector<LigNode*>&	a_newChildren )
{
	LXT_LOG_SPAM( "\nConvertDAEVisualNode(): processing node '%s'\n", 
				 a_visualNode->getId() );
	
	// Process any geometry instances in this visual node
	domInstance_geometry_Array& iga = a_visualNode->getInstance_geometry_array();
	
	if ( iga.getCount() > 0 )
	{
		// Create a  new node with a default transform
		LigNode* newNode = new LigNode;
		
		newNode->m_name		= a_visualNode->getName();
		newNode->m_transform= Mat4::Identity;
		
		BuildTransformForNode( a_visualNode, newNode->m_transform );
		
		for ( size_t i = 0; i < 4; i++ )
		{
			LXT_LOG_SPAM("  (%2.2f %2.2f %2.2f %2.2f)\n",
						 newNode->m_transform.m_column[i].m_x,
						 newNode->m_transform.m_column[i].m_y,
						 newNode->m_transform.m_column[i].m_z,
						 newNode->m_transform.m_column[i].m_w );
		}
		
		// Add
		a_newChildren.push_back( newNode );
		
		for ( size_t i = 0; i < iga.getCount(); i++ )
		{
			// Create children of this node
			if (!ConvertDAEInstanceGeometry( iga.get(i), a_materials,
												newNode->m_meshes ) )
				return false;
			
			LXT_LOG_SPAM("Converting Instance Geometry %s\n", 
						 iga.get(i)->getUrl().getID() );
		}
		
		// Now see if there are any child visual nodes to recurse into
		const domNode_Array& cvna = a_visualNode->getNode_array();
		
		for ( size_t c = 0; c < cvna.getCount(); c++ )
		{
			// Recurse
			if ( !ConvertDAEVisualNode( cvna.get(c), a_materials,
											newNode->m_children ) )
				return false;
		}
	}
	
	return true;
}

bool ConvertDAEInstanceVisualScene(daeElement*	a_root,
								   LigScene*	a_scene )
{
	// get scene
	domCOLLADA::domScene* scene = 
	daeSafeCast<domCOLLADA::domScene>( a_root->getDescendant("scene") );
	
	if ( !scene )
	{
		LXT_LOG_ERROR("Missing scene in document.");
		return false;
	}
	
	// get instance visual scene
	domInstanceWithExtraRef ivs = scene->getInstance_visual_scene();
	
	// get visual scene
	daeElement* vse = ivs->getUrl().getElement();
	domVisual_scene* vs = daeSafeCast<domVisual_scene>(vse);
	
	if ( !vs )
	{
		LXT_LOG_ERROR("Invalid visual scene in document.");
		return false;
	}
	
	// get nodes with geometry
	domNode_Array& nodes = vs->getNode_array();
	
	// Create root node
	a_scene->m_root = new LigNode;
	a_scene->m_root->m_name = "Root";
	a_scene->m_root->m_transform = Mat4::Identity;
	
	for ( size_t n = 0; n < nodes.getCount(); n++ )
	{
		// Recursively create Model::Nodes for any node with an 
		// instance_geometry section.
		
		// Recurse
		if ( !ConvertDAEVisualNode( nodes[n], a_scene->m_materials, 
										a_scene->m_root->m_children ) )
		{
			return false;
		}
	}
	
	return true;
}

bool Lxt::ConvertDAEtoLIG( daeElement* root, LigScene& a_newScene )
{
	return ConvertDAEInstanceVisualScene( root, &a_newScene );	
}
