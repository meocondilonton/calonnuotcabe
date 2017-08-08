#import <UIKit/UIKit.h>
#import <GoogleMobileAds/GoogleMobileAds.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;
@property(nonatomic, strong) GADInterstitial *interstitial;
@property (retain, nonatomic) IBOutlet GADBannerView *vAd;

- (void)rateApp;
-(void)showAdmobInters;

-(void)showAdmobBanner;
-(void)hideAdmobBanner;

@end

