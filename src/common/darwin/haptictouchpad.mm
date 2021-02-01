#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>

void vibrateSmall()
{
    [[NSHapticFeedbackManager defaultPerformer] performFeedbackPattern:NSHapticFeedbackPatternGeneric performanceTime:NSHapticFeedbackPerformanceTimeNow];
}

void vibrateHard() {
    [[NSHapticFeedbackManager defaultPerformer] performFeedbackPattern:NSHapticFeedbackPatternLevelChange performanceTime:NSHapticFeedbackPerformanceTimeNow];
}


