/*******************************************************************************
 * Copyright (c) 2017 UT-Battelle, LLC.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompanies this
 * distribution. The Eclipse Public License is available at
 * http://www.eclipse.org/legal/epl-v10.html and the Eclipse Distribution
 *License is available at https://eclipse.org/org/documents/edl-v10.php
 *
 * Contributors:
 *   Alexander J. McCaskey - initial API and implementation
 *******************************************************************************/
#ifndef XACC_ACCELERATOR_HPP_
#define XACC_ACCELERATOR_HPP_

#include "AcceleratorBuffer.hpp"
#include "IRTransformation.hpp"
#include "CompositeInstruction.hpp"
#include "OptionsProvider.hpp"
#include "Identifiable.hpp"
#include "Utils.hpp"
#include "Observable.hpp"
#include "heterogeneous.hpp"
#include <complex>

namespace xacc {

class Accelerator : public OptionsProvider, public Identifiable {

public:
  virtual void initialize(const HeterogeneousMap& params = {}) = 0;
  virtual void updateConfiguration(const HeterogeneousMap &config) = 0;
  virtual void updateConfiguration(const HeterogeneousMap &&config) {
    updateConfiguration(config);
  }
  virtual const std::vector<std::string> configurationKeys() = 0;

  virtual std::vector<std::shared_ptr<IRTransformation>>
  getIRTransformations() = 0;

  virtual const std::vector<double> getOneBitErrorRates() {
    return std::vector<double>{};
  }

  virtual const std::vector<std::pair<std::pair<int, int>, double>>
  getTwoBitErrorRates() {
    // Return list of ((q1,q2),ERROR_RATE)
    return std::vector<std::pair<std::pair<int, int>, double>>{};
  }

  // Execute a single program. All results persisted to the buffer
  virtual void
  execute(std::shared_ptr<AcceleratorBuffer> buffer,
          const std::shared_ptr<CompositeInstruction> CompositeInstruction) = 0;

  // Execute a vector of programs. A new buffer
  // is expected to be appended as a child of the provided buffer.
  virtual void
  execute(std::shared_ptr<AcceleratorBuffer> buffer,
          const std::vector<std::shared_ptr<CompositeInstruction>>
              CompositeInstructions) = 0;

  virtual void cancel(){};

  virtual std::vector<std::pair<int, int>> getConnectivity() {
    return std::vector<std::pair<int, int>>{};
  }


  virtual const std::vector<std::complex<double>>
  getAcceleratorState(std::shared_ptr<CompositeInstruction> program) {
    return std::vector<std::complex<double>>{};
  }

  virtual bool
  handleOptions(const std::map<std::string, std::string> &arg_map) {
    return false;
  }
  virtual OptionPairs getOptions() { return OptionPairs{}; }

  virtual bool isRemote() { return false; }

  virtual ~Accelerator() {}
};

} // namespace xacc
#endif
