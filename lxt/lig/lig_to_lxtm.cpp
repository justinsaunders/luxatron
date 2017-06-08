/*
 *  lig_to_lxtm.cpp
 *  convert_dae
 *
 *  Created by Justin on 8/07/09.
 *  Copyright 2009 Monkey Style Games. All rights reserved.
 *
 */

#include "lig_to_lxtm.h"

#include "lig_scene.h"
#include "lig_mesh.h"
#include "gfx/mesh.h"
#include "gfx/colour.h"

using namespace Lxt;

// Map each tuple with a unique id (eg index).
typedef std::vector<uint32_t>						Tuple;
typedef std::map< Tuple,IndexBuffer::IndexType >	TupleSet;
typedef std::map< IndexBuffer::IndexType,Tuple >	RTupleSet;

// Temporary structure used while building meshes
struct TempBatch
{
	Lxt::Batch				m_batch;
	std::vector< Tuple >	m_indexTuples;
};

// Convert a LIG Material to LXT Material, using given texture pool.
void ConvertLIGMaterialToLXTMaterial( const LigMaterial&	a_ligMaterial,
									  Lxt::TexturePool&		a_texPool,
									  Lxt::Material&		a_lxtMaterial )
{
	a_lxtMaterial.Set( a_ligMaterial.m_diffuseColour,
					   a_ligMaterial.m_specularColour,
					   a_ligMaterial.m_shininess,
					   a_texPool.Add( a_ligMaterial.m_texturePath ),
					   a_ligMaterial.m_blendType,
					   a_ligMaterial.m_lit );
}

void CreateOutputIndices(const TupleSet&						a_uniqueIndices, 
						 const std::vector<Tuple>&				a_rawIndices,
						 std::vector<IndexBuffer::IndexType>&	a_outIndices )
{
	// for all tuples, find the "index" of the unique version and add to output
	for ( size_t i = 0; i < a_rawIndices.size(); i++ )
	{
		TupleSet::const_iterator it = a_uniqueIndices.find( a_rawIndices[i] );
		
		if ( it != a_uniqueIndices.end() )
		{
			a_outIndices.push_back( it->second );
		}
		else
		{
			LXT_ASSERT( false ); // Couldn't find entry that should be there.
		}
	}
}

// Convert indices into AOS, from SOA
void CreateIndexTuples( const std::vector<const LigIndexStream*>&	a_indexStreams, 
						std::vector<Tuple>&							a_indexTuples )
{
	const size_t numStreams	= a_indexStreams.size();
	const size_t numIndices	= a_indexStreams[0]->m_elements.size();
	
	for ( size_t i = 0; i < numIndices; i++ )
	{
		Tuple t; t.reserve( numStreams ); 
		
		for ( size_t s = 0; s < numStreams; s++ )
		{
			uint32_t index = a_indexStreams[s]->m_elements[i];
			t.push_back( index );
		}
		
		a_indexTuples.push_back( t );
	}
}

// Create tuple maps for a set of PIndicies with a given stride
void CreateTupleMaps(const std::vector<Tuple>&	a_tuples,
					 TupleSet&					a_outTupleSet,
					 RTupleSet&					a_outRTupleSet,
					 IndexBuffer::IndexType&	a_startIndex )
{
	// Create a set of unique tuples.
	const size_t n = a_tuples.size();
	
	for ( size_t i = 0; i < n; i++ )
	{	
		const Tuple& t = a_tuples.at( i );
		
		// try to insert - if its unique, we've found a new point so we
		// should increment the "real" index.
		TupleSet::iterator it 	= a_outTupleSet.find( t );
		
		if ( it == a_outTupleSet.end() )
		{
			/* LXT_LOG_SPAM("Found unique tuple:");
			 for ( size_t j = 0; j < t.size(); j++ )
			 {
				 LXT_LOG_SPAM(" %d,", t[j] );
			 }
			 LXT_LOG_SPAM(" id=%d\n", a_startIndex );
			*/
			
			// insert into forward map
			a_outTupleSet.insert( TupleSet::value_type( t, a_startIndex ) );
			// insert into reverse map
			a_outRTupleSet.insert( RTupleSet::value_type( a_startIndex, t ) );
			
			// create a new index
			a_startIndex++;
		}
	}
}

bool CreateVertexDeclaration(const std::vector<LigSemantic>&		a_semantics,
							 const std::vector<LigVertexStream>&	a_vstreams,
							 VertexDeclaration&						a_vd )
{
	std::vector<LigSemantic>::const_iterator it = a_semantics.begin();
	
	while ( it != a_semantics.end() ) 
	{
		VertexElementUsage	veu = VEU_MAX;
		VertexElementType	vet = VET_MAX;
		size_t				s	= a_vstreams[ it->m_vstreamOffset ].m_stride;
		
		if ( it->m_name == "POSITION" ) 
		{
			veu = VEU_POSITION;
			
			switch ( s ) {
				case 2:
					vet = VET_FLOAT2;
					break;
				case 3:
					vet = VET_FLOAT3;
					break;
				default:
					LXT_LOG_ERROR( "Unsupported position format.\n" );
					return false;
					break;
			}
		}
		else if ( it->m_name == "NORMAL" )
		{
			if ( s != 3 )
			{
				LXT_LOG_ERROR( "Unsupported normal format.\n" );
				return false;
			}
			
			veu = VEU_NORMAL;
			vet = VET_FLOAT3;			
		}
		else if ( it->m_name == "TEXCOORD" )
		{
			if ( s != 2 )
			{
				LXT_LOG_ERROR( "Unsupported texcoord format.\n" );
				return false;
			}
			
			veu = VEU_TEXCOORD;
			vet = VET_FLOAT2;
		}
		else if ( it->m_name == "COLOR" )
		{
			if ( s != 4 )
			{
				LXT_LOG_ERROR( "Unsupported colour format.\n" );
				return false;
			}
			
			veu = VEU_COLOUR;
			vet = VET_UBYTE4;
		}
		else 
		{
			LXT_LOG_ERROR( "Unknown stream semantic '%s'.\n", it->m_name.c_str() );
			return false;
		}
		
		// Create vertex description
		a_vd.Add( vet, veu );
		it++;
	}
	
	return true;
}

bool ConvertLIGNodes(const LigNode*			a_ligNode,
					 const LigMaterialMap&	a_materials,
					 TexturePool&			a_texturePool, 
					 Model::Node*&			a_modelNode )
{
	LXT_ASSERT( a_modelNode == NULL );
	
	// Create model node & meshes
	a_modelNode				= new Model::Node;
	a_modelNode->m_transform= a_ligNode->m_transform;
	const size_t m			= a_ligNode->m_meshes.size();
	
	if ( m > 0 )
	{
		// convert mesh to LXT
		if ( !ConvertLIGMeshToLXTMesh( *(a_ligNode->m_meshes[0]), a_materials,
									  a_texturePool, a_modelNode->m_mesh ) )
		{
			return false;
		}

		if ( m > 1 )
		{
			LXT_LOG_ERROR("Mutliple meshes found on node '%s', only converting " 
						  "first one.\n", a_ligNode->m_name.c_str() );
			return false;
		}
	}
	
	// Create children
	const size_t c = a_ligNode->m_children.size();
	
	if ( c > 0 )
	{
		a_modelNode->m_children.reserve( c );
		
		for ( size_t i = 0; i < c; i++ )
		{
			// Create new child
			Model::Node* newChild = NULL;
			
			if ( !ConvertLIGNodes( a_ligNode->m_children[i], a_materials, 
								 a_texturePool, newChild ) )
			{
				return false;
			}
			
			// add to list
			a_modelNode->m_children.push_back( newChild );
		}
	}
	
	return true;
}

bool Lxt::ConvertLIGScenetoLXTModel(const LigScene&	a_scene, 
									TexturePool&	a_texturePool, 
									Model&			a_model )
{
	// Convert LIG nodes to LXT nodes
	Model::Node* modelRoot = NULL;
	bool status = ConvertLIGNodes( a_scene.m_root, a_scene.m_materials, 
								  a_texturePool, modelRoot );
	
	// Assign to model, which owns nodes
	a_model.SetRoot( modelRoot );
	
	// Process textures
	LigMaterialMap::const_iterator it = a_scene.m_materials.begin();
	
	while ( it != a_scene.m_materials.end() )
	{
		if ( !it->second.m_texturePath.empty() )
		{
			a_texturePool.Add( it->second.m_texturePath );
		}
		
		it++;
	}
	
	return status;
}

bool Lxt::ConvertLIGMeshToLXTMesh(const LigMesh&		a_ligMesh,
								  const LigMaterialMap&	a_materials,
								  TexturePool&			a_texturePool,
								  Mesh&					a_lxtMesh )
{	
	// Vertex declaration and Primitive Type
	a_lxtMesh.m_primitiveType	= a_ligMesh.m_primitiveType;
	if ( !CreateVertexDeclaration( a_ligMesh.m_batches[0].m_semantics, 
							a_ligMesh.m_vertexStreams, a_lxtMesh.m_vd ) )
	{
		return false;
	}
	// TODO - ensure VD is the same for each batch.
	
	// pass 1 -  create maps for every index tuple (for example the P indicies in DAE).
	const size_t			numBatches	= a_ligMesh.m_batches.size();	
	IndexBuffer::IndexType	startIndex	= 0;
	TupleSet				indexSet;		// unique tuples, with order
	RTupleSet				rIndexSet;		// reverse mapping
	
	std::vector< IndexBuffer::IndexType >	outIndices;	

	for ( size_t i = 0; i < numBatches; i++ )
	{
		std::vector< Tuple > indexTuples;
		
		// make a vector of index streams for this batch
		std::vector< const LigIndexStream* > visp;
		
		for ( size_t s = 0; s < a_ligMesh.m_batches[i].m_semantics.size(); s++ )
		{
			const LigSemantic& sem = a_ligMesh.m_batches[i].m_semantics[s];
			visp.push_back( &a_ligMesh.m_indexStreams[ sem.m_istreamOffset ] );
		}
		
		CreateIndexTuples( visp, indexTuples );
		CreateTupleMaps( indexTuples, indexSet, rIndexSet, startIndex );
		
		// Get material
		const std::string matName = a_ligMesh.m_batches[i].m_materialName;
		LigMaterialMap::const_iterator it = a_materials.find( matName );
		
		if ( it == a_materials.end() )
		{
			LXT_LOG_ERROR("Can't find material '%s', skipping element.", 
						  matName.c_str() );
			continue;
		}
		
		Lxt::Batch b;
		
		// Create real indices
		b.m_indexStart	= outIndices.size();
		
		CreateOutputIndices( indexSet, indexTuples, outIndices );
		
		b.m_indexCount	= outIndices.size() - b.m_indexStart;
		
		ConvertLIGMaterialToLXTMaterial( it->second, a_texturePool, b.m_material );
		
		LXT_LOG_SPAM( "Batch #%lu : %d -> %d (%s)\n", 
					 i, b.m_indexStart, b.m_indexCount, matName.c_str() );
		
		a_lxtMesh.m_batches.push_back( b );
	}
	
	// pass 3 - create vertices	
	Buffer outVertices;
	
	for ( RTupleSet::const_iterator it = rIndexSet.begin();
		 it != rIndexSet.end(); it++ )
	{
		const Tuple& tuple		= it->second;
		const size_t tupleSize	= tuple.size();
		LXT_ASSERT( tupleSize == a_lxtMesh.m_vd.Length() );
		
		for ( size_t t = 0; t < tupleSize; t++ )
		{
			// Get the vertex stream we will get data from
			size_t vstreamOffset			= 
					a_ligMesh.m_batches[0].m_semantics[t].m_vstreamOffset;
			const LigVertexStream& vstream	= 
					a_ligMesh.m_vertexStreams[ vstreamOffset ];
			size_t vaIndex					= tuple[t];
			
			// Do conversions from float to other types here
			if ( a_lxtMesh.m_vd.m_decl[t].m_type == VET_UBYTE4 )
			{
				float r = vstream.m_elements[vaIndex * vstream.m_stride + 0];
				float g = vstream.m_elements[vaIndex * vstream.m_stride + 1];
				float b = vstream.m_elements[vaIndex * vstream.m_stride + 2];
				float a = vstream.m_elements[vaIndex * vstream.m_stride + 3];
				
				Colour c = Colour(Vec4(r,g,b,a));
				outVertices.Append(c.m_abgr);
			}
			else 
			{
				// Add everything else as full floats
				for ( size_t k = 0; k < vstream.m_stride; k++ )
				{
					const size_t index = vaIndex * vstream.m_stride + k;					
					float v = vstream.m_elements[ index ];
					outVertices.Append( v );
				}
			}				
		}
	}
	
	a_lxtMesh.m_vertices.Set(outVertices.Data(), outVertices.Size());
	a_lxtMesh.m_indices.Set((void*)&outIndices[0], outIndices.size() * 
                            sizeof(IndexBuffer::IndexType));
	
	return true;
}