//
//  iOSHelper.h
//  ShakeForFood2
//
//  Created by long on 3/27/15.
//
//


#include <stdio.h>
#include "cocos2d.h" 




class  iOSHelper {
    
public:
    iOSHelper() { }
   
    // Admob
    static void rateApp();
    static void hideAdmobBanner();
    static void showAdmobBanner();
    static void showAdmobInters();
    static cocos2d::CCSize getWinSize();
};

 

