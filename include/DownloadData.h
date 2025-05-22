#pragma once

#include <iostream>
#include <libxl.h>
#include "../include/Node.h"
#include <fstream>

namespace DD{

    class Data{
        public:
            Data();
            Data(node::Node &node);
            ~Data();
        private:

    };

};