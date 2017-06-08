//
//  test_runnerAppDelegate.m
//  test_runner
//
//  Created by Justin on 28/09/08.
//  Copyright Monkey Style Games 2008. All rights reserved.
//

#import "test_runnerAppDelegate.h"
#import "RootViewController.h"

@implementation test_runnerAppDelegate

@synthesize window;
@synthesize glView;
@synthesize rootViewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
    [window addSubview:[rootViewController view]];
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [rootViewController release];
	[glView release];
    [window release];
    [super dealloc];
}

@end
