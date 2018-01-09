//
//  OnsetDetector.cpp
//  SvmTrainModelCreator (App)
//
//  Created by 陳柏諺 on 2018/1/9.
//

#include "OnsetDetector.hpp"
#include "Util.hpp"
#include "aubio/aubio.h"

#if !defined(PP_WINDOW_SIZE)
    #define PP_WINDOW_SIZE (1024)
    #define PP_BLOCK_SIZE_TO_READ (PP_WINDOW_SIZE/4)
#endif


OnsetDetector::timestamps_type OnsetDetector::detect(const juce::File& file)
{
    const auto sampleRate = getSampleRate(file);

    timestamps_type timestamps;

    char* filePathCopy = strdup(file.getFullPathName().toStdString().c_str());
    aubio_source_t * source = new_aubio_source(
        filePathCopy, sampleRate, PP_BLOCK_SIZE_TO_READ
    );
    if (source == nullptr) {
        std::free(filePathCopy);
        return timestamps;
    }

    fvec_t * input = new_fvec(PP_BLOCK_SIZE_TO_READ); // input audio buffer
    fvec_t * output = new_fvec(2); // output position

    // create tempo object
    char method[] = "default";
    aubio_onset_t * onset = new_aubio_onset(
        method, PP_WINDOW_SIZE, PP_BLOCK_SIZE_TO_READ, sampleRate
    );

    uint_t read = 0;
    do {
        // put some fresh data in input vector
        aubio_source_do(source, input, &read);
        aubio_onset_do(onset, input, output);

        // do something with the beats
        if (output->data[0] != 0) {
            timestamps.emplace_back(aubio_onset_get_last(onset));
        }
    } while (read == PP_BLOCK_SIZE_TO_READ);

    del_aubio_onset(onset);

    del_fvec(input);
    del_fvec(output);

    std::free(filePathCopy);
    del_aubio_source(source);

    return timestamps;
}
