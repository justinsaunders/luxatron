/*
 *  model.h
 *  A model is a tree of meshes, where each node contains a transform and a mesh.
 *
 *  Created by Justin on 24/10/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_GFX_MODEL_H__
#define __LXT_GFX_MODEL_H__

#include "math/mat4.h"
#include "math/quat.h"
#include "mesh.h"

namespace Lxt 
{	
	class Renderer;
	
	class Model
	{
	public:
		struct Node;
		struct Node
		{	
			Node();
			~Node();
			
			void Upload();
			void Draw( Renderer& a_renderer );
			void Dump( std::string a_indent );
			
			// Serialization
			static size_t Store(const Node*		   a_root, 
								const TexturePool& a_tp,
								Archive&		   a_archive);
			
			static void	Extract(Node*&		 a_root, 
								TexturePool& a_tp,
								Archive&	 a_archive,
								size_t&		 a_offset);
			
			Mat4 m_transform;
			Mesh m_mesh;
			std::vector<Node*> m_children;
		};
		
		Model();
		~Model();
		
		// Load model from filesystem and upload to GPU
		bool Load( const Path& a_path, TexturePool& a_tp );
		
		// Initialize model with node tree, model owns passed in data.
		void SetRoot( Node* a_root );
				
		// Look at the model's hierachy
		const Node* GetRoot() const { return m_root; }
		
		// Upload data to GPU
		void Upload();
		
		// Draw
		void Draw( Renderer& a_renderer );
		
		// Debug
		void Dump();
		
		// Extract a model from an archive.
		static void Extract(Model&			a_model, 
							TexturePool&	a_tp, 
							Archive&		a_source,
							size_t&			a_offset );
		
		// Encode a model into an archive.
		static size_t Store(const Model&		a_model, 
							const TexturePool&	a_tp,
							Archive&			a_destination );
				
	private:
		Node*		m_root;			// node hierarchy
		
	};
	
}

#endif // __LXT_GFX_MODEL_H__

