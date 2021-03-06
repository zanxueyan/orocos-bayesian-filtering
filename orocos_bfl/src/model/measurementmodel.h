// $Id$
// Copyright (C) 2002 Klaas Gadeyne <first dot last at gmail dot com>
//
 /***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public                   *
 *   License as published by the Free Software Foundation;                 *
 *   version 2 of the License.                                             *
 *                                                                         *
 *   As a special exception, you may use this file as part of a free       *
 *   software library without restriction.  Specifically, if other files   *
 *   instantiate templates or use macros or inline functions from this     *
 *   file, or you compile this file and link it with other files to        *
 *   produce an executable, this file does not by itself cause the         *
 *   resulting executable to be covered by the GNU General Public          *
 *   License.  This exception does not however invalidate any other        *
 *   reasons why the executable file might be covered by the GNU General   *
 *   Public License.                                                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public             *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                                    *
 *   Boston, MA  02110-1301  USA                                *
 *                                                                         *
 ***************************************************************************/
//
#ifndef __MEASUREMENT_MODEL__
#define __MEASUREMENT_MODEL__

#include "../pdf/conditionalpdf.h"

namespace BFL
{

#define NUMBER_OF_CONDITIONAL_ARGS 2

  /** Template class representing all possible (continu and discrete)
      Measurement Models
      @todo Check if there should be a "model" base class...
      @note Contrary to the system model, this template class has 2
      template arguments: this is because of the different nature of the
      2 conditional densities \f$ P ( Z | X ) \f$ and \f$ P ( X_k |
      X_{k-1} ) \f$
      If \f$ X_{k-1} \f$ is discrete, then \f$ X_{k} \f$ will also be
      discrete, but a discrete state doesn't automatically imply a
      discrete measurement (as is proven in ASR!)
  */
  template<typename MeasVar, typename StateVar> class MeasurementModel {
  protected:
    /// ConditionalPdf representing \f$ P(Z_k | X_{k}, U_{k}) \f$
    /* @bug Since, for now, the library only supports only conditional
       arguments of the same type, both X and U have to be of the same
       type (ie both continu or both discrete!).  I imagine there must
       be systems for which this approach is not general enough @see
       ConditionalPdf
    */
    ConditionalPdf<MeasVar,StateVar>* _MeasurementPdf;

    /// System with no sensor params??
    bool _systemWithoutSensorParams;

  public:
    /// Constructor
    /** @param Measurementpdf ConditionalPdf<MeasVar,StateVar> representing \f$ P(Z_k
	| X_{k} (, U_{k})) \f$
	@see MEASUREMENT_SIZE, STATE_SIZE, INPUT_SIZE, _MeasurementPdf
    */
    MeasurementModel(ConditionalPdf<MeasVar,StateVar>* Measurementpdf=NULL);
    /// Destructor
    virtual ~MeasurementModel();

    /// Get Measurement Size
    int MeasurementSizeGet() const;

    /// Number of Conditional Arguments
    bool SystemWithoutSensorParams() const;

    /// Get the MeasurementPDF
    ConditionalPdf<MeasVar,StateVar> * MeasurementPdfGet();

    /// Set the MeasurementPDF
    /** @param pdf a pointer to the measurement pdf
     */
    void MeasurementPdfSet(ConditionalPdf<MeasVar,StateVar> * pdf);

    /// Simulate the Measurement, given a certain state, and an input
    /** @param x current state of the system
	@param s sensor parameter
	@return Measurement generated by simulating the measurement model
	@param sampling_method the sampling method to be used while
	sampling from the Conditional Pdf describing the system (if not
	specified = DEFAULT)
	@param sampling_args Sometimes a sampling method can have some
	extra parameters (eg mcmc sampling)
	@note Maybe the return value would better be a Sample<StateVar> instead
	of a StateVar
    */
    MeasVar Simulate (const StateVar& x, const StateVar& s, const SampleMthd sampling_method = SampleMthd::DEFAULT, void * sampling_args = NULL);

    /// Simulate the system (no input system)
    /** @param x current state of the system
	@return State where we arrive by simulating the measurement model
	@note Maybe the return value would better be a Sample<StateVar> instead
	of a StateVar
	@param sampling_method the sampling method to be used while
	sampling from the Conditional Pdf describing the system (if not
	specified = DEFAULT)
	@param sampling_args Sometimes a sampling method can have some
	extra parameters (eg mcmc sampling)
    */
    MeasVar Simulate (const StateVar& x, const SampleMthd sampling_method = SampleMthd::DEFAULT, void * sampling_args = NULL);

    /// Get the probability of a certain measurement
    /** given a certain state and input
	@param z the measurement value
	@param x current state of the system
	@param s the sensor param value
	@return the "probability" of the measurement
    */
    Probability ProbabilityGet(const MeasVar& z, const StateVar& x, const StateVar& s );

    /// Get the probability of a certain measurement
    /** (measurement independent of input) gived a certain state and
	input
	@param z the measurement value
	@param x x current state of the system
	@return the "probability" of the measurement
    */
    Probability ProbabilityGet(const MeasVar& z, const StateVar& x );
  };

#include "measurementmodel.cpp"

} // End namespace BFL

#endif // __MEASUREMENT_MODEL__
