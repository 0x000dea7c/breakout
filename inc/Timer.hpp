#pragma once

#include "pch.hpp"

#include <chrono>

namespace Timer {

    float getTime()
    {
        using namespace std::chrono;
        static const auto start = high_resolution_clock::now();
        return duration<float>(high_resolution_clock::now() - start).count();
    }

};
