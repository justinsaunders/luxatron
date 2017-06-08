/*
 *  texture.mm
 *  Luxatron™
 *
 *  Created by Justin on 15/09/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */
#include "texture.h"
#include "core/debug.h"
#include "gles/gl_headers.h"

namespace Lxt 
{

#if LXT_PLATFORM_CURRENT == LXT_PLATFORM_IPHONE
	
bool TextureLoad( const Path& a_fileName, Lxt::Texture& a_texture )
{
	bool			status = false;
	
	if ( !File::Exists( a_fileName ) )
	{
		LXT_LOG_ERROR("Couldn't find texture file:'%s'\n", a_fileName.c_str());
		return status;
	}
	
	CGImageRef		spriteImage;
	CGContextRef	spriteContext;
	GLubyte*		spriteData;
	size_t			width, height;
	
	NSString*		nsFileName = [[NSString alloc] initWithUTF8String:a_fileName.c_str()];
	
	// Creates a Core Graphics image from an image file
	spriteImage = [UIImage imageWithContentsOfFile: nsFileName ].CGImage;
	
	uint32_t			bytesPerPixel	= 4;
	CGImageAlphaInfo	imageType		= kCGImageAlphaPremultipliedLast;
	uint32_t			glImageType		= GL_RGBA;
	
	if ( spriteImage )
	{
		// Get the width and height of the image
		width = CGImageGetWidth(spriteImage);
		height = CGImageGetHeight(spriteImage);
		
		// Allocated memory needed for the bitmap context
		spriteData = (GLubyte *) malloc(width * height * bytesPerPixel);
		
		// Uses the bitmatp creation function provided by the Core Graphics 
		// framework. 
		spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, 
											  width * bytesPerPixel, 
											  CGImageGetColorSpace(spriteImage), 
											  imageType );
		
		if ( spriteContext )
		{
			// After you create the context, you can draw the sprite image to the 
			// context.
			CGContextDrawImage(spriteContext, 
							   CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), 
							   spriteImage );
			
			// You don't need the context at this point, so you need to release it 
			// to avoid memory leaks.
			CGContextRelease(spriteContext);
			
			// Create LXT texture
			status = a_texture.Create( width, height, 0, 0, glImageType, 
									  GL_UNSIGNED_BYTE, spriteData );
		}
		
		// free image data
		free( spriteData );
	}
	
	return status;
}
	
#else
	
bool TextureLoad( const Path& a_fileName, Lxt::Texture& a_texture )
{
	LXT_ASSERT( false && "Function not implemented yet." );
	return false;
}
	
#endif
	
} // namespace Lxt
