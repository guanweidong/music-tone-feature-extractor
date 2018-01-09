//
//  Util.hpp
//  SvmTrainModelCreator (App)
//
//  Created by 陳柏諺 on 2018/1/9.
//

#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED

#include "JuceHeader.h"

juce::File getAbsolutePath(const juce::String& path);
juce::AudioFormatReader* getReader(const juce::File& file);
unsigned getSampleRate(const juce::File& file);

#endif /* Util_hpp */
