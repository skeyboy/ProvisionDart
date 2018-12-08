#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>
#import <AppKit/AppKit.h>
#import "Common.h"
OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options);
void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview);

/* -----------------------------------------------------------------------------
   Generate a preview for file

   This function's job is to create preview for designated file
   ----------------------------------------------------------------------------- */

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options)
{
#ifdef DEBUG
    NSDate *startDate = [NSDate date];
#endif
    n8log(@"Generating Preview");
    if (QLPreviewRequestIsCancelled(preview))
        return noErr;
    
    
    // Invoke colorize.sh
    CFBundleRef bundle = QLPreviewRequestGetGeneratorBundle(preview);
    int status;
    
    NSData *output = colorizeURL(bundle, url, &status, 0);
    n8log(@"Generated preview html page in %.3f sec",
          -[startDate timeIntervalSinceNow] );
    
    NSData *rtf = nil;
    NSUserDefaults *defaults = [[NSUserDefaults alloc] initWithSuiteName:@"com.sk.ProvisionDart"];
    BOOL use_rtf = [defaults boolForKey:@"rtfPreview"];
 
    if (use_rtf) {
        NSDictionary *attrs;
        NSAttributedString *string = [[NSAttributedString alloc] initWithHTML:output documentAttributes:&attrs];
        NSRange range = NSMakeRange(0, [string length]);
        rtf = [string RTFFromRange:range documentAttributes:attrs];
     }
    
    if (status != 0 || QLPreviewRequestIsCancelled(preview)) {
#ifndef DEBUG
        goto done;
#endif
    }
    // Now let WebKit do its thing
    NSString *textEncoding = [[NSUserDefaults standardUserDefaults]
                              stringForKey:@"webkitTextEncoding"];
    if (!textEncoding || [textEncoding length] == 0)
        textEncoding = @"UTF-8";
    CFDictionaryRef properties =
    (__bridge CFDictionaryRef)[NSDictionary dictionaryWithObject:textEncoding
                                                 forKey:(NSString *)kQLPreviewPropertyTextEncodingNameKey];
    
    if (use_rtf)
        QLPreviewRequestSetDataRepresentation(preview, (__bridge CFDataRef)rtf, kUTTypeRTF, properties);
    else
        QLPreviewRequestSetDataRepresentation(preview, (__bridge CFDataRef)output, kUTTypeHTML, properties);
    
#ifndef DEBUG
done:
#endif
    n8log(@"Finished preview in %.3f sec", -[startDate timeIntervalSinceNow] );
    return noErr;
}

void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview)
{
    // Implement only if supported
}
