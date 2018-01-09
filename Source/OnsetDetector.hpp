//
//  OnsetDetector.hpp
//  SvmTrainModelCreator (App)
//
//  Created by 陳柏諺 on 2018/1/9.
//

#ifndef ONSET_DETECTOR_HPP_INCLUDED
#define ONSET_DETECTOR_HPP_INCLUDED

#include "JuceHeader.h"
#include <deque>


class OnsetDetector
{
public:
    using timestamp_type = juce::int64;
    using timestamps_type = std::deque<timestamp_type>;

public:
    static timestamps_type detect(const juce::File& file);
};

#endif /* OnsetDetector_hpp */
