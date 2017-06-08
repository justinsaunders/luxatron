/*
 *  dae_geometry_to_lig.cpp
 *  convert_dae
 *
 *  Created by Justin on 18/08/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "dae_geometry_to_lig.h"

#include <dae.h>
#include <dom/domCOLLADA.h>

#include "gfx/gfx.h"
#include "lig_mesh.h"

using namespace Lxt;

// Get source element given a URI
domSource* GetSourceFromURI( const domURIFragmentType&	a_sourceURI )
{	
	domSource*	source = NULL;
	source = (domSource*) (domElement*) a_sourceURI.getElement();
	LXT_ASSERT( source );
	
	return source;
}


// Copy the domSource array and stride to a LIG stream structure
bool ConvertDomSourceToLigVertexStream(	const domSource*	a_inputSource, 
									   LigVertexStream&		a_stream )
{
	// Copy array of elements
	const domFloat_arrayRef	array = a_inputSource->getFloat_array();
	
	a_stream.m_elements.reserve( array->getCount() );
	for ( size_t i = 0; i < array->getCount(); i++ )
		a_stream.m_elements.push_back( array->getValue()[i] );
	
	// Copy stride
	const domSource::domTechnique_commonRef	tech = 
										a_inputSource->getTechnique_common();
	const domAccessorRef	accessor= tech->getAccessor();
	
	a_stream.m_stride = accessor->getStride();
	
	return true;
}

// Copy the indicies for a given stream out to a flat array
bool ConvertPIndicesToIndexStream( const domListOfUInts&	a_PIndices, 
								  const size_t				a_stride,
								  const size_t				a_offset, 
								  LigIndexStream&			a_stream )
{
	LXT_ASSERT( a_stream.m_elements.empty() );
	
	if ( a_offset >= a_stride )
	{
		LXT_LOG_ERROR("Offset %lu is >= than stride %lu\n", a_offset, 
					  a_stride );
		return false;
	}
	
	// reserve space in array
	const size_t p = a_PIndices.getCount();
	a_stream.m_elements.reserve( p / a_stride );
	
	// fill array
	for ( size_t i = 0; i < p; i+= a_stride )
	{
		size_t index = a_PIndices[a_offset + i];
		a_stream.m_elements.push_back( index );
	}
	
	return true;
}
	
bool Lxt::ConvertDGMToLigMeshes( const DomGeometryMesh&	a_dgm, 
								 const LigMaterialMap&	a_materials,
								 std::vector<LigMesh*>&	a_outMeshes	)
{
	// Create Vertex Streams
	std::vector< LigVertexStream > vstreams;
	
	for ( size_t i = 0; i < a_dgm.m_sources.size(); i++ )
	{
		LigVertexStream vs;
		
		if ( !ConvertDomSourceToLigVertexStream( a_dgm.m_sources[i], vs ) )
		{
			LXT_LOG_ERROR("Invalid vertex stream %lu found in geometry.", i );
			return false;
		}
		
		vstreams.push_back( vs );
	}
	
	// Create Index Streams
	std::vector< LigIndexStream >	istreams;
	const size_t					numTriangles = a_dgm.m_triangles.size();
	
	// FIXME - this is a bit silly building up another vector of istreams,
	// look at combining with loop below
	for ( size_t t = 0; t < numTriangles; t++ )
	{
		const DomTriangles& dt = a_dgm.m_triangles[t];
		const size_t numInputs = dt.m_inputs.size();
		
		for ( size_t i = 0; i < numInputs; i++ )
		{
			istreams.push_back( dt.m_inputs[i].m_indices );
		}
	}
	
	// Create Batches
	std::vector< LigBatch > batches;
	size_t currentInput = 0;
	
	for ( size_t t = 0; t < numTriangles; t++ )
	{ 
		// for each triangle
		const DomTriangles& dt = a_dgm.m_triangles[t];
		const size_t numInputs = dt.m_inputs.size();
	
		// create batch
		LigBatch b;
		b.m_materialName = dt.m_triangles->getMaterial();
	
		for ( size_t i = 0; i < numInputs; i++ )
		{
			LigSemantic semantic = { dt.m_inputs[i].m_semantic, -1, -1 };
			
			semantic.m_istreamOffset = currentInput++;
			
			// find referenced vertex stream
			for ( size_t v = 0; v < a_dgm.m_sources.size(); v++ )
			{
				if ( a_dgm.m_sources[v] == dt.m_inputs[i].m_source )
				{
					semantic.m_vstreamOffset = v;
					break;
				}
			}
			
			b.m_semantics.push_back( semantic );
		}
		
		batches.push_back( b );
	}
	LXT_ASSERT( currentInput == istreams.size() );
	
	// Return
	
	// Create Mesh
	LigMesh* mesh = new LigMesh;
	
	// copy values
	mesh->m_primitiveType	= LXT_PT_TRIANGLE;
	mesh->m_vertexStreams	= vstreams;
	mesh->m_indexStreams	= istreams;
	mesh->m_batches			= batches;
	
	// return mesh
	a_outMeshes.push_back( mesh );
	
	return true;
}

bool Lxt::ConvertDomGeometryToDGM( domGeometry*		a_geometry,
								   DomGeometryMesh&	a_dgm )
{
	if ( !a_geometry )
	{
		LXT_LOG_ERROR("Invalid geometry node.");
		return false;
	}
	
	// Get the mesh for this geometry
	domMesh* meshElement = a_geometry->getMesh();
	
	if ( !meshElement )
	{
		LXT_LOG_ERROR("No mesh element specified in geometry node.");
		return false;
	}
	
	// Do sources first, as they are referenced later
	const size_t numSources		= meshElement->getSource_array().getCount();
	
	for ( size_t i = 0; i < numSources; i++ )
	{
		domSource*	source = meshElement->getSource_array()[i];
		a_dgm.m_sources.push_back( source );
	}
	
	// Now do triangles
	const size_t numTriangles	= meshElement->getTriangles_array().getCount();
	domVertices* vertices		= meshElement->getVertices();
	
	for ( size_t t = 0; t < numTriangles; t++ )
	{
		// create triangle
		DomTriangles	dt;
		dt.m_triangles = meshElement->getTriangles_array()[t];
		
		// for each triangle element, get its inputs (including the default
		// vertices inputs)
		const size_t numInputs	= dt.m_triangles->getInput_array().getCount();
		const domListOfUInts& p = dt.m_triangles->getP()->getValue();
		
		for ( size_t i = 0; i < numInputs; i++ )
		{
			domInputLocalOffset* localInput = dt.m_triangles->getInput_array()[i];
			
			if ( strcmp( localInput->getSemantic(), "VERTEX" ) == 0 )
			{
				// insert inputs from vertices inputs
				const size_t numVertexInputs = vertices->getInput_array().getCount();
				
				for ( size_t vi = 0; vi < numVertexInputs; vi++ )
				{
					domInputLocal* vertexInput = vertices->getInput_array()[vi];
					
					// create vertex input
					DomInput di;
					di.m_inputOffset= localInput->getOffset();
					di.m_semantic	= vertexInput->getSemantic();
					di.m_source		= GetSourceFromURI( vertexInput->getSource() );
						
					if ( !ConvertPIndicesToIndexStream( p, numInputs, 
									di.m_inputOffset, di.m_indices ) )
					{
						LXT_LOG_ERROR("Bad PIndices for input %lu.", vi );
						return false;
					}
							
					// add input to triangle
					dt.m_inputs.push_back( di );
				}
			}
			else
			{
				// insert inputs from local inputs
				DomInput di;
				
				di.m_inputOffset	= localInput->getOffset();
				di.m_semantic		= localInput->getSemantic();
				di.m_source			= GetSourceFromURI( localInput->getSource() );
				
				if ( !ConvertPIndicesToIndexStream( p, numInputs, 
											di.m_inputOffset, di.m_indices ) )
				{
					return false;
				}
				
				// add input to triangle
				dt.m_inputs.push_back( di );
			}
		} // for each input
 		
		// add triangle to group
		a_dgm.m_triangles.push_back( dt );
	}

	return true;
}

// Convert a COLLADA geometry to lig meshes
bool Lxt::ConvertDomGeometryToLigMeshes(domGeometry*			a_geometry, 
										const LigMaterialMap&	a_materials,
										std::vector<LigMesh*>&	a_outMeshes)
{
	DomGeometryMesh dgm;
	
	if ( ConvertDomGeometryToDGM( a_geometry, dgm ) )
	{
		return ConvertDGMToLigMeshes( dgm, a_materials, a_outMeshes );
	}
	
	return false;
}

