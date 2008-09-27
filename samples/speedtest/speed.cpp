
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

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "../../lib/ec_defs.h"
#include "../../lib/ec_engine.h"

#define SPEED_NUM (long) 150000


class wxInlineDemoApp: public wxApp
{
    public:
        virtual bool OnInit();
};

    DECLARE_APP(wxInlineDemoApp)
    IMPLEMENT_APP(wxInlineDemoApp)

bool wxInlineDemoApp::OnInit()
{
    wxApp::OnInit();

    //-- Initializes
    long i, start, end;
    double speed;
    wxEcEngine calculator;
    calculator.SetFormula(wxT("(1+sqrt(5))/2-2*cos(2*pi/5)"));    //= 1

    //-- Computes
    start = wxGetLocalTime();
    for (i=0 ; i<SPEED_NUM ; i++)
        calculator.Compute();
    end = wxGetLocalTime();

    //-- Displays the result
    if (end != start)
    {
        speed = SPEED_NUM / (end-start);
        wxMessageDialog(NULL, wxString::Format(wxT("%.0f per second"), speed), wxT("Speed"), wxOK).ShowModal();
    }
    return false;
}
