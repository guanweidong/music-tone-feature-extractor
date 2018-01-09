//
//  SvmFeaturesExtractor.cpp
//  SvmTrainModelCreator (App)
//
//  Created by 陳柏諺 on 2018/1/9.
//

#include "aubio/aubio.h"
#include "OnsetDetector.hpp"
#include "SvmFeaturesExtractor.hpp"
#include "Util.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>


#if !defined(PP_FFT_SIZE)
    #define PP_FFT_SIZE (1024)
    #define PP_NUM_MFCC_FILTERS (40)
    #define PP_NUM_MFCC_COEFFS (13)
#endif

#define PP_NUM_EXPECTED_TIMESTAMPS (5)


namespace detail
{
    SvmFeaturesExtractor::features_type getMFCC(
        juce::AudioFormatReader& reader,
        juce::int64 startSample
    )
    {
        aubio_fft_t *fft = new_aubio_fft(PP_FFT_SIZE);
        aubio_mfcc_t *mfcc = new_aubio_mfcc(
            PP_FFT_SIZE, PP_NUM_MFCC_FILTERS, PP_NUM_MFCC_COEFFS, static_cast<unsigned int>(reader.sampleRate)
        );
        fvec_t *tmpMfcc = new_fvec(PP_NUM_MFCC_COEFFS);
        fvec_t *audioBuffer = new_fvec(PP_FFT_SIZE);
        cvec_t *complexFFTOutput = new_cvec(PP_FFT_SIZE);


        juce:AudioSampleBuffer readBuffer(1, PP_FFT_SIZE);
        reader.read(&readBuffer, 0, PP_FFT_SIZE, startSample, true, false);

        std::copy(
            readBuffer.getReadPointer(0),
            readBuffer.getReadPointer(0) + readBuffer.getNumSamples(),
            audioBuffer->data
        );

        aubio_fft_do(fft, audioBuffer, complexFFTOutput);
        aubio_mfcc_do(mfcc, complexFFTOutput, tmpMfcc);

        SvmFeaturesExtractor::features_type coefficients(
            tmpMfcc->data, tmpMfcc->data + tmpMfcc->length
        );

        del_fvec(audioBuffer);
        del_cvec(complexFFTOutput);
        del_aubio_fft(fft);
        del_fvec(tmpMfcc);
        del_aubio_mfcc(mfcc);

        return std::move(coefficients);
    }

    SvmFeaturesExtractor::feature_type getMean(const SvmFeaturesExtractor::features_type& features)
    {
        return std::accumulate(
            std::begin(features), std::end(features), 0.0
        ) / features.size();
    }

    SvmFeaturesExtractor::feature_type getVariance(
        const SvmFeaturesExtractor::features_type& features,
        SvmFeaturesExtractor::feature_type mean
    )
    {
        assert(1 < features.size());

        SvmFeaturesExtractor::feature_type sum = 0.0;
        for (const auto feature : features) {
            sum += std::pow(feature - mean, 2);
        }

        return sum / (features.size() - 1);
    }
}

SvmFeaturesExtractor::features_type SvmFeaturesExtractor::extract(const juce::File& file)
{
    const auto timestamps = OnsetDetector::detect(file);
    assert(PP_NUM_EXPECTED_TIMESTAMPS < timestamps.size());

    features_type features;
    juce::ScopedPointer<juce::AudioFormatReader> reader = getReader(file);
    for (auto index = 0; index < PP_NUM_EXPECTED_TIMESTAMPS; ++index) {
        const auto startSample = timestamps[index];

        const auto sampleCountBeforeNextOnset = (timestamps[index + 1] - startSample);
        if (sampleCountBeforeNextOnset < PP_FFT_SIZE) {
            continue;
        }

        const auto coefs = detail::getMFCC(*reader, startSample);
        const auto mean = detail::getMean(coefs);
        const auto variance = detail::getVariance(coefs, mean);

        features.emplace_back(mean);
        features.emplace_back(variance);
    }

    return std::move(features);
}
