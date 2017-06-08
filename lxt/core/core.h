/*
 *  core.h
 *  Luxatron
 *
 *  Created by Justin on 26/06/08.
 *  Copyright 2008 Monkey Style Games. All rights reserved.
 *
 */
#ifndef __LXT_CORE_H__
#define __LXT_CORE_H__

// Common headers for entire LXT library. This file should be used as precompiled
// (prefix) file. Only common platform and Lxt core headers should go in here.
// OpenGL ES is platform specific, so it is not included here.

// System headers
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

// Luxatron headers
#include "platforms.h"	// Must be first LXT header
#include "types.h"		// Basic types
#include "memory.h"		// Override C/C++ memory functions here
#include "timer.h"
#include "debug.h"
#include "array.h"
#include "archive.h"
#include "pmonitor.h"

#endif // __LXT_CORE_H__