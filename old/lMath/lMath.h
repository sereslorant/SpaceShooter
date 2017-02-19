#ifndef L_MATH
#define L_MATH

#include <cmath>

#define PI 3.14159265359

#include "lmVector2D.h"

#include "lmGeometry2D.h"

/** \brief Statikus tömb
 * \tparam class T:             A tárolt objektum típusa
 * \tparam unsigned int SIZE:   A tömb hossza
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T,unsigned int SIZE>
struct lmStaticArray
{
private:
    T Data[SIZE];
public:

    /** \brief Tömbelem lekérdező operátor
     *
     * \param i unsigned int: az elem indexe
     * \return T: a tömb i-edik eleme
     *
     */
    T &operator[](unsigned int i)
    {
        return Data[i];
    }

    /** \brief Tömbelem lekérdező operátor - konstans tagfüggvény
     *
     * \param i unsigned int: az elem indexe
     * \return const T: a tömb i-edik eleme
     *
     */
    const T &operator[](unsigned int i) const
    {
        return Data[i];
    }

    /** \brief Default konstruktor
     *
     */
    lmStaticArray()
    {

    }

    /** \brief Copy konstruktor
     *
     * \param Array const lmStaticArray<T,SIZE> &: az átmásolandó tömb.
     *
     */
    lmStaticArray(const lmStaticArray<T,SIZE> &Array)
    {
        for(int i=0;i < SIZE;i++)
        {
            Data[i] = Array.Data[i];
        }
    }
};

/** \brief Signum függvény
 *
 * \param val T: az érték, aminek az előjelére kíváncsiak vagyunk.
 * \return -1, ha negtív, 1, ha pozitív, 0, ha 0.
 *
 * \author Random ember stackoverflow-n.
 *
 */
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/** \brief A paraméterként kapott szöget átkonvertálja fokból radiánba.
 *
 * \param Deg lmScalar: A kapott szög fokokban.
 * \return lmScalar - a radiánba konvertált szög.
 *
 * \author Seres Lóránt Gábor
 *
 */
lmScalar lmDegToRad(lmScalar Deg);

/** \brief A paraméterként kapott szöget átkonvertálja radiánból fokba.
 *
 * \param Rad lmScalar: A kapott szög radiánban.
 * \return lmScalar - a fokokba konvertált szög.
 *
 * \author Seres Lóránt Gábor
 *
 */
lmScalar lmRadToDeg(lmScalar Rad);

/** \brief A paraméterként kapott x értéket beleclampeli a Min - Max által körülhatárolt tartományba.
 *
 * \param Min lmScalar: Az intervallum minimuma.
 * \param Max lmScalar: Az intervallum maximuma.
 * \param x lmScalar:   A clampelendő érték.
 * \return lmScalar - a clampelt érték.
 *
 * \author Seres Lóránt Gábor
 *
 */
lmScalar lmClamp(lmScalar Min,lmScalar Max,lmScalar x);

/** \brief A három vektor által definiált háromszög területét számolja ki a Heron képlettel.
 *
 * \param A const lmVector3D &: A háromszög első csúcsa.
 * \param B const lmVector3D &: A háromszög második csúcsa.
 * \param C const lmVector3D &: A háromszög harmadik csúcsa.
 * \return lmScalar - a háromszög területe.
 *
 * \author Seres Lóránt Gábor
 *
 */
lmScalar lmHeronFormula(const lmVector2D &A,const lmVector2D &B,const lmVector2D &C);

#endif
