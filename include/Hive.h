/**
 * @file
 *
 * Declaration of the Hive template class
 */

#ifndef _HIVE_H
#define _HIVE_H

#include <vector>
#include <sstream>
#include <type_traits>
#include "HiveConfig.h"
#include "Pollinator.h"
#include "AbstractHive.h"

/**
 * The Hive template class
 * This defines a concrete Hive class for a specified type of pollinator.
 * Hence, the template class type P must be a subclass of the Pollinator class.
 * All Hives are derived from the base class AbstractHive and inherit some
 * functionality and interface from that.
 */
template<typename P>
class Hive : public AbstractHive {

public:
    /*
     * Constructor for a concrete Hive of a specified type of Pollinator
     * The Pollinator class P must be derived from the Pollinator class
     * NB You probably don't want to call this constructor directly;
     * Try the AbstractHive::makeHive() factory method instead
     */
    Hive(Environment* pEnv, const HiveConfig& hc) :
        AbstractHive(pEnv, hc)
    {
        static_assert( std::is_base_of<Pollinator, P>(), "Template class type of Hive must be derived from Pollinator class" );

        m_Pollinators.reserve(hc.num);
        for (int i = 0; i < hc.num; ++i)
        {
            m_Pollinators.push_back( P((AbstractHive*)this) );
        }
    }

    ~Hive() {}

    //std::vector<P>& getPollinators() {return m_Pollinators;}

    /**
     *
     */
    virtual int getNumPollinators() const {return m_Pollinators.size();}
    
    /**
     *
     */
    virtual Pollinator* getPollinator(size_t idx)
    {
        if (idx >= m_Pollinators.size())
        {
            std::stringstream msg;
            msg << "Request for non-existent Pollinator number " << idx;
            throw std::runtime_error(msg.str());
        }
        return static_cast<Pollinator*>( &m_Pollinators[idx] );
    }

private:
    std::vector<P> m_Pollinators;
};

#endif /* _HIVE_H */
