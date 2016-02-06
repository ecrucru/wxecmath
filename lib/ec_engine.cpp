
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

#include "ec_engine.h"


//-------------- CONSTRUCTOR ---------------

wxEcEngine::wxEcEngine()
{
    Reset(true, true);
    m_trigomode = wxECA_RADIAN;
    m_internallog = false;
}

wxEcEngine::~wxEcEngine()
{
}

wxString wxEcEngine::GetFormula()
{
    return m_formula;
}

wxString wxEcEngine::GetIndicator()
{
    if (m_indicator.Len() != 0)
        return m_indicator;
    else
        return wxEmptyString;
}

int wxEcEngine::GetLastError()
{
    return m_errorcode;
}

double wxEcEngine::GetLastResult()
{
    return m_lastresult;
}

#ifdef wxECM_USELOG
    void wxEcEngine::LogAction(wxString message, bool verbose)
    {
        if (!m_internallog)
            return;
        m_debugoutput.Add(message);
        if (verbose)
            printf("%s\n", message.c_str());
    }

    wxArrayString wxEcEngine::GetLog()
    {
        return m_debugoutput;
    }
#endif

void wxEcEngine::Reset(bool formulaToo, bool constantsToo)
{
    long i;
    #ifdef wxECM_USELOG
        m_debugoutput.Clear();
    #endif
    m_errorcode = wxECE_NOERROR;
    m_lastresult = 0;
    m_indicator = wxEmptyString;
    m_offset = 0;
    for (i=0 ; i<wxECD_STACKMAX ; i++)
    {
        m_pool[i] = 0;
        m_antecomp[i] = 0;
    }
    if (formulaToo)
        m_formula = wxEmptyString;
    if (constantsToo)
        ResetConstants();
}

wxString wxEcEngine::TranslateError(int code)
{
    switch (code)
    {
        case wxECE_NOERROR:         return wxString::Format(wxT("%d - No error found"), code);
        case wxECE_SYNTAX:          return wxString::Format(wxT("%d - Syntax error"), code);
        case wxECE_UNDEFCONSTANT:   return wxString::Format(wxT("%d - Undefined constant"), code);
        case wxECE_MEMORYEXCEEDED:  return wxString::Format(wxT("%d - No memory to allocate"), code);
        case wxECE_DIVBYZERO:       return wxString::Format(wxT("%d - Division by zero"), code);
        case wxECE_DOMAINERROR:     return wxString::Format(wxT("%d - Function not defined on the domain"), code);
        case wxECE_UNKNOWNFUNC:     return wxString::Format(wxT("%d - Unknown function"), code);
        case wxECE_STACKERROR:      return wxString::Format(wxT("%d - Stack overflow or desynchronized"), code);
        case wxECE_UNCOMPUTABLE:    return wxString::Format(wxT("%d - Expression cannot be evaluated"), code);
        default:                    return wxString::Format(wxT("%d - Unexpected error"), code);
    }
}


//--------------- CONSTANTS ----------------

bool wxEcEngine::DeleteConstant(wxString name)
{
    long index = FindConstant(name);
    if (index == wxNOT_FOUND)
        return false;
    else
    {
        m_constants[index].Name = wxEmptyString;
        m_constants[index].Value = 0;
        return true;
    }
}

long wxEcEngine::FindConstant(wxString name)
{
    long i;
    name = name.Lower().Trim(false).Trim(true);
    for (i=0 ; i<wxECD_STACKMAX ; i++)
        if (m_constants[i].Name == name)
            return i;
    return wxNOT_FOUND;
}

bool wxEcEngine::GetConstant(wxString expression, double *destination)
{
    long index = FindConstant(expression);
    if (index != wxNOT_FOUND)
    {
        *destination = m_constants[index].Value;
        return true;
    } else
        if (expression.ToDouble(destination))
            return true;
        else {
            m_indicator = expression;
            m_errorcode = wxECE_UNDEFCONSTANT;
            return false;
        }
}

int wxEcEngine::GetConstantCount()
{
    int i, result = 0;
    for (i=0 ; i<wxECD_STACKMAX ; i++)
        if (m_constants[i].Name.Len() > 0)
            result++;
    return result;
}

bool wxEcEngine::ListAllConstants(wxControlWithItems *destination)
{
    if (destination == NULL)
        return false;
    long i;
    destination->Clear();
    for (i=0 ; i<wxECD_STACKMAX ; i++)
        if (m_constants[i].Name.Len() > 0)
            destination->Append(wxString::Format(wxT("%s = %f"), m_constants[i].Name.c_str(), m_constants[i].Value));
    return true;
}

void wxEcEngine::ResetConstants()
{
    long i;
    for (i=0 ; i<wxECD_STACKMAX ; i++)
    {
        m_constants[i].Name = wxEmptyString;
        m_constants[i].Value = 0;
    }
    SetConstant(wxT("deg"), M_PI/180);
    SetConstant(wxT("e"), exp(1));
    SetConstant(wxT("g"), 9.80665);
    SetConstant(wxT("pi"), M_PI);
    SetConstant(wxT("percent"), 0.01);
    return;
}

bool wxEcEngine::SetConstant(wxString name, wxDouble value)
{
    //-- Initialization
    long index;
    if (name.Len() == 0)
        return false;
    //-- Finds offset
    name = name.Lower().Trim(false).Trim(true);
    index = FindConstant(name);                    //already existing ?
    if (index == wxNOT_FOUND)
        index = FindConstant(wxEmptyString);    //if not, find an empty field
    //-- Action !
    if (index == wxNOT_FOUND)
    {
        m_errorcode = wxECE_MEMORYEXCEEDED;
        return false;
    } else {
        m_constants[index].Name = name;
        m_constants[index].Value = value;
        return true;
    }
}


//----------------- TOOLS ------------------

static double PowerCalc(double base, double exponent)
{
    double expIntpart, k, result;
    bool doInverse = false;

    if (exponent == 0)
        return 1;
    else if (base == 0)
        return 0;
    else if (exponent == 1)
        return base;
    else if (modf(exponent, &expIntpart) == 0)
    {
        if (expIntpart<0)
        {
            expIntpart = -expIntpart;
            doInverse = true;
        }
        k = 0;
        result = 1;
        while (k < expIntpart)
        {
            k = k+1;
            result = result * base;
        }
        if (doInverse)
            result = 1/result;
        return result;
    } else
        return exp(exponent * log(base));   //log is LN in fact !
}

wxEcPosition wxEcEngine::BracketLookup(wxString *expression)
{
    //-- Initialization
    size_t i, implication = 0;
    wxEcPosition result;
    result.Start = wxNOT_FOUND;
    result.End = wxNOT_FOUND;
    result.Function = wxEmptyString;

    //-- Action !
    for (i=0 ; i<expression->Len() ; i++)
    {
        if (expression->GetChar(i) == wxT('('))
            implication++;
        if (expression->GetChar(i) == wxT(')'))
            implication--;

        if ((implication == 1) && (result.Start == wxNOT_FOUND))
            result.Start = i;
        if ((implication == 0) && (result.Start != wxNOT_FOUND) && (result.End == wxNOT_FOUND))
            result.End = i;
    }

    //-- Validity
    if (result.End < result.Start)
    {
        result.Start = wxNOT_FOUND;
        result.End = wxNOT_FOUND;
    }

    //-- Associated function
    if (result.Start != wxNOT_FOUND)
        result.Function = FunctionLookUp(expression, result.Start);

    //-- Result
    return result;
}

double wxEcEngine::ConvertToRadian(double angle)
{
    switch (m_trigomode)
    {
        case wxECA_DEGREE:      return angle*M_PI/180;
        case wxECA_GRADIAN:     return angle*M_PI/200;
        case wxECA_RADIAN:
        default:                return angle;
    }
}

wxUint32 wxEcEngine::CountChar(wxString *input, wxChar what)
{
    wxUint32 result = 0;
    for (size_t i=0 ; i<input->Len() ; i++)
        if (input->GetChar(i) == what)
            result++;
    return result;
}

long wxEcEngine::FindOperator(wxString *expression)
{
    size_t i;
    for (i=0 ; i<expression->Len() ; i++)
        if (IsOperator(expression, i))
            return i;
    return wxNOT_FOUND;
}

size_t wxEcEngine::Function2ID(wxString *function)
{
    size_t i, result = 0;
    if ((function->Len()>0) && (function->Len()<=4))
        for (i=0 ; i<function->Len() ; i++)
            result = result*0x100 + (char) function->GetChar(i);
    return result;
}

wxString wxEcEngine::FunctionLookUp(wxString *expression, size_t bracketPosition)
{
    //-- Initialization
    size_t i;
    wxString delim;
    bool found = false;

    //-- Action !
    if (expression->GetChar(bracketPosition) != wxT('('))
        return wxEmptyString;
    else
    {
        delim = wxString(wxECD_OPERATORS).Append(wxString(wxECD_NUMERIC)).Append(wxString(wxECD_EXTRASYMBOLS));
        for (i=bracketPosition ; i>0 ; i--)
            if (delim.Find(expression->GetChar(i-1)) != wxNOT_FOUND)
            {
                found = true;
                break;
            }
        if (found)
            return expression->Mid(i, bracketPosition-i);
        else
            return wxEmptyString;
    }
}

bool wxEcEngine::IsOperator(wxString *expression, size_t charPosition)
{
    return (    (wxString(wxECD_OPERATORS).Find(expression->GetChar(charPosition)) != wxNOT_FOUND)
                && !IsParitySign(expression, charPosition)
            );
}

bool wxEcEngine::IsParitySign(wxString *expression, size_t charPosition)
{
    if (expression->GetChar(charPosition) != wxT('-'))
        return false;
    else
    {
        if (charPosition == 0)
            return true;
        else
            return (wxString("*/^(").Find(expression->GetChar(charPosition-1)) != wxNOT_FOUND);
    }
}

bool wxEcEngine::IsValid(wxString *expression)
{
    size_t i;
    bool curIsOp, lastIsOp = false;
    //-- Checks for null string
    if (expression->Len() == 0)
        return false;
    //-- Checks the characters
    for (i=0 ; i<expression->Len() ; i++)
        if (wxString(wxECD_PERMITTED).Find(expression->GetChar(i)) == wxNOT_FOUND)
            return false;
    //-- Checks the brackets
    if (CountChar(expression, wxT('(')) != CountChar(expression, wxT(')')))
        return false;
    //-- Checks if there are 2 operators put together
    for (i=0 ; i<expression->Len() ; i++)
    {
        curIsOp = IsOperator(expression, i);
        if (curIsOp && lastIsOp)
            return false;
        lastIsOp = curIsOp;
    }
    return true;
}

void wxEcEngine::LeftPool(int offset)
{
    int i;
    for (i=offset+1 ; i<=wxECD_STACKMAX ; i++)
        m_pool[i] = m_pool[i+1];
}

long wxEcEngine::NextOperator(wxString *expression, size_t charPosition)
{
    size_t i;
    for (i=charPosition+1 ; i<expression->Len() ; i++)
        if (IsOperator(expression, i))
            return i;
    return wxNOT_FOUND;
}

void wxEcEngine::Simplify(wxString *expression)
{
    wxString buffer;
    wxChar car, lastCar;
    size_t i;

    buffer.Clear();
    lastCar = 0;
    for (i=0 ; i<expression->Len() ; i++)
    {
        car = expression->GetChar(i);

        //-- Char variations
        if (car == wxT('['))        car = wxT('(');
        if (car == wxT(']'))        car = wxT(')');
        if (car == wxT('{'))        car = wxT('(');
        if (car == wxT('}'))        car = wxT(')');
        if (car == wxT(','))        car = wxT('.');
        if (car == wxT('�'))    {   car = wxT('2'); lastCar = wxT('^'); buffer.Append(lastCar); }
        if (car == wxT('�'))    {   car = wxT('3'); lastCar = wxT('^'); buffer.Append(lastCar); }

        //-- Sign operation
        if ((lastCar == wxT('-')) && (car == wxT('+')))
            continue;
        if ((lastCar == wxT('+')) && (car == wxT('+')))
            continue;
        if ((lastCar == wxT('+')) && (car == wxT('-')))
            buffer.RemoveLast();
        if ((lastCar == wxT('-')) && (car == wxT('-')))
        {
            buffer.RemoveLast();
            car = wxT('+');
        }
        if ((wxString(wxECD_NUMERIC).Find(lastCar)!=wxNOT_FOUND) && (car==wxT('(')))
            buffer.Append(wxT("*"));
        if ((wxString(wxECD_NUMERIC).Find(lastCar)!=wxNOT_FOUND) && (wxString(wxECD_ALPHABET).Find(car)!=wxNOT_FOUND))
            buffer.Append(wxT("*"));

        //-- Char validated
        if (wxString(wxECD_PERMITTED).Find(car) != wxNOT_FOUND)
        {
            buffer.Append(car);
            lastCar = car;
        }
    }

    *expression = buffer;
    expression->Shrink();
}


//-------------- INTERPRETER ---------------

bool wxEcEngine::ApplyFunction(wxString *function, double *value)
{
    switch (Function2ID(function))
    {
        case 0:
            break;
        case '-':
            *value = - *value;
            break;
    //usual
        case 6382195: //abs
            if (*value < 0)
                *value = - *value;
            break;
        case 1667590508: //ceil
            *value = ceil(*value);
            break;
        case 6518114: //cub
            *value = (*value) * (*value) * (*value);
            break;
        case 6846057: //hvi
            if (*value < 0)
                *value = 0;
            else
                *value = 1;
            break;
        case 6909556: //int
            *value = floor(*value);
            break;
        case 6909558: //inv
            if (*value == 0)
                m_errorcode = wxECE_DIVBYZERO;
            else
                *value = 1 / *value;
            break;
        case 1919247220: //rect
            if ((*value >= -0.5) && (*value <= 0.5))
                *value = 1;
            else
                *value = 0;
            break;
        case 7563118: //sgn
            if (*value < 0)
                *value = -1;
            else
                *value = 1;
            break;
        case 7565682: //sqr
            *value = *value * *value;
            break;
        case 1936814708: //sqrt
            if (*value < 0)
                m_errorcode = wxECE_DOMAINERROR;
            else
                *value = sqrt(*value);
            break;
    //logarithm
        case 27758: //ln
        case 1819174256: //lnep
            if (*value<=0)
                m_errorcode = wxECE_DOMAINERROR;
            else
                *value = log(*value);
            break;
        case 6649968: //exp
            *value = exp(*value);
            break;
        case 7106407: //log
            if (*value<=0)
                m_errorcode = wxECE_DOMAINERROR;
            else
                *value = log(*value)/log(10);
            break;
    //trigometry
        case 6516595: //cos
            *value = cos(ConvertToRadian(*value));
            break;
        case 7563630: //sin
            *value = sin(ConvertToRadian(*value));
            break;
        case 7627118: //tan
            *value = tan(ConvertToRadian(*value));
            break;
        case 1633906547: //acos
            *value = ConvertToRadian(*value);
            if ((*value<=-M_PI/2) || (*value>=M_PI/2))
                m_errorcode = wxECE_DOMAINERROR;
            else
                *value = acos(*value);
            break;
        case 1634953582: //asin
            *value = ConvertToRadian(*value);
            if ((*value<=-M_PI/2) || (*value>=M_PI/2))
                m_errorcode = wxECE_DOMAINERROR;
            else
                *value = asin(*value);
            break;
        case 1635017070: //atan
            *value = atan(ConvertToRadian(*value));
            break;
    //hyperbolic trigonometry
        case 1668248424: //cosh
            *value = cosh(*value);
            break;
        case 1936289384: //sinh
            *value = cosh(*value);
            break;
        case 1952542312: //tanh
            *value = tanh(*value);
            break;
        case 1633907560: //acsh
            if (*value<1)
                m_errorcode = wxECE_DOMAINERROR;
            else
                *value = log(*value + sqrt(*value**value - 1));
                //See: http://mathworld.wolfram.com/InverseHyperbolicCosine.html
            break;
        case 1634954856: //asnh
            *value = log(*value + sqrt(*value**value + 1));
            //See: http://mathworld.wolfram.com/InverseHyperbolicSine.html
            break;
        case 1635020392: //atnh
            if ((*value<=-1) || (*value>=1))
                m_errorcode = wxECE_DOMAINERROR;
            else
                *value = log((1+*value)/(1-*value))/2;
                //See: http://mathworld.wolfram.com/InverseHyperbolicTangent.html
            break;
    //default
        default:
            m_indicator = *function;
            m_errorcode = wxECE_UNKNOWNFUNC;
            break;
    }
    //-- Result
    if (m_errorcode != wxECE_NOERROR)
    {
        m_indicator = *function;
        return false;
    } else {
        #ifdef wxECM_USELOG
            if (function->Len() > 0)
                LogAction(wxString::Format(wxT("   > %s applied > %f"), function->c_str(), *value));
        #endif
        return true;
    }
}

double wxEcEngine::Compute()
{
    #ifdef wxECM_USELOG
        LogAction(wxString::Format(wxT("Asked to solve : %s"), m_formula.c_str()));
    #endif
    m_errorcode = wxECE_NOERROR;
    m_offset = 0;
    m_lastresult = evalexp(&m_formula);
    return m_lastresult;
}

double wxEcEngine::evalexp(wxString *expression)
{
    wxEcPosition brLoc;
    double subResult;
    wxString buffer, subExp;

    buffer = *expression;
DoItAgain:
    brLoc = BracketLookup(&buffer);

    //-- If invalid bracket, it is a problem !
    if (((brLoc.Start == wxNOT_FOUND) || (brLoc.End == wxNOT_FOUND)) && (brLoc.Start != brLoc.End))
    {
        m_errorcode = wxECE_SYNTAX;
        return 0;
    }

    //-- If no bracket, it is a simple expression
    if ((brLoc.Start == wxNOT_FOUND) && (brLoc.End == wxNOT_FOUND))
        return evalf(&buffer);

    //-- Else it is hard, because of RECURSIVITY ^^ :D
    subExp = buffer.Mid(brLoc.Start+1, brLoc.End-brLoc.Start-1);
    subResult = evalexp(&subExp);
    if (m_errorcode != wxECE_NOERROR)                       //error in a formula for example
        return 0;
    if (!ApplyFunction(&(brLoc.Function), &subResult))      //domain definition error, here
        return 0;
    if (!StackIt(&subResult))                               //too much data to store, you can expand manually MAX_STACK and recompile
        return 0;
    buffer = buffer.Mid(0, brLoc.Start-brLoc.Function.Len()) + wxT("#") + buffer.Mid(brLoc.End+1);
    //Simplify(&buffer);
    goto DoItAgain;
}

double wxEcEngine::evalf(wxString *expression)
{
    //-- Initialization
    wxString poolSign = wxEmptyString, priorities = wxECD_OPERATORS, buffer, value;
    size_t terms=0, index, j, k;
    double oldPoolK;
    struct { size_t NumDeclared; size_t ID; } sharps;

    //-- Explodes the expression
    buffer = *expression;
    if (buffer.StartsWith(wxT("-")))        //This forces "-" to be always an operator, not a simple minus indicator.
        buffer = wxT('0') + buffer;            //Try to draw "x^2" without this trick...
    #ifdef wxECM_USELOG
        LogAction(wxString::Format(wxT("\r\nReceived: %s"), buffer.c_str()));
    #endif
    sharps.NumDeclared = CountChar(&buffer, wxT('#'));
    sharps.ID = 0;
    while (true)
    {
        index = NextOperator(&buffer);
        if (index == wxNOT_FOUND)
            index = buffer.Len();

        value = buffer.Mid(0, index);
        if (value.Last() == wxT('#'))
        {
            m_pool[terms] = m_antecomp[m_offset - sharps.NumDeclared + sharps.ID];
            sharps.ID++;
            if (value.Len() > 1)                        //sometimes we get "ln#". It is a value on which a function must be executed
            {
                value = value.BeforeFirst(wxT('#'));
                ApplyFunction(&value, &(m_pool[terms]));
            }
        } else
            if (!GetConstant(value, &(m_pool[terms])))
                break;
        poolSign.Append(buffer.GetChar(index));
        buffer = buffer.Mid(index+1);
        terms++;

        if (buffer.Len() == 0) 
            break;
    }
    if (poolSign.Len() >= wxECD_STACKMAX-1)
        m_errorcode = wxECE_STACKERROR;
    if (m_errorcode != wxECE_NOERROR)
        return 0;

    //-- Everything is now stored in memory, we apply the priorities
    #ifdef wxECM_USELOG
        for (j=0 ; j<poolSign.Len() ; j++)
            LogAction(wxString::Format(wxT(" [%d] = %f"), j, m_pool[j]));
    #endif

    for (j=0 ; j<priorities.Len() ; j++)
    {
RedoForOperator:
        k = poolSign.Find(priorities.GetChar(j));
        if (k != wxNOT_FOUND)
        {
            oldPoolK = m_pool[k];                        //useful to log the operation
            switch (priorities.GetChar(j))
            {
                case wxT('^'):
                    m_pool[k] = PowerCalc(m_pool[k], m_pool[k+1]);
                    break;
                case wxT('/'):
                    if (m_pool[k+1] == 0)
                        m_errorcode = wxECE_DIVBYZERO;
                    else
                        m_pool[k] = m_pool[k] / m_pool[k+1];
                    break;
                case wxT('*'):
                    m_pool[k] = m_pool[k] * m_pool[k+1];
                    break;
                case wxT('+'):
                    m_pool[k] = m_pool[k] + m_pool[k+1];
                    break;
                case wxT('-'):
                    m_pool[k] = m_pool[k] - m_pool[k+1];
                    break;
            }

            #ifdef wxECM_USELOG
                LogAction(wxString::Format(wxT("   >   %f %c %f = %f"), oldPoolK, priorities.GetChar(j), m_pool[k+1], m_pool[k]));
            #endif
            LeftPool(k);
            poolSign = poolSign.Mid(0, k) + poolSign.Mid(k+1);
            goto RedoForOperator;
        }
    }
    m_offset -= sharps.NumDeclared;
    return m_pool[0];
}

bool wxEcEngine::Derivate(double where, double *result, double *atwhere)
{
    double fx, fxm, fxp;
    //-- Computes
    SetConstant(wxT("x"), where);
    fx = Compute();
    if (GetLastError() != wxECE_NOERROR)
        return false;
    SetConstant(wxT("x"), where - wxECD_DXSTEP);
    fxm = Compute();
    if (GetLastError() != wxECE_NOERROR)
        return false;
    SetConstant(wxT("x"), where + wxECD_DXSTEP);
    fxp = Compute();
    if (GetLastError() != wxECE_NOERROR)
        return false;
    //-- Store the results
    if (atwhere != NULL)
        *atwhere = fx;
    if (result != NULL)
        *result = (fxp - fxm)/(2*wxECD_DXSTEP);
    return true;
}

bool wxEcEngine::SetFormula(wxString expression)
{
    Reset(true, false);
    Simplify(&expression);
    if (IsValid(&expression))
    {
        m_formula = expression;
        //m_errorcode = wxECE_NOERROR; already done with Reset()
        return true;
    } else {
        m_errorcode = wxECE_SYNTAX;
        return false;
    }
}

bool wxEcEngine::StackIt(double *value)
{
    if (m_offset >= wxECD_STACKMAX)
    {
        m_errorcode = wxECE_STACKERROR;
        return false;
    }
    //Store the value
    m_antecomp[m_offset] = *value;
    m_offset++;
    return true;
}
