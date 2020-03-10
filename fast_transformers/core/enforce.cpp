// Copyright 2020 Tencent
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "enforce.h"

#include <sstream>

#include "absl/debugging/symbolize.h"
namespace fast_transformers {
namespace core {
namespace details {
static constexpr size_t gBufSize = 128;
static thread_local char gBuffer[gBufSize];
const char *EnforceNotMet::what() const noexcept {
  if (!stack_added_) {
    std::ostringstream sout;
    sout << msg_ << "\n";
    sout << "Callstack " << n_ << "\n";
    for (size_t i = 0; i < n_; ++i) {
      void *frame = stacks_[i];
      sout << "\t" << frame;
      if (absl::Symbolize(frame, gBuffer, gBufSize)) {
        sout << "\t" << gBuffer;
      }
      sout << "\n";
    }
    msg_ = sout.str();
    stack_added_ = true;
  }
  return msg_.c_str();
}
}  // namespace details
}  // namespace core
}  // namespace fast_transformers
