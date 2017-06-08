/*
 *  font.cpp
 *  render
 *
 *  Created by Justin on 13/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#include "font.h"

#include <libxml/xmlreader.h>
#include "file/file.h"

namespace Lxt 
{
	Font::Font()
		: m_info( NULL )
	{
		
	}
	
	Font::~Font()
	{
		if ( m_info )
			delete m_info;
		
		m_info = NULL;
	}
	
	bool Font::Create( const char* a_fontFile )
	{		
		if ( m_info )
		{
			LXT_LOG_ERROR("Font object already created.\n");
			return false;
		}

		// make file path
		Path filepath;
		File::GetApplicationPath( filepath );
		filepath += a_fontFile;
		
		xmlDocPtr	doc;
		xmlNodePtr	cur;
		
		doc = xmlParseFile( filepath.c_str() );
		
		if ( !doc ) 
		{
			LXT_LOG_ERROR("XML document not formatted correctly.\n")
			return false;
		}
		
		cur = xmlDocGetRootElement(doc);
		
		bool status = false;
		
		if (cur != NULL && xmlStrcmp(cur->name, (const xmlChar *) "font") == 0) 
		{
			// Create the info struct
			m_info = new FontInfo;
	
			// it looks like a font XML file, grab the info, common pages and
			// chars nodes and look for stuff there.
			cur = cur->xmlChildrenNode;
			while ( cur != NULL )
			{
				if ( xmlStrcmp(cur->name, (const xmlChar *) "info") == 0 )
				{
					// info
					m_info->m_face = (const char*) 
						xmlGetProp( cur, (const xmlChar* ) "face" );
				}
				else if ( xmlStrcmp(cur->name, (const xmlChar *) "common" ) == 0 )
				{
					// common
					const char* lineHeightStr = (const char*) 
						xmlGetProp( cur, (const xmlChar*) "lineHeight" );
					const char* baseStr = (const char*)
						xmlGetProp( cur, (const xmlChar*) "base" );
					
					m_info->m_size = atoi( lineHeightStr );
					m_info->m_base = atoi( baseStr );
				}
				else if ( xmlStrcmp(cur->name, (const xmlChar *) "pages" ) == 0 )
				{
					// pages
					xmlNodePtr pageNode = cur->xmlChildrenNode;
					while( pageNode != NULL )
					{
						if ( xmlStrcmp(pageNode->name, (const xmlChar*) "page" ) == 0 )
						{
							Path path; File::GetApplicationPath( path );
							m_info->m_texturePageFile = path;
							m_info->m_texturePageFile += (const char*) 
								xmlGetProp( pageNode, (const xmlChar*) "file" );
							
							// only one texture page supported
							break;
						}
						
						pageNode = pageNode->next;
					}
				}
				else if ( xmlStrcmp(cur->name, (const xmlChar *) "chars" ) == 0 )
				{
					// glyphs
					
					// get number of chars and reserve 
					const char* numCharsStr = (const char*) 
									xmlGetProp( cur, (const xmlChar*) "count");
					size_t numChars = atoi( numCharsStr );
					m_info->m_glyphs.reserve( numChars );
					
					// set initial range
					m_info->m_startIndex = 0;
					
					// grab each one
					xmlNodePtr charNode = cur->xmlChildrenNode;
					while ( charNode != NULL )
					{
						if ( xmlStrcmp(charNode->name, (const xmlChar*) "char" ) == 0 )
						{
							GlyphInfo gi;
							
							// Set start index using first character
							const char* idStr = (const char*)
								xmlGetProp( charNode, (const xmlChar*)"id" );
							
							if ( m_info->m_startIndex == 0 )
							{
								m_info->m_startIndex = uint16_t(atoi( idStr ));
							}
							
							// get dimensions
							const char* xStr = (const char*)
							xmlGetProp( charNode, (const xmlChar*)"x" );
							const char* yStr = (const char*)
							xmlGetProp( charNode, (const xmlChar*)"y" );
							const char* wStr = (const char*)
							xmlGetProp( charNode, (const xmlChar*)"width" );
							const char* hStr = (const char*)
							xmlGetProp( charNode, (const xmlChar*)"height" );
							const char* xoStr = (const char*)
							xmlGetProp( charNode, (const xmlChar*)"xoffset" );
							const char* yoStr = (const char*)
							xmlGetProp( charNode, (const xmlChar*)"yoffset" );
							const char* xaStr = (const char*)
							xmlGetProp( charNode, (const xmlChar*)"xadvance" );
							
							gi.m_start.m_x	= atoi( xStr );
							gi.m_start.m_y	= atoi( yStr );
							gi.m_width = gi.m_end.m_x	= atoi( wStr );
							gi.m_height = gi.m_end.m_y	= atoi( hStr );
							gi.m_xOffset	= atoi( xoStr );
							gi.m_yOffset	= atoi( yoStr );
							gi.m_xAdvance	= atoi( xaStr );
							
							// add to array
							m_info->m_glyphs.push_back( gi );
						}
						
						charNode = charNode->next;
					}
					
					// set end index, assume a continuous range
					m_info->m_endIndex = m_info->m_startIndex + m_info->m_glyphs.size();
				}
				
				cur = cur->next;
			}
			
			status = true;
		}
		// Finished parsing, throw document away
		xmlFreeDoc( doc );
		
		// Now, do some processing of the data
		
		// Load textures
		status = Lxt::TextureLoad( m_info->m_texturePageFile.c_str() , m_texture );
	
		// Convert glyph UVs into correct scale
		if ( status )
		{
			const size_t g = m_info->m_glyphs.size();
			const float tw = m_texture.GetWidth();
			const float th = m_texture.GetHeight();
			
			for ( size_t i = 0; i < g; i++ )
			{
				GlyphInfo& gi = m_info->m_glyphs[i];
				
				// make start and end from start and extent
				gi.m_end.m_x += gi.m_start.m_x;
				gi.m_end.m_y += gi.m_start.m_y;
				
				// scale to texture size
				gi.m_start.m_x /= tw;
				gi.m_start.m_y /= th;
				gi.m_end.m_x /= tw;
				gi.m_end.m_y /= th;				
			}
		}
		
		return status;
	}
	
	uint32_t Font::CalculateWidth( const char* a_string )
	{
		const char* p = a_string;
		uint32_t	w = 0;
		
		while ( *p ) 
		{
			uint32_t index = *p - m_info->m_startIndex;
			
			w += m_info->m_glyphs[ index ].m_xAdvance;
			p++;
		}
		
		return w;
	}	
}