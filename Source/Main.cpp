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
        std::cerr << "usage: programe [audio files folder] [label]" << std::endl;
        return EXIT_FAILURE;
    }

    const juce::File inputDirectory = getAbsolutePath(argv[1]);
    if (!inputDirectory.exists() || !inputDirectory.isDirectory()) {
        std::cerr << "input directory is not exist:" << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    const std::string label = argv[2];
    juce::DirectoryIterator input(inputDirectory, false);
    while (input.next()) {
        if (!input.getFile().existsAsFile()) {
            continue;
        }

        if (input.getFile().getFileExtension() != ".wav") {
            continue;
        }

        const auto features = SvmFeaturesExtractor::extract(input.getFile());
        std::cout << FeaturesFormatter::format(label, std::begin(features), std::end(features)) << std::endl;
    }

    return EXIT_SUCCESS;
}
