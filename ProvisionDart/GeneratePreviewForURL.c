#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>
#import <AppKit/AppKit.h>
OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options);
void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview);

/* -----------------------------------------------------------------------------
   Generate a preview for file

   This function's job is to create preview for designated file
   ----------------------------------------------------------------------------- */

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options)
{
    // To complete your generator please implement the function GeneratePreviewForURL in GeneratePreviewForURL.c
    NSURL *URL = (__bridge NSURL *)url;
NSString * dartContent = [[NSString alloc] initWithContentsOfURL:URL
                                                        encoding:NSUTF8StringEncoding
                                                           error:nil];
    
    NSDictionary *properties = @{ // properties for the HTML data
        (__bridge NSString *)kQLPreviewPropertyTextEncodingNameKey : @"UTF-8",
        (__bridge NSString *)kQLPreviewPropertyMIMETypeKey : @"text/html" };
//    DART_CONTENT
    NSURL *bundleURL = [[NSBundle bundleWithIdentifier:@"com.sk.ProvisionDart"] URLForResource:@"heightlight" withExtension:@"bundle"];
    NSURL *templateURL =  [[[NSBundle alloc] initWithURL:bundleURL] URLForResource:@"Template" withExtension:@"html"];
    
    NSMutableString * templateContent = [[NSMutableString alloc] initWithContentsOfURL:templateURL
encoding:NSUTF8StringEncoding error:nil];
    
    NSString *finalContent = [templateContent stringByReplacingOccurrencesOfString:@"$DART_CONTENT" withString:dartContent];

    
    QLPreviewRequestSetDataRepresentation(preview, (__bridge CFDataRef)[finalContent dataUsingEncoding:NSUTF8StringEncoding], kUTTypeHTML, (__bridge CFDictionaryRef)properties);
    
    return noErr;
}

void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview)
{
    // Implement only if supported
}
