//
//  iOSHelper.cpp
//  ShakeForFood2
//
//  Created by long on 3/27/15.
//
//

#include "iOSHelper.h"
#include "AppController.h"


void iOSHelper::rateApp(){
//    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
//    return [appDelegate rateApp];
}






void iOSHelper::hideAdmobBanner(){
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate hideAdmobBanner];
}

void iOSHelper::showAdmobBanner(){
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate showAdmobBanner];
}

void iOSHelper::showAdmobInters(){
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate showAdmobInters];
}

 

cocos2d::CCSize iOSHelper:: getWinSize(){
      AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    CGRect rect = [[UIScreen mainScreen] bounds];
   return cocos2d::CCSize(rect.size.width, rect.size.height) ;
}
