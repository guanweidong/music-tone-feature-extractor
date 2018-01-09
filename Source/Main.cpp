//
//  Main.cpp
//  SvmTrainModelCreator (App)
//
//  Created by 陳柏諺 on 2018/1/9.
//

#include "JuceHeader.h"
#include "Util.hpp"
#include "OnsetDetector.hpp"
#include "SvmFeaturesExtractor.hpp"
#include "FeaturesFormatter.hpp"

#include <cstdlib>
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: programe [audio file] [label]" << std::endl;
        return EXIT_FAILURE;
    }

    const juce::File inputFile = getAbsolutePath(argv[1]);
    if (!inputFile.existsAsFile()) {
        std::cerr << "input file is not exist:" << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    const std::string label = argv[2];
    const auto features = SvmFeaturesExtractor::extract(inputFile);
    std::cout << FeaturesFormatter::format(label, std::begin(features), std::end(features)) << std::endl;

    return EXIT_SUCCESS;
}
