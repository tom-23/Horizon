#include <AppKit/AppKit.h>
#include <QtWidgets>

@interface TouchBarProvider: NSResponder <NSTouchBarDelegate, NSApplicationDelegate, NSWindowDelegate>

@property (strong) NSCustomTouchBarItem *touchBarLibraryItem;
@property (strong) NSCustomTouchBarItem *touchBarItem2;
@property (strong) NSButton *touchBarLibraryButton;
@property (strong) NSButton *touchBarButton2;

@property (strong) NSObject *qtDelegate;

@end

// Create identifiers for two button items.
static NSTouchBarItemIdentifier libraryButtonIdentifier = @"com.horizon.library";
static NSTouchBarItemIdentifier mixerButtonIdentifier = @"com.horizon.mixer";

@implementation TouchBarProvider

- (NSTouchBar *)makeTouchBar
{
    // Create the touch bar with this instance as its delegate
    NSTouchBar *bar = [[NSTouchBar alloc] init];
    bar.delegate = self;

    // Add touch bar items: first, the very important emoji picker, followed
    // by two buttons. Note that no further handling of the emoji picker
    // is needed (emojii are automatically routed to any active text edit). Button
    // actions handlers are set up in makeItemForIdentifier below.
    bar.defaultItemIdentifiers = @[NSTouchBarItemIdentifierCharacterPicker,
                                   libraryButtonIdentifier, mixerButtonIdentifier];

    return bar;
}

- (NSTouchBarItem *)touchBar:(NSTouchBar *)touchBar makeItemForIdentifier:(NSTouchBarItemIdentifier)identifier
{
    Q_UNUSED(touchBar);

    // Create touch bar items as NSCustomTouchBarItems which can contain any NSView.
    if ([identifier isEqualToString:libraryButtonIdentifier]) {
        QString title = "Library";
        self.touchBarLibraryItem = [[[NSCustomTouchBarItem alloc] initWithIdentifier:identifier] autorelease];
        self.touchBarLibraryButton = [[NSButton buttonWithTitle:title.toNSString() target:self
                                          action:@selector(button1Clicked)] autorelease];
        self.touchBarLibraryItem.view =  self.touchBarLibraryButton;
         return self.touchBarLibraryItem;
    } else if ([identifier isEqualToString:mixerButtonIdentifier]) {
        QString title = "Mixer";
        self.touchBarItem2 = [[[NSCustomTouchBarItem alloc] initWithIdentifier:identifier] autorelease];
        self.touchBarButton2 = [[NSButton buttonWithTitle:title.toNSString() target:self
                                          action:@selector(button2Clicked)] autorelease];
        self.touchBarItem2.view =  self.touchBarButton2;
        return self.touchBarItem2;
    }
   return nil;
}

- (void)installAsDelegateForWindow:(NSWindow *)window
{
    _qtDelegate = window.delegate; // Save current delegate for forwarding
    window.delegate = self;
}

- (void)installAsDelegateForApplication:(NSApplication *)application
{
    _qtDelegate = application.delegate; // Save current delegate for forwarding
    application.delegate = self;
}

- (BOOL)respondsToSelector:(SEL)aSelector
{
    // We want to forward to the qt delegate. Respond to selectors it
    // responds to in addition to selectors this instance resonds to.
    return [_qtDelegate respondsToSelector:aSelector] || [super respondsToSelector:aSelector];
}

- (void)forwardInvocation:(NSInvocation *)anInvocation
{
    // Forward to the existing delegate. This function is only called for selectors
    // this instance does not responds to, which means that the qt delegate
    // must respond to it (due to the respondsToSelector implementation above).
    [anInvocation invokeWithTarget:_qtDelegate];
}

- (void)button1Clicked
{
    qDebug() << "button1Clicked";
}

- (void)button2Clicked
{
    qDebug() << "button2Clicked";
}

@end

void initTouchbar(QWidget *parent) {
   {
       TouchBarProvider *touchBarProvider = [[TouchBarProvider alloc] init];
       [touchBarProvider installAsDelegateForApplication:[NSApplication sharedApplication]];
    }
    {
            // Install TouchBarProvider as window delegate
            NSView *view = reinterpret_cast<NSView *>(parent->winId());
            TouchBarProvider *touchBarProvider = [[TouchBarProvider alloc] init];
            [touchBarProvider installAsDelegateForWindow:view.window];
        }
}


