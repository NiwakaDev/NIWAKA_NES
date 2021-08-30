#pragma once
#include "common.h"
using namespace std;

class InterruptManager:public Object{
    private:
        bool nmi;
    public:
        InterruptManager();
        bool HasNmi();
        void SetNmi();
        void ClearNmi();
};