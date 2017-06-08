//
//  test_runnerAppDelegate.h
//  test_runner
//
//  Created by Justin on 28/09/08.
//  Copyright Monkey Style Games 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;
@class GLView;

@interface test_runnerAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	GLView *glView;
    RootViewController *rootViewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet GLView *glView;
@property (nonatomic, retain) IBOutlet RootViewController *rootViewController;

@end

