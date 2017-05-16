/**
 * @file
 *
 * Declaration of the Hive template class
 */

#ifndef _HIVE_H
#define _HIVE_H

#include <vector>
#include <type_traits>
#include "HiveConfig.h"
#include "AbstractHive.h"

/**
 * The HoneyBeeHive class ...
 */
template<typename H>
class Hive : public AbstractHive {

public:
    //HoneyBeeHive();
    Hive(const HiveConfig& hc) {

        static_assert( std::is_base_of<AbstractHive, Hive<H>>(), "" );
        ///@todo
        m_Pollinators.reserve(hc.num);
        for (int i = 0; i < hc.num; ++i)
        {
            m_Pollinators.push_back( H() );
        }
    }

    ~Hive() {}

private:
    std::vector<H> m_Pollinators;
};

#endif /* _HIVE_H */
