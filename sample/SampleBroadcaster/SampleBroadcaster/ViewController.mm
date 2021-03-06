//
//  ViewController.m
//  SampleBroadcaster
//
//  Created by James Hurley on 5/6/14.
//  Copyright (c) 2014 videocore. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_btnConnect release];
    [_imgPreview release];
    [super dealloc];
}

- (IBAction)btnConnectTouch:(id)sender {
    
    if([self.btnConnect.titleLabel.text isEqualToString:@"Connect"]) {
        [self.btnConnect setTitle:@"Connecting..." forState:UIControlStateNormal];
        NSString* rtmpUrl = @"rtmp://192.168.2.1/live/myStream";
        
        _sampleGraph.reset(new videocore::sample::SampleGraph([self](videocore::sample::SessionState state){
            [self connectionStatusChange:state];
        }));
    
        
        _sampleGraph->setPBCallback([=](const uint8_t* const data, size_t size) {
            [self gotPixelBuffer: data withSize: size];
        });
        
        float scr_w = 480;
        float scr_h = 320;
        
        _sampleGraph->startRtmpSession([rtmpUrl UTF8String], scr_w, scr_h, 500000 /* video bitrate */, 15 /* video fps */);
    }
    else if ( [self.btnConnect.titleLabel.text isEqualToString:@"Connected"]) {
        // disconnect
        _sampleGraph.reset();
        [self.btnConnect setTitle:@"Connect" forState:UIControlStateNormal];

    }
    
}

- (void) connectionStatusChange:(videocore::sample::SessionState) state
{
    NSLog(@"Connection status: %d", state);
    if(state == videocore::sample::kSessionStateStarted) {
        NSLog(@"Connected");
        [self.btnConnect setTitle:@"Connected" forState:UIControlStateNormal];
        [self.btnConnect.titleLabel sizeToFit];

        
    } else if(state == videocore::sample::kSessionStateError || state == videocore::sample::kSessionStateEnded) {
        NSLog(@"Disconnected");
        [self.btnConnect setTitle:@"Connect" forState:UIControlStateNormal];
        _sampleGraph.reset();
    }
}
- (void) gotPixelBuffer: (const uint8_t* const) data withSize: (size_t) size {
    
    @autoreleasepool {
        
       
        CVPixelBufferRef pb = (CVPixelBufferRef) data;
        CVPixelBufferLockBaseAddress(pb, 1);
        CIImage *ciImage = [CIImage imageWithCVPixelBuffer:pb];
            
        CIContext *temporaryContext = [CIContext contextWithOptions:nil];
        CGImageRef videoImage = [temporaryContext
                                     createCGImage:ciImage
                                     fromRect:CGRectMake(0, 0,
                                                         CVPixelBufferGetWidth(pb),
                                                         CVPixelBufferGetHeight(pb))];
            
        UIImage *uiImage = [UIImage imageWithCGImage:videoImage];
        CVPixelBufferUnlockBaseAddress(pb, 0);
        
        [self.imgPreview performSelectorOnMainThread:@selector(setImage:) withObject:uiImage waitUntilDone:NO];
    
        CGImageRelease(videoImage);

    }
}
@end
