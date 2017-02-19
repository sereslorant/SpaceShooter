#ifndef L_INPUT_H
#define L_INPUT_H


/** \brief A billentyűzet- és kontrollerinputot összefogó absztrakciós réteg. Az lInput class dolga,
 * hogy rajta keresztül a program elkérhesse, hogy milyen gomb van lenyomva, az egeret megmozgatták-e, stb.
 *
 * \todo Controller input még nincs, és a billentyűzet lekérdezésére valami civilizáltabb módszert kéne találni.
 * Csúnya dolog ennyi bool változót létrehozni minden billentyűre. :P Alapos átszabáson fog keresztülmenni,
 * szal sok értelme nincs szétdokumentálni.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lInput
{
public:

	class liController
	{
	public:

		enum L_HAT_STATE
		{
			L_CNTRL_NORMAL,
			L_CNTRL_UP,
			L_CNTRL_DOWN,
			L_CNTRL_LEFT,
			L_CNTRL_RIGHT,
			L_CNTRL_UP_LEFT,
			L_CNTRL_UP_RIGHT,
			L_CNTRL_DOWN_LEFT,
			L_CNTRL_DOWN_RIGHT,

			L_CNTRL_UNHANDLED,
			L_CNTRL_HAT_NOT_FOUND,
		};

		class liHat
		{
		public:
			virtual L_HAT_STATE GetHatState() = 0;

			liHat(){}
			virtual ~liHat(){}
		};

		class liAxis
		{
		public:
			virtual unsigned int GetDeadzone() = 0;
			virtual void SetDeadzone(unsigned int value) = 0;

			virtual signed int GetValue() = 0;
			virtual signed int GetMaxValue() = 0;

			liAxis(){}
			virtual ~liAxis(){}
		};

		class liButton
		{
		public:
			virtual bool IsPressed() = 0;

			liButton(){}
			virtual ~liButton(){}
		};

		virtual unsigned int GetNumHats() = 0;
		virtual unsigned int GetNumAxes() = 0;
		virtual unsigned int GetNumButtons() = 0;

		virtual liHat	 *GetHat(unsigned int i) = 0;
		virtual liAxis	 *GetAxis(unsigned int i) = 0;
		virtual liButton *GetButton(unsigned int i) = 0;

		liController(){}
		virtual ~liController(){}
	};

    /** \brief A billentyűzet állapotát tartalmazó adatstruktúra.
     *
     */
    struct lKeyboardState
    {
        //Minden billentyűhöz, amit megfigyelünk, van egy bool változó, ami megadja, hogy le van-e nyomva.
        bool Escape;

        bool Enter;
        bool LShift;
        bool RShift;
        bool LCtrl;
        bool RCtrl;
        bool Space;

        bool Up;
        bool Down;
        bool Left;
        bool Right;

        bool W;
        bool A;
        bool S;
        bool D;

        lKeyboardState()
            :Escape(false),Enter(false),LShift(false),RShift(false),LCtrl(false),RCtrl(false),Space(false),
            Up(false),Down(false),Left(false),Right(false),
             W(false),A(false),S(false),D(false)
        {};
        ~lKeyboardState(){};
    };

    /** \brief Az egér állapotát (pozíció, egérgomblenyomás) tartalmazó adatstruktúra.
     *
     */
	struct lMouseState
	{
		int X; /**< Az egér X pozíciója */
		int Y; /**< Az egér Y pozíciója */

		//Minden egérgombhoz eltároljuk, hogy le van-e nyomva, vagy sem.
		bool LmbPressed;
		bool MmbPressed;
		bool RmbPressed;

		lMouseState()
            :X(0),Y(0),LmbPressed(false),MmbPressed(false),RmbPressed(false)
        {};
		~lMouseState(){};

	};

private:
    bool Quit; /**< Itt tároljuk, hogy rákattintottak-e az ablak sarkában az X-re. */

    lKeyboardState  KeyboardState;
	lMouseState     MouseState;

public:
    void SetQuit(bool Val){Quit = Val;}
    void SetEscape(bool Val);

    void SetEnter(bool Val);
    void SetLShift(bool Val);
    void SetRShift(bool Val);
    void SetLCtrl(bool Val) {KeyboardState.LCtrl = Val;};
    void SetRCtrl(bool Val) {KeyboardState.RCtrl = Val;};
    void SetSpace(bool Val);

    void SetUp(bool Val);
    void SetDown(bool Val);
    void SetLeft(bool Val);
    void SetRight(bool Val);

    void SetW(bool Val){KeyboardState.W = Val;};
    void SetA(bool Val){KeyboardState.A = Val;};
    void SetS(bool Val){KeyboardState.S = Val;};
    void SetD(bool Val){KeyboardState.D = Val;};

    void SetMX(int x);
    void SetMY(int y);

    void SetLmb(bool lmb);
    void SetMmb(bool mmb);
    void SetRmb(bool rmb);


    bool GetQuit() const{return Quit;}
    bool GetEscape() const;

    bool GetEnter() const;
    bool GetLShift() const;
    bool GetRShift() const;
    bool GetLCtrl() const{return KeyboardState.LCtrl;};
    bool GetRCtrl() const{return KeyboardState.RCtrl;};
    bool GetSpace() const;

    bool GetUp() const;
    bool GetDown() const;
    bool GetLeft() const;
    bool GetRight() const;

    bool GetW(){return KeyboardState.W;};
    bool GetA(){return KeyboardState.A;};
    bool GetS(){return KeyboardState.S;};
    bool GetD(){return KeyboardState.D;};

    int GetX() const;
    int GetY() const;

    bool GetLmb() const;
    bool GetMmb() const;
    bool GetRmb() const;

    lMouseState       &GetMouseState();
    const lMouseState &GetMouseState() const;

    lKeyboardState       &GetKeyboardState();
    const lKeyboardState &GetKeyboardState() const;

    lInput():Quit(false){}
    virtual ~lInput(){}

    /** \brief Ez az a függvény, ami lehívja a platformspecifikus rendszerhívásokat, amelyekkel updatelni lehet a billentyűzet és az egér állapotát.
     *
     * \return void
     *
     */
    virtual void CheckInput() = 0;

};

#endif
