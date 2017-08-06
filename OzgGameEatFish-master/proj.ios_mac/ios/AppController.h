#import <UIKit/UIKit.h>
#import <GoogleMobileAds/GoogleMobileAds.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;
@property(nonatomic, strong) GADInterstitial *interstitial;
- (void)rateApp;
-(void)showAdmobInters;

@end

