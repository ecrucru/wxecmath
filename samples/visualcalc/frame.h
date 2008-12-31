
/*  wxEcMath - version 0.6.1
 *  Copyright (C) 2008-2009, http://sourceforge.net/projects/wxecmath/
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef frame_h
#define frame_h

//------------------------------------------

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//------------------------------------------

#define  wxECM_USEDEBUG

#include "wx/aboutdlg.h"
#include "wx/clipbrd.h"
#include "../../lib/ec_defs.h"
#include "../../lib/ec_engine.h"
#include "../../imgs/icon.xpm"
#include "debugwin.h"

#define     MEMORY_NAME     wxT("memory")
#define     STATUS_INFO     1
#define     GRID_ROWS       4
#define     GRID_COLS       9

//------------------------------------------

class wxCalcFrame: public wxFrame
{
    DECLARE_EVENT_TABLE()
    private:

        enum {
            MENU_ABOUT = wxID_ABOUT,
            MENU_QUIT = wxID_EXIT,
            ID_BASE = 1024,
            /*pad*/         ID_0, ID_1, ID_2, ID_3, ID_4, ID_5, ID_6, ID_7, ID_8, ID_9,
            /*checkbox*/    ID_ARCBOX, ID_HYPERBOLIC,
            /*combobox*/    ID_CONSTS, ID_FUNCLIST, ID_TRIGO,
            /*edit*/        ID_FORMULA,
            /*button*/      ID_BRACKETCLOSE, ID_BRACKETOPEN, ID_CALL, ID_CLR, ID_COMMA, ID_COS, ID_DEF, ID_DIV,
                            ID_E, ID_EGAL, ID_EXP, ID_LN, ID_LOG, ID_MENU, ID_MINUS, ID_MP, ID_MULT, ID_PERCENT,
                            ID_PI, ID_PLUS, ID_PLUSMINUS, ID_POW10, ID_POWER, ID_REVERSE, ID_SET, ID_SIN, ID_SQ2,
                            ID_SQRT, ID_TAN, 
            /*menu*/        MENU_COPYRESULT, MENU_COPYFORMULA, MENU_PASTE, MENU_DEBUG, MENU_SIMPLIFY, MENU_ISVALID,
                            MENU_CLOSEBRACKETS, MENU_RESETCONST, MENU_COMPACT
        };

        wxEcEngine    *Calc;
        double        Memory;

        wxComboBox    *ComboFunctions, *ComboConstants, *ComboTrigoMode;
        wxCheckBox    *CheckArc, *CheckHyperbolic;
        wxTextCtrl    *EditFormula;
        wxMenuBar    *MenuBar;
        wxMenu        *MenuEdit, *MenuTools, *MenuQuestionMark;
        wxMenuItem    *MenuCopyResult, *MenuCopyFormula, *MenuPaste, *MenuDebug, *MenuQuit, *MenuSimplify,
                    *MenuValidate, *MenuCloseBrackets, *MenuResetConst, *MenuCompact, *MenuAbout;
        wxStatusBar    *StatusBar;
        wxButton    *ButtonClr, *ButtonEgal, *ButtonCall, *ButtonSin, *ButtonExp, *ButtonLn, *ButtonBracketOpen,
                    *Button7, *Button8, *Button9, *ButtonDiv, *ButtonSet, *ButtonCos, *ButtonPow10, *ButtonLog,
                    *ButtonBracketClose, *Button4, *Button5, *Button6, *ButtonMult, *ButtonMP, *ButtonTan,
                    *ButtonPower, *ButtonSq2, *ButtonSqrt, *Button1, *Button2, *Button3, *ButtonPlus, *ButtonDef,
                    *ButtonPi, *ButtonPercent, *ButtonE, *ButtonReverse, *Button0, *ButtonComma, *ButtonPlusMinus,
                    *ButtonMinus;

        wxBoxSizer    *VSizer;
        wxGridSizer    *GridSizer;
        wxBoxSizer    *ComboSizer, *FormulaSizer, *FormulaSizerC1;

        void ApplyBracket(wxChar Car);
        void ApplyChar(wxChar Car);
        void ApplyFunction(wxString SelFunc);
        void ApplyOperator(wxChar Car);
        void ApplyString(wxString Str);
        wxString GetCurrentFormula();
        bool IsArcTrigo();
        bool IsHyperbolic();
        wxString TranslateTrigoFunction(wxString Function);

        bool CopyToClipboard(wxString Content);

    public:
        /*constructor*/ wxCalcFrame(const bool DoShow, bool RequireCompact = false);
        /*destructor*/ ~wxCalcFrame();
        void Compact(bool DoCompact);
        void ShowMessage(wxString Message, int IconType = wxICON_INFORMATION);
        void UpdateUI();

        void OnMenuCopyResultClick(wxCommandEvent& event);
        void OnMenuCopyFormulaClick(wxCommandEvent& event);
        void OnMenuPasteClick(wxCommandEvent& event);
        void OnMenuDebugClick(wxCommandEvent& event);
        void OnMenuQuitClick(wxCommandEvent& event);
        void OnMenuSimplifyClick(wxCommandEvent& event);
        void OnMenuIsValidClick(wxCommandEvent& event);
        void OnMenuCloseBracketsClick(wxCommandEvent& event);
        void OnMenuResetConstsClick(wxCommandEvent& event);
        void OnMenuCompactClick(wxCommandEvent& event);
        void OnMenuAboutClick(wxCommandEvent& event);
        void OnCloseEvent(wxCloseEvent& event);
        void OnFunctionSelected(wxCommandEvent& event);
        void OnConstantSelected(wxCommandEvent& event);
        void OnTrigoSelected(wxCommandEvent& event);
        void OnCheckBoxClick(wxCommandEvent& event);

        void OnClearClick(wxCommandEvent& event);
        void OnEgalClick(wxCommandEvent& event);
        void OnCallClick(wxCommandEvent& event);
        void OnSinClick(wxCommandEvent& event);
        void OnExpClick(wxCommandEvent& event);
        void OnLnClick(wxCommandEvent& event);
        void OnBracketOpenClick(wxCommandEvent& event);
        void On7Click(wxCommandEvent& event);
        void On8Click(wxCommandEvent& event);
        void On9Click(wxCommandEvent& event);
        void OnDivClick(wxCommandEvent& event);
        void OnSetClick(wxCommandEvent& event);
        void OnCosClick(wxCommandEvent& event);
        void OnPow10Click(wxCommandEvent& event);
        void OnLogClick(wxCommandEvent& event);
        void OnBracketCloseClick(wxCommandEvent& event);
        void On4Click(wxCommandEvent& event);
        void On5Click(wxCommandEvent& event);
        void On6Click(wxCommandEvent& event);
        void OnMultClick(wxCommandEvent& event);
        void OnMemoryPlusClick(wxCommandEvent& event);
        void OnTanClick(wxCommandEvent& event);
        void OnPowerClick(wxCommandEvent& event);
        void OnSq2Click(wxCommandEvent& event);
        void OnSqrtClick(wxCommandEvent& event);
        void On1Click(wxCommandEvent& event);
        void On2Click(wxCommandEvent& event);
        void On3Click(wxCommandEvent& event);
        void OnPlusClick(wxCommandEvent& event);
        void OnDefClick(wxCommandEvent& event);
        void OnPiClick(wxCommandEvent& event);
        void OnPercentClick(wxCommandEvent& event);
        void OnEClick(wxCommandEvent& event);
        void OnReverseClick(wxCommandEvent& event);
        void On0Click(wxCommandEvent& event);
        void OnCommaClick(wxCommandEvent& event);
        void OnPlusMinusClick(wxCommandEvent& event);
        void OnMinusClick(wxCommandEvent& event);
};

#endif
