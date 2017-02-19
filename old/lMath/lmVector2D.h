#ifndef LM_VECTOR_2D
#define LM_VECTOR_2D

#include <cmath>

#include "lmTypedef.h"

/** \brief Egy 2D-s vektort definiáló osztály
 *
 * \author Seres Lóránt Gábor
 *
 */
class lmVector2D
{
public:

    static lmVector2D NULL_VECTOR; /**< Nullvektor */

    lmScalar X; /**< X koordináta */
    lmScalar Y; /**< Y koordináta */

    /** \brief Konstruktor
     *
     * \param x lmScalar:   Az új X koordináta
     * \param y lmScalar:   Az új Y koordináta
     *
     */
    lmVector2D(lmScalar x,lmScalar y);

    /** \brief Default konstruktor
     *
     * Nullvektornak inicializálja a vektort.
     *
     */
    lmVector2D();

    /** \brief Destruktor - nem csinál semmit
     *
     */
    ~lmVector2D();

    /** \brief Visszatér a vektor hosszának négyzetével.
     *
     * \return lmScalar - a vektor hosszának a négyzete
     *
     */
    lmScalar LengthSquared() const;

	void Normalize()
	{
		lmScalar Len = LengthSquared();

		if(Len > 1e-6)
		{
			Len = std::sqrt(Len);

			X /= Len;
			Y /= Len;
		}
	}

    //Operátorok

    /** \brief Két vektor összeadása
     *
     * \param U const lmVector2D &: A vektorhoz hozzáadandó vektor.
     * \return lmVector2D - az összeadás eredménye
     *
     */
    lmVector2D operator+(const lmVector2D &U) const;

    /** \brief Két vektor kivonása
     *
     * \param U const lmVector2D &: A vektorból kivonandó vektor.
     * \return lmVector2D - a kivonás eredménye
     *
     */
    lmVector2D operator-(const lmVector2D &U) const;

    /** \brief Vektor skalárral való szorzása jobbról
     *
     * \param l lmScalar: A skalár.
     * \return lmVector2D - a szorzás eredménye
     *
     */
    lmVector2D operator*(lmScalar l) const;

    /** \brief Vektorhoz másik vektor hozzáadása
     *
     * \param U const lmVector2D &: A vektorhoz hozzáadandó vektor.
     * \return const lmVector2D & - *this
     *
     */
    const lmVector2D &operator+=(const lmVector2D &U);

    /** \brief Vektorból másik vektor kivonása
     *
     * \param U const lmVector2D &: A vektorból kivonandó vektor.
     * \return const lmVector2D & - *this
     *
     */
    const lmVector2D &operator-=(const lmVector2D &U);

    /** \brief Vektor beszorzása skalárral
     *
     * \param l lmScalar: A skalár.
     * \return const lmVector2D & - *this
     *
     */
    const lmVector2D &operator*=(lmScalar l);
};

/** \brief Két vektor skaláris szorzata
 *
 * \param U const lmVector2D &: az egyik vektor
 * \param V const lmVector2D &: a másik vektor
 * \return lmScalar - a művelet eredménye
 *
 * \author Seres Lóránt Gábor
 *
 */
lmScalar lmDot(const lmVector2D &U,const lmVector2D &V);

/** \brief Vektor skalárral való szorzása balról
 *
 * \param l lmScalar:            a skalár
 * \param V const lmVector2D &:  a vektor
 * \return lmVector2D - a művelet eredménye
 *
 * \author Seres Lóránt Gábor
 *
 */
lmVector2D operator *(lmScalar l,const lmVector2D &V);
#endif
