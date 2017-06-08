/*
 *  log.h
 *  Luxatron
 *
 *  Created by Justin on 29/08/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */

#ifndef __LXT_CORE_LOG_H__
#define __LXT_CORE_LOG_H__

#include <stdio.h>

// Enable or disable verbose ( file,line,function ) prefix for log messages
//define LXT_VERBOSE_ERROR_LOGS 0
#define LXT_VERBOSE_ERROR_LOGS 1

#define LXT_LOG_ERROR( ... ) \
	if ( LXT_VERBOSE_ERROR_LOGS ) \
		printf( "ERROR %s(%d):%s() ", __FILE__, __LINE__, __FUNCTION__ ); \
	printf( __VA_ARGS__ );

#define LXT_LOG_SPAM( ... )	\
	printf( __VA_ARGS__ );

#endif // __CORE_LOG_H__