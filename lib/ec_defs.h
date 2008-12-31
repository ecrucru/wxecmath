
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

/*--------------------------------------------------------------------------------------
 * This file is also the root of the documentation. Comments do not interfer with the
 * software. They are taken into consideration only by Doxygen, a generator of docs.
--------------------------------------------------------------------------------------*/

/** @mainpage wxEcMath
 *
 * \a wxEcMath is a tiny library to bind to your software. It gives you the means to evaluate
 * mathematical expressions and draw curves. The rich samples provided will help you to
 * set up a new project very quickly.
 *
 * The four main interests of wxEcMath are :
 *      - the ability to evaluate complex expressions with pretty nice performances
 *      - the support of Windows, Linux, Macintosh...
 *      - the support of ANSI and UNICODE compilations
 *      - the documentation to give you all the clues
 *
 * The intended audience is developers and researchers.
 *
 * To use the library, you must write your software using the wxWidgets framework.
 * You must also be aware of the terms under which the library is released (see below).
 *
 * \section topics Topics
 *      - \subpage overview     "Overview"
 *      - \subpage naming       "Naming convention of the constants"
 *      - \subpage syntax       "Syntax to load a curve"
 *      - \subpage funclist     "Supported functions"
 *      - \subpage license      "License"
 *      .
 * See online : http://sourceforge.net/projects/wxecmath/
 */

/** \page overview Overview
 *
 * \a wxEcMath is the name given to the library, which contains sub-classes.
 * You really need to know wxEcEngine (the calculator) and wxEcPlot (the renderer).
 *
 * To include the library, you just need the header files to be loaded in your workspace
 * and type the following code. All new names start with the "wxEc" prefix.
 * \code
 *      #define wxECM_USEDEBUG        //read the docs about this feature
 *      #include "ec_defs.h"
 *      #include "ec_engine.h"
 *      #include "ec_plot.h"        //if you need a plot
 * \endcode
 * \see wxECM_USEDEBUG
 *
 * To perform a simple computation, it is not harder than :
 * \code
 *  {
 *      double result;
 *      wxEcEngine calc;
 *      calc.SetFormula(wxT("3+4"));
 *      result = calc.Compute();
 *      if (calc.GetLastError() == wxECE_NOERROR)
 *          ; //well done !
 *      else
 *          ; //better luck next time !
 *  }
 * \endcode
 *
 * To create the component, it is the same method as you do most of the time :
 * \code
 *  {
 *      wxEcPlot *Plot;
 *      ...
 *      Plot = new wxEcPlot(this, wxID_ANY);
 *      yourMainSizer->Add(Plot, 1, wxEXPAND|wxALL, 5);
 *  }
 * \endcode
 *
 * To draw a curve into a wxEcPlot, it is also easy :
 * \code
 *  {
 *      wxEcPlot *Plot; //must be normally initialized, this is just an example
 *      int curveID;
 *      wxEcCurve newcurve;
 *      newcurve.Parse(wxT("sqr(x)/10 $ -7 4"));
 *      curveID = Plot->AddCurve(&newcurve);
 *      Plot->Refresh();
 *  }
 * \endcode
 */

/** \page naming Naming convention
 *
 * The symbols of wxEcMath have the magic prefix "wxEc" or "wxEC".
 * The first one defines a class or a structure. The second one
 * represents a definition or an enumeration.
 *
 * Symbols "wxEC" have a special syntax : wxEC(key)_(name)
 *  - key :
 *      - A : angle
 *      - D : definition
 *      - E : error
 *      - M : mode
 *      - T : type of curve
 *      .
 *  - name : written in uppercase
 *  .
 */

/** \page syntax Syntax to load a curve
 *
 * With wxEcPlot, you can draw 3 types of curves :
 *      - cartesian : y = f(x) marked as \a wxECT_CARTESIAN.
 *      - parametric : x = f(t) and y = f(t) marked as \a wxECT_PARAMETRIC.
 *      - polar : r = f(t) marked as \a wxECT_POLAR.
 *      .
 *
 * However, you can also restrict the domain definition. So a syntax must be adopted.
 * Because of \a theta, parametric and polar curves must be restricted.
 *      - cartesian normal : "3*x+2"
 *      - cartesian restricted : "sqr(x) $ -5 5"
 *      - parametric : "t+2 ; t/2 $ -pi pi"
 *      - polar : "t $ 0 2*pi"
 *      .
 *
 * These values must be parsed with wxEcCurve::Parse(), but you can also
 * define a wxEcCurve on your own via the constructor or the methods.
 * \warning If the expression is polar, wxEcCurve::Parse() takes a second parameter !
 *
 * As you can see :
 *      - ";" delimits the two members of a parametric curve.
 *      - "$" defines a domain, min and max are separated with a space.
 *      - if these characters are not found, the expression will be treated as a non-restricted cartesian curve.
 *      .
 * If no domain is given, default values will be chosen. The use of the range will be set accordingly.
 *
 * You can read the \ref overview "overview" to see a sample. Or you can try the
 * full sample named "plot" given in the distribution.
 *
 * When you parse an expression, a default colour is chosen, randomly among twenty colours.
 * It is a good thing to randomize this automatic choice in your \a wxApp::OnInit() :
 * \code
 * srand(time(NULL));
 * \endcode
 */

/** \page funclist Supported functions
 *
 * The library is able to interpret several functions. Their length cannot be longer than 4 characters.
 * Indeed wxEcEngine::Function2ID() converts the name (8 or 16 bits per char) into an integer (32 bits).
 * We assumed it should be faster to interpret.
 *
 * \section normalfuncs Normal functions
 *      - abs : the absolute value, it removes the sign
 *      - ceil : the smallest integer greater than or equal to x
 *      - cub : x^3, safe way to compute such an exponent
 *      - exp : the exponential value
 *      - hvi : Heaviside's function, =0 if x<0, =1 if x>=0
 *      - int : the largest integer less than or equal to x
 *      - inv : inverts, 1/x
 *      - floor : not supported, see \a int
 *      - ln : the neperian logarithm
 *      - lnep : see \a ln
 *      - log : the logarithm base 10
 *      - sgn : the sign, -1 or +1
 *      - sqr : x^2, safe way to compute such an exponent
 *      - sqrt : the root square
 *      .
 *
 * \section trigofuncs Trigonometric functions
 *      - acos : the secant (inverse of cosine)
 *      - acsh : the inverse of the hyperbolic cosine
 *      - asin : the cosecant (inverse of sine)
 *      - asnh : the inverse of the hyperbolic sine
 *      - atan : the cotangent (inverse of tangent)
 *      - atnh : the inverse of the hyperbolic tangent
 *      - cos : the cosine
 *      - cosh : the hyperbolic cosine
 *      - sin : the sine
 *      - sinh : the hyperbolic sine
 *      - tan : the tangent
 *      - tanh : the hyperbolic tangent
 *      .
 */

/** \page license License
 *
 * \a wxEcMath is released under the terms of the "GNU General Public License version 2".
 *
 * The official download center is SourceForge.net
 *
 * \pre
 *  wxEcMath - version 0.6.1 <br>
 *  Copyright (C) 2008-2009, http://sourceforge.net/projects/wxecmath/
 *
 * \pre
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 * \pre
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 * \pre
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

//--------------------------------------------------------------------------------------

/**
 * \file ec_defs.h
 * \brief Definition of the simpliest classes and constants
 * \author ecrucru
 * \version 0.6.1
 * \date January 2009
 *
 * Implements the definition of core elements needed by the wxEcMath classes.
 * Before compiling a release, please take a look to wxECM_USEDEBUG. This can
 * increase the performances of your software very significantly.
 */

#ifndef ec_defs_h
#define ec_defs_h

    #if wxUSE_UNICODE
        #pragma message("wxEcMath - Compilation mode: UNICODE")
        #define uniCStr wc_str
    #else
        #pragma message("wxEcMath - Compilation mode: ANSI")
        #define uniCStr c_str
    #endif

//--------------- MODES --------------------

/** \def wxECM_USEDEBUG
 * When defined, extra features are compiled inside wxEcEngine. They were implemented
 * during the development of wxEcMath to track bugs. For a lambda-user, there is no
 * need to use them (performances would be more than 2 times slower).
 *
 * You can enable the additional features without editing the library. Just do in your
 * .cpp code :
 * \code
 *      #define  wxECM_USEDEBUG
 *      #include "ec_defs.h"
 *      #include "ec_engine.h"
 * \endcode
 *
 * \see wxEcEngine
 * \see wxEcEngine::LogAction()
 * \see wxEcEngine::GetLog()
 */
//DOXYGEN: #define wxECM_USEDEBUG

//--------------- COPYRIGHT ----------------

/** \def wxECD_SOFTWARE
 * Name of the software.
 */
#define        wxECD_SOFTWARE               wxT("wxEcMath")
/** \def wxECD_URL
 * Internet address where you can download the software.
 */
#define        wxECD_URL                    wxT("http://sourceforge.net/projects/wxecmath/")
/** \def wxECD_VERSION
 * Version of the software currently used.
 */
#define        wxECD_VERSION                wxT("0.6.1")
/** \def wxECD_AVERAGESPEED
 * Average computations you can make in 1 second.
 * Allows you to predict the time needed in complex algorithms.
 * The value depends on \a wxECM_USEDEBUG.
 *
 * \remarks The formula chosen to evaluate the speed is "(1+sqrt(5))/2-2*cos(2*pi/5)".
 * Depending on the length of your formula, you won't get exactly this speed. Maybe more, maybe less.
 * However this value should be representative of most of cases.
 *
 * \see wxECM_USEDEBUG
 */
#ifdef wxECM_USEDEBUG
    #define    wxECD_AVERAGESPEED           9000        //per second, arbitrary because the function to evaluate can be complex
#else
    #define    wxECD_AVERAGESPEED           28000
#endif

//--------------- ENGINE -------------------

/** \def wxECD_OPERATORS
 * All the operators supported, ordered by mathematical priority.
 */
#define        wxECD_OPERATORS              wxT("^/*-+")
/** \def wxECD_ALPHABET
 * All the letters of the alphabet, written in lowercase (a-z).
 */
#define        wxECD_ALPHABET               wxT("abcdefghijklmnopqrstuvwxyz")
/** \def wxECD_NUMERIC
 * All the numbers supported (0-9).
 */
#define        wxECD_NUMERIC                wxT("0123456789")
/** \def wxECD_EXTRASYMBOLS
 * Extra symbols available when evaluated.
 */
#define        wxECD_EXTRASYMBOLS           wxT("().#")
/** \def wxECD_PERMITTED
 * The allowed ANSI characters are defined here.
 * Since 0.6.1, you can define a constant with the name of your choice.
 * It means you can use a chinese or japanese name if you have enabled "UNICODE".
 * This choice is automatically performed.
 */
#define        wxECD_PERMITTED              wxT("abcdefghijklmnopqrstuvwxyz0123456789^/*-+().#")
/** \def wxECD_STACKMAX
 * Maximal number of constants you may define.
 *
 * Maximal number of entities that an inline expression may contain (a+b*c = 3 entities).
 */
#define        wxECD_STACKMAX               128


/** \struct wxEcPosition
 *  \brief Stores an expression and two indexes (Start/End)
 *
 * This structure is only used by wxEcEngine to mark the position of two brackets in an expression.
 * Normally, you are not expected to need this resource.
 * \see wxEcEngine
 */
typedef struct wxEcPosition {
    long Start;             /**< A start position. */
    long End;               /**< An end position. */
    wxString Function;      /**< The expression. */
} wxEcPosition;

/** \struct wxEcConstMap
 *  \brief It is a wxHashMap(string -> double) which can manage a list of constants.
 *
 * Since version 0.6.1, wxHashMap is the new system used to manage constants.
 * Before, an array of 128 cells was used. Performances have not been probed.
 */
WX_DECLARE_STRING_HASH_MAP(double, wxEcConstMap);

/** Error codes returned by wxEcEngine::Compute().
 * \see wxEcEngine::GetLastError()
 */
enum {
    wxECE_NOERROR = 0,      /**< No error. */
    wxECE_SYNTAX,           /**< The syntax is not correct. */
    wxECE_UNDEFCONSTANT,    /**< The constant is not defined. */
    wxECE_MEMORYEXCEEDED,   /**< The maximal number of constants/curves that you may define is reached.
                                 \see wxECD_STACKMAX */
    wxECE_DIVBYZERO,        /**< Division by zero */
    wxECE_DOMAINERROR,      /**< The function is not defined for the given value. */
    wxECE_UNKNOWNFUNC,      /**< The name of the function is unknown. */
    wxECE_STACKERROR,       /**< The expression is too long to be evaluated.
                                 \see wxECD_STACKMAX */
    wxECE_UNCOMPUTABLE      /**< Generic error. */
};

/** Trigonometric modes. Radian is the default mode.
 * \see wxEcEngine::GetTrigonometricMode()
 * \see wxEcEngine::SetTrigonometricMode()
 */
enum {
    wxECA_RADIAN = 0,       /**< Radian. */
    wxECA_DEGREE,           /**< Degree. */
    wxECA_GRADIAN           /**< Gradian. */
};

//--------------- PLOT ---------------------

/** \def wxECD_CURVEMAX
 * Maximal number of curves you can define.
 * \see wxEcPlot::AddCurve()
 */
#define        wxECD_CURVEMAX               64
/** \def wxECD_STEPSMAX
 * Maximal number of graduations a plot can contain.
 */
#define        wxECD_STEPSMAX               30
/** \def wxECD_RESOLUTION
 * Default number of points used to draw a parametric or a polar curve.
 * The value applies to a curve, not to all the curves loaded in a wxEcPlot.
 * \see wxEcCurve::NumPoints
 */
#define        wxECD_RESOLUTION             500
/** \def wxECD_DXSTEP
 * The step used to approximate the derivative of a curve at a given point.
 * \see wxEcPlot::DrawDerivative()
 */
#define        wxECD_DXSTEP                 0.0001
/** \def wxECD_RETICULESIZE
 * The radius of the reticule.
 * \see wxEcPlot::SetReticule()
 */
#define        wxECD_RETICULESIZE           3
/** \def wxECD_ZOOMFACTOR
 * The default zoom factor.
 * \see wxEcPlot::SetZoomFactor()
 */
#define        wxECD_ZOOMFACTOR             1.5


/** Types of curves you can draw.
 * \see wxEcCurve
 */
enum {
    wxECT_CARTESIAN = 0,    /**< Cartesian: y = f(x) */
    wxECT_PARAMETRIC,       /**< Parametric: x = f(t) && y = f(t) */
    wxECT_POLAR             /**< Polar : r = f(t) */
};

/** \class wxEcPointDouble
 * \brief Defines an X-Y position with floating values
 */
class wxEcPointDouble {
    public:
        double X; /**< The X-coordinate. */
        double Y; /**< The Y-coordinate. */

        /** The default constructor. */
        wxEcPointDouble(double x = 0.0, double y = 0.0) : X(x), Y(y) { }
        /** The default destructor. */
        ~wxEcPointDouble() { }
};

#endif
