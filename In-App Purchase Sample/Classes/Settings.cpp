//
//  Settings.cpp
//  In-App Purchase Sample
//
//  Created by 下川 北斗 on 2013/07/12.
//
//

#include "Settings.h"

#include <libGSS/libGSS.h>

void Settings::initNetwork(){
    libgss::Network::instance()->setHost("localhost");
    libgss::Network::instance()->setConsumerSecret("cefadb98d551472bb1915fde3cd1ca4f");
    libgss::Network::instance()->setHttpPort(3000);
    libgss::Network::instance()->setSslPort(3001);
    libgss::Network::instance()->setUseSslLogin(false);
}

std::string Settings::playerId(){
    return "1000001";
}

std::string Settings::nickname(){
    return "ichiro";
}