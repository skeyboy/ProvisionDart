#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>
#import <AppKit/AppKit.h>
OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize);
void CancelThumbnailGeneration(void *thisInterface, QLThumbnailRequestRef thumbnail);

/* -----------------------------------------------------------------------------
    Generate a thumbnail for file

   This function's job is to create thumbnail for designated file as fast as possible
   ----------------------------------------------------------------------------- */

OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize)
{
    // To complete your generator please implement the function GenerateThumbnailForURL in GenerateThumbnailForURL.c
    
    NSURL *iconURL = [[NSBundle bundleWithIdentifier:@"com.sk.ProvisionDart"] URLForResource:@"ic_launcher" withExtension:@"png"];

    NSImage *appIcon = [[NSImage alloc] initWithContentsOfURL:iconURL];
    NSSize canvasSize = appIcon.size;
    NSRect renderRect = NSMakeRect(0.0, 0.0, appIcon.size.width, appIcon.size.height);

    CGContextRef _context = QLThumbnailRequestCreateContext(thumbnail, canvasSize, false, NULL);
    if (_context) {
        NSGraphicsContext *_graphicsContext = [NSGraphicsContext graphicsContextWithCGContext:(void *)_context flipped:NO];
        
        [NSGraphicsContext setCurrentContext:_graphicsContext];
       
            [appIcon drawInRect:renderRect];
        
        QLThumbnailRequestFlushContext(thumbnail, _context);
        CFRelease(_context);
    }
    return noErr;
}

void CancelThumbnailGeneration(void *thisInterface, QLThumbnailRequestRef thumbnail)
{
    // Implement only if supported
}
