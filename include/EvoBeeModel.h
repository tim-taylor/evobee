#ifndef _EVOBEEMODEL_H
#define _EVOBEEMODEL_H

class EvoBeeModel {

public:
    EvoBeeModel();
    ~EvoBeeModel() {}

    void step();

private:
    int m_iStep;
};

#endif /* _EVOBEEMODEL_H */
