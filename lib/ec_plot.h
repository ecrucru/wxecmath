
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

/**
 * \file ec_plot.h
 * \brief Includes the area to draw curves
 * \author ecrucru
 * \version 0.6.1
 * \date January 2009
 */

#ifndef ec_plot_h
#define ec_plot_h

//------------------------------------------

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/colour.h"
#include "wx/window.h"
#include "ec_defs.h"
#include "ec_engine.h"

//------------------------------------------

/** \class wxEcAxis
 *  \brief Describes an axis.
 */
class WXDLLEXPORT wxEcAxis
{
    public:
        double MinVal;          /**< The minimal value of the axis. */
        double MaxVal;          /**< The maximal value of the axis. */
        double StepVal;         /**< The interval between two graduations. */
        wxString Format;        /**< The format for the text. The default value is "%.1f". */

        /** The default constructor.
         * \param min The lower limit of the axis.
         * \param max The upper limit of the axis.
         * \param step The interval between two graduations.
         * \remarks To set up the format of the text, you must call wxEcAxis::Format = wxT("...") ;
         */
        wxEcAxis(double min = -10, double max = 10, double step = 2) : MinVal(min), MaxVal(max), StepVal(step), Format(wxT("%.1f")) { }
        /** The default destructor.
         */
        ~wxEcAxis() { }
        /** Validates the axis to make it useful without generating unexpected bugs.
         *  The call is only required when you define an axis from a user's input that
         *  you have not checked beforehand.
         */
        void CorrectMe();
};

//------------------------------------------

/** \class wxEcCurve
 *  \brief Defines a curve and its display.
 */
class WXDLLEXPORT wxEcCurve
{
    public:
        bool Defined;               /**< Marks if a curve should be considerated. */
        bool Enabled;               /**< If disabled, the curve won't be displayed. */
        wxString ExpressionX,       /**< The expression used for \a wxECT_CARTESIAN, \a wxECT_PARAMETRIC and \a wxECT_POLAR. */
                 ExpressionY;       /**< The complementary expression needed for \a wxECT_PARAMETRIC. */
        int Type;                   /**< The type of curve.
                                     * \see ec_defs.h (wxECT_*)
                                     */
        wxColour Colour;            /**< The colour used for the display. */
        int Width;                  /**< Pen's width for the curve. */
        bool DotStyle;              /**< The Y-coordinate will be a single point. No line is used to link to the previous computed point. */
        bool RangeEnabled;          /**< This marks the curve to be restricted to a specific domain delimited with RangeMin and RangeMax.
                                     *   If the type of curve is wxECT_PARAMETRIC or wxECT_POLAR, this value shall be \a true.
                                     */
        double RangeMin;            /**< The lower limit, for X or T. To be taken into consideration, RangeEnabled must be \a true. */
        double RangeMax;            /**< The upper limit, for X or T. To be taken into consideration, RangeEnabled must be \a true. */
        unsigned long NumPoints;    /**< The resolution to draw parametric and polar curves. */

        /** The default constructor.
         */
        wxEcCurve(      wxString formulaX = wxEmptyString,
                        wxString formulaY = wxEmptyString,
                        int typeofcurve = wxECT_CARTESIAN,
                        wxColour pencolour = *wxBLUE,
                        int boldness = 2,
                        bool usedots = false,
                        bool userange = false,
                        double minrange = -10,
                        double maxrange = 10,
                        unsigned long numpoints = wxECD_RESOLUTION)
            :
                        Defined(true), Enabled(true),
                        ExpressionX(formulaX),
                        ExpressionY(formulaY),
                        Type(typeofcurve),
                        Colour(pencolour),
                        Width(boldness),
                        DotStyle(usedots),
                        RangeEnabled(userange),
                        RangeMin(minrange),
                        RangeMax(maxrange),
                        NumPoints(numpoints)
            {}
        /** The default destructor.
         */
        ~wxEcCurve() {}

        /** Validates the curve to make it useful without generating unexpected bugs.
         *  The call is only required when you define a curve from a user's input that
         *  you have not checked beforehand.
         */
        void CorrectMe();
        /** Parses a formula and applies the parameters. See the \ref syntax "HowTo" to know more about.
         * \param def The definition.
         * \param isPolar Indicates if the input is expected to define a \a wxECT_POLAR curve.
         * \remarks A colour is randomly chosen among a set of colours. You can edit it with wxEcCurve::Colour.
         */
        void Parse(wxString def, bool isPolar = false);
};

//------------------------------------------

/** \class wxEcPlot
 *  \brief The component which draws the curves.
 *
 * wxEcPlot is a component derived from \a wxWindow. It has a canvas on which you
 * can draw. Most of the time, the curves are drawn when a onPaint event occurs.
 * If you call \a wxWindowDC(myPlot), it is possible to draw on the client area.
 *
 * Before using the component, you must be aware that most of the methods does NOT
 * refresh the component automatically. So you must call wxEcPlot::Refresh() to
 * see any modification. The aim is to have better performances.
 */
class WXDLLEXPORT wxEcPlot : public wxWindow
{
    DECLARE_EVENT_TABLE()
    private:
        wxEcEngine *m_engine;
        wxEcAxis m_axisx, m_axisy;
        wxEcCurve m_curves[wxECD_CURVEMAX];
        wxColour m_bgcolour, m_axiscolour, m_gridcolour, m_reticulecolour;
        bool m_flatborder, m_showgrid, m_gridpolar, m_showaxis, m_showreticule;
        int m_axisarrowsize;
        wxFont m_axisfont;
        bool m_locked;
        double m_ymaxfound, m_yminfound, m_zoomfactor;
        bool m_ymarker;
        wxEcPointDouble m_reticule;
        wxString m_lasttangent;

        void DoDrawAxis(wxDC *context);
        void DoDrawCurve(wxDC *context, wxEcCurve *curve);
        void DoDrawParametricCurve(wxDC *context, wxEcCurve *curve);
        void DoDrawPolarCurve(wxDC *context, wxEcCurve *curve);
        void DoDrawReticule(wxDC *context);
        void DoRedraw();

    public:
        //-- Herited functions (to make them documentable by Doxygen)
        /** Refreshes the component. It will cause the entire canvas/DC to be redrawn.
         */
        void Refresh() { wxWindow::Refresh(); }

        //-- Normal behaviour
        /** The default constructor.
         * \param parent The parent of the component. Can be a wxFrame, a wxDialog, a wxPanel...
         * \param id The unique identifier.
         * \param pos The default position.
         * \param size The default size.
         * \param style The style (see \a wxWindow).
         */
        wxEcPlot(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
        /** The default destructor.
         */
        ~wxEcPlot();
        /** Locks or unlocks the automatic refresh.
         * \param state \a true to lock, \a false to unlock.
         */
        void Lock(bool state = true) { m_locked = state; }
        /** Resets the component, but colours are not affected. All the curves will be deleted.
         */
        void Reset()
            {
                m_ymarker = false;
                RemoveAllCurves();
                SetDefaultGrid();
                SetAxisVisible(true);
                SetGridVisible(true);
                SetReticuleVisible(false);
                Lock(false);
                Refresh();
            }
        /** Returns the equation you obtained at the last call of DrawTangent().
         */
        wxString GetLastTangentEquation() { return m_lasttangent; }
        /** Draws a temporary tangent.
         * \param index The identifier of the curve (obtained with AddCurve()).
         * \param position The X-coordinate where the tangent shall be drawn.
         * \return \a true on success, \a false on failure.
         * \remarks It only works with \a wxECT_CARTESIAN.
         * \see GetLastTangentEquation()
         */
        bool DrawTangent(int index, double position);
        /** Draws in entire but temporarily the derivative of a curve, restricted on the correct domain.
         * \param index The identifier of the curve (obtained with AddCurve()).
         * \return \a true on success, \a false on failure.
         * \remarks It only works with \a wxECT_CARTESIAN.
         * \see wxEcEngine::Derivate()
         */
        bool DrawDerivative(int index);

        //-- Curves
        /** Adds a new curve.
         * \param curve A pointer to the definition of the curve.
         * \return \a wxNOT_FOUND if memory is exceeded, a positive or null value on success.
         */
        int AddCurve(wxEcCurve *curve)
            {
                int i;
                for (i=0 ; i<wxECD_CURVEMAX ; i++)
                    if (!m_curves[i].Defined)
                        if (SetCurve(i, curve))
                            return i;
                        else
                            return wxNOT_FOUND;
                return wxNOT_FOUND;
            }
        /** Returns the number of curves currently defined.
         */
        int CountCurve()
            {
                int i, result = 0;
                for (i=0 ; i<wxECD_CURVEMAX ; i++)
                    if (m_curves[i].Defined)
                        result++;
                return result;
            }
        /** Removes the curve specified by its identifier.
         * \param index The identifier of the curve (obtained with AddCurve()).
         */
        void DeleteCurve(int index)
            {
                m_curves[index].Defined = false;
                m_curves[index].Enabled = false;
                m_curves[index].ExpressionX.Clear();
                m_curves[index].ExpressionY.Clear();
            }
        /** Enables or disables the curve specified by its identifier.
         * \param index The identifier of the curve (obtained with AddCurve()).
         * \param state \a true to enable, \a false to disable.
         */
        void EnableCurve(int index, bool state)
            {
                m_curves[index].Enabled = state;
            }
        /** Returns a curve.
         * \param index The identifier of the curve (obtained with AddCurve()).
         * \return The curve "as is" : defined or not, enabled or not.
         */
        wxEcCurve GetCurve(int index)
            {
                return m_curves[index];
            }
        /** Sets a new curve at the given index.
         * \param index The position to edit.
         * \param curve The pointer to the new curve.
         */
        bool SetCurve(int index, wxEcCurve *curve)
            {
                m_engine->Simplify(&(curve->ExpressionX));
                if (!m_engine->IsValid(&(curve->ExpressionX)))
                    return false;
                m_engine->Simplify(&(curve->ExpressionY));
                if ((curve->Type==wxECT_PARAMETRIC) && !m_engine->IsValid(&(curve->ExpressionY)))
                    return false;
                m_curves[index] = *curve;
                m_curves[index].Defined = true;
                return true;
            }
        /** Deletes all the curves.
         * \remarks This function call DeleteCurve() \a wxECD_CURVEMAX times.
         */
        void RemoveAllCurves()
            {
                int i;
                for (i=0 ; i<wxECD_CURVEMAX ; i++)
                    DeleteCurve(i);
                //m_lasttangent.Clear(); It is not necessarily useful
            }

        //-- Conversions
        /** Converts floating values into coordinates of the plan.
         *  The result depends on how the axis are defined (see SetAxis()).
         * \param X The X-coordinate.
         * \param Y The Y-coordinate.
         * \return A structure containing X and Y converted.
         */
        wxPoint CoupleValueToXY(double X, double Y);
        /** Indicates if the point (x,y) relative to the axis is visible.
         * \param X The X-coordinate.
         * \param Y The Y-coordinate.
         * \return \a true if the point belongs to the visible client area, \a false if the point is out of range
         */
        bool IsVisible(double X, double Y);
        /** Sets the unit per pixel.
         * \param perpixel The value per pixel (can be retrieved from XPerPixel() and YPerPixel())
         * \param forX If \a true, X-axis will be modified, else it will be the Y-axis.
         * \remarks If the control is resized, this function will not be recalled.
         */
        void SetUnit(double perpixel, bool forX);
        /** Converts a value using the system of coordinates of the control, relative to X.
         * \remarks The result can be outer than the physical limits.
         */
        int ValueToX(double value);
        /** Converts a value using the system of coordinates of the control, relative to Y.
         * \remarks The result can be outer than the physical limits.
         */
        int ValueToY(double value);
        /** Gives the interval represented by 1 pixel on the X-axis.
         */
        double XPerPixel();
        /** Converts a physical coordinate into the corresponding value, relative to the X-axis.
         */
        double XToValue(int X);
        /** Gives the interval represented by 1 pixel on the Y-axis.
         */
        double YPerPixel();
        /** Converts a physical coordinate into the corresponding value, relative to the Y-axis.
         */
        double YToValue(int Y);

        //-- Background & Axis
        /** Returns the colour of the background.
         */
        wxColour GetBackgroundColor() { return m_bgcolour; }
        /** Sets the new colour of the background.
         * \param value The new colour to apply.
         */
        void SetBackgroundColor(wxColour value) { m_bgcolour = value; }
        /** Indicates if the control have a black border.
         */
        bool GetFlatBorder() { return m_flatborder; }
        /** Applies a new border, or remove it.
         * \param value \a true to apply, \a false to remove.
         */
        void SetFlatBorder(bool value) { m_flatborder = value; }

        /** Resets the grid.
         */
        void SetDefaultGrid() { wxEcAxis def; m_axisx = def; m_axisy = def; m_gridpolar = false; }
        /** Is the grid visible ?
         */
        bool GetGridVisible() { return m_showgrid; }
        /** Shows or hides the grid.
         * \param value \a true to show, \a false to hide.
         */
        void SetGridVisible(bool value) { m_showgrid = value; }
        /** Returns \a true if the grid is currently displayed in polar mode. \a false indicates the cartesian mode.
         */
        bool GetGridPolar() { return m_gridpolar; }
        /** Sets or unsets the polar mode of the grid.
         * \param value \a true to set, \a false to unset.
         */
        void SetGridPolar(bool value) { m_gridpolar = value; }
        /** Returns the color of the grid.
         */
        wxColour GetGridColor() { return m_gridcolour; }
        /** Sets the color of the grid.
         */
        void SetGridColor(wxColour value) { m_gridcolour = value; }

        /** Applies a new configuration to the designed axis.
         * \param axis The new definition.
         * \param forX If \a true, X-axis will be modified, else it will be the Y-axis.
         */
        void SetAxis(wxEcAxis axis, bool forX=true) { if (forX) m_axisx = axis; else m_axisy = axis; }
        /** Returns the visibility of the axis.
         */
        bool GetAxisVisible() { return m_showaxis; }
        /** Shows or hides both axis.
         * \param value \a true to show, \a false to hide.
         */
        void SetAxisVisible(bool value) { m_showaxis = value; }
        /** Returns the color of the axis.
         */
        wxColour GetAxisColor() { return m_axiscolour; }
        /** Sets the new color on both axis.
         */
        void SetAxisColor(wxColour value) { m_axiscolour = value; }
        /** Gets the current font used for the text of the axis.
         */
        wxFont GetAxisFont() { return m_axisfont; }
        /** Sets the new font to be used for the text of the axis.
         * \param value The font.
         * \remarks The colour cannot be changed with this function.
         */
        void SetAxisFont(wxFont value) { m_axisfont = value; }
        /** Fits the Y-axis to make all the enabled curves visible.
         *  You cannot use this function if the curves have not been drawn at least one time.
         *  Use Refresh() to do so.
         * \see GetYMin()
         * \see GetYMax()
         */
        void FitYAxis()
            {
                if (m_ymarker)
                {
                    m_axisy.MaxVal = m_ymaxfound;
                    m_axisy.MinVal = m_yminfound;
                    RecalibrateAxis(false);
                }
            }
        /** Returns the maximal value of Y found during the last refresh.
         * \see FitYAxis()
         */
        double GetYMax() { if (m_ymarker) return m_ymaxfound; else return 0; }
        /** Returns the minimal value of Y found during the last refresh.
         * \see FitYAxis()
         */
        double GetYMin() { if (m_ymarker) return m_yminfound; else return 0; }
        /** Makes the grid to be like squares. In such a grid, right angles are conserved.
         *  Technically, it solves : YPerPixel() = XPerPixel();
         *  The Y-axis will be symmetrical compared to zero.
         */
        void Orthonormalize()
            {
                int w, h;
                this->GetSize(&w, &h);
                double unit = XPerPixel();
                m_axisy.MaxVal = unit*h/2;
                m_axisy.MinVal = -unit*h/2;
            }
        /** Makes so that a reasonable number of graduations will be displayed on the given axis.
         * \param forX If \a true, X-axis will be modified, else it will be the Y-axis.
         */
        void RecalibrateAxis(bool forX)
            {
                wxEcAxis *goal;
                if (forX)
                    goal = &m_axisx;
                else
                    goal = &m_axisy;
                if ((goal->MaxVal - goal->MinVal) / goal->StepVal > wxECD_STEPSMAX)
                    goal->StepVal = (goal->MaxVal - goal->MinVal)/wxECD_STEPSMAX;
            }
        /** Returns the current zoom factor.
         */
        double GetZoomFactor() { return m_zoomfactor; }
        /** Changes the zoom factor. The default value is \a wxECD_ZOOMFACTOR defined in ec_defs.h
         */
        bool SetZoomFactor(double value) { if (value<=0) return false; m_zoomfactor = value; return true; }
        /** Zooms out.
         * \see SetZoomFactor()
         */
        void Unzoom()
            {
                m_axisx.MinVal *= m_zoomfactor;
                m_axisx.MaxVal *= m_zoomfactor;
                m_axisy.MinVal *= m_zoomfactor;
                m_axisy.MaxVal *= m_zoomfactor;
                RecalibrateAxis(false);
                RecalibrateAxis(true);
            }
        /** Zooms in.
         * \see SetZoomFactor()
         */
        void Zoom()
            {
                m_axisx.MinVal /= m_zoomfactor;
                m_axisx.MaxVal /= m_zoomfactor;
                m_axisy.MinVal /= m_zoomfactor;
                m_axisy.MaxVal /= m_zoomfactor;
                RecalibrateAxis(false);
                RecalibrateAxis(true);
            }

        /** Sets the position of the reticule, relative to the axis.
         * \param X The X-coordinate.
         * \param Y The Y-coordinate.
         */
        void SetReticule(double X, double Y) { m_reticule = wxEcPointDouble(X,Y); }
        /** Shows or hides the reticule.
         * \param value \a true to show, \a false to hide.
         */
        void SetReticuleVisible(bool value) { m_showreticule = value; }
        /** Gets the color of the reticule.
         */
        wxColour GetReticuleColor() { return m_reticulecolour; }
        /** Changes the color of the reticule.
         */
        void SetReticuleColor(wxColour value) { m_reticulecolour = value; }

        //-- Events
        /** The event when a repaint is called.
         */
        void OnPaint(wxPaintEvent &event);
        /** The event on the move of the mouse.
         */
        void OnMouse(wxMouseEvent &event);
        /** The event when the control is resized.
         */
        void OnResize(wxSizeEvent &event);
};

#endif
