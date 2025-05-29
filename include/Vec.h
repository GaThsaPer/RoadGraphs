#pragma once

namespace VEC{
    class vector2{
        public:
            vector2() {}
            ~vector2() {}
            vector2(int _x, int _y): x(_x), y(_y) {}
            bool operator<(const vector2& other) const {
                return y < other.y;
            }

            int x;
            int y;
        private:

    };
};