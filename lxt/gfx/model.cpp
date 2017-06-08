/*
 *  model.cpp
 *  model
 *
 *  Created by Justin on 24/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "model.h"

#include "mesh.h"
#include "texture.h"
#include "math/math.h"
#include "renderer.h"
#include "file/file.h"
#include "gles/gl_headers.h"

namespace Lxt 
{
	Model::Node::Node() 
	:	m_transform( Mat4::Identity )
	{}
	
	Model::Node::~Node()
	{
		for ( size_t i = 0; i < m_children.size(); i++ )
		{
			delete m_children[i];
		}
	}
	
	void Model::Node::Upload()
	{
		// Upload me
		if ( !m_mesh.IsEmpty() ) m_mesh.Upload();
		
		// Upload children
		const size_t numChildren = m_children.size();
		for ( size_t i = 0; i < numChildren; i++ )
			m_children[i]->Upload();
	}
	
	void Model::Node::Draw( Renderer& a_renderer )
	{
		// Draw me
		a_renderer.ApplyModelView( m_transform );

		if ( !m_mesh.IsEmpty() ) 
		{
			a_renderer.Render( m_mesh );
		}
		
		// Draw children
		const size_t numChildren = m_children.size();
		for ( size_t i = 0; i < numChildren; i++ )
		{
			glPushMatrix();
			m_children[i]->Draw( a_renderer );
			glPopMatrix();
		}
	}
	
	void Model::Node::Dump( std::string a_indent )
	{
		// Dump me
		
		LXT_LOG_SPAM( "%sNode (%p):\n", a_indent.c_str(), this )
		a_indent += "\t";
		
		for ( size_t i = 0; i < 4; i++ )
		{
			LXT_LOG_SPAM("%s(%2.2f %2.2f %2.2f %2.2f)\n", a_indent.c_str(),
						 m_transform.m_column[i].m_x,
						 m_transform.m_column[i].m_y,
						 m_transform.m_column[i].m_z,
						 m_transform.m_column[i].m_w );
		}
		
		if ( m_mesh.IsEmpty() )
		{
			LXT_LOG_SPAM( "%sMesh: <empty>\n", a_indent.c_str() );
		}
		else
		{
			m_mesh.Dump();
		}
		
	    // Dump children
		const size_t numChildren = m_children.size();
		for ( size_t i = 0; i < numChildren; i++ )
			m_children[i]->Dump( a_indent + "\t" );
	}
				
	size_t Model::Node::Store( const Node*			a_node, 
							   const TexturePool&	a_tp,
							   Archive&				a_archive )
	{
		size_t written = 0;
		
		// Store me
		written += Mat4::Store( a_node->m_transform, a_archive );
		written += Mesh::Store( a_node->m_mesh, a_tp, a_archive );
		
		// Store children
		written += a_archive.Storage().Append( 
								(uint32_t) a_node->m_children.size() );
		
		for ( size_t i = 0; i < a_node->m_children.size(); i++ )
		{
			written += Store( a_node->m_children[i], a_tp, a_archive );
		}
		
		return written;
	}
	
	void Model::Node::Extract(Node*&		a_node, 
							  TexturePool&	a_tp, 
							  Archive&		a_archive,
							  size_t&		a_offset )
	{
		Buffer& from = a_archive.Storage();
		
		// Extract me
		a_node = new Node;
		Mat4::Extract( a_node->m_transform, a_archive, a_offset );
		Mesh::Extract( a_node->m_mesh, a_tp, a_archive, a_offset );
		
		// Extract children
		uint32_t numChildren = 0;
		a_offset += from.Read( numChildren, a_offset );
		a_node->m_children.resize( numChildren );
		
		for ( size_t i = 0; i < numChildren; i++ )
		{
			Extract( a_node->m_children[i], a_tp, a_archive, a_offset );
		}
	}
	
	// Model -------------------------------------------------------------------
	Model::Model() 
		:	m_root( NULL )
	{
	}
	
	Model::~Model()
	{
		SetRoot( NULL );
	}
	
	bool Model::Load( const Path& a_path, TexturePool& a_texturePool )
	{
		bool status = false;
		Archive archive;
		
		// load archive
		if ( archive.Load( a_path ) )
		{
			// Clear existing model
			SetRoot( NULL );
			
			// Extract from archive
			size_t offset = 0;
			Lxt::Model::Extract( *this, a_texturePool, archive, offset );
			
			if ( offset != 0 )
			{
				// upload geometry to GPU
				Upload();
				status = true;
			}
		}
		
		return status;
	}

	void Model::SetRoot( Node* a_root )
	{
		delete m_root;
		m_root = a_root;
	}
	
	void Model::Upload()
	{
		m_root->Upload();
	}
	
	void Model::Draw( Renderer& a_renderer )
	{
		m_root->Draw( a_renderer );
	}
	
	void Model::Dump()
	{
		m_root->Dump("");
	}
	
	// Extract a model from an archive.
	void Model::Extract(Model&			a_model, 
						TexturePool&	a_tp, 
						Archive&		a_source,
						size_t&			a_offset )
	{
		// Hierarchy
		Node::Extract( a_model.m_root, a_tp, a_source, a_offset );
	}
	
	// Encode a model into an archive.
	size_t Model::Store(const Model&		a_model, 
						const TexturePool&	a_tp,
						Archive&			a_destination )
	{
		size_t written = 0;
		
		// Hierarchy
		written += Node::Store( a_model.m_root, a_tp, a_destination );
		
		return written;
	}
	
}

