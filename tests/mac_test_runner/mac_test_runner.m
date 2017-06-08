#import <Foundation/Foundation.h>
#include "UnitTest++.h"

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	int results = UnitTest::RunAllTests();
	
    [pool drain];
	
    return results;
}
