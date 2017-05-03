#ifndef _MODELPARAMS_H
#define _MODELPARAMS_H


/**
 Class to hold model parameters
*/
class ModelParams {

public:
    static void setEnvSize(int x, int y);
    static void setVisualisation(bool vis);
    static void setMaxScreenFrac(float f);
    static void setMaxScreenFracW(float fw);
    static void setMaxScreenFracH(float fh);

    static int   getEnvSizeX() {return m_iEnvSizeX;}
    static int   getEnvSizeY() {return m_iEnvSizeY;}
    static bool  getVisualisation() {return m_bVisualisation;}
    static float getMaxScreenFracW() {return m_fMaxScreenFracW;}
    static float getMaxScreenFracH() {return m_fMaxScreenFracH;}

private:
    static bool  m_bVisualisation;   ///< Use visualiation for this run?
    static int   m_iEnvSizeX;        ///< Environment size (num patches) in x direction
    static int   m_iEnvSizeY;        ///< Environment size (num patches) in y direction
    static float m_fMaxScreenFracW;  ///< Max fraction of screen size for vis window width
    static float m_fMaxScreenFracH;  ///< Max fraction of screen size for vis window height
};

#endif /* _MODELPARAMS_H */
