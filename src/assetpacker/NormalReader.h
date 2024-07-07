#pragma once
#include <string>
#include <filesystem>
#include "Reader.h"
#include "Meta.h"
using namespace std;
namespace assetpacker {
    class NormalReader : public Reader
    {
    private:
    public:
        NormalReader(std::string dir);
        bool Validate() override;
        void Read() override;
        void operator >> (AssetFile* as) override;
    };

}