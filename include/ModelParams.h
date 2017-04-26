#ifndef _MODELPARAMS_H
#define _MODELPARAMS_H


/**
 Class to hold model parameters
*/
class ModelParams {

public:
    static void setEnvSize(int x, int y);
    static void setVisualisation(bool vis);

    static int getEnvSizeX() {return m_iEnvSizeX;}
    static int getEnvSizeY() {return m_iEnvSizeY;}
    static bool getVisualisation() {return m_bVisualisation;}

private:
    static bool m_bVisualisation;   ///< Use visualiation for this run?
    static int  m_iEnvSizeX;        ///< Environment size (num patches) in x direction
    static int  m_iEnvSizeY;        ///< Environment size (num patches) in y direction
};

#endif /* _MODELPARAMS_H */
