//
//  Util.cpp
//  SvmTrainModelCreator (App)
//
//  Created by 陳柏諺 on 2018/1/9.
//

#include "Util.hpp"


juce::File getAbsolutePath(const juce::String& path)
{
    if (juce::File::isAbsolutePath(path)) {
        return path;
    }

    return juce::File::getCurrentWorkingDirectory().getChildFile(path);
}

juce::AudioFormatReader* getReader(const juce::File& file)
{
    juce::AudioFormatManager manager;
    manager.registerBasicFormats();

    return manager.createReaderFor(file);
}

unsigned getSampleRate(const juce::File& file)
{
    using reader_holder_type = juce::ScopedPointer<juce::AudioFormatReader>;

    return static_cast<unsigned>(
        std::floor(reader_holder_type(getReader(file))->sampleRate)
    );
}
