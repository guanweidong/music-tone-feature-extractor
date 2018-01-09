//
//  SvmFeaturesExtractor.hpp
//  SvmTrainModelCreator (App)
//
//  Created by 陳柏諺 on 2018/1/9.
//

#ifndef SVM_FEATURES_EXTRACTOR_HPP_INCLUDED
#define SVM_FEATURES_EXTRACTOR_HPP_INCLUDED

#include "JuceHeader.h"

#include <deque>

class SvmFeaturesExtractor
{
public:
    using feature_type = double;
    using features_type = std::deque<feature_type>;

public:
    static features_type extract(const juce::File& file);
};

#endif
