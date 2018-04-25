//
//  Constants.h
//  JavaCompiler
//
//  Created by Ahmed on 4/22/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//

#ifndef Constants_h
#define Constants_h

#include <string>


namespace Constants {
    std::string const LAMBDA = "\\L";
    enum class RuleTokenType { TERMINAL, NON_TERMINAL, LAMBDA_TERMINAL};

}

#endif /* Constants_h */
