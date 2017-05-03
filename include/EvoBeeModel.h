/**
 * @file
 *
 * Declaration of the EvoBeeModel class
 */

#ifndef _EVOBEEMODEL_H
#define _EVOBEEMODEL_H

/**
 * The EvoBeeModel class ...
 */
class EvoBeeModel {

public:
    EvoBeeModel();
    ~EvoBeeModel() {}

    void step();

private:
    int m_iStep;
};

#endif /* _EVOBEEMODEL_H */
