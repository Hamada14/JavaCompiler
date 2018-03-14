//
//  NFA_Builder.hpp
//  JavaCompiler
//
//  Created by Ahmed on 3/14/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//

#ifndef NFA_Builder_hpp
#define NFA_Builder_hpp

#include <unordered_set> 
#include <unordered_map>
#include <string>
#include "NFA.hpp"

using namespace std;

class NFA_Builder {
    unordered_map <string,NFA> keys, symTable;
    public:
        NFA solve(string s);
};

#endif /* NFA_Builder_hpp */
