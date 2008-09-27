
/*  wxEcMath - version 0.6 beta
 *  Copyright (C) 2008, http://sourceforge.net/projects/wxecmath/
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

#ifndef plotdemo_h
#define plotdemo_h

//------------------------------------------

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/checklst.h"
#include "wx/fontdlg.h"
#include "../../lib/ec_plot.h"

//------------------------------------------

class wxPlotFrame : public wxFrame
{
    DECLARE_EVENT_TABLE()
    private:
        enum
        {
            ID_EDIT = 1000,
            ID_ADD,
            ID_USEDEMO,
            ID_FUNCLIST,
            ID_DELETE,
            ID_TANGENT,
            ID_DX,
            ID_AXISFONT,
            ID_DEFAULTGRID,
            ID_APPLY,
            ID_FITAXIS,
            ID_ORTHO,
            ID_RESET,
            ID_SHOWGRID,
            ID_SHOWAXIS,
            ID_SHOWRETICULE,
            ID_POLARGRID,
            ID_UNZOOM
        };

        wxEcPlot *Plot;

        wxTextCtrl *editExpression;
        wxCheckBox *checkPolar;
        wxButton *buttonAdd;
        wxButton *buttonUseDemo;
        wxStaticText *editAllDefFunc;
        wxCheckListBox *listFunction;
        wxButton *buttonDelete;
        wxButton *buttonTangent;
        wxButton *buttonDX;
        wxStaticText *labelX;
        wxTextCtrl *editXMin;
        wxTextCtrl *editXMax;
        wxStaticText *labelY;
        wxTextCtrl *editYMin;
        wxTextCtrl *editYMax;
        wxStaticText *labelFormat;
        wxTextCtrl *editFormat;
        wxButton *buttonAxisFont;
        wxButton *buttonDefaultGrid;
        wxButton *buttonApplyGrid;
        wxButton *buttonFitAxis;
        wxButton *buttonReset;
        wxButton *buttonOrtho;
        wxButton *buttonUnzoom;
        wxStaticText *labelShow;
        wxCheckBox *checkShowGrid;
        wxCheckBox *checkShowAxis;
        wxCheckBox *checkReticule;
        wxCheckBox *checkPolarGrid;

        void DoRefreshCurveList();

    public:
        wxPlotFrame(wxWindow *parent);
        ~wxPlotFrame();

        void OnAddClick(wxCommandEvent& event);
        void OnAddDemoClick(wxCommandEvent& event);
        void OnFuncListCheck(wxCommandEvent& event);
        void OnDeleteClick(wxCommandEvent& event);
        void OnTangentClick(wxCommandEvent& event);
        void OnDerivateClick(wxCommandEvent& event);
        void OnAxisFontClick(wxCommandEvent& event);
        void OnDefaultGridClick(wxCommandEvent& event);
        void OnGridApplyClick(wxCommandEvent& event);
        void OnFitAxisClick(wxCommandEvent& event);
        void OnResetClick(wxCommandEvent& event);
        void OnOrthonormalizeClick(wxCommandEvent& event);
        void OnUnzoomClick(wxCommandEvent& event);
        void OnShowGridClick(wxCommandEvent& event);
        void OnShowAxisClick(wxCommandEvent& event);
        void OnShowReticuleClick(wxCommandEvent& event);
        void OnPolarGridClick(wxCommandEvent& event);
};

#endif
