//
//  FeaturesFormater.hpp
//  SvmTrainModelCreator (App)
//
//  Created by 陳柏諺 on 2018/1/9.
//

#ifndef FEATURES_FORMATTER_HPP_INCLUDED
#define FEATURES_FORMATTER_HPP_INCLUDED

#include <string>
#include <sstream>


class FeaturesFormatter
{
public:
    template <typename Label, typename InputIterator>
    static std::string format(const Label& label, InputIterator first, InputIterator last)
    {
        std::ostringstream oss;
        oss << label;

        unsigned index = 1;
        for (; first != last; ++first) {
            oss << " " << index++ << ":" << *first;
        }

        return oss.str();
    }
};


#endif
