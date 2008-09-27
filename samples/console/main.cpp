
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

//------------------------------------------

#include "wx/file.h"
#include "../../lib/ec_defs.h"
#include "../../lib/ec_engine.h"

#define wxCONSOLEBUFFER        4096

//------------------------------------------

size_t ProcessPipe()
{
    //-- Initialization
    wxEcEngine *calc;
    wxString *data;
    char stdinBuffer[wxCONSOLEBUFFER+1];
    wxFile fInput, fOutput;
    size_t numRead, exitCode;

    //-- Computes
    fInput.Attach(wxFile::fd_stdin);
    numRead = fInput.Read(&stdinBuffer, wxCONSOLEBUFFER);
    if ((numRead>0) && (numRead<=wxCONSOLEBUFFER))
    {
        stdinBuffer[numRead] = (char) 0;
        data = new wxString(stdinBuffer);
        *data = data->BeforeFirst('\r');
        *data = data->BeforeFirst('\n');
            calc = new wxEcEngine();
            calc->UseDebug(false);
                calc->SetFormula(*data);
                *data = wxString::Format("%f", calc->Compute());
                exitCode = calc->GetLastError();

                //-- Output
                fOutput.Attach(wxFile::fd_stdout);
                fOutput.Write(*data);
                fOutput.Close();
            wxDELETE(calc);
        wxDELETE(data);
    }
    fInput.Detach();
    return exitCode;
}

size_t ProcessConsole()
{
    //-- Initialization
    char userBuffer[wxCONSOLEBUFFER+1];
    wxEcEngine *calc;
    wxArrayString calcDump;
    double result;
    size_t i;

    //-- Computes
    calc = new wxEcEngine();
    calc->UseDebug(true);

    printf("***********************************************************\n\n\
     %s %s - Sample: console\n\n\
     Online: %s\n\n\
***********************************************************\n\n\
   \"exit\" to quit, \"ans\" to call the last result.\n\n", wxECD_SOFTWARE, wxECD_VERSION, wxECD_URL);

RedoConsole:
        printf("> ");
        fflush(stdin);
        scanf("%s", &userBuffer);

        if ((userBuffer[0]!='e') || (userBuffer[1]!='x') || (userBuffer[2]!='i') || (userBuffer[3]!='t'))
        {
            calc->SetFormula(wxString(userBuffer));
            result = calc->Compute();
            calc->SetConstant(wxT("ans"), result);


            #ifdef wxECM_USELOG
                printf("\n");
                calcDump = calc->GetLog();
                for (i=0 ; i<calcDump.GetCount() ; i++)
                    printf("     | %s\n", calcDump.Item(i).Trim(false).Trim(true).c_str());
                printf("     |\n     | %s\n\nResult = %f\n\n", calc->TranslateError(calc->GetLastError()).c_str(), result);
            #else
                printf("%f\n\n", result);
            #endif
            goto RedoConsole;
        }

    wxDELETE(calc);
    return 0;
}

int main(int argc, char* argv[])
{
    return ProcessConsole();

    //To use as a pipe interpreter, replace with:     return ProcessPipe();
}
