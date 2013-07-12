//
//  Settings.h
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/12.
//
//

#ifndef __In_App_Purchase_Sample__Settings__
#define __In_App_Purchase_Sample__Settings__

#include <iostream>

class Settings {
public:
    static void initNetwork();
    
    static std::string playerId();
    static std::string nickname();
};

#endif /* defined(__In_App_Purchase_Sample__Settings__) */
