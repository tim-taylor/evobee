/**
 * @file
 *
 * Declaration of the ModelParams class
 */

#ifndef _MODELPARAMS_H
#define _MODELPARAMS_H

/**
 * The ModelParams Class holds a record of all model parameters. 
 *
 * All methods and variables of this class are static and can therefore be accessed
 * by any other object in the system.
 */
class ModelParams {

public:
    static void setEnvSize(int x, int y);
    static void setVisualisation(bool vis);
    static void setMaxScreenFrac(float f);
    static void setMaxScreenFracW(float fw);
    static void setMaxScreenFracH(float fh);
    static void setVisUpdatePeriod(int p);

    static int   getEnvSizeX() {return m_iEnvSizeX;}
    static int   getEnvSizeY() {return m_iEnvSizeY;}
    static bool  getVisualisation() {return m_bVisualisation;}
    static float getMaxScreenFracW() {return m_fMaxScreenFracW;}
    static float getMaxScreenFracH() {return m_fMaxScreenFracH;}
    static int   getVisUpdatePeriod() {return m_iVisUpdatePeriod;}

private:
    static bool  m_bVisualisation;   ///< Use visualiation for this run?
    static int   m_iEnvSizeX;        ///< Environment size (num patches) in x direction
    static int   m_iEnvSizeY;        ///< Environment size (num patches) in y direction
    static float m_fMaxScreenFracW;  ///< Max fraction of screen size for vis window width
    static float m_fMaxScreenFracH;  ///< Max fraction of screen size for vis window height
    static int   m_iVisUpdatePeriod; ///< Number of model steps between each update of visualisation
};

#endif /* _MODELPARAMS_H */
